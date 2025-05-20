#include "boxDelivery.h"
#include "helpfulStructure.h"
#include "mapping.h"

// Function to validate weight
int isValidWeight(int weight) {

    //White Box Test Case
    printf("Checking if weight %d is valid.\n", weight);
    int isValid = (weight >= 1 && weight <= MAX_WEIGHT_CAPACITY);
    printf("Weight validity: %s\n", isValid ? "Valid" : "Invalid");

    return isValid;
}

// Function to validate size
int isValidSize(float size) {

    //White Box Test Case 
    printf("Checking if size %.2f is valid.\n", size);
    int isValid = (size == HALF_CUBIC_METER || size == ONE_CUBIC_METER || size == TWO_CUBIC_METERS);
    printf("Size validity: %s\n", isValid ? "Valid" : "Invalid");

    return isValid;
}

// Function to check if a destination is valid based on rows and columns
int isValidDestination(const char* destination, const struct Map* map) {
    // Validate the input format (e.g., "12A")
    int len = strlen(destination);
    if (len < 2 || len > 3) {
        return 0;  // Invalid destination length
    }

    // Extract the numeric part (row index)
    int row = 0;
    int i = 0;
    while (i < len - 1 && isdigit(destination[i])) {
        row = row * 10 + (destination[i] - '0');
        i++;
    }

    // Validate row index
    if (row < 0 || row >= getNumRows(map)) {
        return 0;  // Row is out of bounds
    }

    // Extract the column part (letter)
    char col = destination[i];
    if (!isalpha(col) || toupper(col) - 'A' >= getNumCols(map)) {
        return 0;  // Column is invalid or out of bounds
    }

    return 1;  // Destination is valid
}


void calculateRemainingTruckCapacity(struct Truck* truck, int* remainingWeight, double* remainingVolume) {
    *remainingWeight = truck->maxWeight - truck->currentWeight;
    *remainingVolume = truck->maxVolume - truck->currentVolume;

    if (*remainingWeight < 0) *remainingWeight = 0;
    if (*remainingVolume < 0.0) *remainingVolume = 0.0;
}

// Function to determine if a truck can handle a shipment
int canTruckHandleShipment(struct Truck* truck, struct Shipment* shipment) {
    int totalWeight = truck->currentWeight;
    double totalVolume = truck->currentVolume;

    for (int i = 0; i < shipment->numPackages; i++) {
        totalWeight += shipment->packages[i].weight;
        totalVolume += shipment->packages[i].size;
    }

    return totalWeight <= truck->maxWeight && totalVolume <= truck->maxVolume;
}

// Function to generate diversions
void generateDiversions(int destinationRow, int destinationCol, const struct Map* map, struct Package diversions[], int* diversionCount) {
    int numRows = getNumRows(map);
    int numCols = getNumCols(map);

    *diversionCount = 0;
    for (int rowOffset = -2; rowOffset <= 2; rowOffset++) {
        for (int colOffset = -2; colOffset <= 2; colOffset++) {
            int newRow = destinationRow + rowOffset;
            int newCol = destinationCol + colOffset;

            if (newRow >= 0 && newRow < numRows && newCol >= 0 && newCol < numCols) {
                struct Package diversion = { .weight = 0, .size = 0.0, .destinationRow = newRow, .destinationCol = newCol };
                if (isValidDestination(&diversion, map)) {
                    diversions[*diversionCount] = diversion;
                    (*diversionCount)++;
                }
            }
        }
    }
}

// Function to assign a shipment to a truck
int assignShipmentToTruck(struct Shipment* shipment, struct Truck* trucks[], int numTrucks, struct Map* map) {
    struct Truck* selectedTruck = NULL;
    double minDistance = INT_MAX;

    struct Package diversions[20];
    int diversionCount = 0;
    generateDiversions(shipment->packages[0].destinationRow, shipment->packages[0].destinationCol, map, diversions, &diversionCount);

    for (int i = 0; i < numTrucks; i++) {
        struct Truck* truck = trucks[i];
        if (!canTruckHandleShipment(truck, shipment)) continue;

        for (int j = 0; j < diversionCount; j++) {
            struct Point diversionPoint = { diversions[j].destinationRow, diversions[j].destinationCol };
            struct Point truckPoint = { truck->routePathRows, truck->routePathCols };

            struct Route route = shortestPath(map, truckPoint, diversionPoint);
            double routeDistance = distance(&route.points[route.numPoints - 1], &diversionPoint);

            if (routeDistance < minDistance) {
                selectedTruck = truck;
                minDistance = routeDistance;
            }
        }
    }

    if (selectedTruck) {
        selectedTruck->shipment = *shipment;
        for (int k = 0; k < shipment->numPackages; k++) {
            selectedTruck->currentWeight += shipment->packages[k].weight;
            selectedTruck->currentVolume += shipment->packages[k].size;
        }

        shipment->truckID = selectedTruck->truckID;
        printf("Shipment %d assigned to Truck %d.\n", shipment->shipmentID, selectedTruck->truckID);
        return selectedTruck->truckID;
    }
    else {
        printf("Failed to assign Shipment %d. No suitable truck.\n", shipment->shipmentID);
        return -1;
    }
}

// Manage shipment assignment and diversions
void manageShipment(struct Shipment* shipment, struct Truck* trucks[], int numTrucks, struct Map* map) {
    int truckID = assignShipmentToTruck(shipment, trucks, numTrucks, map);

    if (truckID == -1) {
        printf("No trucks available for Shipment %d after considering diversions.\n", shipment->shipmentID);
    }
}

