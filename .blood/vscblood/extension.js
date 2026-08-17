const vscode = require("vscode");
const path = require("path");
const fs = require("fs");
const os = require("os");

let PATH_WORKSPACE = null;

function get_workspace_path()
{
	if ( !vscode.workspace.workspaceFolders || vscode.workspace.workspaceFolders.length === 0 )
	{
		vscode.window.showErrorMessage( "Not found a window" );
		return null;
	}

	return vscode.workspace.workspaceFolders[0].uri.fsPath;
}

function PlatformIsWindows()
{
	return process.platform === "win32";
}

function fsExistsWithTimeout( filePath, timeoutMs )
{
	return new Promise( ( resolve, reject ) =>
	{
		const start = Date.now();

		const check = () =>
		{
			if ( fs.existsSync( filePath ) )
			{
				resolve( true );
				return;
			}
			if ( Date.now() - start >= timeoutMs )
			{
				reject( new Error( `Timeout waiting for ${filePath}` ) );
				return;
			}
			setTimeout( check, 100 );
		};

		check();
	} );
}

// Widgets
const WIDGET_GENERETE = vscode.window.createStatusBarItem( vscode.StatusBarAlignment.Left, 100 );
const WIDGET_BUILD = vscode.window.createStatusBarItem( vscode.StatusBarAlignment.Left, 99 );
const WIDGET_RUN = vscode.window.createStatusBarItem( vscode.StatusBarAlignment.Left, 98 );
const WIDGET_CLEAN = vscode.window.createStatusBarItem( vscode.StatusBarAlignment.Left, 97 );


// ----------------------------------------------------------------------
// Terminal helpers
// ----------------------------------------------------------------------

let g_Terminal;
let g_TerminalPID;

async function ensure_terminal()
{
	if ( !g_Terminal )
	{
		g_Terminal = vscode.window.createTerminal( "Blood" );
		g_TerminalPID = await g_Terminal.processId;

		vscode.window.onDidCloseTerminal( async ( event ) =>
		{
			if ( await event.processId === g_TerminalPID )
			{
				g_Terminal = undefined;
				g_TerminalPID = undefined;
			}
		} );
	}
}

// Fire-and-forget: sends the command and returns as soon as it's been sent.
// Does NOT wait for the command to actually finish running in the shell.
async function TerminalRunCommand( args )
{
	if ( !args || !args.command )
	{
		vscode.window.showErrorMessage( "termExec: no command specified to execute." );
		return;
	}

	await ensure_terminal();

	if ( args.saveAll )
	{
		await vscode.workspace.saveAll( false );
	}

	g_Terminal.sendText( args.command, true );
	g_Terminal.show( true );
}

// Runs a command and waits (via lock file) for it to actually finish
// before resolving. Rejects if the command fails or times out, so callers
// can safely chain steps that depend on each other (e.g. rebuild).
async function CommandRunWait( args )
{
	if ( !args || !args.command )
	{
		vscode.window.showErrorMessage( "termExec: no command specified to execute." );
		return;
	}

	await ensure_terminal();

	if ( args.saveAll !== false )
	{
		await vscode.workspace.saveAll( false );
	}

	const pathDotVscode = args.workspaceRoot
		? path.join( args.workspaceRoot, ".vscode" )
		: os.tmpdir();

	if ( !fs.existsSync( pathDotVscode ) )
	{
		fs.mkdirSync( pathDotVscode, { recursive: true } );
	}

	const lockPath = path.join( pathDotVscode, "termExec.lock" );

	// Clear any stale lock left from a previous crashed/interrupted run,
	// otherwise CommandRunWait can return immediately on a false positive.
	try { fs.rmSync( lockPath ); } catch { /* didn't exist, fine */ }

	// IMPORTANT: use && (not ; or &) so the lock is only written if the
	// actual command succeeded. Otherwise a failed build silently looks
	// like a success to the caller.
	const writeLock = PlatformIsWindows() ? " && echo lock> " : " && touch ";
	const command = args.command + writeLock + `"${lockPath}"`;

	g_Terminal.sendText( command, true );
	g_Terminal.show( true );

	const lockTimeout = args.timeoutMs || 30000;

	try
	{
		await fsExistsWithTimeout( lockPath, lockTimeout );
	}
	finally
	{
		try { fs.rmSync( lockPath ); } catch { /* already gone or never created */ }
	}
}

function close_terminals()
{
	const terminals = vscode.window.terminals;
	terminals.forEach( terminal => { terminal.dispose(); } );
	g_Terminal = undefined;
	g_TerminalPID = undefined;
}


// ----------------------------------------------------------------------
// Commands
// ----------------------------------------------------------------------

async function command_generate()
{
	const root = get_workspace_path();
	if ( !root ) return;

	const generatorSrc = path.join( root, ".blood/vscblood/generator/generator.cpp" );
	const generatorBin = path.join( root, ".blood/vscblood/generator/generator" );
	const coreDebug = path.join( root, "source/core/debug.cpp" );
	const includeDir = path.join( root, "source" );

	const cmd = [
		`clang++ "${generatorSrc}" "${coreDebug}" -o "${generatorBin}" -I"${includeDir}"`,
		`"${generatorBin}"`,
		`rm -f "${generatorBin}"`
	].join( " && " );

	await TerminalRunCommand( { command: cmd, workspaceRoot: root } );
}

async function command_build()
{
	const root = get_workspace_path();
	if ( !root ) return;

	const cmd = `mkdir -p build build/objects build/bin && ninja`;

	await TerminalRunCommand( { command: cmd, workspaceRoot: root } );
}

async function command_run()
{
	const root = get_workspace_path();
	if ( !root ) return;

	const binary = path.join( root, "build/bin/flappy" );
	await TerminalRunCommand( { command: `"${binary}"`, saveAll: false } );
}

async function command_clean()
{
	const root = get_workspace_path();
	if ( !root ) return;

	await CommandRunWait( { command: `ninja -t clean`, workspaceRoot: root } );
}

async function command_rebuild()
{
	try
	{
		await command_clean();
		await command_generate();
		await command_build();
	}
	catch ( err )
	{
		vscode.window.showErrorMessage( `Blood: rebuild failed - ${err.message}` );
	}
}


// ----------------------------------------------------------------------
// Activation
// ----------------------------------------------------------------------

async function activate( context )
{
	PATH_WORKSPACE = get_workspace_path();

	close_terminals();

	// Commands
	context.subscriptions.push(
		vscode.commands.registerCommand( "vscblood.generete", command_generate ),
		vscode.commands.registerCommand( "vscblood.build", command_build ),
		vscode.commands.registerCommand( "vscblood.run", command_run ),
		vscode.commands.registerCommand( "vscblood.clean", command_clean ),
		vscode.commands.registerCommand( "vscblood.rebuild", command_rebuild ),
	);

	// Widgets
	WIDGET_GENERETE.text = "$(tools) Generate";
	WIDGET_GENERETE.command = "vscblood.generete";
	WIDGET_GENERETE.tooltip = "Blood: Generate build.ninja";
	WIDGET_GENERETE.show();

	WIDGET_BUILD.text = "$(check) Build";
	WIDGET_BUILD.command = "vscblood.build";
	WIDGET_BUILD.tooltip = "Blood: Ninja build";
	WIDGET_BUILD.show();

	WIDGET_RUN.text = "$(play) Run";
	WIDGET_RUN.command = "vscblood.run";
	WIDGET_RUN.tooltip = "Blood: Run runtime";
	WIDGET_RUN.show();

	WIDGET_CLEAN.text = "$(trash) Clean";
	WIDGET_CLEAN.command = "vscblood.clean";
	WIDGET_CLEAN.tooltip = "Blood: Clean";
	WIDGET_CLEAN.show();

	context.subscriptions.push(
		WIDGET_GENERETE,
		WIDGET_BUILD,
		WIDGET_RUN,
		WIDGET_CLEAN
	);
}

function deactivate() { }

module.exports =
{
	activate,
	deactivate,
};