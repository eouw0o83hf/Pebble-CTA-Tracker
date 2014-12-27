#pragma once

typedef struct TransitStop {
	char *RouteName;
	char *StopName;
} TransitStop;

typedef struct {
	int Time;
	char *VehicleId;
	char *Direction;
} EstimatedArrival;

typedef struct {
	char *ArrivalInfo;
	int Index;
} EstimatedArrivalIterator;

TransitStop* transit_stop_parse(char *routeName, char *stopName);
EstimatedArrivalIterator* get_estimated_arrival_iterator(char *arrivalInfo); 
EstimatedArrival* get_next_estimated_arrival(EstimatedArrivalIterator *iterator);