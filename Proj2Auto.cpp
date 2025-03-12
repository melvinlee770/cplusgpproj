#include "VehicleDetails.h"
#include "SatComRelay.h"
#include "VehData.h"
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <regex>
#include <cstdlib>
#include <queue>
#include <cmath>
using namespace std;

struct Node {
    int x, y, dist;
};
struct TerrainInfo {
    int movementEnergy;
    int shieldEnergy;
};

const int dx[] = {-1, 1, 0, 0};
const int dy[] = {0, 0, -1, 1};
const string WHITE_BG = "\033[47m";  // White background
const string RESET = "\033[0m";       // Reset color

void RegexValidate(const std::string& user_input, const std::string& pattern);
void MainMenu();
void AutoMappingSettings();
void ConfigTerrExpSet();
void AutoPilotMenu();
string filenameQuestion();
bool decrypQuestion();
string mapreportQuestion();
string routereportQuestion();
string routecriteriaQuestion();
string vehicletypeQuestion();
bool booldisplayterminalQuestion();
bool boolstartQuestion();

vector<string> loadMap(const string &filename);
vector<string> presimulatemap;
vector<string> simulatemap;

std::string ScenarioFile = "default.dat";
bool BoolDecryption = true;
std::string MapReportFile = "map-rpt.txt";
std::string RouteReportFile = "route-rpt.txt";
std::string RouteCriteria = "minimized total energy expenditure";
std::string VehicleType = "HighLander";

std::string srcFileBranch = "/home/vboxuser/Downloads/cplusgpproj/Scenes/";
//std::string srcFileBranch = "/home/student/Downloads/cplusgpproj/Scenes/";
int PreMissionType = 6;     
bool randomizeStartPosition = true; 
std::string srcFileName = "/home/vboxuser/Downloads/cplusgpproj/Scenes/MapWW.dat";	//home/student/Downloads/cplusgpproj/Scenes/MapWW.dat
bool PreDecryption = true;     
bool PreRandomize = true;           
const int horizontal = 45;
const int vertical = 30;
    
int main() {
    MainMenu();
    return 0;
}


void RegexValidate(const std::string& user_input_a, const std::string& pattern) {
	std::regex regex_pattern(pattern);

	if (user_input_a.empty()) {
        throw std::invalid_argument("ERROR_CODE:EMPTY_INPUT\n");
    }
	if (!std::regex_match(user_input_a, regex_pattern)) {
		throw std::invalid_argument("ERROR_CODE:INVALID_INPUT\n");
    }
    //int convert_number = std::stoi(user_input_a);
    //std::cout << "Your input is valid: " << user_input_a << std::endl;
}


string filenameQuestion() {
	std:: string tmpInput;
	do {
        try {
            std::cout << "\nPlease enter INPUT scenario file's name (current: "<<ScenarioFile<<"): ";
            getline(std::cin, tmpInput);
            
            RegexValidate(tmpInput,R"(^[^\s]+\.dat$)");
            break;

        } catch (const std::invalid_argument& e) {
            std::cout << "\n" << e.what();
        }
    } while (true); // Keep asking until valid input is provided
    ScenarioFile = tmpInput;
	std::cout<< "INPUT scenario filename successfully set to '"<< ScenarioFile<<"' !"<<std::endl;
	return ScenarioFile;
}


bool decrypQuestion() {
	std::string encryption;
    do {
        try {
            std::cout << "\nIs the Scenario file encrypted? (current: "<< (BoolDecryption ? "Y" : "N") <<", y/n): ";
            getline(std::cin, encryption);

            size_t first = encryption.find_first_not_of(" \t");
            size_t last = encryption.find_last_not_of(" \t");
            if (first != std::string::npos && last != std::string::npos) {
                encryption = encryption.substr(first, (last - first + 1));
            }

            // Validate using RegexValidate function
            RegexValidate(encryption, "^[yYnN]$");

            // Convert input to lowercase for consistency
            char response = tolower(encryption[0]);

            if (response == 'y') {
            	std::cout<< "Scenario file encrypted successfully set to 'Y' !"<<std::endl;
            	BoolDecryption = response;
                return true;
            } else if (response == 'n') {
            	std::cout<< "Scenario file encrypted successfully set to 'N' !"<<std::endl;
            	BoolDecryption = response;
                return false;
            }

            break; // Exit loop on valid input
        }
        catch (const std::invalid_argument& e) {
            std::cout << "\n" << e.what();
        }

    } while (true); 
    std::cout<< "Something ERROR, scenario file enrypted set to '"<< (BoolDecryption ? "Y" : "N")<<"' !"<<std::endl;
    return false;
}


string mapreportQuestion() {
std::string tmpInput;
	do {
		
        try {
            std::cout << "\nPlease enter OUPUT map report file's name (current: "<< MapReportFile<<"): ";
            getline(std::cin, tmpInput);

            // Validate the filename using regex
            RegexValidate(tmpInput,R"(^[^\s]+\.txt$)");
            break; // Exit loop if filename is valid

        } catch (const std::invalid_argument& e) {
            std::cout << "\n" << e.what();
        }
    } while (true); // Keep asking until valid input is provided
    MapReportFile = tmpInput;
    std::cout<< "OUTPUT report filename successfully set to '"<< MapReportFile<<"' !"<<std::endl;
	return MapReportFile;
}


string routereportQuestion() {
std::string tmpInput;
	do {
          try {
              std::cout << "\nPlease enter OUTPUT route report filename (current: "<< RouteReportFile <<"): ";
              getline(std::cin, tmpInput);

              RegexValidate(tmpInput,R"(^[^\s]+\.txt$)");
              break; 
        } catch (const std::invalid_argument& e) {
            std::cout << "\n" << e.what();
        }
    } while (true); 
    RouteReportFile  = tmpInput;
    std::cout<< "OUTPUT route report filename successfully set to '"<< RouteReportFile <<"' !"<<std::endl;
	return RouteReportFile;
}


string routecriteriaQuestion() {
  std::string tmpChoice;
  do {
    std::cout << "\n[ Specify Ideal Route Criteria ( current : " << RouteCriteria << " ) ]\n" << std::endl;
    std::cout << "a) minimized total energy expenditure" << std::endl;
    std::cout << "b) shortest distance from Start to End locations" << std::endl;
    std::cout << "c) go back to sub menu\n" << std::endl;
    std::cout << "Please enter your choice (a – c): ";

    std::getline(std::cin, tmpChoice);

    try {
      RegexValidate(tmpChoice, "^[a-cA-C]$");
      break;  
    } catch (const std::invalid_argument& e) {
      std::cout << "\n" << e.what();
    }
  } while (true);
  char choice_letter = tolower(tmpChoice[0]);
  
  switch (choice_letter) {
  case 'a':
    RouteCriteria = "minimized total energy expenditure";
    std::cout << "\nIdeal Route Criteria successfully set to '" << RouteCriteria << "'!\n";
    break;
  case 'b':
    RouteCriteria = "shortest distance from Start to End locations";
    std::cout << "\nIdeal Route Criteria successfully set to '" << RouteCriteria << "'!\n";
    break;
  case 'c':
    std::cout << "\nGoing back to sub-menu...\n";
    ConfigTerrExpSet();
    break;
  default:
    std::cout << "\nInvalid choice. This should never happen.\n";
  }
  routecriteriaQuestion();
  return RouteCriteria;
}


string vehicletypeQuestion() {
  std::string tmpChoice;
  do {
    std::cout << "\n[ Specify Vehicle Type ( current : " << VehicleType << " ) ]\n" << std::endl;
    std::cout << "a) HighLander (Hill h, Mountain M)" << std::endl;
    std::cout << "b) DragonFly (Swamp w, Water ~)" << std::endl;
    std::cout << "c) ShieldHero (Danger area X)" << std::endl;
    std::cout << "d) go back to sub menu\n" << std::endl;
    std::cout << "Please enter your choice (a – d): ";

    std::getline(std::cin, tmpChoice);

    try {
      RegexValidate(tmpChoice, "^[a-dA-D]$");
      break;  
    } catch (const std::invalid_argument& e) {
      std::cout << "\n" << e.what();
    }
  } while (true);
  char choice_letter = tolower(tmpChoice[0]);  
  
  switch (choice_letter) {
  case 'a':
    VehicleType = "HighLander";
    std::cout << "\nVehicle Type successfully set to '" << VehicleType << "'!\n";
    break;
  case 'b':
    VehicleType = "DragonFly";
    std::cout << "\nVehicle Type successfully set to '" << VehicleType << "'!\n";
    break;
  case 'c':
    VehicleType = "ShieldHero";
    std::cout << "\nVehicle Type successfully set to '" << VehicleType << "'!\n";
    break;
  case 'd':
    std::cout << "\nGoing back to sub-menu...\n";
    ConfigTerrExpSet();
    break;
  default:
    std::cout << "\nInvalid choice. This should never happen.\n";
  }  
  vehicletypeQuestion();
  return VehicleType;
}


bool booldisplayterminalQuestion() {
  std::string tmpChoice;
    do {
        try {
            std::cout << "\nWhen mapping ends, do you wish to display map report on Ubuntu terminal? (Y/N): ";
            getline(std::cin, tmpChoice);

            size_t first = tmpChoice.find_first_not_of(" \t");
            size_t last = tmpChoice.find_last_not_of(" \t");
            if (first != std::string::npos && last != std::string::npos) {
                tmpChoice = tmpChoice.substr(first, (last - first + 1));
            }

            RegexValidate(tmpChoice, "^[yYnN]$");
            char response = tolower(tmpChoice[0]);

            if (response == 'y') {
                return true;
            } else if (response == 'n') {
                return false;
            }
            break; // Exit loop on valid input
        }
        catch (const std::invalid_argument& e) {
            std::cout << "\n" << e.what();
        }

    } while (true); 
    std::cout<< "Something ERROR"<<std::endl;
    return false; 
}


bool boolstartQuestion() {
  std::string tmpChoice;
    do {
        try {
            std::cout << "\nBEGIN NOW? (Y/N): ";
            getline(std::cin, tmpChoice);

            size_t first = tmpChoice.find_first_not_of(" \t");
            size_t last = tmpChoice.find_last_not_of(" \t");
            if (first != std::string::npos && last != std::string::npos) {
                tmpChoice = tmpChoice.substr(first, (last - first + 1));
            }

            RegexValidate(tmpChoice, "^[yYnN]$");
            char response = tolower(tmpChoice[0]);

            if (response == 'y') {
                return true;
            } else if (response == 'n') {
                return false;
            }
            break; // Exit loop on valid input
        }
        catch (const std::invalid_argument& e) {
            std::cout << "\n" << e.what();
        }
    } while (true); 
    std::cout<< "Something ERROR"<<std::endl;
    return false;
}


vector<string> loadMap(const string &filename) {
    ifstream file(filename);
    //vector<string> map;
    string line;
    while (getline(file, line)) {
        presimulatemap.push_back(line);
    }
    return presimulatemap;
}


vector<string> extractMap(const string &filename) {
    ifstream file(filename);
    //vector<string> map;
    string line;
    bool mapStarted = false;

    while (getline(file, line)) {
        // Detect start of map by looking for the first row with numbers
        if (!mapStarted && line.find("0  1  2") != string::npos) {
            mapStarted = true; // The next lines contain the map
            continue; // Skip this line
        }

        // Detect end of map (when terrain info starts)
        if (mapStarted && line.find("Terrain Symbol") != string::npos) {
            break; // Stop reading
        }

        // Process and clean the map row
        if (mapStarted) {
            stringstream ss(line);
            string index, rowContent;
            ss >> index; // Remove row number
            getline(ss, rowContent); // Get the rest of the row
            if (!rowContent.empty()) {
                presimulatemap.push_back(rowContent.substr(1)); // Remove leading space
            }
        }
    }

    return presimulatemap;
}


void printMap(const vector<string> &simulatemap) {
    for (const string &row : simulatemap) {
      for (char cell : row) {
        std::cout << cell;
      }
      std::cout << endl;
    }
}


void printMapWithCoordinates(const vector<string> &mapData) {
    for (size_t row = 0; row < mapData.size(); ++row) {
        for (size_t col = 0; col < mapData[row].size()-1; ++col) {
            cout << "(" << row << "," << col << "," << mapData[row][col] << ")";
            if (col < mapData[row].size() - 1) {
                cout << ", ";
            }
        }
        cout << "\n\n"; // Add a blank line between rows
    }
}


vector<string> extractEveryThird(const vector<string> &mapData) {
    for (const string &row : mapData) {
        string extractedRow; // Store the extracted columns from this row

        for (size_t col = 1; col < row.size(); col += 3) { // Start at index 1, increment by 3
            extractedRow += row[col]; // Add the character to the extracted row
        }

        simulatemap.push_back(extractedRow); // Store the modified row in the new vector
    }

    return simulatemap;
}


void printCharacterLocations(const vector<string> &mapData, char target) {
    vector<pair<int, int>> locations;

    // Iterate through the map to find target characters
    for (size_t row = 0; row < mapData.size(); ++row) {
        for (size_t col = 0; col < mapData[row].size(); ++col) {
            if (mapData[row][col] == target) {
                locations.emplace_back(row, col);
            }
        }
    }

    // Print locations
    cout << "Locations of '" << target << "':" << endl;
    for (const auto &loc : locations) {
        cout << "(" << loc.first << "," << loc.second << ")" << endl;
    }
    cout << endl; // Add space for readability
}


std::map<char, TerrainInfo> captureTerrainData(const std::string& filename) {
    std::map<char, TerrainInfo> terrainData;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file." << std::endl;
        return terrainData;
    }
    
    std::string line;
    bool startReading = false;
    std::regex pattern("'(.?)'\\s+(\\d+)\\s+(\\d+)");
    std::smatch match;
    
    while (std::getline(file, line)) {
        if (line.find("Terrain Symbol") != std::string::npos) {
            startReading = true;
            std::getline(file, line); // Skip the header separator line
            continue;
        }
        
        if (startReading && std::regex_search(line, match, pattern)) {
            char symbol = match[1].str().empty() ? ' ' : match[1].str()[0];
            int movtEnergy = std::stoi(match[2].str());
            int shldEnergy = std::stoi(match[3].str());
            terrainData[symbol] = {movtEnergy, shldEnergy};
        }
    }
    
    file.close();
    return terrainData;
}


int findSinglePath(vector<string> simulatemap, pair<int, int> start, pair<int, int> end) {
    static int routeCount = 1;
    int rows = simulatemap.size();
    int cols = simulatemap[0].size();

    queue<Node> q;
    vector<vector<int>> dist(rows, vector<int>(cols, numeric_limits<int>::max()));
    vector<vector<pair<int, int>>> parent(rows, vector<pair<int, int>>(cols, {-1, -1}));

    // Initialize BFS from the specific 'S'
    q.push({start.first, start.second, 0});
    dist[start.first][start.second] = 0;

    bool pathFound = false;

    while (!q.empty()) {
        Node curr = q.front();
        q.pop();

        if (curr.x == end.first && curr.y == end.second) {
            pathFound = true;
            break;
        }

        for (int i = 0; i < 4; i++) {
            int newX = curr.x + dx[i];
            int newY = curr.y + dy[i];

            if (newX >= 0 && newX < rows && newY >= 0 && newY < cols &&
                simulatemap[newX][newY] != '#' && dist[newX][newY] > curr.dist + 1) {
                
                dist[newX][newY] = curr.dist + 1;
                parent[newX][newY] = {curr.x, curr.y};
                q.push({newX, newY, curr.dist + 1});
            }
        }
    }

    if (!pathFound) {
        cout << "No Path Found between S(" << start.first << "," << start.second 
             << ") and E(" << end.first << "," << end.second << ")!\n";
        return -1;
    }

    // Create a matrix to track spaces and characters on the path
    vector<vector<bool>> pathSpaces(rows, vector<bool>(cols, false));
    vector<string> outputMap(rows, string(cols, ' '));  // Start with all empty spaces

    // Backtrack to mark the correct route and collect the path sequence
    vector<pair<int, int>> pathSequence;
    pair<int, int> current = end;

    while (current != start) {
        pathSequence.push_back(current);
        pair<int, int> prev = parent[current.first][current.second];
        if (prev.first == -1 && prev.second == -1) break;

        // If it's a letter, '~', or '#', keep it
        if (isalpha(simulatemap[current.first][current.second]) || 
            simulatemap[current.first][current.second] == '~' || 
            simulatemap[current.first][current.second] == '#') {
            outputMap[current.first][current.second] = simulatemap[current.first][current.second];
        }
        // If it's a space, mark it for white background
        else if (simulatemap[current.first][current.second] == ' ') {
            pathSpaces[current.first][current.second] = true;
        }

        current = prev;
    }

    pathSequence.push_back(start);
    reverse(pathSequence.begin(), pathSequence.end());

    // Keep 'S' and 'E' positions
    outputMap[start.first][start.second] = 'S';
    outputMap[end.first][end.second] = 'E';

    // Copy all walls (#) into the output
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (simulatemap[i][j] == '#') {
                outputMap[i][j] = '#';
            }
        }
    }
    
    cout << "\nRoute 0"<< routeCount++<<" - ";

    // Display the route sequence
    cout << "From [" << start.first << "," << start.second
         << "] to [" << end.first << "," << end.second << "] \n"<<endl;

    cout << "Sequence: \n";
    for (size_t i = 0; i < pathSequence.size() - 1; i++) {
        cout << "[" << pathSequence[i].first << "," << pathSequence[i].second << "] -> ";
    }
    cout << "[" << pathSequence.back().first << "," << pathSequence.back().second << "]\n"<<endl;
    
    cout<< "Tot. No. of grid area travelled (excluding 'S' and 'E') = "<<dist[end.first][end.second]-1<<"\n"<<endl;

    // **1) Print column numbering with proper alignment**
    cout << "    ";  // Space to align with row numbers
    for (int j = 0; j < cols; j++) {
        if (j> 9) {
          cout << j<< " ";
        }
        else {
          cout << " "<< j<< " "; 
        }
    }
    cout << endl;

    // **2) Print each row, starting with row number, then the contents**
    for (int i = 0; i < rows; i++) {
        cout << setw(3) << i << " ";  // Row number, properly spaced

        for (int j = 0; j < cols; j++) {
            cout << " ";  // Space before each character
            if (outputMap[i][j] == 'S' || outputMap[i][j] == 'E' ||
                isalpha(outputMap[i][j]) || outputMap[i][j] == '~' || outputMap[i][j] == '#') 
            {
                cout << outputMap[i][j];
            } 
            // If it's on the shortest path and originally a space, show a white background
            else if (pathSpaces[i][j]) {
                cout << WHITE_BG << " " << RESET;
            } 
            else {
                cout << " ";
            }
            cout << " ";  // Space after each character to match alignment
        }
        cout << endl;
    }

    return dist[end.first][end.second];
}

void findAllPaths(vector<string> simulatemap) {
    int rows = simulatemap.size();
    int cols = simulatemap[0].size();
    
    // Locate all 'S' (start positions) and 'E' (end positions)
    vector<pair<int, int>> starts, ends;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (simulatemap[i][j] == 'S') starts.push_back({i, j});
            if (simulatemap[i][j] == 'E') ends.push_back({i, j});
        }
    }

    // Find shortest path for each S-E pair
    for (const auto &s : starts) {
        for (const auto &e : ends) {
            findSinglePath(simulatemap, s, e);
        }
    }
}

void AutoPilotMenu() {  //marking
  VehicleDetails vd;
  SecondVehicleDetails svd(vd);
  const int horizontal = 45;
  const int vertical = 30;
    
  bool BoolDisplayTerminal;
  bool BoolStartRun;
  std::cout << "\n[Start Autopilot Mapping]\n"<< std::endl;
  std::cout << "INPUT scenario file name = " << ScenarioFile << std::endl;
  std::cout << "INPUT sceario file encrypted =" << (BoolDecryption ? "Y" : "N") << std::endl;
  std::cout << "OUTPUT map report filename = " << MapReportFile << std::endl;
  BoolDisplayTerminal = booldisplayterminalQuestion();
  BoolStartRun = boolstartQuestion();
  
  if (BoolStartRun == false ) {
    std::cout << "\nStop Autopilot Mapping! Retrun to main menu ..."<<std::endl;
    MainMenu();
  }
  
    // hide the output to the terminal [START]
    std::streambuf* oldCout = std::cout.rdbuf();
    std::ofstream nullStream("/dev/null"); 
    std::cout.rdbuf(nullStream.rdbuf());
    FILE* originalStdout = stdout;
    stdout = fopen("/dev/null", "w");
    
    vd.PrePareVehicle(srcFileBranch+ScenarioFile, PreDecryption, PreRandomize, PreMissionType);
    vd.CreateAndDisplayMap(horizontal, vertical);
  
    svd.SecondPrepareVehicle(srcFileBranch+ScenarioFile, BoolDecryption, randomizeStartPosition, PreMissionType);
    svd.CornerCheck();	
    
    // hide the output to the terminal [FINSIH]
    std::cout.rdbuf(oldCout);	// code turn output on 
    fclose(stdout);				// code turn output on
    stdout = originalStdout;	// code turn output on

    svd.AutoMapping(ScenarioFile, BoolDecryption, MapReportFile, BoolDisplayTerminal);
    std::cout<<std::endl;
    //svd.printArray();
}


void SimulationMenu() { // third marking
  VehicleDetails vd;
  SecondVehicleDetails svd(vd);
  std::map<char, TerrainInfo> terrain;
  terrain = captureTerrainData(MapReportFile);
  
  std::cout << "\n[Start Simulation Route]\n"<< std::endl;
  std::cout << "INPUT map details filename  = " << MapReportFile << std::endl;
  std::cout << "OUTPUT route report         = " << RouteReportFile << std::endl;
  std::cout << "Ideal Route Criteria        = " << RouteCriteria << std::endl;
  std::cout << "Vehicle Type                = " << VehicleType << std::endl;
  std::cout<<std::endl;
  std::cout << "Start datetime stamp          : " << svd.getFormattedTimestamp() << std::endl;
  
  std::cout << "Captured Terrain Data:\n";
  for (const auto& entry : terrain) {
    std::cout << "Symbol: " << (entry.first == ' ' ? "[space]" : std::string(1, entry.first)) 
                  << ", Movement Energy: " << entry.second.movementEnergy
                  << ", Shield Energy: " << entry.second.shieldEnergy << std::endl;
  }

  //presimulatemap = loadMap(MapReportFile); 
  presimulatemap = extractMap(MapReportFile);
  //printMap(presimulatemap); 
  //printMapWithCoordinates(presimulatemap);
  extractEveryThird(presimulatemap);
  //printMap(simulatemap); 
  //findShortestPath(simulatemap);
  findAllPaths(simulatemap);
  //printCharacterLocations(simulatemap, 'S');
  //printCharacterLocations(simulatemap, 'E');
}


void ConfigTerrExpSet() {
    std::string choice; // Use string for input validation

    std::cout << "\n-------------------------------------------------\n" << std::endl;
    std::cout << "[ Configure Terrain Exploration Simulator settings ]\n" << std::endl;
    std::cout << "a) Specify INPUT map details filename ( current: " << MapReportFile << ")" << std::endl;
    std::cout << "b) Specify OUTPUT route report filename (current: " << RouteReportFile << ")" << std::endl;
    std::cout << "c) Specify ideal route criteria (current: " << RouteCriteria << ")" << std::endl;
    std::cout << "d) Specify vehicle type (current: " << VehicleType << ")" << std::endl;
    std::cout << "e) Back to main menu\n" << std::endl;

    while (true) {
        std::cout << "Please enter your choice: ";
        std::getline(std::cin, choice);

        try {
            // Validate input: Must be a single character from a to e (case insensitive)
            RegexValidate(choice, "^[a-eA-E]$");
            break;
        } catch (const std::invalid_argument& e) {
            std::cout << e.what() ;
        }
    }

    // Convert input to lowercase for consistency
    char choice_letter = tolower(choice[0]);

    // Handle menu options
    switch (choice_letter) {
        case 'a':
            std::cout << "\nNew input for INPUT map details filename..." << std::endl;
            break;
        case 'b':
            RouteReportFile = routereportQuestion();
            break;
        case 'c':
            RouteCriteria = routecriteriaQuestion();
            break;
        case 'd':
            VehicleType = vehicletypeQuestion();
            break;
        case 'e':
            std::cout << "\nReturning to Main Menu..." << std::endl;
            MainMenu(); // Go back to main menu
            return;
        default:
            std::cout << "\nInvalid choice. Please try again." << std::endl;
    }
    ConfigTerrExpSet();
}


void AutoMappingSettings() {
    std::string userInput;
    int autoMappingChoice = 0;
    const std::string validPattern = "^[a-dA-D]$"; // Only allows 'a', 'b', 'c' (case insensitive)

    do {
        try {
            std::cout << "\n[Configure Autopilot Mapping Exploration settings]\n" << std::endl;
            std::cout << "a) Specify INPUT scenario filename (current: " << ScenarioFile << ")" << std::endl;
            std::cout << "b) Specify INPUT scenario file encrypted (current: " 
                      << (BoolDecryption ? "Y" : "N") << ")" << std::endl;
            std::cout << "c) Specify OUTPUT map report filename (current: " << MapReportFile << ")" << std::endl;
            std::cout << "d) Back to main menu"<<std::endl;
            std::cout << "\nEnter your choice (a-d): ";
            getline(std::cin, userInput);

            size_t first = userInput.find_first_not_of(" \t");
            size_t last = userInput.find_last_not_of(" \t");
            if (first != std::string::npos && last != std::string::npos) {
                userInput = userInput.substr(first, (last - first + 1));
            }

            RegexValidate(userInput, validPattern);

            char choice = tolower(userInput[0]); 
            autoMappingChoice = (choice == 'a') ? 1 :
                                (choice == 'b') ? 2 :
                                (choice == 'c') ? 3 : 4;

            break; // exit loop

        } catch (const std::invalid_argument& e) {
            std::cout << "\n" << e.what() << "Please enter 'a', 'b', 'c' or 'd' only.\n";
        }
    } while (true);

    switch (autoMappingChoice) {
        case 1:
            ScenarioFile = filenameQuestion();
            AutoMappingSettings();
            return;
        case 2:
            BoolDecryption = decrypQuestion();
            AutoMappingSettings();
            break;
        case 3:
            MapReportFile = mapreportQuestion();
            AutoMappingSettings();
            break;
        case 4:
            std::cout << "\nGoing back to main menu ...\n"<<std::endl;
            MainMenu();
            break;
        	
        default:
            std::cout << "\nInvalid choice. This should never happen." << std::endl;
    }
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
                ConfigTerrExpSet();
                break;
            case 3:
                AutoPilotMenu();
                MainMenu();
                break;
            case 4:
                SimulationMenu();
            	break;
            case 5:
                std::cout << "\nExiting the program." << std::endl;
                exit(0);
            default:
                std::cout << "\nInvalid choice. Please try again." << std::endl;
        }
}


