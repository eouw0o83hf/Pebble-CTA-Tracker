#include <pebble.h>
#include <stdlib.h>
#include <string.h>
#include "model.h"

//**********************************************
//	Keys

#define KEY_ROUTE_NAME			0
#define KEY_STOP_NAME			1
#define KEY_PREDICTED_ARRIVAL	2
#define KEYS_COUNT 				3

EstimatedArrivalIterator* get_estimated_arrival_iterator(DictionaryIterator *iterator) {
	EstimatedArrivalIterator* response = malloc(16);
	
	response->Iterator = iterator;
	response->Index = 0;
	
	return response;
}

EstimatedArrival* get_next_estimated_arrival(EstimatedArrivalIterator *iterator) {
	int offset = iterator->Index * KEYS_COUNT;
	
	Tuple *routeName = dict_find(iterator->Iterator, KEY_ROUTE_NAME + offset);
	if(routeName == NULL) {
		return NULL;
	}
	
	if(iterator->Current != NULL) {
		free(iterator->Current);
	}
	
	Tuple *stopName = dict_find(iterator->Iterator, KEY_STOP_NAME + offset);	
	Tuple *arrival = dict_find(iterator->Iterator, KEY_PREDICTED_ARRIVAL + offset);
		
	EstimatedArrival* response = malloc(128);
	response->PredictedTime = arrival->value->cstring;
	response->StopName = stopName->value->cstring;
	response->RouteName = routeName->value->cstring;
	
	iterator->Current = response;
	iterator->Index++;
	
	return response;
}