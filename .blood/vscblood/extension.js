const vscode = require("vscode");
const path = require("path");
const fs = require("fs");
const child_process = require("child_process");

var PATH_WORKSPACE = vscode.workspace.workspaceFolders[0].uri.fsPath;

var PATH_ENGINE = PATH_WORKSPACE;
var PATH_GENERATOR = path.join(PATH_ENGINE, ".blood/vscblood/generator");
var PATH_RUNTIME = path.join(PATH_ENGINE, "runtime/flappy")

// Register widget
const WIDGET_BUILD = vscode.window.createStatusBarItem();

const WIDGET_RUN = vscode.window.createStatusBarItem();

function command_build_generator() {

	const generatorSrc = path.join(PATH_GENERATOR, "generator.cpp");
	const generatorBin = path.join(PATH_GENERATOR, "generator");
	const buildScript = path.join(PATH_GENERATOR, "build.sh");

	const chained = [
		`clang++ "${generatorSrc}" "source/core/debug.cpp" -o "${generatorBin}" -I/home/satoshi/HotFolder/Blood/source`, // compila
		`"${generatorBin}"`,                          // executa o binário gerado
		`rm -f "${generatorBin}"`,                    // remove o binário
		`bash "${buildScript}"`                       // roda o build.sh
	].join(" && ");

	TerminalRunCommand({ saveAll: true, command: chained });
	//CloseTerminals();
}

function command_run() {
	const RunRuntime = path.join(PATH_RUNTIME, "build.sh");
	TerminalRunCommand({ saveAll: true, command: RunRuntime });

	const Binary = path.join(PATH_RUNTIME, "build/flappy");
	TerminalRunCommand({ saveAll: true, command: Binary });

}

async function activate(context) {

	CloseTerminals();
	context.subscriptions.push(vscode.commands.registerCommand("vscblood.build_generator", command_build_generator));

	context.subscriptions.push(vscode.commands.registerCommand("vscblood.run", command_run));

	WidgetCreate(context, WIDGET_BUILD, "Generate Build ⚒", "vscblood.build_generator", "Orange");
	WIDGET_BUILD.tooltip = "Build Generator";
	WidgetCreate(context, WIDGET_RUN, "Run ⚒", "vscblood.run", "Green");
	WIDGET_RUN.tooltip = "Run runtime";
}

function deactivate() { }

module.exports =
{
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

async function TerminalRunCommand(args) {
	if (!args) {
		vscode.window.showErrorMessage("termExec: no arguments specified to execute.");
		return;
	}

	if (!args.command) {
		vscode.window.showErrorMessage("termExec: no command specified to execute.");
		return;
	}

	if (!g_Terminal) {
		// Create Terminal
		g_Terminal = vscode.window.createTerminal("terminal");
		g_TerminalPID = await g_Terminal.processId;

		// Make sure to delete our reference if the user closes the terminal.
		vscode.window.onDidCloseTerminal(async (event) => {
			if (await event.processId === g_TerminalPID) {
				g_Terminal = undefined;
				g_TerminalPID = undefined;
			}
		});
	}

	if (args.saveAll) {
		await vscode.workspace.saveAll(false);
	}

	g_Terminal.sendText(args.command, true);
	g_Terminal.show(true);
}


function CloseTerminals() {
	const terminals = vscode.window.terminals;
	terminals.forEach(terminal => { terminal.dispose(); });
}