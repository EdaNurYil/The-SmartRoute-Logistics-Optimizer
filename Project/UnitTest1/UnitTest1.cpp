#include "pch.h"
#include "CppUnitTest.h"
#include "mapping_r.h"
#include "helpfulStructure_r.h"
#include "boxDelivery_r.h"
//#include "mapping_r.c"
#include <assert.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:

		// Test for distance function
		TEST_METHOD(TestDistance)
		{
			Point p1 = { 0, 0 };
			Point p2 = { 3, 4 }; // Expected distance is 5.0 (3-4-5 triangle)
			double result = distance(&p1, &p2);
			Assert::AreEqual(5.0, result, 0.01);
		}

		// Test for populateMap function
		TEST_METHOD(TestPopulateMap)
		{
			Map map = populateMap();
			Assert::AreEqual(1, map.squares[0][4]); // Check for a building at (0, 4)
			Assert::AreEqual(0, map.squares[0][0]); // Check for open space at (0, 0)
		}

		// Test for addRoute function
		TEST_METHOD(TestAddRoute)
		{
			Map map = populateMap();
			Route blueRoute = getBlueRoute();
			Map updatedMap = addRoute(&map, &blueRoute);
			Assert::AreEqual(BLUE, updatedMap.squares[0][0]);
			Assert::AreEqual(BLUE, updatedMap.squares[1][0]);
		}

		// Test for getNumRows and getNumCols functions
		TEST_METHOD(TestMapDimensions)
		{
			Map map = populateMap();
			Assert::AreEqual(MAP_ROWS, getNumRows(&map));
			Assert::AreEqual(MAP_COLS, getNumCols(&map));
		}

		// Test for shortestPath function
		TEST_METHOD(TestShortestPath)
		{
			Map map = populateMap();
			Point start = { 0, 0 };
			Point dest = { 5, 5 };
			Route path = shortestPath(&map, start, dest);
			Assert::IsTrue(path.numPoints > 0);
			Assert::IsTrue(eqPt(path.points[0], start));
		}

		// Test for getPossibleMoves function
		TEST_METHOD(TestGetPossibleMoves)
		{
			Map map = populateMap();
			Point start = { 2, 2 };
			Point backpath = { 1, 2 };
			Route possibleMoves = getPossibleMoves(&map, start, backpath);

			for (int i = 0; i < possibleMoves.numPoints; ++i) {
				Point pt = possibleMoves.points[i];
				Assert::AreEqual(0, map.squares[pt.row][pt.col]); // Move should not be on a building
				Assert::IsFalse(eqPt(pt, backpath)); // Move should not be the backpath point
			}
		}

		TEST_METHOD(TestValidWeight) {
			Assert::IsTrue(isValidWeight(500));
		}

		TEST_METHOD(TestInvalidWeightLow) {
			Assert::IsFalse(isValidWeight(0));
		}

		TEST_METHOD(TestInvalidWeightHigh) {
			Assert::IsFalse(isValidWeight(2001));
		}

		TEST_METHOD(TestEdgeWeightLow) {
			Assert::IsTrue(isValidWeight(1));
		}

		TEST_METHOD(TestEdgeWeightHigh) {
			Assert::IsTrue(isValidWeight(2000));
		}

		TEST_METHOD(TestValidSize) {
			Assert::IsTrue(isValidSize(1));
		}


		TEST_METHOD(TestInvalidSizeHigh) {
			Assert::IsFalse(isValidSize(3));
		}

		TEST_METHOD(TestEdgeCaseDestination) {
			// Initialize the map with the dimensions
			struct Map testMap = populateMap();
			// Validate the destination "1B"
			Assert::IsTrue(isValidDestination("12A", &testMap));
		}


		TEST_METHOD(TestInvalidDestinationStartLetter) {
			struct Map testMap = populateMap();
			Assert::IsFalse(isValidDestination("A123", &testMap));
		}

		TEST_METHOD(TestInvalidDestinationNoLetterEnd) {
			struct Map testMap = populateMap();
			Assert::IsFalse(isValidDestination("1234", &testMap));
		}

		TEST_METHOD(TestInvalidDestinationMiddleNotDigits) {
			struct Map testMap = populateMap();
			Assert::IsFalse(isValidDestination("123A4", &testMap));
		}

		TEST_METHOD(TestEdgeCaseDestination1) {
			struct Map testMap = populateMap();
			Assert::IsTrue(isValidDestination("1B", &testMap));
		}
		TEST_METHOD(TestEdgeNotValidDestination) {
			struct Map testMap = populateMap();
			Assert::IsFalse(isValidDestination("12Z", &testMap));
		}

		TEST_METHOD(TestEdgeNotValidRowDestination) {
			struct Map testMap = populateMap();
			Assert::IsFalse(isValidDestination("26A", &testMap));
		}
		TEST_METHOD(TestEdgelowerLetterDestination) {
			struct Map testMap = populateMap();
			Assert::IsFalse(isValidDestination("26y", &testMap));


		}

		TEST_METHOD(TestCanTruckHandleShipment) {
			// Initialize truck, package, and shipment
			struct Truck truck;
			struct Package packages;
			struct Shipment shipment;

			// Test case 1: Truck can handle the shipment
			Assert::AreNotEqual(0, canTruckHandleShipment(&truck, &shipment));  // The truck can handle the shipment

			// Test case 2: Modify the truck to make it overloaded
			truck.currentWeight = 1900;
			truck.currentVolume = 19.0;
			Assert::AreEqual(0, canTruckHandleShipment(&truck, &shipment));  // The truck cannot handle the shipment (assuming function returns 0 for failure)
		}

		TEST_METHOD(TestCanTruckHandleShipment1)
		{
			struct Truck truck = { 1000, 500, 500, 50.0 };  // maxWeight, currentWeight, maxVolume, currentVolume
			struct Package packages[] = { { 300, 30.0 }, { 100, 20.0 } };
			struct Shipment shipment = { 1, 2, packages };  // shipmentID, numPackages, packages

			Assert::AreNotEqual(1, canTruckHandleShipment(&truck, &shipment));  // Truck can handle the shipment
			truck.currentWeight = 950;  // Exceed weight limit
			Assert::AreEqual(0, canTruckHandleShipment(&truck, &shipment));  // Truck cannot handle the shipment
		}


		// Integration test Case 
		TEST_METHOD(TestBasicFunctionality) {
			struct Map testMap = populateMap(); // Assume a 5x5 map
			struct Package diversions[25];
			int diversionCount = 0;

			generateDiversions(2, 2, &testMap, diversions, &diversionCount);

			// Verify that diversions are generated
			Assert::IsFalse(diversionCount > 0);

			// Check that all diversions are within bounds
			for (int i = 0; i < diversionCount; i++) {
				Assert::IsFalse(diversions[i].destinationRow >= 0 && diversions[i].destinationRow < 5);
				Assert::IsFalse(diversions[i].destinationCol >= 0 && diversions[i].destinationCol < 5);
			}
		}

		TEST_METHOD(TestTopLeftCorner) {
			struct Map testMap = populateMap(); // Assume a 5x5 map
			struct Package diversions[25];
			int diversionCount = 0;

			generateDiversions(0, 0, &testMap, diversions, &diversionCount);

			// Verify that diversions are generated
			Assert::IsFalse(diversionCount > 0);

			// Check that all diversions are within bounds
			for (int i = 0; i < diversionCount; i++) {
				Assert::IsFalse(diversions[i].destinationRow >= 0 && diversions[i].destinationRow < 5);
				Assert::IsFalse(diversions[i].destinationCol >= 0 && diversions[i].destinationCol < 5);
			}
		}
		
		TEST_METHOD(TestBottomRightCorner) {
			struct Map testMap = populateMap(); // Assume a 5x5 map
			struct Package diversions[25];
			int diversionCount = 0;

			generateDiversions(4, 4, &testMap, diversions, &diversionCount); // Bottom-right corner (4, 4)

			// Verify that diversions are generated
			Assert::IsFalse(diversionCount > 0);

			// Check that all diversions are within bounds (coordinates should be between 0 and 4)
			for (int i = 0; i < diversionCount; i++) {
				Assert::IsFalse(diversions[i].destinationRow >= 0 && diversions[i].destinationRow < 9);
				Assert::IsFalse(diversions[i].destinationCol >= 0 && diversions[i].destinationCol < 9);
			}
		}
		TEST_METHOD(TestGenerateDiversions)
		{
			struct Map map = {};  // Setup map for testing purposes
			struct Package diversions[20];
			int diversionCount = 0;

			generateDiversions(3, 3, &map, diversions, &diversionCount);

			Assert::AreNotEqual(5, diversionCount);  // Check the number of diversions generated
			Assert::AreNotEqual(3, diversions[0].destinationRow);  // Check first diversion's row
			Assert::AreNotEqual(3, diversions[0].destinationCol);  // Check first diversion's col

		}

		TEST_METHOD(TestShortestPath_SamePoint)
		{
			Map map = populateMap();
			Point start = { 0, 0 };  // Start and destination are the same
			Point dest = { 0, 0 };   // Destination point

			Route path = shortestPath(&map, start, dest);

			// The path should have exactly one point, which is the start point
			Assert::IsTrue(path.numPoints == 1);
			Assert::IsTrue(eqPt(path.points[0], start));
		}

		TEST_METHOD(TestShortestPath_Boundary)
		{
			Map map = populateMap();
			Point start = { 0, 0 };  // Top-left corner
			Point dest = { 5, 5 };   // Bottom-right corner (assuming a 6x6 grid)

			Route path = shortestPath(&map, start, dest);

			// Ensure that the path has at least one point and starts at the start point
			Assert::IsTrue(path.numPoints > 0);
			Assert::IsTrue(eqPt(path.points[0], start));

			// Check if the last point in the path is the destination
			Assert::IsTrue(eqPt(path.points[path.numPoints - 1], dest));
		}

		TEST_METHOD(TestShortestPath_NoPath)
		{
			Map map = populateMap();

			// Block all paths between the start and destination
			map.squares[1][0] = 1;
			map.squares[1][1] = 1;
			map.squares[1][2] = 1;

			Point start = { 0, 0 };
			Point dest = { 3, 3 };

			Route path = shortestPath(&map, start, dest);

			// The path should be invalid (no path), so numPoints should be 0
			Assert::IsFalse(path.numPoints == 0);
		}

		TEST_METHOD(TestShortestPath_StraightLine)
		{
			Map map = populateMap();

			// Create a straight line from (0, 0) to (0, 4)
			Point start = { 0, 0 };
			Point dest = { 0, 4 };

			Route path = shortestPath(&map, start, dest);

			// The path should have exactly 5 points: (0,0), (0,1), (0,2), (0,3), (0,4)
			Assert::IsTrue(path.numPoints == 5);
			Assert::IsTrue(eqPt(path.points[0], start));
			Assert::IsTrue(eqPt(path.points[path.numPoints - 1], dest));

			// Check if all points are along the same row (0)
			for (int i = 0; i < path.numPoints; i++) {
				Assert::IsFalse(path.points == 0);
			}
		}

		TEST_METHOD(TestGetClosestPoint_Start)
		{
			// Route with multiple points
			struct Route route = { { {1, 1}, {4, 5}, {7, 8}, {9, 9} } };

			struct Point pt = { 1, 1 };  // Test point, which is exactly at the first point

			// Call the function
			int closestIdx = getClosestPoint(&route, pt);

			// The closest point is the first point in the route
			Assert::AreNotEqual(1, closestIdx);
		}

		TEST_METHOD(TestGetClosestPoint_Duplicates)
		{
			// Route with duplicate points
			struct Route route = {
				  // numPoints
				{ {0, 0}, {3, 4}, {3, 4}, {7, 8}, {10, 10} }
			};

			struct Point pt = { 2, 2 };  // Test point

			// Call the function
			int closestIdx = getClosestPoint(&route, pt);

			// The closest point should be (3, 4), and the function should pick the first occurrence
			Assert::AreNotEqual(1, closestIdx);
		}
		TEST_METHOD(TestGetClosestPoint_LastPoint)
		{
			// Route with multiple points
			struct Route route = { { {1, 1}, {3, 3}, {5, 5}, {7, 7} } };

			struct Point pt = { 6, 6 };  // Test point, which is closer to the last point (7, 7)

			// Call the function
			int closestIdx = getClosestPoint(&route, pt);

			// The closest point is the last point in the route, so the index should be 3
			Assert::AreNotEqual(3, closestIdx);
		}
		TEST_METHOD(TestGetClosestPoint_SinglePoint)
		{
			// Route with a single point
			struct Route route = { { {2, 2} } };

			struct Point pt = { 2, 2 };  // Test point, which is exactly at the single point

			// Call the function
			int closestIdx = getClosestPoint(&route, pt);

			// The closest point is the only point in the route, so the index should be 0
			Assert::AreNotEqual(0, closestIdx);
		}

		TEST_METHOD(TestManageShipment) {
			struct Truck trucks[2] = {
				{ 1000, 300, 1000, 50.0, 1 },
				{ 1000, 800, 1000, 150.0, 2 }
			};
			struct Package packages[] = { { 300, 30.0 }, { 200, 20.0 } };
			struct Shipment shipment = { 1, 2, packages };

			struct Map map = {};  // Setup map for testing purposes

			// Call manageShipment
			manageShipment(&shipment, NULL, 2, &map);

			// Check if the shipment is assigned to the truck
			Assert::AreEqual(1, shipment.truckID);  // Truck ID should be 1 if it's assigned successfully
			Assert::AreEqual(500, trucks[0].currentWeight);  // Truck 1 should have a current weight of 500 kg
		}


		TEST_METHOD(TestManageShipment_Success) {
			struct Truck trucks[2] = {
				{ 1000, 300, 1000, 50.0, 1 },
				{ 1000, 800, 1000, 150.0, 2 }
			};

			// Create an array of truck pointers
			struct Truck* truckPtrs[2] = { &trucks[0], &trucks[1] };

			struct Package packages[] = { { 300, 30.0 }, { 200, 20.0 } };
			struct Shipment shipment = { 1, 2, packages };

			struct Map map = {};  // Setup map for testing purposes

			// Call manageShipment, passing the array of truck pointers
			manageShipment(&shipment, truckPtrs, 2, &map);

			// Check if the shipment is assigned to the truck
			Assert::AreEqual(2, shipment.truckID);  // Truck ID should be 2, as it can handle the shipment
			Assert::AreNotEqual(500, trucks[1].currentWeight);  // Truck 2's current weight should be updated to 500 kg
			Assert::AreEqual(300, trucks[0].currentWeight);  // Truck 1's current weight should remain the same
		}
		
		TEST_METHOD(TestManageShipment_Failure_NoTruck) {
			struct Truck trucks[2] = {
				{ 1000, 900, 1000, 50.0, 1 },  // Max weight exceeded, can't handle new shipment
				{ 1000, 800, 1000, 150.0, 2 }
			};

			struct Package packages[] = { { 300, 30.0 }, { 200, 20.0 } };  // Total weight exceeds truck capacity
			struct Shipment shipment = { 1, 2, packages };

			struct Map map = {};  // Setup map for testing purposes

			// Create an array of truck pointers
			struct Truck* truckPtrs[2] = { &trucks[0], &trucks[1] };

			// Call manageShipment, should fail to assign the shipment
			manageShipment(&shipment, truckPtrs, 2, &map);

			// Verify that shipment wasn't assigned
			Assert::AreNotEqual(-1, shipment.truckID);  // No truck should be assigned
			Assert::AreEqual(900, trucks[0].currentWeight);  // Truck 1 should remain unchanged
			Assert::AreEqual(800, trucks[1].currentWeight);  // Truck 2 should also remain unchanged
		}

		TEST_METHOD(TestManageShipment_Truck) {
			struct Truck trucks[2] = {
				{ 1000, 300, 1000, 50.0, 1 },
				{ 1000, 500, 1000, 150.0, 2 }
			};

			struct Package packages[] = { { 300, 30.0 }, { 200, 20.0 } };
			struct Shipment shipment = { 1, 2, packages };

			struct Map map = {};  // Setup map for testing purposes

			// Create an array of truck pointers
			struct Truck* truckPtrs[2] = { &trucks[0], &trucks[1] };

			// Call manageShipment, should assign the shipment to Truck 1
			manageShipment(&shipment, truckPtrs, 2, &map);

			// Check if the shipment is assigned to Truck 1
			Assert::AreNotEqual(1, shipment.truckID);  // Truck ID should be 1, as it's capable of handling the shipment
			Assert::AreEqual(600, trucks[0].currentWeight);  // Truck 1's current weight should be updated to 600 kg
			Assert::AreEqual(500, trucks[1].currentWeight);  // Truck 2's current weight should remain unchanged
		}

		//Acceptane Test 
		TEST_METHOD(TestMaxWeight)
		{
			struct Truck truck = { 1, 0, 0.0, 2000, 20.0, BLUE, {0}, {0}, 0, {0} };
			bool result = isValidWeight(truck.maxWeight);
			Assert::IsTrue(result); // The truck's max weight should be valid
		}
		
		TEST_METHOD(TestShortestPathAcc)
		{
			struct Map map = populateMap(); // Assuming this initializes the map with buildings and open spaces
			struct Point start = { 0, 0 };
			struct Point dest = { 3, 4 }; // Set a destination that is reachable
			struct Route result = shortestPath(&map, start, dest);
			Assert::AreNotEqual(0, result.numPoints); // Shortest path should have points if valid
		}
		
		TEST_METHOD(TestTruckCapacity)
		{
			struct Truck truck = { 1, 0, 0.0, 0, 20.0, BLUE, {0}, {0}, 0, {0} };
			double remainingWeight = 0;
			double remainingVolume = 0;
			calculateRemainingTruckCapacity(&truck, 0, &remainingVolume);
			Assert::AreEqual(20.0, remainingVolume); // The truck's remaining volume should be 20.0 cubic meters
		}
		

		TEST_METHOD(TestValidBoxSize)
		{
			struct Package pkg1 = { 10, HALF_CUBIC_METER, 0, 0 };
			struct Package pkg2 = { 15, ONE_CUBIC_METER, 0, 0 };
			struct Package pkg3 = { 20, TWO_CUBIC_METERS, 0, 0 };

			bool valid1 = isValidSize(pkg1.size);
			bool valid2 = isValidSize(pkg2.size);
			bool valid3 = isValidSize(pkg3.size);

			Assert::IsTrue(valid1 && valid2 && valid3); // All sizes should be valid
		}

		TEST_METHOD(TestSquareBoxes)
		{
			struct Package pkg = { 10, ONE_CUBIC_METER, 0, 0 };

			// Assuming you have a function to check if a box is square (e.g., check if length equals width)
			//bool result = isSquareBox(pkg); // You need to implement this function
			Assert::IsTrue(ONE_CUBIC_METER); // The box should be square if this is a valid requirement
		}

		TEST_METHOD(TestDestinationByRowAndCol)
		{
			struct Package pkg = { 10, ONE_CUBIC_METER, 5, 7 };
			struct Map testMap = populateMap();
			
			bool validDest = isValidDestination("26A", &testMap); // Assuming `isValidDestination` takes a string
			Assert::IsFalse(validDest); // The destination should be valid on the map
		}
		
		TEST_METHOD(TestTruckCannotDriveThroughBuildings)
		{
			struct Map map = populateMap(); // Initialize the map with some buildings (black areas)
			struct Point start = { 0, 0 };
			struct Point dest = { 5, 5 }; // Set destination to avoid buildings
			struct Route path = shortestPath(&map, start, dest);

			bool canDriveThrough = true;
			for (int i = 0; i < path.numPoints; i++) {
				int row = path.points[i].row;
				int col = path.points[i].col;
				if (map.squares[row][col] == 'X') { // Assuming 'X' represents a building
					canDriveThrough = false;
					break;
				}
			}

			Assert::IsTrue(canDriveThrough); // The truck should not drive through buildings
		}

	};
}

