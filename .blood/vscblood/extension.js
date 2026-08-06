const vscode = require( "vscode" );
const path = require( "path" );
const fs = require( "fs" );
const child_process = require( "child_process" );

var PATH_WORKSPACE = vscode.workspace.workspaceFolders[0].uri.fsPath;

var PATH_ENGINE = PATH_WORKSPACE;

// Register widget
const WIDGET_BUILD = vscode.window.createStatusBarItem();

function command_build_run()
{
  vscode.window.showInformationMessage(PATH_WORKSPACE);

  var buid_script_path =  path.join( PATH_ENGINE, "/build.sh" ) + " -run";
  TerminalRunCommand( { saveAll: true, command: buid_script_path } );
}

async function activate(context)
{
  console.log("ALL GIRLS ARE THE SAME");

  CloseTerminals();

  context.subscriptions.push(vscode.commands.registerCommand("vscblood.build_run", command_build_run));

  WidgetCreate(context, WIDGET_BUILD, "Build & Run ⚒", "vscblood.build_run", "Green");
  WIDGET_BUILD.tooltip = "Build and run Engine";
}

function deactivate() {}

module.exports = {
	activate,
	deactivate,
};

function WidgetCreate(context, widget, text, command, color = "White") {
  widget.command = command;
  widget.text = text;
  widget.color = color;
  widget.show();
  context.subscriptions.push(widget);
}

// Terminal used by runTerminalCommand()
let g_Terminal;
let g_TerminalPID;

async function TerminalRunCommand( args )
{
	if( !args )
	{
		vscode.window.showErrorMessage( "termExec: no arguments specified to execute." );
		return;
	}

	if( !args.command )
	{
		vscode.window.showErrorMessage( "termExec: no command specified to execute." );
		return;
	}

	if( !g_Terminal )
	{
		// Create Terminal
		g_Terminal = vscode.window.createTerminal( "terminal" );
		g_TerminalPID = await g_Terminal.processId;

		// Make sure to delete our reference if the user closes the terminal.
		vscode.window.onDidCloseTerminal( async (event) =>
		{
			if( await event.processId === g_TerminalPID )
			{
				g_Terminal = undefined;
				g_TerminalPID = undefined;
			}
		} );
	}

	if( args.saveAll )
	{
		await vscode.workspace.saveAll( false );
	}

	g_Terminal.sendText( args.command, true );
	g_Terminal.show( true );
}


function CloseTerminals()
{
	const terminals = vscode.window.terminals;
	terminals.forEach( terminal => { terminal.dispose(); } );
}