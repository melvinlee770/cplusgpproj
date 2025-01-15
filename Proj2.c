#include "SatComRelay.h" 
#include "VehData.h"
#include <iostream>
#include <string>
#include <cmath>

using namespace std;

// Function declarations
void DispEnergy();
void EnergyMenu();
void ScanMap();
void DispMap();
void Movement();
void TerrainC();
void MainMenu();
void AutoMappingSettings();

int main() {
    
    
    string srcFilename;
    string encryption;
    string startposition;
    bool fileNeedsDecryption = false;
    bool randomizeStartPosition = false;
    int counter = 0;
   

// Get the scenario file name
    std::cout << "\nPlease enter the scenario file's name: \n";
    getline(cin, srcFilename);
    std::cout << "You entered: " << srcFilename << std::endl;

    // Check if the file is encrypted
    while (counter == 0) {
        std::cout << "\nIs the Scenario file encrypted? (y/n):\n";
        getline(cin, encryption);

        if (!encryption.empty()) {
            encryption[0] = tolower(encryption[0]);
        }

        if (encryption == "y") {
            fileNeedsDecryption = true;
            counter++;
        } else if (encryption == "n") {
            fileNeedsDecryption = false;
            counter++;
        } else {
            std::cout << "\nError. Re-enter value.\n";
        }
    }

    // Check if the start position should be randomized
    while (counter == 1) {
        std::cout << "\nDo you wish to randomize start position (y/n):\n";
        getline(cin, startposition);

        if (!startposition.empty()) {
            startposition[0] = tolower(startposition[0]);
        }

        if (startposition == "y") {
            randomizeStartPosition = true;
            counter++;
        } else if (startposition == "n") {
            randomizeStartPosition = false;
            counter++;
        } else {
            std::cout << "\nError. Re-enter value.\n";
        }
    }

    // Display the results for debugging or confirmation
    std::cout << "\nScenario file: " << srcFilename << std::endl;
    std::cout << "File needs decryption: " << (fileNeedsDecryption ? "Yes" : "No") << std::endl;
    std::cout << "Randomize start position: " << (randomizeStartPosition ? "Yes" : "No") << std::endl;

    while (counter == 2) {
        MainMenu();
        std::cout << "\nEnter 1 to exit or 0 to continue: ";
        cin >> counter;
        cin.ignore(); // Clear newline from input buffer
    }
    return 0;
}

// Define your functions
void DispEnergy() {

}

void EnergyMenu() {

}

void ScanMap() {

}

void DispMap() {

}

void Movement() {

}

void TerrainC() {

}

void MainMenu() {
    int choice;
    std::cout << "\nTeam number                : 4" << std::endl;
    std::cout << "Team leader name           : Lwin Moe Aung" << std::endl;
    std::cout << "\n-------------------------------------------------\n" << std::endl;
    std::cout << "Welcome to Team 4 Group Project\n" << std::endl;
    std::cout << "1) Configure Autopilot Mapping Exploration settings" << std::endl;
    std::cout << "2) Configure Terrain Exploration Simulator settings" << std::endl;
    std::cout << "3) Start Autopilot Mapping!" << std::endl;
    std::cout << "4) Start Simulation!" << std::endl;
    std::cout << "5) End\n" << std::endl;
    std::cout << "Please enter your choice: ";
    cin >> choice;
    cin.ignore(); // Clear newline from input buffer

    switch (choice) {
        case 1:
            AutoMappingSettings();
            break;
        case 2:
            std::cout << "\nConfiguring Terrain Exploration Simulator settings..." << std::endl;
            break;
        case 3:
            std::cout << "\nStarting Autopilot Mapping!" << std::endl;
            break;
        case 4:
            std::cout << "\nStarting Simulation!" << std::endl;
            break;
        case 5:
            std::cout << "\nExiting the program." << std::endl;
            exit(0);
        default:
            std::cout << "\nInvalid choice. Please try again." << std::endl;
    }
}

void AutoMappingSettings() {
	int autoMappingChoice;
	string autoMappingFile = "default.dat";
	bool autoMappingDecryption = true;
	string autoMappingOutputFile = "map-rpt.txt";

	std::cout << "\n[Configure Autopilot Mapping Exploration settings]\n" << std::endl;
	std::cout << "a) Specify INPUT scenario filename (current: " << autoMappingFile << ")" << std::endl;
	std::cout << "b) Specify INPUT scenario file encrypted (current: " << (autoMappingDecryption ? "Y" : "N") << ")" << std::endl;
	std::cout << "c) Specify OUTPUT map report filename (current: " << autoMappingOutputFile << ")" << std::endl;
	std::cout << "d) Back to main menu\n" << std::endl;
	std::cout << "Please enter your choice (a-d): ";
	cin >> autoMappingChoice;
	cin.ignore();
	
}



