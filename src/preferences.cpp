#include "global.hpp"
#include <fstream>
#include <filesystem>

const std::string preferencesDefault[2] = {"[Settings]", "music = 1"};

void InitCfg() {
    // Create preferences.cfg if it doesn't exist.
    if (!std::filesystem::exists("preferences.cfg")) {
        std::ofstream currentFileO ("preferences.cfg");
        if (!currentFileO.is_open()) {
            std::cout << "Unable to create preferences.cfg" << "\n";
            closeGame = true;
            return;   
        }
        for (int i = 0; i < size(preferencesDefault); i++) {
            currentFileO << preferencesDefault[i] << "\n";
        }
        currentFileO.close();
        std::cout << "Created preferences.cfg with default values" << std::endl;
    }

    // Reads the content of preferences.cfg and sets the values accordingly.
    std::string currentLine;
    std::ifstream currentFileI ("preferences.cfg");
    if (!currentFileI.is_open()) {
        std::cout << "Unable to open preferences.cfg" << "\n";
        closeGame = true;
        return;   
    }
    while (std::getline(currentFileI, currentLine)) {
        // Music
        if (currentLine.find("music") != std::string::npos) {
            if (currentLine.substr(currentLine.find("=") + 2, currentLine.find("=") + 3) == "0") {
                musicOn = false;
            } else {
                musicOn = true;
            }
        }
    }
    currentFileI.close();
    std::cout << "Loaded setting preferences" << "\n";
}

void CfgUpdate(std::string file, std::string setting, std::string value) {
    std::ifstream currentFileI (file);
    if (!currentFileI.is_open()) {
        std::cout << "Unable to open " + file << "\n";
        closeGame = true;
        return;   
    }

    std::string currentLine;
    std::vector<std::string> newText; 
    while (std::getline(currentFileI, currentLine)) {
        if (currentLine.find(setting) == std::string::npos) {
            newText.push_back(currentLine);
        } else {
            newText.push_back(setting + " = " + value);
        }
    }
    currentFileI.close();
    std::ofstream currentFileO (file, std::ios::out | std::ios::trunc);
    for (std::string i: newText) {
        currentFileO << i << "\n";
    }
    currentFileO.close();
    std::cout << "Updated " + file << "\n";
}