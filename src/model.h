#pragma once

typedef struct {
	char *RouteName;
	char *StopName;
	char *PredictedTime;
	char *Direction;
} EstimatedArrival;

typedef struct {
	DictionaryIterator *Iterator;
	int Index;
	EstimatedArrival *Current;
} EstimatedArrivalIterator;

EstimatedArrivalIterator* get_estimated_arrival_iterator(DictionaryIterator *iterator); 
EstimatedArrival* get_next_estimated_arrival(EstimatedArrivalIterator *iterator);