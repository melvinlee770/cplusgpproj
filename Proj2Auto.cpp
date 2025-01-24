#include <iostream>
#include <string>
#include "SatComRelay.h"
#include <vector>
#include <fstream>
#include <cstdio>

class VehicleDetails {
public:
    int	 thisMapStartX;
    int	 thisMapStartY;
    int	 totalHorizontal;
    int  totalVertical;
    
    SatComRelay satComRelay;
    VehData vehicleData;
    
    std::vector<std::vector<char>> map; 
    
    void PrePareVehicle(const std::string &srcFileName, bool fileNeedsDecryption, bool randomizeStartPosition, int missionType);
    void CreateAndDisplayMap(int horizontal, int vertical);
    void CalibrateHorizontal(int centerX, int centerY);
    void GetTotalHorizontal(int centerX, int centerY);
    void CalibrateVertical(int centerX, int centerY);
    void GetTotalVertical(int centerX, int centerY);
};
             

int main() {
	VehicleDetails vd;
	
    // parameters for the function
    std::string srcFileName = "/home/student/Downloads/cplusgpproj/Scenes/Scenario06.dat";
    bool fileNeedsDecryption = true;     
    bool randomizeStartPosition = true;         
    int missionType = 1;          
    
    // other settings parameter
    const int horizontal = 45;
    const int vertical = 30;              

	vd.PrePareVehicle(srcFileName, fileNeedsDecryption, randomizeStartPosition, missionType);
	vd.CreateAndDisplayMap(horizontal, vertical);

    return 0;
}


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
  	
	map = std::vector<std::vector<char>>(vertical, std::vector<char>(horizontal, '.')); 

    // Calculate the center position
    int centerX = horizontal / 2;
    int centerY = vertical / 2;
    thisMapStartX = centerX;
    thisMapStartY = centerY;

    // Mark the initial position
    map[centerY][centerX] = 'S';
    CalibrateHorizontal(thisMapStartX, thisMapStartY);
    GetTotalHorizontal(thisMapStartX, thisMapStartY);
    CalibrateVertical(thisMapStartX, thisMapStartY);
    GetTotalVertical(thisMapStartX, thisMapStartY);
    map[thisMapStartY][thisMapStartX] = '_';
    // Print the map
    
    for (const auto& row : map) {
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
            map[centerY][centerX - 1] = scanResult; 
            std::cout << "Scanned Result: " << scanResult << std::endl;
        }

        if (scanResult == '#') {
            std::cout << "Stopping as '#' is encountered!" << std::endl;
            map[centerY][centerX - 1] = scanResult;
            break; // Exit 
        }

        // Move the vehicle west (.moveLeftWest will not update ur location, need add another line)
        satComRelay.moveLeftWest();
        std::cout << "Current Shield Energy: " << vehicleData.getCurrentShieldEnergy() << std::endl;
        centerX -= 1;

        // Avoid run to the boundary of map array
        if (centerX <= 0) {
            std::cout << "Vehicle reached the map boundary!" << std::endl;
            break;
        }
    }
    
    // hide the output to the terminal [FINSIH]
    std::cout.rdbuf(oldCout);	// code turn output on 
    fclose(stdout);				// code turn output on
    stdout = originalStdout;	// code turn output on
    
    //map[centerY][centerX] = '_';
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
            map[centerY][centerX + 1] = scanResult; 
            std::cout << "Scanned Result: " << scanResult << std::endl;
            tmpHorizontal++;
            std::cout << "testing: " << tmpHorizontal << std::endl;
        }

        if (scanResult == '#') {
            std::cout << "Stopping as '#' is encountered!" << std::endl;
            map[centerY][centerX + 1] = scanResult;
            tmpHorizontal++;
            std::cout << "testing: " << tmpHorizontal << std::endl;
            break; // Exit 
        }

        // Move the vehicle west (.moveLeftWest will not update ur location, need add another line)
        satComRelay.moveRightEast();
        std::cout << "Current Shield Energy: " << vehicleData.getCurrentShieldEnergy() << std::endl;
        centerX += 1;

        // Avoid run to the boundary of map array
        if (centerX <= 0) {
            std::cout << "Vehicle reached the map boundary!" << std::endl;
            break;
        }
    }

    totalHorizontal = tmpHorizontal;
    // hide the output to the terminal [FINSIH]
    std::cout.rdbuf(oldCout);	// code turn output on 
    fclose(stdout);				// code turn output on
    stdout = originalStdout;	// code turn output on
    
   	//map[centerY][centerX] = '_';
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
            map[centerY - 1][centerX] = scanResult; 
            std::cout << "Scanned Result: " << scanResult << std::endl;
        }

        if (scanResult == '#') {
            std::cout << "Stopping as '#' is encountered!" << std::endl;
            map[centerY - 1][centerX] = scanResult;
            break; // Exit 
        }

        // Move the vehicle west (.moveLeftWest will not update ur location, need add another line)
        satComRelay.moveUpNorth();
        std::cout << "Current Shield Energy: " << vehicleData.getCurrentShieldEnergy() << std::endl;
        centerY -= 1;

        // Avoid run to the boundary of map array
        if (centerY <= 0) {
            std::cout << "Vehicle reached the map boundary!" << std::endl;
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
            map[centerY+1][centerX] = scanResult; 
            std::cout << "Scanned Result: " << scanResult << std::endl;
            tmpVertical++;
            //std::cout << "testing: " << tmpVertical << std::endl;
        }

        if (scanResult == '#') {
            std::cout << "Stopping as '#' is encountered!" << std::endl;
            map[centerY+1][centerX] = scanResult;
            tmpVertical++;
            //std::cout << "testing: " << tmpVertical << std::endl;
            break; // Exit 
        }

        // Move the vehicle west (.moveLeftWest will not update ur location, need add another line)
        satComRelay.moveDownSouth();
        std::cout << "Current Shield Energy: " << vehicleData.getCurrentShieldEnergy() << std::endl;
        centerY += 1;

        // Avoid run to the boundary of map array
        if (centerY <= 0) {
            std::cout << "Vehicle reached the map boundary!" << std::endl;
            break;
        }
    }

    totalVertical = tmpVertical;
    // hide the output to the terminal [FINSIH]
   
    std::cout.rdbuf(oldCout);	// code turn output on 
    fclose(stdout);				// code turn output on
    stdout = originalStdout;	// code turn output on
    
    
   	//map[centerY][centerX] = '_';
   	//std::cout << "tmp horizontal: " << tmpVertical << std::endl; // include the border
   	thisMapStartY = centerY;

}
