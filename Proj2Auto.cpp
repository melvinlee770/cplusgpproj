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
    
    SatComRelay satComRelay;
    VehData vehicleData;
    
    std::vector<std::vector<char>> map; 
    
    void PrePareVehicle(const std::string &srcFileName, bool fileNeedsDecryption, bool randomizeStartPosition, int missionType);
    void CreateAndDisplayMap(int horizontal, int vertical);
    void CalibrateHorizontal(int centerX, int centerY);
    
};
             

int main() {
	VehicleDetails vd;
	
    // parameters for the function
    std::string srcFileName = "/home/student/Downloads/GrpProj_Pkg/Scenes/Scenario06.dat";
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

    // Print the map
    
    for (const auto& row : map) {
        for (const auto& cell : row) {
            std::cout << " " << cell << " ";
        }
        std::cout << std::endl;
    }
    
    std::cout << "my location: " <<  thisMapStartX << std::endl;
    char testResult = satComRelay.scanWest(vehicleData);
    std::cout << "test: " <<  testResult << std::endl;
    
}

void VehicleDetails::CalibrateHorizontal(int centerX, int centerY) {	// use the method of "MEMBER FUNCTION"
	//int centerX = thisMapStartX;
    //int centerY = thisMapStartY;
    int tmpHorizontal = 0;
    
    std::streambuf* oldCout = std::cout.rdbuf();

    // hide the output to the terminal [START]
    std::ofstream nullStream("/dev/null"); 
    std::cout.rdbuf(nullStream.rdbuf());
    
    FILE* originalStdout = stdout;
    stdout = fopen("/dev/null", "w"); 
    
    for (;;) {
        char scanResult = satComRelay.scanWest(vehicleData);

        if (scanResult != '.') {
            map[centerY][centerX - 1] = scanResult; 
            tmpHorizontal++;
            std::cout << "Scanned Result: " << scanResult << std::endl;
        }

        if (scanResult == '#') {
            std::cout << "Stopping as '#' is encountered!" << std::endl;
            map[centerY][centerX - 1] = scanResult;
            tmpHorizontal++;
            break; // Exit 
        }

        // Move the vehicle west (.moveLeftWest will not update ur location, need add another line)
        satComRelay.moveLeftWest();
        centerX -= 1;

        // Avoid run to the boundary of map array
        if (centerX <= 0) {
            std::cout << "Vehicle reached the map boundary!" << std::endl;
            break;
        }
    }
    // hide the output to the terminal [FINSIH]
    std::cout.rdbuf(oldCout);
    fclose(stdout);
    stdout = originalStdout;
    
    map[centerY][centerX] = '_';
    //std::cout << "my location: " << centerX << std::endl;
    //std::cout << "tmp horizontal: " << tmpHorizontal << std::endl; // include the border
    thisMapStartX = centerX;
	
}





