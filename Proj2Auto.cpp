#include <iostream>
#include <string>
#include "SatComRelay.h"

void PrePareVehicle(const std::string &srcFileName, bool fileNeedsDecryption, bool randomizeStartPosition, int missionType);
                    

int main() {
	
    // parameters for the function
    std::string srcFileName = "/home/student/Downloads/GrpProj_Pkg/Scenes/Scenario06.dat";
    bool fileNeedsDecryption = true;           // Set to true if the file requires decryption
    bool randomizeStartPosition = true;         // Set as needed for your scenario
    int missionType = 1;                        // Example mission type; adjust based on your needs

	PrePareVehicle(srcFileName, fileNeedsDecryption, randomizeStartPosition, missionType);

    return 0;
}

void PrePareVehicle(const std::string &srcFileName, bool fileNeedsDecryption, bool randomizeStartPosition, int missionType) {
	
	SatComRelay satComRelay;

	VehData vehicleData = satComRelay.initializeVehicle(
		srcFileName, fileNeedsDecryption, randomizeStartPosition, missionType
	);
	vehicleData = satComRelay.allocateEnergyToShield(50000);
	
	std::cout << std::endl;
	std::cout << "##VEHICLE STATUS##" << std::endl;
    std::cout << "Initial Energy: " << vehicleData.getInitialEnergy() << std::endl;
    std::cout << "Current Energy: " << vehicleData.getCurrentEnergy() << std::endl;
    std::cout << "Current Shield Energy: " << vehicleData.getCurrentShieldEnergy() << std::endl;
  	
  	
}






