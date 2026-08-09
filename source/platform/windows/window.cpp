#include <platform/window.hpp>

#include <core/types.hpp>
#include <core/debug.hpp>
#include <core/debug.hpp>
#include <pipeline.hpp>

#if PLATFORM_WINDOWS

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

const UINT FRAME_COUNT = 2;

HWND g_hwnd = nullptr;
ComPtr<ID3D12Device> g_device;
ComPtr<ID3D12CommandQueue> g_commandQueue;
ComPtr<IDXGISwapChain3> g_swapChain;
ComPtr<ID3D12DescriptorHeap> g_rtvHeap;
ComPtr<ID3D12Resource> g_renderTargets[FRAME_COUNT];
ComPtr<ID3D12CommandAllocator> g_commandAllocator;
ComPtr<ID3D12GraphicsCommandList> g_commandList;
ComPtr<ID3D12Fence> g_fence;
UINT64 g_fenceValue = 0;
HANDLE g_fenceEvent = nullptr;
UINT g_rtvDescriptorSize = 0;
UINT g_frameIndex = 0;

void WaitForGpu()
{
    const UINT64 fenceToWaitFor = ++g_fenceValue;
    g_commandQueue->Signal(g_fence.Get(), fenceToWaitFor);

    if (g_fence->GetCompletedValue() < fenceToWaitFor)
    {
        g_fence->SetEventOnCompletion(fenceToWaitFor, g_fenceEvent);
        WaitForSingleObject(g_fenceEvent, INFINITE);
    }
}

void InitD3D12(HWND hwnd, UINT width, UINT height)
{
    UINT dxgiFactoryFlags = 0;

#if defined(_DEBUG)
    ComPtr<ID3D12Debug> debugController;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
    {
        debugController->EnableDebugLayer();
        dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
    }
#endif

    ComPtr<IDXGIFactory6> factory;
    CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory));

    // Cria o device com o adaptador de hardware padrao
    D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&g_device));

    // Command Queue
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    g_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&g_commandQueue));

    // Swap Chain
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.BufferCount = FRAME_COUNT;
    swapChainDesc.Width = width;
    swapChainDesc.Height = height;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.SampleDesc.Count = 1;

    ComPtr<IDXGISwapChain1> swapChain1;
    factory->CreateSwapChainForHwnd(
        g_commandQueue.Get(), hwnd, &swapChainDesc, nullptr, nullptr, &swapChain1);

    swapChain1.As(&g_swapChain);
    g_frameIndex = g_swapChain->GetCurrentBackBufferIndex();

    // RTV Descriptor Heap
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.NumDescriptors = FRAME_COUNT;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    g_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&g_rtvHeap));

    g_rtvDescriptorSize = g_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = g_rtvHeap->GetCPUDescriptorHandleForHeapStart();
    for (UINT i = 0; i < FRAME_COUNT; i++)
    {
        g_swapChain->GetBuffer(i, IID_PPV_ARGS(&g_renderTargets[i]));
        g_device->CreateRenderTargetView(g_renderTargets[i].Get(), nullptr, rtvHandle);
        rtvHandle.ptr += g_rtvDescriptorSize;
    }

    // Command Allocator e Command List
    g_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&g_commandAllocator));
    g_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
        g_commandAllocator.Get(), nullptr, IID_PPV_ARGS(&g_commandList));
    g_commandList->Close();

    // Fence para sincronizacao CPU/GPU
    g_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&g_fence));
    g_fenceValue = 1;
    g_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

#include <vector>
std::vector<float> color_normalize( std::vector<unsigned char> colors )
{
    std::vector<float> new_vec;

    for ( unsigned char color : colors )
    {
        const float cast_color = static_cast<float>( color );
        new_vec.push_back( cast_color );
    }

    return new_vec;
}

void RenderFrame()
{
    g_commandAllocator->Reset();
    g_commandList->Reset(g_commandAllocator.Get(), nullptr);

    // Transicao: PRESENT -> RENDER_TARGET
    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Transition.pResource = g_renderTargets[g_frameIndex].Get();
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    g_commandList->ResourceBarrier(1, &barrier);

    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = g_rtvHeap->GetCPUDescriptorHandleForHeapStart();
    rtvHandle.ptr += (SIZE_T)g_frameIndex * g_rtvDescriptorSize;

    std::vector<unsigned char> clearColor = { 255, 0, 0, 255 }; // azul escuro
    g_commandList->ClearRenderTargetView(rtvHandle, color_normalize( clearColor ).data(), 0, nullptr);

    // Transicao: RENDER_TARGET -> PRESENT
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
    g_commandList->ResourceBarrier(1, &barrier);

    g_commandList->Close();

    ID3D12CommandList* lists[] = { g_commandList.Get() };
    g_commandQueue->ExecuteCommandLists(1, lists);

    g_swapChain->Present(1, 0);

    WaitForGpu();
    g_frameIndex = g_swapChain->GetCurrentBackBufferIndex();
}

#include <vendor/windows/windows.hpp>

static struct
{
	WNDCLASSW wndClass;
	HWND hwnd;
	bool is_running;
} window_handle = { .is_running = false_value };

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if ( msg == WM_DESTROY )
	{
        PostQuitMessage(0);
		window_handle.is_running = false_value;
        return 0;
    }

    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK InputProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return DefWindowProcW( hwnd, msg, wParam, lParam );
}

bool CoreWindow::init()
{
    HINSTANCE Instance = GetModuleHandleW( nullptr );

	window_handle.wndClass = {};
    //window_handle.wndClass.cbSize = sizeof(WNDCLASS);
    window_handle.wndClass.style = CS_HREDRAW | CS_VREDRAW;
    #if API_OPENGL
    window_handle.wndClass.style = 0;
    #endif
    window_handle.wndClass.cbClsExtra = 0;
    window_handle.wndClass.cbWndExtra = 0;
    window_handle.wndClass.lpfnWndProc = WindowProc;
    window_handle.wndClass.hInstance = Instance;
    window_handle.wndClass.lpszClassName = TE( "BloodClassWindow" );

    if ( !RegisterClassW( &window_handle.wndClass ) )
    {
        // TODO:
        return false_value;
    }

	UINT width = static_cast<UINT>( WindowConfig::Get::width() );
	UINT height = static_cast<UINT>( WindowConfig::Get::height() );

	window_handle.hwnd = CreateWindowExW(
        #if API_D3D12
        WS_EX_NOREDIRECTIONBITMAP,
        #else
        0,
        #endif
		window_handle.wndClass.lpszClassName,
        nullptr,
		WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        width,
        height,
        nullptr,
        nullptr, Instance, nullptr);

	InitD3D12( window_handle.hwnd , width, height );

	window_handle.is_running = true_value;

    CoreWindow::update_config();

	return true_value;
}

void CoreWindow::show()
{
	ShowWindow( window_handle.hwnd, SW_NORMAL );
}

bool CoreWindow::should_close()
{
	return !window_handle.is_running;
}

void CoreWindow::pool()
{
	MSG msg = {};
    while (msg.message != WM_QUIT)
    {
        if (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
		}
		else
        {
            RenderFrame();
        }
    }

}


// TODO: Move
#define ARRAY_SIZE( array ) sizeof(array) / sizeof(array[0])

void CoreWindow::update_config()
{
    {
        static wchar_t title[ 512 ];

        int length = static_cast<int>( strlen( WindowConfig::Get::title() ) );
        int size = MultiByteToWideChar( CP_UTF8, 0, WindowConfig::Get::title(), length, NULL, 0 );

        if ( size < static_cast<int>( ARRAY_SIZE( title ) ) )
        {
            MultiByteToWideChar( CP_UTF8, 0, WindowConfig::Get::title(), length, title, size );
            title[ size ] = L'\0';

            SetWindowTextW( window_handle.hwnd, title );
        }

    }
}

void CoreWindow::terminate()
{
	WaitForGpu();
    CloseHandle( g_fenceEvent );
}

#endif