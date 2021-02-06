/*
Main.js - Contains main server file
*/

/*
 * Copyright (c) Aaron Becker
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial expressApplications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 *    1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    expressAppreciated but is not required.
 *
 *    2. Altered source versions must be plainly marked as such, and must not
 *    be misrepresented as being the original software.
 *
 *    3. This notice may not be removed or altered from any source
 *    distribution.
 */

 /* Dependency initialization */

 //Basic Dependencies
const { app, BrowserWindow } = require('electron')
const fs = require('fs');
const path = require('path');
const express = require('express');
const ejs = require('ejs');
const expressApp = express();
const http = require('http').Server(expressApp);
const io = require('socket.io')(http, {
  cors: {
    origin: "http://localhost",
    methods: ["GET", "POST"]
  },
  'pingTimeout': 180000,
  'pingInterval': 25000
});

//Express dependencies
const bodyParser = require('body-parser');
const cors = require('cors');

//Addtl core deps
const RequestHandler = require("./core/requestHandler.js");

const transmitter = require('./core/transmitter.js'); //require the driver;
const handleConnectDisconnect = conn => {
	console.log("txmitter connectDisconnect: "+conn);
	transmitterConnected = conn;
	if (conn) {
		io.emit("t-connect");

		transmitter.sendCommand("outputEnable", "true", "outputEnable", "true").then(() => {
			console.log("Enabled packets");
		}).catch(e => {
			console.log("Packet enabling failed? Physically restart txmitter or server");
		})
	} else {
		io.emit("t-disconnect");
	}
}

const handleDefaultCommand = command => { //when no other currently active command is matched
	console.log("GOT COMMAND FROM ARDUINO: "+command)
}

transmitter.init(handleConnectDisconnect, handleDefaultCommand);


io.on('connection', client => {
	// client.on('disconnect', () => {});
	console.log("CLIENT CONNECT");
	
	//Client uiButton clicked
	client.on('uiButton', event => {
		console.log("UI BUTTON FIRE: "+event);
	})
});

expressApp.use(cors()); //enable cors

expressApp.use(bodyParser.urlencoded({ extended: true })); //, limit: '50mb' })); //bodyparser for getting json data
expressApp.use(bodyParser.json());
expressApp.use(express.static(path.join(__dirname,"assets"))); //define a static assets directory
expressApp.set('view engine', 'ejs'); //ejs gang

expressApp.get('/status', (req, res) => {
    return res.end(RequestHandler.SUCCESS());
});

expressApp.use(function(req, res, next){ //anything else that doesn't match those filters
	res.render('index');
});

console.log("Starting server");
const port = process.env.PORT || 80;

const server = http.listen(port, () => {
  console.log('Planeside is running on port', server.address().port);
});

function createWindow () {
	const win = new BrowserWindow({
		width: 1920,
		height: 1080,
		// autoHideMenuBar: true,
		// useContentSize: true,
		resizable:true
	})

	// win.loadFile(path.join(__dirname,"index.html"))
	win.loadURL('http://localhost:80/');
}

app.whenReady().then(createWindow)

app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') {
    app.quit()
  }
})

app.on('activate', () => {
  if (BrowserWindow.getAllWindows().length === 0) {
    createWindow()
  }
})