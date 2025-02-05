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

void SecondVehicleDetails::SecondPrepareVehicle(const std::string &srcFileName, bool fileNeedsDecryption, bool randomizeStartPosition, int missionType) {
	
	SecondVehicleData = SecondSatComRelay.initializeVehicle(
		srcFileName, fileNeedsDecryption, randomizeStartPosition, missionType
	);
	SecondVehicleData = SecondSatComRelay.allocateEnergyToShield(400000);
	
	std::cout << std::endl;
	std::cout << "##VEHICLE STATUS##" << std::endl;
    std::cout << "Initial Energy: " << SecondVehicleData.getInitialEnergy() << std::endl;
    std::cout << "Current Energy: " << SecondVehicleData.getCurrentEnergy() << std::endl;
    std::cout << "Current Shield Energy: " << SecondVehicleData.getCurrentShieldEnergy() << std::endl;
    
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

void SecondVehicleDetails::CornerCheck() {
	
    // Calculate the center position
    int secondMapHorizontal = vehicleDetailsRef.totalHorizontal;
    int secondMapVertical = vehicleDetailsRef.totalVertical;
    
    //initialise second map
    secondmap = std::vector<std::vector<char>>(secondMapVertical, std::vector<char>(secondMapHorizontal, '.')); 
    
	// hide the output to the terminal [START]
    std::streambuf* oldCout = std::cout.rdbuf();
    std::ofstream nullStream("/dev/null"); 
    std::cout.rdbuf(nullStream.rdbuf());
    FILE* originalStdout = stdout;
    stdout = fopen("/dev/null", "w"); 
    

    for (;;) {
        char scanResult = SecondSatComRelay.scanWest(SecondVehicleData);

        if (scanResult != '.' && scanResult !='#') {
            //autoscanmap[centerY][centerX - 1] = scanResult; 
            std::cout << "Scanned Result: " << scanResult;
            SecondSatComRelay.moveLeftWest();
        }
        if (scanResult == '#') {
            std::cout << "Stopping as '#' is encountered!" << std::endl;
            //autoscanmap[centerY][centerX - 1] = scanResult;
            std::cout << "Current Shield Energy: " << SecondVehicleData.getCurrentShieldEnergy() << std::endl;
            break; // Exit 
        }

        std::cout << "Current Shield Energy: " << SecondVehicleData.getCurrentShieldEnergy() << std::endl;
        //centerX -= 1;
    }
    
    for (;;) {
    	char scanResult = SecondSatComRelay.scanNorth(SecondVehicleData);

        if (scanResult != '.' && scanResult !='#') {
            //autoscanmap[centerY][centerX - 1] = scanResult; 
            std::cout << "Scanned Result: " << scanResult;
            SecondSatComRelay.moveUpNorth();;
        }
        if (scanResult == '#') {
            std::cout << "Stopping as '#' is encountered!" << std::endl;
            secondmap[0][1] = scanResult;
            std::cout << "Current Shield Energy: " << SecondVehicleData.getCurrentShieldEnergy() << std::endl;
            break; // Exit 
        }
        std::cout << "Current Shield Energy: " << SecondVehicleData.getCurrentShieldEnergy() << std::endl;
    }
    
    
    // hide the output to the terminal [FINSIH]
    std::cout.rdbuf(oldCout);	// code turn output on 
    fclose(stdout);				// code turn output on
    stdout = originalStdout;	// code turn output on
    
}


//horizontal & vertical get data from other function
//horizontal:26 & vertical:16

void SecondVehicleDetails::TopLeftScanMove(int n, int m) {

	// hide the output to the terminal [START]
    std::streambuf* oldCout = std::cout.rdbuf();
    std::ofstream nullStream("/dev/null"); 
    std::cout.rdbuf(nullStream.rdbuf());
    FILE* originalStdout = stdout;
    stdout = fopen("/dev/null", "w"); 
	
	char scanResult_a = SecondSatComRelay.scanEast(SecondVehicleData);
	char scanResult_b = SecondSatComRelay.scanNorthEast(SecondVehicleData);
	char scanResult_c = SecondSatComRelay.scanWest(SecondVehicleData);
	std::cout << "##Top Left Corner##"<< std::endl;
	std::cout << "automapping scan result (right) :"<< scanResult_a << std::endl;
	secondmap[m][n+1] = scanResult_a;
	std::cout << "automapping scan result (above) :"<< scanResult_b << std::endl;
	secondmap[m-1][n-1] = scanResult_b;
	std::cout << "automapping scan result (left) :"<< scanResult_c << std::endl;
	secondmap[m][n-1] = scanResult_b;
			
	SecondSatComRelay.moveRightEast();
			
	char scanResult_d = SecondSatComRelay.scanWest(SecondVehicleData);
	char scanResult_e = SecondSatComRelay.scanNorth(SecondVehicleData);
	std::cout << "automapping scan result (left) :"<< scanResult_d << std::endl;
	secondmap[m][n] = scanResult_d;
	std::cout << "automapping scan result (above) :"<< scanResult_e<< std::endl; // #
	secondmap[m-1][n+1] = scanResult_e;	

    // hide the output to the terminal [FINSIH]
    std::cout.rdbuf(oldCout);	// code turn output on 
    fclose(stdout);				// code turn output on
    stdout = originalStdout;	// code turn output on	

}

void SecondVehicleDetails::FirstRowLeftScanMove(int n, int m) {

	// hide the output to the terminal [START]
    std::streambuf* oldCout = std::cout.rdbuf();
    std::ofstream nullStream("/dev/null"); 
    std::cout.rdbuf(nullStream.rdbuf());
    FILE* originalStdout = stdout;
    stdout = fopen("/dev/null", "w"); 

	std::cout << "##Normal Scan##"<< std::endl;
	char scanResult_a = SecondSatComRelay.scanEast(SecondVehicleData);
	std::cout << "automapping scan result (right) :"<< scanResult_a << std::endl;
	secondmap[m][n+1] = scanResult_a;
	
	SecondSatComRelay.moveRightEast();
	
	char scanResult_b = SecondSatComRelay.scanNorth(SecondVehicleData);
	std::cout << "automapping scan result (above) :"<< scanResult_b << std::endl;
	secondmap[m-1][n+1] = scanResult_b;

    // hide the output to the terminal [FINSIH]
    std::cout.rdbuf(oldCout);	// code turn output on 
    fclose(stdout);				// code turn output on
    stdout = originalStdout;	// code turn output on	
}

void SecondVehicleDetails::TopRightScanMove(int n, int m) {

	// hide the output to the terminal [START]
    std::streambuf* oldCout = std::cout.rdbuf();
    std::ofstream nullStream("/dev/null"); 
    std::cout.rdbuf(nullStream.rdbuf());
    FILE* originalStdout = stdout;
    stdout = fopen("/dev/null", "w"); 
	
	std::cout << "##Top Right Corner##"<< std::endl;
	char scanResult_a = SecondSatComRelay.scanEast(SecondVehicleData);
	std::cout << "automapping scan result (right) :"<< scanResult_a << std::endl;
	secondmap[m][n+1] = scanResult_a;

	char scanResult_c = SecondSatComRelay.scanNorthEast(SecondVehicleData);
	std::cout << "automapping scan result (topright) :"<< scanResult_c << std::endl;
	secondmap[m-1][n+1] = scanResult_a;
	
	SecondSatComRelay.moveDownSouth(); // move down to next row
    
    // hide the output to the terminal [FINSIH]
    std::cout.rdbuf(oldCout);	// code turn output on 
    fclose(stdout);				// code turn output on
    stdout = originalStdout;	// code turn output on	
}


void SecondVehicleDetails::MiddleRightLeftScanMove(int n, int m) {

	// hide the output to the terminal [START]
    std::streambuf* oldCout = std::cout.rdbuf();
    std::ofstream nullStream("/dev/null"); 
    std::cout.rdbuf(nullStream.rdbuf());
    FILE* originalStdout = stdout;
    stdout = fopen("/dev/null", "w"); 
	
	// move until the thrid last (not count the boundary)
	
	char scanResult_a = SecondSatComRelay.scanEast(SecondVehicleData);
	std::cout << "automapping scan result (right) :"<< scanResult_a << std::endl;
	secondmap[m][vehicleDetailsRef.totalHorizontal - n] = scanResult_a;
	
	SecondSatComRelay.moveLeftWest(); 
	
    // hide the output to the terminal [FINSIH]
    std::cout.rdbuf(oldCout);	// code turn output on 
    fclose(stdout);				// code turn output on
    stdout = originalStdout;	// code turn output on	
}

void SecondVehicleDetails::SideRightLeftScanMove(int n, int m) {
	// hide the output to the terminal [START]
    std::streambuf* oldCout = std::cout.rdbuf();
    std::ofstream nullStream("/dev/null"); 
    std::cout.rdbuf(nullStream.rdbuf());
    FILE* originalStdout = stdout;
    stdout = fopen("/dev/null", "w"); 
    
	char scanResult_a = SecondSatComRelay.scanWest(SecondVehicleData);
	std::cout << "automapping scan result (left) :"<< scanResult_a << std::endl;
	secondmap[m][(vehicleDetailsRef.totalHorizontal - n)-2] = scanResult_a;
	
	char scanResult_b = SecondSatComRelay.scanEast(SecondVehicleData);
	std::cout << "automapping scan result (right) :"<< scanResult_b << std::endl;
	secondmap[m][vehicleDetailsRef.totalHorizontal - n] = scanResult_b;
	
	SecondSatComRelay.moveRightEast(); 
	
	char scanResult_c = SecondSatComRelay.scanWest(SecondVehicleData);
	std::cout << "automapping scan result (left) :"<< scanResult_c << std::endl;
	secondmap[m][(vehicleDetailsRef.totalHorizontal - n)-1] = scanResult_c;
	
	SecondSatComRelay.moveLeftWest(); 
	SecondSatComRelay.moveDownSouth(); // move down to next row

    // hide the output to the terminal [FINSIH]
    std::cout.rdbuf(oldCout);	// code turn output on 
    fclose(stdout);				// code turn output on
    stdout = originalStdout;	// code turn output on	
	
}

void SecondVehicleDetails::MiddleLeftRightScanMove(int n, int m) {
	
	// move until the thrid last (not count the boundary)
	
	char scanResult_a = SecondSatComRelay.scanWest(SecondVehicleData);
	std::cout << "automapping scan result (left) :"<< scanResult_a << std::endl;
	secondmap[m][n-1] = scanResult_a;
	
	SecondSatComRelay.moveRightEast(); 
}

void SecondVehicleDetails::SideLeftRightScanMove(int n, int m) {
	
	char scanResult_a = SecondSatComRelay.scanEast(SecondVehicleData);
	std::cout << "automapping scan result (right) :"<< scanResult_a << std::endl;
	//std::cout << "automapping scan result (right) :"<< n << std::endl;
	secondmap[m][n+1] = scanResult_a;
	
	char scanResult_b = SecondSatComRelay.scanWest(SecondVehicleData);
	std::cout << "automapping scan result (left) :"<< scanResult_b << std::endl;
	secondmap[m][n-1] = scanResult_b;

	SecondSatComRelay.moveLeftWest();

	char scanResult_c = SecondSatComRelay.scanEast(SecondVehicleData);
	std::cout << "automapping scan result (right) :"<< scanResult_c << std::endl;
	secondmap[m][n] = scanResult_c;
	
	SecondSatComRelay.moveRightEast(); 
	SecondSatComRelay.moveDownSouth(); // move down to next row
	
}

void SecondVehicleDetails::AutoMapping() {

	for (int l = 1; l < vehicleDetailsRef.totalVertical; l++ ) {
		for (int i = 1; i < vehicleDetailsRef.totalHorizontal - 1; i++ ) {
			if (l == 1) {
				if (i == 1) {	// first row 
					TopLeftScanMove(i,l);
				}
				else if (i == (vehicleDetailsRef.totalHorizontal - 2)) {
					TopRightScanMove(i,l); 
				}
				else if (i !=1 && i !=(vehicleDetailsRef.totalHorizontal - 2)) {
					FirstRowLeftScanMove(i,l);
				}
			}
			else if ( l == vehicleDetailsRef.totalVertical - 1) {	// last row
				break;
			}
			else if ( l % 2 == 0 ) {
				if (i == (vehicleDetailsRef.totalHorizontal - 2)) {
					SideRightLeftScanMove(i,l);
					break;
				}
				//std::cout << i << std::endl;
				MiddleRightLeftScanMove(i,l);
			}
			else if (l % 2 != 0) {
				if (i == (vehicleDetailsRef.totalHorizontal - 2)) {
				    std::cout << i << std::endl;
					SideLeftRightScanMove(i,l);
					break;
				}
				MiddleLeftRightScanMove(i,l);
			}
		}
	}

	
	
   	for (const auto& row : secondmap) {
        for (const auto& cell : row) {
        	   std::cout << " " << cell << " ";
        }
        std::cout << std::endl;
    }
    
    std::cout << "Current Shield Energy: " << SecondVehicleData.getCurrentShieldEnergy() << std::endl;
    std::cout << "##VEHICLE STATUS##" << std::endl;
    std::cout << "Initial Energy: " << SecondVehicleData.getInitialEnergy() << std::endl;
    std::cout << "Current Energy: " << SecondVehicleData.getCurrentEnergy() << std::endl;
    
    std::cout << "Current Shield Energy: " << SecondVehicleData.getCurrentShieldEnergy() << std::endl;
}








