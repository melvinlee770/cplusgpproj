#include "SatComRelay.h"
#include "VehData.h"
#include <iostream>
#include <regex>
#include <cstdlib>
#include <string>
#include <cmath>

using namespace std;

// Main Function declarations
void DispEnergy();
void EnergyMenu();
void ScanMap();
void DispMap();
void Movement();
void TerrainC();
void MainMenu();
void AutoMappingSettings();

// Extra Function declarations
void RegexValidate(const std::string& user_input, const std::string& pattern);
void convertCharToNumber(int numAlphanumerics, int &result);

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
    std::string choice; // Use a string for input to validate it first
    int choice_number;

    std::cout << "\nTeam number                : 4" << std::endl;
    std::cout << "Team leader name           : Lwin Moe Aung" << std::endl;
    std::cout << "\n-------------------------------------------------\n" << std::endl;
   	std::cout << "Welcome to Team 4 Group Project\n" << std::endl;
   	std::cout << "1) Configure Autopilot Mapping Exploration settings" << std::endl;
	std::cout << "2) Configure Terrain Exploration Simulator settings" << std::endl;
	std::cout << "3) Start Autopilot Mapping!" << std::endl;
    std::cout << "4) Start Simulation!" << std::endl;
	std::cout << "5) End\n" << std::endl;

    while (true) {
		std::cout << "Please enter your choice: ";
		std::getline(std::cin, choice); // use getline bec read entire line, including spaces

		try {
			RegexValidate(choice, "^[1-5]$");
			choice_number = std::stoi(choice);
			break;
		} catch(const std::invalid_argument& e) {
			std::cout << e.what() << std::endl;
		}
    }
        // Handle the menu options
        switch (choice_number) {
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
	string autoMappingFile = "default.dat";
	bool autoMappingDecryption = true;
	string autoMappingOutputFile = "map-rpt.txt";
	int totalAlpha = 4;
	int autoMappingChoice = 0;


	std::cout << "\n[Configure Autopilot Mapping Exploration settings]\n" << std::endl;
	std::cout << "a) Specify INPUT scenario filename (current: " << autoMappingFile << ")" << std::endl;
	std::cout << "b) Specify INPUT scenario file encrypted (current: " << (autoMappingDecryption ? "Y" : "N") << ")" << std::endl;
	std::cout << "c) Specify OUTPUT map report filename (current: " << autoMappingOutputFile << ")" << std::endl;
	std::cout << "d) Back to main menu\n" << std::endl;
	
	convertCharToNumber(totalAlpha, autoMappingChoice);
    
    // Handle the menu options
    switch (autoMappingChoice) {
    	case 1:
			std::cout << "\nSelect A" << std::endl;
            break;
        case 2:
            std::cout << "\nSelect B" << std::endl;
            break;
        default:
            std::cout << "\nInvalid choice. Please try again." << std::endl;
	}

}

void RegexValidate(const std::string& user_input_a, const std::string& pattern) {
	//std::regex main_menu_pattern("^[1-4]$"); // only positive integer
	std::regex regex_pattern(pattern);

	if (user_input_a.empty()) {
        throw std::invalid_argument("ERROR_CODE:EMPTY_INPUT\n");
    }

	if (!std::regex_match(user_input_a, regex_pattern)) {
		throw std::invalid_argument("ERROR_CODE:INVALID_INPUT\n");
    }

    //int convert_number = std::stoi(user_input_a);
    std::cout << "Your input is valid: " << user_input_a << std::endl;
}

void convertCharToNumber(int numAlphanumerics, int &result) {
    char input;
    std::cout << "Enter a character (a to " << static_cast<char>('a' + numAlphanumerics - 1) << "): ";
    std::cin >> input;

    // Check if the input character is within the valid range
    if (input >= 'a' && input < 'a' + numAlphanumerics) {
        result = input - 'a' + 1; // Store the result
    } else {
        std::cout << "Invalid input! Please enter a character between a and "
                  << static_cast<char>('a' + numAlphanumerics - 1) << "." << std::endl;
        result = -1; // Indicating an invalid input
    }
}

