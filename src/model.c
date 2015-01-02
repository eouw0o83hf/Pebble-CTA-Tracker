#include <pebble.h>
#include <stdlib.h>
#include <string.h>
#include "model.h"

//**********************************************
//	Keys

#define KEY_ROUTE_NAME			0
#define KEY_STOP_NAME			1
#define KEY_PREDICTED_ARRIVAL	2
#define KEYS_BUS_DIRECTION		3
#define KEYS_COUNT 				4

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
		
	EstimatedArrival* response = malloc(128);
	response->PredictedTime = dict_find(iterator->Iterator, KEY_PREDICTED_ARRIVAL + offset)->value->cstring;
	response->StopName = dict_find(iterator->Iterator, KEY_STOP_NAME + offset)->value->cstring;
	response->RouteName = routeName->value->cstring;
	response->Direction = dict_find(iterator->Iterator, KEYS_BUS_DIRECTION + offset)->value->cstring;
	
	iterator->Current = response;
	iterator->Index++;
	
	return response;
}