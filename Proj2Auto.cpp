#include <iostream>
#include <string>
#include "SatComRelay.h"
#include <vector>

class VehicleDetails {
public:
    long thisInitialEnergy;
    long thisCurrentEnergy;
    long thisCurrentShieldEnergy;
    int	 thisMapStartX;
    int	 thisMapStartY;
    
    SatComRelay satComRelay;
    VehData vehicleData;
    
    void PrePareVehicle(const std::string &srcFileName, bool fileNeedsDecryption, bool randomizeStartPosition, int missionType);
    void CreateAndDisplayMap(int horizontal, int vertical);
};
             

int main() {
	VehicleDetails vd;
	
    // parameters for the function
    std::string srcFileName = "/home/student/Downloads/GrpProj_Pkg/Scenes/Scenario06.dat";
    bool fileNeedsDecryption = true;     
    bool randomizeStartPosition = true;         
    int missionType = 1;          
    
    // other settings parameter
    const int horizontal = 30;
    const int vertical = 30;              

	vd.PrePareVehicle(srcFileName, fileNeedsDecryption, randomizeStartPosition, missionType);
	vd.CreateAndDisplayMap(horizontal, vertical);

    return 0;
}


void VehicleDetails::PrePareVehicle(const std::string &srcFileName, bool fileNeedsDecryption, bool randomizeStartPosition, int missionType) {
	
	vehicleData = satComRelay.initializeVehicle(
		srcFileName, fileNeedsDecryption, randomizeStartPosition, missionType
	);
	vehicleData = satComRelay.allocateEnergyToShield(50000);
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
  	
	std::vector<std::vector<char>> map(vertical, std::vector<char>(horizontal, '.'));

    // Calculate the center position
    int centerX = horizontal / 2;
    int centerY = vertical / 2;
    thisMapStartX = centerX;
    thisMapStartY = centerY;

    // Mark the initial position
    map[centerY][centerX] = 'X';

    // Print the map
    /*
    for (const auto& row : map) {
        for (const auto& cell : row) {
            std::cout << " " << cell << " ";
        }
        std::cout << std::endl;
    }
    */
    
    // TESTING
    //vehicleData = satComRelay.moveLeftWest();
    //char result = satComRelay.scanNorth(vehicleData);
	//std::cout << "Scanned object to the North: " << result << std::endl;
}






