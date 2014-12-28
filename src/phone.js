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

function tagRegExp(tag) {
	return new RegExp("<" + tag + ">((.|[\r\n])*?)<\/" + tag + ">", "gmi");
}

function getElements(data, tag) {
	var groups = data.match(tagRegExp(tag));
	var output = [];
	for(var i = 0; i < groups.length; ++i) {
		var matches = tagRegExp(tag).exec(groups[i]);
		if(matches.length >= 2) {
			output.push(matches[1]);
		}
	}
	return output;
}

function getElement(data, tag) {
	return getElements(data, tag)[0];
}