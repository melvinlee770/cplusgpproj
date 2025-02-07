#include <iostream>
#include <string>
#include "VehicleDetails.h"
#include <fstream>
          

int main() {
	VehicleDetails vd;
	SecondVehicleDetails svd(vd);
	
    // parameters for the function
    std::string srcFileName = "/home/student/Downloads/cplusgpproj/Scenes/scenarioB.dat";	//Scenario06.dat ; scenarioA.dat; scenarioB.dat
    bool fileNeedsDecryption = true;     
    bool randomizeStartPosition = true;         
    int missionType = 6;          
    
    // other settings parameter
    const int horizontal = 45;
    const int vertical = 30;
                  
	// hide the output to the terminal [START]
    std::streambuf* oldCout = std::cout.rdbuf();
    std::ofstream nullStream("/dev/null"); 
    std::cout.rdbuf(nullStream.rdbuf());
    FILE* originalStdout = stdout;
    stdout = fopen("/dev/null", "w"); 
    
	vd.PrePareVehicle(srcFileName, fileNeedsDecryption, randomizeStartPosition, missionType);

	// hide the output to the terminal [FINSIH]
    std::cout.rdbuf(oldCout);	// code turn output on 
    fclose(stdout);				// code turn output on
    stdout = originalStdout;	// code turn output on
    
	vd.CreateAndDisplayMap(horizontal, vertical);
    
    //std::cout << "\nTotal Horizontal Distance(test): " << vd.totalHorizontal << std::endl;
    //std::cout << "Total Vertical Distance(test): " << vd.totalVertical << std::endl;

    svd.SecondPrepareVehicle(srcFileName, fileNeedsDecryption, randomizeStartPosition, missionType);
    svd.CornerCheck();
    svd.AutoMapping();

    return 0;
}



