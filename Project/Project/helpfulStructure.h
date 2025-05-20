#pragma once
#ifndef MS2_NEW_DATA_STRUCTURE_H
#define MS2_NEW_DATA_STRUCTURE_H

// Constants for truck capacity and package sizes
#define MAX_WEIGHT_CAPACITY 2000      // Maximum weight a truck can carry in kilograms
#define MAX_VOLUME_CAPACITY 20        // Maximum volume a truck can carry in cubic meters
#define HALF_CUBIC_METER 0.5          // Box size in cubic meters
#define ONE_CUBIC_METER 1.0           // Box size in cubic meters
#define TWO_CUBIC_METERS 2.0          // Box size in cubic meters


// Structure representing a single package for delivery
struct Package {
    int weight;                       // Weight of the package in kilograms
    double size;                      // Size of the package in cubic meters (0.5, 1, or 2)
    int destinationRow;               // Row coordinate of the destination on the map
    int destinationCol;               // Column coordinate of the destination on the map
};

// Structure representing a shipment that contains multiple packages
struct Shipment {
    int shipmentID;
    int truckID;                      // ID of the truck associated with this shipment
    struct Package* packages;         // Dynamic array of packages in the shipment
    int numPackages;                  // Number of packages in the shipment
};

// Structure representing a delivery truck with capacity, route, and shipment details
struct Truck {
    int truckID;                       // Unique identifier for each truck
    int currentWeight;                 // Current total weight of the packages in the truck (kg)
    double currentVolume;              // Current total volume of the packages in the truck (cubic meters)
    int maxWeight;                     // Maximum weight the truck can carry (kg)
    double maxVolume;                  // Maximum volume the truck can carry (cubic meters)
    int routeSymbol;                   // Symbol representing the route assigned to the truck (BLUE, GREEN, YELLOW)
    int routePathRows;                // Array of row indices defining the route path
    int routePathCols;                // Array of column indices defining the route path
    int numRoutePoints;                // Number of points in the route path
    struct Shipment shipment;          // Shipment associated with this truck
};
 


#endif // MS2_NEW_DATA_STRUCTURE_H