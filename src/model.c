#include <pebble.h>
#include <stdlib.h>
#include <string.h>
#include "model.h"


TransitStop* transit_stop_parse(char *routeName, char *stopName) {
	
	TransitStop* response = malloc(128);
	
	response->RouteName = routeName;
	response->StopName = stopName;
	
	return response;
}

EstimatedArrivalIterator* get_estimated_arrival_iterator(char *arrivalInfo) {
	EstimatedArrivalIterator *response = malloc(sizeof(arrivalInfo) + sizeof(int));
	
	response->ArrivalInfo = arrivalInfo;
	response->Index = 0;
	
	return response;
}

EstimatedArrival* get_next_estimated_arrival(EstimatedArrivalIterator *iterator) {
	if(iterator->Index >= 2) {
		return NULL;
	}
	
	iterator->Index++;
	
	EstimatedArrival* response = malloc(128);
	response->Time = iterator->Index;
	response->VehicleId = "Random";
	response->Direction = "East";
	return response;
}