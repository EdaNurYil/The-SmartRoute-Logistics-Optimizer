#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "mapping.h"
#include "boxDelivery.h"
#include "helpfulStructure.h"


#ifndef TESTING // Only include main if not in testing mode
int main(void)
{
    // struct Map baseMap = populateMap();
     //struct Route blueRoute = getBlueRoute();
     //struct Map routeMap = addRoute(&baseMap, &blueRoute);

     //printMap(&routeMap, 1, 1);

    // return 0;

    struct Map map = populateMap();  // Load map with buildings
    struct Route blueRoute = getBlueRoute();  // Blue route
    struct Route greenRoute = getGreenRoute();  // Green route
    struct Route yellowRoute = getYellowRoute();  // Yellow route
    struct Point destination;
    int weight, size;
    char dest[3];  // Assuming destination input like 12L
    // Add routes to the map for visualization
    map = addRoute(&map, &blueRoute);
    map = addRoute(&map, &greenRoute);
    map = addRoute(&map, &yellowRoute);
    struct Route shortestPathRoute = { { {0, 0} }, 0, 0 };


    struct Point startingPoint = { 0, 0 }; // Example starting point

    printf("=================\nSeneca Polytechnic Deliveries:\n=================\n");

    while (1) {
        printf("Enter shipment weight, box size and destination (0 0 x to stop): ");
        int result = scanf("%d %d %s", &weight, &size, dest);

        // Validate input
        if (!isValidWeight(weight)) {
            printf("Invalid weight (must be 1-2000 kg).\n");
            continue;
        }
        if (!isValidSize(size)) {
            printf("Invalid size (must be 0.5, 1 or 2 cubic meters).\n");
            continue;
        }



        if (!isValidDestination(dest, &map)) {
            printf("Invalid destination.\n");
            continue;
        }

        // Convert destination input (like "12L") to struct Point
        destination.row = dest[0] - '1';  // Convert '1'-based row to '0'-based index
        destination.col = dest[1] - 'A';  // Convert 'A'-based column to index

        // Find the shortest path
        shortestPathRoute = shortestPath(&map, startingPoint, destination);

        if (shortestPathRoute.numPoints == 0) {
            printf("No path to the destination at %s.\n", dest);
            continue;
        }

        // Add the points from the shortest path route to the route
        for (int i = 0; i < shortestPathRoute.numPoints; i++) {
            addPtToRoute(&shortestPathRoute, shortestPathRoute.points[i]);
        }

        // Determine the shipping route by checking each route's points
        const char* routeName = NULL;
        for (int i = 0; i < blueRoute.numPoints; i++) {
            if (eqPt(blueRoute.points[i], destination)) {
                routeName = "BLUE LINE";
                break;
            }
        }

        if (!routeName) {
            for (int i = 0; i < greenRoute.numPoints; i++) {
                if (eqPt(greenRoute.points[i], destination)) {
                    routeName = "GREEN LINE";
                    break;
                }
            }
        }

        if (!routeName) {
            for (int i = 0; i < yellowRoute.numPoints; i++) {
                if (eqPt(yellowRoute.points[i], destination)) {
                    routeName = "YELLOW LINE";
                    break;
                }
            }
        }

        // Skip if no route is found
        if (!routeName) {
            printf("Invalid destination route.\n");
            continue;
        }

        // Check for diversions
        if (shortestPathRoute.numPoints == 2) {
            printf("Ship on %s, no diversion\n", routeName);
        }
        else {
            printf("Ship on %s, divert:", routeName);
            for (int i = 1; i < shortestPathRoute.numPoints; i++) {
                printf(" %d%c", shortestPathRoute.points[i].row + 1, 'A' + shortestPathRoute.points[i].col);
                if (i < shortestPathRoute.numPoints - 1) {
                    printf(",");
                }
            }
            printf("\n");
        }
    }


  

    struct Map testMap = populateMap(); // Assume a 5x5 map
    struct Package diversions[25];
    int diversionCount = 0;

    //integrationTestFunction();
    generateDiversions(2, 2, &testMap, diversions, &diversionCount);





    return 0;
}

#endif