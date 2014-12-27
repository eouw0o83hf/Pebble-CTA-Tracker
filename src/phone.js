function _keysPrototype() {
	this.RouteName = 0;
	this.StopName = 1;
	this.StopData = 2;
}
var _keys = new _keysPrototype();

// Called on appMessage wireup
Pebble.addEventListener('ready', handleDataRequest);
						
// Called when an appMessage is received
Pebble.addEventListener('appmessage', handleDataRequest);

function handleDataRequest(e) {	
	var data = getDummyData();	
	Pebble.sendAppMessage(data, sendSuccessCallback, sendErrorCallback);
}

function sendSuccessCallback(e) {
}
	
function sendErrorCallback(e) {
}

function getDummyData() {
	var response = {};
	
	response[_keys.RouteName] = "77";
	response[_keys.StopName] = "Washtenaw";
	response[_keys.StopData] = "{thing}";
	
	return response;
}