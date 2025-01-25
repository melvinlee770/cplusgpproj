#include "VehicleDetails.h"
#include <iostream>
#include <fstream>
#include <cstdio>

void VehicleDetails::PrePareVehicle(const std::string &srcFileName, bool fileNeedsDecryption, bool randomizeStartPosition, int missionType) {
	
	vehicleData = satComRelay.initializeVehicle(
		srcFileName, fileNeedsDecryption, randomizeStartPosition, missionType
	);
	vehicleData = satComRelay.allocateEnergyToShield(80000);
	/*
	std::cout << std::endl;
	std::cout << "##VEHICLE STATUS##" << std::endl;
    std::cout << "Initial Energy: " << vehicleData.getInitialEnergy() << std::endl;
    std::cout << "Current Energy: " << vehicleData.getCurrentEnergy() << std::endl;
    std::cout << "Current Shield Energy: " << vehicleData.getCurrentShieldEnergy() << std::endl;
  	*/
  	
}

void VehicleDetails::CreateAndDisplayMap(int horizontal, int vertical) {

	std::cout << std::endl;
	std::cout << "##VEHICLE STATUS##" << std::endl;
    std::cout << "Initial Energy: " << vehicleData.getInitialEnergy() << std::endl;
    std::cout << "Current Energy: " << vehicleData.getCurrentEnergy() << std::endl;
    std::cout << "Current Shield Energy: " << vehicleData.getCurrentShieldEnergy() << std::endl;
  	
	premap = std::vector<std::vector<char>>(vertical, std::vector<char>(horizontal, '.')); 

    // Calculate the center position
    int centerX = horizontal / 2;
    int centerY = vertical / 2;
    thisMapStartX = centerX;
    thisMapStartY = centerY;

    // Mark the initial position
    premap[centerY][centerX] = 'S';
    CalibrateHorizontal(thisMapStartX, thisMapStartY);
    GetTotalHorizontal(thisMapStartX, thisMapStartY);
    CalibrateVertical(thisMapStartX, thisMapStartY);
    GetTotalVertical(thisMapStartX, thisMapStartY);
    premap[thisMapStartY][thisMapStartX] = '_';
    // Print the premap
    
    for (const auto& row : premap) {
        for (const auto& cell : row) {
            std::cout << " " << cell << " ";
        }
        std::cout << std::endl;
    }
   
    std::cout<< "my Y location:  " << thisMapStartY << std::endl;
    std::cout << "my X location: " <<  thisMapStartX << std::endl;
   	std::cout << "total horizontal: " << totalHorizontal << std::endl; // include the border (both)
   	std::cout << "total vertical: " << totalVertical << std::endl; // include the border (both)
   	std::cout << "Current Shield Energy: " << vehicleData.getCurrentShieldEnergy() << std::endl;
}


void VehicleDetails::AutoMapping(int horizontal, int vertical) {
	autoscanmap = std::vector<std::vector<char>>(vertical, std::vector<char>(horizontal, '.')); 
	CornerCheck();
	autoscanmap[14][24] = '_'; // starting point
	
	
    for (const auto& row : autoscanmap) {
        for (const auto& cell : row) {
            std::cout << " " << cell << " ";
        }
        std::cout << std::endl;
    }
}


void VehicleDetails::CalibrateHorizontal(int centerX, int centerY) {	// use the method of "MEMBER FUNCTION"
    
    // hide the output to the terminal [START]
    std::streambuf* oldCout = std::cout.rdbuf();
    std::ofstream nullStream("/dev/null"); 
    std::cout.rdbuf(nullStream.rdbuf());
    
    FILE* originalStdout = stdout;
    stdout = fopen("/dev/null", "w"); 
    
    for (;;) {
        char scanResult = satComRelay.scanWest(vehicleData);

        if (scanResult != '.') {
            premap[centerY][centerX - 1] = scanResult; 
            std::cout << "Scanned Result: " << scanResult << std::endl;
        }

        if (scanResult == '#') {
            std::cout << "Stopping as '#' is encountered!" << std::endl;
            premap[centerY][centerX - 1] = scanResult;
            break; // Exit 
        }

        // Move the vehicle west (.moveLeftWest will not update ur location, need add another line)
        satComRelay.moveLeftWest();
        std::cout << "Current Shield Energy: " << vehicleData.getCurrentShieldEnergy() << std::endl;
        centerX -= 1;

        // Avoid run to the boundary of premap array
        if (centerX <= 0) {
            std::cout << "Vehicle reached the premap boundary!" << std::endl;
            break;
        }
    }
    
    // hide the output to the terminal [FINSIH]
    std::cout.rdbuf(oldCout);	// code turn output on 
    fclose(stdout);				// code turn output on
    stdout = originalStdout;	// code turn output on
    
    //premap[centerY][centerX] = '_';
    //std::cout << "my location: " << centerX << std::endl;
    thisMapStartX = centerX;
	
}

void VehicleDetails::GetTotalHorizontal(int centerX, int centerY){
	
	int tmpHorizontal = 1; // start with 1 because need to cout the "#" at the left side
	
	// hide the output to the terminal [START]
    std::streambuf* oldCout = std::cout.rdbuf();
    std::ofstream nullStream("/dev/null"); 
    std::cout.rdbuf(nullStream.rdbuf());
    
    
    FILE* originalStdout = stdout;
    stdout = fopen("/dev/null", "w"); 
   
    for (;;) {
        char scanResult = satComRelay.scanEast(vehicleData);

        if (scanResult != '.') {
            premap[centerY][centerX + 1] = scanResult; 
            std::cout << "Scanned Result: " << scanResult << std::endl;
            tmpHorizontal++;
            std::cout << "testing: " << tmpHorizontal << std::endl;
        }

        if (scanResult == '#') {
            std::cout << "Stopping as '#' is encountered!" << std::endl;
            premap[centerY][centerX + 1] = scanResult;
            tmpHorizontal++;
            std::cout << "testing: " << tmpHorizontal << std::endl;
            break; // Exit 
        }

        // Move the vehicle west (.moveLeftWest will not update ur location, need add another line)
        satComRelay.moveRightEast();
        std::cout << "Current Shield Energy: " << vehicleData.getCurrentShieldEnergy() << std::endl;
        centerX += 1;

        // Avoid run to the boundary of premap array
        if (centerX <= 0) {
            std::cout << "Vehicle reached the premap boundary!" << std::endl;
            break;
        }
    }

    totalHorizontal = tmpHorizontal;
    // hide the output to the terminal [FINSIH]
    std::cout.rdbuf(oldCout);	// code turn output on 
    fclose(stdout);				// code turn output on
    stdout = originalStdout;	// code turn output on
    
   	//premap[centerY][centerX] = '_';
   	//std::cout << "tmp horizontal: " << tmpHorizontal << std::endl; // include the border
   	thisMapStartX = centerX;
}

void VehicleDetails::CalibrateVertical(int centerX, int centerY) {
    
    
    // hide the output to the terminal [START]
    std::streambuf* oldCout = std::cout.rdbuf();
    std::ofstream nullStream("/dev/null"); 
    std::cout.rdbuf(nullStream.rdbuf());
    
    FILE* originalStdout = stdout;
    stdout = fopen("/dev/null", "w"); 
    
    
    for (;;) {
        char scanResult = satComRelay.scanNorth(vehicleData);

        if (scanResult != '.') {
            premap[centerY - 1][centerX] = scanResult; 
            std::cout << "Scanned Result: " << scanResult << std::endl;
        }

        if (scanResult == '#') {
            std::cout << "Stopping as '#' is encountered!" << std::endl;
            premap[centerY - 1][centerX] = scanResult;
            break; // Exit 
        }

        // Move the vehicle west (.moveLeftWest will not update ur location, need add another line)
        satComRelay.moveUpNorth();
        std::cout << "Current Shield Energy: " << vehicleData.getCurrentShieldEnergy() << std::endl;
        centerY -= 1;

        // Avoid run to the boundary of premap array
        if (centerY <= 0) {
            std::cout << "Vehicle reached the premap boundary!" << std::endl;
            break;
        }
    }
    
   
    // hide the output to the terminal [FINSIH]
    std::cout.rdbuf(oldCout);	// code turn output on 
    fclose(stdout);				// code turn output on
    stdout = originalStdout;	// code turn output on
    
    //std::cout << "my location: " << centerY << std::endl;
    thisMapStartY = centerY;
	
}

void VehicleDetails::GetTotalVertical(int centerX, int centerY) {

	int tmpVertical = 1; // start with 1 because need to cout the "#" at the left side
	
	
	// hide the output to the terminal [START]
    std::streambuf* oldCout = std::cout.rdbuf();
    std::ofstream nullStream("/dev/null"); 
    std::cout.rdbuf(nullStream.rdbuf());
    
    
    FILE* originalStdout = stdout;
    stdout = fopen("/dev/null", "w"); 
    
   
    for (;;) {
        char scanResult = satComRelay.scanSouth(vehicleData);

        if (scanResult != '.') {
            premap[centerY+1][centerX] = scanResult; 
            std::cout << "Scanned Result: " << scanResult << std::endl;
            tmpVertical++;
            //std::cout << "testing: " << tmpVertical << std::endl;
        }

        if (scanResult == '#') {
            std::cout << "Stopping as '#' is encountered!" << std::endl;
            premap[centerY+1][centerX] = scanResult;
            tmpVertical++;
            //std::cout << "testing: " << tmpVertical << std::endl;
            break; // Exit 
        }

        // Move the vehicle west (.moveLeftWest will not update ur location, need add another line)
        satComRelay.moveDownSouth();
        std::cout << "Current Shield Energy: " << vehicleData.getCurrentShieldEnergy() << std::endl;
        centerY += 1;

        // Avoid run to the boundary of premap array
        if (centerY <= 0) {
            std::cout << "Vehicle reached the premap boundary!" << std::endl;
            break;
        }
    }

    totalVertical = tmpVertical;
    // hide the output to the terminal [FINSIH]
   
    std::cout.rdbuf(oldCout);	// code turn output on 
    fclose(stdout);				// code turn output on
    stdout = originalStdout;	// code turn output on
    
    
   	//premap[centerY][centerX] = '_';
   	//std::cout << "tmp horizontal: " << tmpVertical << std::endl; // include the border
   	thisMapStartY = centerY;
}

void VehicleDetails::CornerCheck() {
	 char scanResult1 = satComRelay.scanEast(vehicleData);
	 char scanResult2 = satComRelay.scanSouth(vehicleData);
	 char scanResult3 = satComRelay.scanSouthEast(vehicleData);
	 
	 if (scanResult1 = '#') {
	 	autoscanmap[14][25] = scanResult1;
	 	if (scanResult2 = '#') {
	 		autoscanmap[15][24] = scanResult2;
	 		if (scanResult3 = '#') {
	 			autoscanmap[15][25] = scanResult2;
	 			std::cout << "Currently at bottom right corner (1 unit distance from '#')" << std::endl;
	 		}
	 	}
	 }
}
