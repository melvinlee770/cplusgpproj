#ifndef VEHICLE_DETAILS_H
#define VEHICLE_DETAILS_H

#include <string>
#include <vector>
#include "SatComRelay.h" // Required for SatComRelay
#include "VehData.h"     // Required for VehData

class VehicleDetails {
public:
    int thisMapStartX;
    int thisMapStartY;
    int totalHorizontal;
    int totalVertical;

    SatComRelay satComRelay;
    VehData vehicleData;

    std::vector<std::vector<char>> premap;

    void PrePareVehicle(const std::string &srcFileName, bool fileNeedsDecryption, bool randomizeStartPosition, int missionType);
    void CreateAndDisplayMap(int horizontal, int vertical);
    void CalibrateHorizontal(int centerX, int centerY);
    void GetTotalHorizontal(int centerX, int centerY);
    void CalibrateVertical(int centerX, int centerY);
    void GetTotalVertical(int centerX, int centerY);
    
};

class SecondVehicleDetails {

private:
    VehicleDetails &vehicleDetailsRef;

public:
	SatComRelay SecondSatComRelay;
    VehData SecondVehicleData;
    //std::vector<std::vector<char>> autoscanmap;
    SecondVehicleDetails(VehicleDetails &vehicleDetails) : vehicleDetailsRef(vehicleDetails) {}
    
    std::vector<std::vector<char>> secondmap;
    
    void SecondPrepareVehicle(const std::string &srcFileName, bool fileNeedsDecryption, bool randomizeStartPosition, int missionType);
    void CornerCheck();
    void TopLeftScanMove(int n, int m);
    void FirstRowLeftScanMove(int n, int m);
    void TopRightScanMove(int n, int m);
    
    void MiddleRightLeftScanMove(int n, int m);
    void SideRightLeftScanMove(int n, int m);
    void MiddleLeftRightScanMove(int n, int m);
    void SideLeftRightScanMove(int n, int m);
    
    void AutoMapping();
};

#endif // VEHICLE_DETAILS_H

