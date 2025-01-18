#ifndef SATELLITE_COMMS_RELAY_H
#define SATELLITE_COMMS_RELAY_H

#include <string>
#include "VehData.h"

// forward declaration for struct VehController
struct VehicleController;

class SatComRelay
{
private:
    VehicleController* vcPtr;
    bool isClosed;
    bool isInitialized;

    VehData copyDataFromVCTToVDT();

public:
    SatComRelay();
    ~SatComRelay();

    VehData initializeVehicle(const std::string &srcFilename, bool fileNeedsDecryption, bool randomizeStartPosition, int missionType);
    void closeVehicle();
    VehData allocateEnergyToShield(long amountOfEnergy);

    VehData moveUpNorth();
    VehData moveDownSouth();
    VehData moveRightEast();
    VehData moveLeftWest();

    char scanNorth(VehData &vdt);
    char scanNorth2(VehData &vdt);
    char scanNorthEast(VehData &vdt);
    char scanNorthEast2(VehData &vdt);
    char scanEast(VehData &vdt);
    char scanEast2(VehData &vdt);
    char scanSouthEast(VehData &vdt);
    char scanSouthEast2(VehData &vdt);
    char scanSouth(VehData &vdt);
    char scanSouth2(VehData &vdt);
    char scanSouthWest(VehData &vdt);
    char scanSouthWest2(VehData &vdt);
    char scanWest(VehData &vdt);
    char scanWest2(VehData &vdt);
    char scanNorthWest(VehData &vdt);
    char scanNorthWest2(VehData &vdt);

    void printEntireMap(const std::string &password, bool underlineVehLocation);
    void printScannedMap(const std::string &password, bool underlineVehLocation);
    void printExploredMap(const std::string &password, bool underlineVehLocation);
    void printReport(const std::string &password);
};

#endif // SATELLITE_COMMS_RELAY_H
