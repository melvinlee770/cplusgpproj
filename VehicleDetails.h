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
    std::vector<std::vector<char>> autoscanmap;

    void PrePareVehicle(const std::string &srcFileName, bool fileNeedsDecryption, bool randomizeStartPosition, int missionType);
    void CreateAndDisplayMap(int horizontal, int vertical);
    void CalibrateHorizontal(int centerX, int centerY);
    void GetTotalHorizontal(int centerX, int centerY);
    void CalibrateVertical(int centerX, int centerY);
    void GetTotalVertical(int centerX, int centerY);
    
    void AutoMapping(int horizontal, int vertical);
    void CornerCheck();
    
};

#endif // VEHICLE_DETAILS_H

