function _keysPrototype() {
	this.ExampleHeader = 0;
	this.ExampleBody = 1;
}
var _keys = new _keysPrototype();

// Called on appMessage wireup
Pebble.addEventListener('ready', handleDataRequest);
						
// Called when an appMessage is received
Pebble.addEventListener('appmessage', handleDataRequest);

function handleDataRequest(e) {
	console.log("AppMessage request received");
	
	// Initialize dictionary to send back
	var data = {};
	data[_keys.ExampleHeader] = 'example header';
	data[_keys.ExampleBody] = 'example body';
	
	Pebble.sendAppMessage(data, sendSuccessCallback, sendErrorCallback);
	console.log("AppMessage request received");
}

function sendSuccessCallback(e) {
}
	
function sendErrorCallback(e) {
}