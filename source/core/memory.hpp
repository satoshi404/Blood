#pragma once

#include <core/types.hpp>

#include <core/debug.hpp>

#include <vendor/libc/stdlib.hpp>
#include <vendor/libc/string.hpp>

struct TimeSpec
{
    i64 tv_sec;
    i64 tv_nsec;
};

// MOVE
extern "C" int clock_gettime( int, TimeSpec* );
static constexpr int MONOTONIC = 1;

static INLINE u64 alloc_time_ns()
{
#if PLATFORM_LINUX
    TimeSpec ts{};
    clock_gettime( MONOTONIC, &ts );

    return static_cast<u64>( ts.tv_sec ) * 1000000000ULL + static_cast<u64>( ts.tv_nsec );

#elif PLATFORM_WINDOWS
    // TODO:
   // return GetTickCount64() * 1000000ULL;
   return 0;
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//inline void* operator new( unsigned long size , void* pointer ) noexcept { return pointer; }
inline void operator delete( void*, void* ) noexcept {}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


static constexpr u32 MAX_CHUNKS    = 1024;
static constexpr u64 INVALID_CHUNK = 0xFFFFFFFFFFULL;
static constexpr u32 DEFAULT_ALIGN = 16;

struct SourceLocation
{
	const char* file = nullptr;
	u32 line = 0;
};

#define ARENA_LOCATION ( SourceLocation{ __FILE__, ( u32 )__LINE__ } )

struct ChunkMeta
{
	u64 offset;
	u64 size;
	u32 index;
	u32 ref_count;
	bool in_use;
	u64 alloc_ns;
	u64 free_ns;
	u64 lifetime_ns;
	SourceLocation alloc_loc;
	SourceLocation free_loc;
	u32 alloc_count;
};

struct ChunkHandle
{
    u32 index;
    u32 id; // Arena id

    NO_DISCARD bool valid() const
    {
        return index != static_cast<u32>( INVALID_CHUNK );
    }

    static ChunkHandle null()
    {
        return ChunkHandle{ static_cast<u32>( INVALID_CHUNK ), 0 };
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


struct Arena;

struct ScopeHandle
{
    ChunkHandle handle;
    Arena* arena;
    bool  owns;

    ScopeHandle()
        :handle( ChunkHandle::null() ), arena( nullptr ), owns( false_value ) {}

    ScopeHandle( const ScopeHandle& ) = delete;
    ScopeHandle& operator=( const ScopeHandle& ) = delete;

    ScopeHandle( ScopeHandle&& scope ) noexcept
        :handle( scope.handle ), arena( scope.arena ), owns( scope.owns )
    {
        scope.handle = ChunkHandle::null();
        scope.arena  = nullptr;
        scope.owns   = false_value;
    }

    ScopeHandle& operator=( ScopeHandle&& scope ) noexcept
    {
        if ( this != &scope )
        {
            release();

            handle = scope.handle;
            arena  = scope.arena;
            owns   = scope.owns;

            scope.handle = ChunkHandle::null();
            scope.arena  = nullptr;
            scope.owns   = false_value;
        }

        return *this;
    }

    ~ScopeHandle() { release(); }

    void release();

    NO_DISCARD bool valid() const;
    NO_DISCARD void* ptr() const;
    NO_DISCARD usize size() const;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


struct Arena
{
    u8* base;
    u64 capacity;
    u64 head;
    u32 id;
    u32 chunk_count;
    u32 live_allocs;
    u32 peak_live;
    u64 total_alloc_ns;

    ChunkMeta chunks[ MAX_CHUNKS ];

    static u32 s_next_id;

    static Arena create( u64 capacity_bytes )
    {
        Arena arena{};

        arena.base = static_cast<u8*>( malloc( capacity_bytes ) );
        arena.capacity = capacity_bytes;
        arena.head = 0;
        arena.id = ++s_next_id;
        arena.chunk_count = 0;
        arena.live_allocs = 0;
        arena.peak_live = 0;
        arena.total_alloc_ns = 0;

        if ( arena.base )
        {
            memset( arena.base, 0, capacity_bytes );
        }
        else
        {
            Debug::Println( PrintColor_Red, "[Arena #%u] Failed to allocate %llu bytes", arena.id, static_cast<u64>( capacity_bytes ) );
        }

        return arena;
    }

    void destroy( SourceLocation location = ARENA_LOCATION )
    {
        report_leaks( location );

        free( base );
        base = nullptr;
        capacity = 0;
        head = 0;
    }

    static u64 align_up( u64 offset, u64 align )
    {
        return ( ( offset + ( align - 1 ) ) & ~( align - 1 ) );
    }

    NO_DISCARD ChunkHandle alloc( u64 size, u64 align = DEFAULT_ALIGN, SourceLocation location = ARENA_LOCATION )
    {
        if ( !base || size == 0 ) return ChunkHandle::null();

        u64 aligned_head = align_up( head, align );
        u64 end = aligned_head + size;

        if ( end > capacity )
        {
            Debug::Println( PrintColor_Red, "[Arena #%u] OutOfMemory: lost=%llu bytes, free=%llu", id, static_cast<u64>( size ), static_cast<u64>( capacity - head ) );
            return ChunkHandle::null();
        }

        for ( u32 index = 0; index < chunk_count; index++ )
        {
            ChunkMeta& meta_chunk = chunks[ index ];
            if ( !meta_chunk.in_use && meta_chunk.ref_count == 0 && meta_chunk.size >= size )
            {
                meta_chunk.in_use = true_value;
                meta_chunk.ref_count = 1;
                // TODO: alloc_time_ns()
                meta_chunk.alloc_ns = alloc_time_ns();
                meta_chunk.free_ns = 0;
                meta_chunk.alloc_loc = location;
                meta_chunk.free_loc = {};
                meta_chunk.alloc_count++;

                memset( base + meta_chunk.offset , 0, meta_chunk.size );

                live_allocs++;
                if ( live_allocs > peak_live ) peak_live = live_allocs;

                return ChunkHandle{ static_cast<u32>( index ), id };
            }
        }

        if ( chunk_count >= MAX_CHUNKS )
        {
            Debug::Println( PrintColor_Red, "[Arena #%u] Chunk limit reached", id );
            return ChunkHandle::null();
        }

        u32 idx = chunk_count++;
        ChunkMeta& meta_chunk = chunks[ idx ];

        meta_chunk.offset = aligned_head;
	    meta_chunk.size = size;
	    meta_chunk.index = idx;
	    meta_chunk.ref_count = 1;
	    meta_chunk.in_use = true_value;
        // TODO: alloc_time_ns()
	    meta_chunk.alloc_ns = alloc_time_ns();
	    meta_chunk.free_ns = 0;
	    meta_chunk.lifetime_ns = 0;
	    meta_chunk.alloc_loc = location;
	    meta_chunk.free_loc = {};
	    meta_chunk.alloc_count = 1;

        head = end;

        live_allocs++;
        if ( live_allocs > peak_live ) peak_live = live_allocs;

        return ChunkHandle{ idx, id };
    }

    // RAII

    NO_DISCARD ScopeHandle alloc_scoped( u64 size, u64 align = DEFAULT_ALIGN, SourceLocation location = ARENA_LOCATION )
    {
        ScopeHandle scope;
        scope.handle = alloc( size, align, location );
        scope.arena = this;

        const bool check_scope = scope.handle.valid();
        scope.owns = check_scope;

        return scope;
    }

    #define CheckHandleRet( handle, ret ) \
        if ( !( handle ).valid() || ( handle ).id != id || ( handle ).index >= chunk_count ) return ret;
    #define ChunkMetaInUseRet( chunk, ret ) \
        if ( !( chunk ).in_use ) return ret;

    NO_DISCARD void* ptr_of( ChunkHandle handle ) const
    {
        CheckHandleRet( handle, nullptr );

        const ChunkMeta& meta_chunk = chunks[ handle.index ];
        ChunkMetaInUseRet( meta_chunk, nullptr );

        return base + meta_chunk.offset;
    }

    NO_DISCARD u64 size_of( ChunkHandle handle ) const
    {
        if ( !handle.valid() || handle.index >= chunk_count ) return 0;

        return chunks[ handle.index ].size;
    }


    bool ref( ChunkHandle handle )
    {
        CheckHandleRet( handle, false_value );

        ChunkMeta& meta_chunk = chunks[ handle.index ];
        ChunkMetaInUseRet( meta_chunk, false_value );

        meta_chunk.ref_count++;

        return true_value;
    }

    bool release( ChunkHandle handle, SourceLocation location = ARENA_LOCATION )
    {
        CheckHandleRet( handle, false_value );

        ChunkMeta& meta_chunk = chunks[ handle.index ];
        if ( !meta_chunk.in_use || meta_chunk.ref_count == 0 ) return false_value;

        meta_chunk.ref_count--;
        if ( meta_chunk.ref_count == 0 )
        {
             // TODO: alloc_time_ns()
            u64 now = alloc_time_ns();
            meta_chunk.lifetime_ns += now - meta_chunk.alloc_ns;
            meta_chunk.free_ns = now;
            meta_chunk.in_use = false_value;
            meta_chunk.free_loc = location;

            live_allocs--;
        }

        return true_value;
    }

    void reset()
    {
        memset( base, 0, head );
        head = 0;
        chunk_count = 0;
        live_allocs = 0;
    }

    NO_DISCARD ChunkMeta* meta_of( ChunkHandle handle )
    {
        CheckHandleRet( handle, nullptr );

        return &chunks[ handle.index ];
    }

    NO_DISCARD const ChunkMeta* meta_of( ChunkHandle handle ) const
    {
        CheckHandleRet( handle, nullptr );

        return &chunks[ handle.index ];
    }

    NO_DISCARD u64 age_ns( ChunkHandle handle ) const
    {
        const ChunkMeta* meta_chunk = meta_of( handle );
        if ( !meta_chunk || !meta_chunk->in_use ) return 0;

        return alloc_time_ns() - meta_chunk->alloc_ns;
    }

    void report_leaks( SourceLocation location_caller = ARENA_LOCATION ) const
    {
        u32 leaks = 0;
        for ( u32 i = 0; i < chunk_count; i++ )
        {
            const ChunkMeta& meta_chunk = chunks[ i ];
            if ( meta_chunk.in_use )
            {
                leaks++;
                Debug::Println( PrintColor_Yellow, "[Arena #%u] Leak detected: chunk #%u, size=%llu bytes, alloc_loc=%s:%u", id, i, meta_chunk.size, meta_chunk.alloc_loc.file, meta_chunk.alloc_loc.line );
            }
        }

        Debug::Println( PrintColor_Yellow, "[Arena #%u] Leak report generated at %s:%u", id, location_caller.file, location_caller.line );
        Debug::Println( PrintColor_Yellow, "[Arena #%u] Total capacity: %llu bytes, used: %llu bytes, free: %llu bytes", id, capacity, head, capacity - head );
        Debug::Println( PrintColor_Yellow, "[Arena #%u] Leak report complete. Total leaks: %u", id, leaks );

        if ( leaks > 0 )
        {
            for ( u32 i = 0; i < chunk_count; i++ )
            {
                const ChunkMeta& meta_chunk = chunks[ i ];
                if ( !meta_chunk.in_use ) continue;

                Debug::Println( PrintColor_Red, "[Arena #%u] Memory leaks detected! Please check the above report.", id );
                Debug::Println( PrintColor_Red, "[Arena #%u] Leak details: chunk #%u, size=%llu bytes, alloc_loc=%s:%u", id, i, meta_chunk.size, meta_chunk.alloc_loc.file, meta_chunk.alloc_loc.line );
                Debug::Println( PrintColor_Red, "[Arena #%u] Leak age: %llu ns, ref_count: %u", id, alloc_time_ns() - meta_chunk.alloc_ns, meta_chunk.ref_count );
                Debug::Println( PrintColor_Red, "[Arena #%u] Total lifetime: %llu ns, alloc_count: %u", id, meta_chunk.lifetime_ns, meta_chunk.alloc_count );
            }
        }
    }

    void print_status() const
    {
        u64 free_memory = capacity - head;
        Debug::Println( PrintColor_Cyan, "[Arena #%u] Status: capacity=%llu bytes, used=%llu bytes, free=%llu bytes, live_allocs=%u, peak_live=%u, total_alloc_ns=%llu", id, capacity, head, free_memory, live_allocs, peak_live, total_alloc_ns );
    }

};

u32 Arena::s_next_id = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

INLINE void ScopeHandle::release()
{
    if ( owns && arena && handle.valid() )
    {
        arena->release( handle, ARENA_LOCATION );
        owns = false_value;
        handle = ChunkHandle::null();
        arena = nullptr;
    }
}

INLINE void* ScopeHandle::ptr() const
{
    if ( !arena || !handle.valid() ) return nullptr;

    return arena->ptr_of( handle );
}

INLINE usize ScopeHandle::size() const
{
    if ( !arena || !handle.valid() ) return 0;

    return static_cast<usize>( arena->size_of( handle ) );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
struct TypedHandle
{
    ChunkHandle handle;
    Arena* arena;

    NO_DISCARD bool valid() const
    {
        return handle.valid();
    }

    NO_DISCARD T* ptr( Arena* arena ) const
    {
        if ( !arena || !handle.valid() ) return nullptr;

        return static_cast<T*>( arena->ptr_of( handle ) );
    }

    NO_DISCARD usize size( Arena* arena ) const
    {
        if ( !arena || !handle.valid() ) return 0;

        return arena->size_of( handle );
    }

    NO_DISCARD ChunkMeta* meta( Arena* arena ) const
    {
        if ( !arena || !handle.valid() ) return nullptr;

        return arena->meta_of( handle );
    }

    NO_DISCARD T* get() const
    {
        return ptr( arena );
    }

    NO_DISCARD T& operator*() const
    {
        return *get();
    }

    NO_DISCARD T* operator->() const
    {
        return get();
    }

    void release( SourceLocation location = ARENA_LOCATION )
    {
        if ( arena && handle.valid() )
        {
            arena->release( handle, location );
        }
    }
};

template<typename T>
NO_DISCARD TypedHandle<T> make_typed_handle( ChunkHandle handle, Arena* arena )
{
    return TypedHandle<T>{ handle, arena };
}

template<typename T>
NO_DISCARD TypedHandle<T> make_typed_handle( ScopeHandle scope )
{
    return TypedHandle<T>{ scope.handle, scope.arena };
}

template<typename T>
NO_DISCARD static TypedHandle<T> arena_alloc_typed( Arena& arena, SourceLocation location = ARENA_LOCATION )
{
    ChunkHandle handle = arena.alloc( sizeof(T), alignof(T), location );
    if ( !handle.valid() ) return TypedHandle<T>{ handle, &arena };

    void* pointer = arena.ptr_of( handle );
    new ( pointer ) T();

    return TypedHandle<T>{ handle, &arena };
}