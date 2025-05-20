#pragma once
#ifndef BOX_DELIVERY_H
#define BOX_DELIVERY_H

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// Constants
#define MAX_DIVERSION_LOCATIONS 10  


// Function prototypes
int isValidWeight(int weight);
int isValidSize(float size);
int isValidDestination(const char* destination, const struct Map* map);
void calculateRemainingTruckCapacity(struct Truck* truck, int* remainingWeight, double* remainingVolume);
int canTruckHandleShipment(struct Truck* truck, struct Shipment* shipment);
void generateDiversions(int destinationRow, int destinationCol, const struct Map* map, struct Package diversions[], int* diversionCount);
int assignShipmentToTruck(struct Shipment* shipment, struct Truck* trucks[], int numTrucks, struct Map* map);
void manageShipment(struct Shipment* shipment, struct Truck* trucks[], int numTrucks, struct Map* map);



#endif // BOX_DELIVERY_H
