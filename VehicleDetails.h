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
    
    int LastRowTrigger;
    
    std::vector<std::vector<std::string>> uniqueScans;
    std::vector<std::vector<char>> secondmap;
    
    char captureLetter;
    int tmpCurrentEnergy;
    int tmpCurrentShield;
    
    void SecondPrepareVehicle(const std::string &srcFileName, bool fileNeedsDecryption, bool randomizeStartPosition, int missionType);
    void CornerCheck();
    
    // function for first row 
    void TopLeftScanMove(int n, int m);
    void FirstRowLeftScanMove(int n, int m);
    void TopRightScanMove(int n, int m);
    
    // function for the row in between first and last row 
    void MiddleRightLeftScanMove(int n, int m);
    void SideRightLeftScanMove(int n, int m);
    void MiddleLeftRightScanMove(int n, int m);
    void SideLeftRightScanMove(int n, int m);
    
    void BottomLeftScanMove(int n, int m);
    void BottomLeftRightScanMove(int n, int m);
    void BottomRightScanMove(int n, int m);
    
    void AutoMapping(const std::string &srcFileName, bool fileNeedsDecryption, const std::string &srcMapReport, bool displayTerminal);
    void printArray();
    
    void calculateData(int CurrmoveEnergy, int CurrshieldEnergy);
    void collectData(char letters, int moveEnergy, int shieldEnergy);
    std::string getFormattedTimestamp();
};

#endif // VEHICLE_DETAILS_H

