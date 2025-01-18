#include <iostream>
#include <string>
#include "SatComRelay.h"

int main() {
    // Instantiate the SatComRelay object
    SatComRelay satComRelay;

    // parameters for the function
    std::string srcFilename = "/home/student/Downloads/GrpProj_Pkg/Scenes/Scenario06.dat";
    bool fileNeedsDecryption = true;           // Set to true if the file requires decryption
    bool randomizeStartPosition = true;         // Set as needed for your scenario
    int missionType = 1;                        // Example mission type; adjust based on your needs

	VehData vehicleData = satComRelay.initializeVehicle(srcFilename, fileNeedsDecryption, randomizeStartPosition, missionType);
	/*
    std::cout << "Initial Energy: " << vehicleData.getInitialEnergy() << std::endl;
    std::cout << "Current Energy: " << vehicleData.getCurrentEnergy() << std::endl;
    std::cout << "Current Shield Energy: " << vehicleData.getCurrentShieldEnergy() << std::endl;
  	*/

    return 0;
}

