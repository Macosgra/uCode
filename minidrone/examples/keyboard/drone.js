const keypress = require('keypress');
const Drone = require('parrot-minidrone');
const drone = new Drone({
    autoconnect: true,
});
let timeout = null;

var data2;
var negative = false;
var valor = 0;
var array = [];
var SerialPort = require("serialport");
	var serialport = new SerialPort("/dev/tty.usbmodem1411");
	
	serialport.on('open', function(){
		console.log('Serial Port Opend');
			serialport.on('data', function(data){
				console.log(data[0]);
				array.push(data[0]);
			});
	});
	
/* 	for(//var i = 0; i < 6;i++){ */
        //(function(){
        //    //var j = i;
        //    //console.log("Loading message %d".green, j);
        //    //htmlMessageboardString += MessageToHTMLString(BoardMessages[j]);
        //    
    //}	//
	
	



keypress(process.stdin);

// listen for the "keypress" event
process.stdin.on('keypress', (ch, key) => {

    const keyName = key && key.name ? key.name : false;
    const inputSensitivity = 70;
	
	
		
	
    if (!keyName) {
        return;
    }
    if (timeout) {
        clearTimeout(timeout);
        timeout = null;
    }

    const flightParams = {
        yaw: 0,
        pitch: 0,
        roll: 0,
        altitude: 0,
    };

    switch (keyName) {
    case 'up':
        flightParams.pitch = inputSensitivity;
        break;
    case 'down':
        flightParams.pitch = -inputSensitivity;
        break;
    case 'left':
        flightParams.roll = -inputSensitivity;
        break;
    case 'right':
        flightParams.roll = inputSensitivity;
        break;
    case 'w':
        flightParams.altitude = inputSensitivity;
        break;
    case 's':
        flightParams.altitude = -inputSensitivity;
        break;
    case 'a':
        flightParams.yaw = -inputSensitivity;
        break;
    case 'd':
        flightParams.yaw = inputSensitivity;
        break;
    case 't':
        drone.takeoffOrLand();
        break;
    case 'f':
        drone.trim();
        break;
    case 'escape':
        drone.emergency();
        break;
    case 'c':
        process.exit();
        break;
    default:
        break;
    }

    drone.setFlightParams(flightParams);
    timeout = setTimeout(() => {
        drone.setFlightParams({
            yaw: 0,
            pitch: 0,
            roll: 0,
            altitude: 0,
        });
    }, 400);
});

process.stdin.setRawMode(true);
process.stdin.resume();
