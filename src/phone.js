function _keysPrototype() {
	this.RouteName = 0;
	this.Direction = 1;
	this.EstimatedArrivalTime = 2;
	this.StopName = 3;
	this.TotalFieldCount = 4;
}
var _keys = new _keysPrototype();

// Called on appMessage wireup
Pebble.addEventListener('ready', handleDataRequest);
						
// Called when an appMessage is received
Pebble.addEventListener('appmessage', handleDataRequest);

function handleDataRequest(e) {
	console.log("AppMessage request received");
	
	var data = getDummyData();
	
	Pebble.sendAppMessage(data, sendSuccessCallback, sendErrorCallback);
	console.log("AppMessage responded");
}

function sendSuccessCallback(e) {
}
	
function sendErrorCallback(e) {
}

function getDummyData() {
	var response = {};
	
	response[_keys.RouteName] = "77";
	response[_keys.Direction] = "East";
	response[_keys.EstimatedArrivalTime] = "11";
	response[_keys.StopName] = "Washtenaw";
	
	return response;
}