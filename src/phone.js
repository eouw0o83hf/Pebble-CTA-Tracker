function _keysPrototype() {	
	this.RouteName = 0;
	this.StopName = 1;
	this.PredictedArrival = 2;
	this.KeyCount = 3;
}
var _keys = new _keysPrototype();

function _settingsPrototype() {
	this.UseRealData = true;
}
var _settings = new _settingsPrototype();


// Called on appMessage wireup
Pebble.addEventListener('ready', handleDataRequest);
						
// Called when an appMessage is received
Pebble.addEventListener('appmessage', handleDataRequest);

function handleDataRequest(e) {	
	console.log("AppMessage received in CTA tracker!");	
	console.log("Use real data: " + _settings.UseRealData);
	
	if(_settings.UseRealData) {
		console.log('using real data');
		handleDataReal(e);
	} else {
		console.log('using test data');
		handleDataTest(e);		
	}
}

function sendSuccessCallback(e) {
}
	
function sendErrorCallback(e) {
}

function handleDataTest(e) {
	var response = {};
	
	response[_keys.RouteName] = "77";
	response[_keys.StopName] = "Washtenaw";
	response[_keys.PredictedArrival] = "{thing}";
	
	Pebble.sendAppMessage(response, sendSuccessCallback, sendErrorCallback);
}

function handleDataReal(e) {
	var url = 'http://www.ctabustracker.com/bustime/api/v1/getpredictions?key=jPwQTMjHnxnjHqdHfBQD8aBFe&stpid=7866';
	
	console.log("Getting estimates");
	
	xhrRequest(url, 'GET', function(responseText) {		
		var response = {};
		var predictions = getElements(responseText, 'prd');
		for(var i = 0; i < predictions.length; ++i) {
			var offset = i * _keys.KeyCount;
			response[_keys.RouteName + offset] = getElement(predictions[i], 'rt');
			response[_keys.StopName + offset] = getElement(predictions[i], 'stpnm');
			response[_keys.PredictedArrival + offset] = getElement(predictions[i], 'prdtm');
		}		
		
		Pebble.sendAppMessage(response, sendSuccessCallback, sendErrorCallback);
	});
}

//********************************************************************
//	Helper/Library Methods

//	These do simple regex-based xml reading, with a couple of assumptions
//	which seem to be true of the CTA API:
//		1. No node type is a descendent of itself.
//		2. We only care about actual, textual body of a node; no attributes

/// Gets the body of all elements of the given tag, case-invariant
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

/// Gets the first element of the given tag, case-invariant
function getElement(data, tag) {
	return getElements(data, tag)[0];
}

/// Regex to pull matches of an xml tag, line- and case-invariant
function tagRegExp(tag) {
	return new RegExp("<" + tag + ">((.|[\r\n])*?)<\/" + tag + ">", "gmi");
}

/// XML HTTP Request helper
function xhrRequest(url, type, callback) {
	var xhr = new XMLHttpRequest();
	xhr.onload = function() {
		callback(this.responseText);
	};
	xhr.open(type, url);
	xhr.send();
}
//********************************************************************