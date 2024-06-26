#ifndef INI_CONFIGURATION_MANAGER_HPP
#define INI_CONFIGURATION_MANAGER_HPP

#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include "StringUtil.hpp"

class IniConfigurationManager{
    private:
        std::map<std::string, std::map<std::string, std::string>> data;
    public:
        IniConfigurationManager(std::string filename){
            std::ifstream file(filename);
            if(!file.is_open()){
                std::cerr << "Error: Could not open the file." << std::endl;
                return;
            }
            std::string line;
            std::string currentSection;
            while(std::getline(file, line)){
                if(line.empty() || line[0] == ';') continue;
                if(line[0] == '['){
                    currentSection = trim(line.substr(1, line.size() - 2));
                    continue;
                }
                auto delimiter = line.find('=');
                if(delimiter == std::string::npos) continue;
                auto key = trim(line.substr(0, delimiter));
                auto value = trim(line.substr(delimiter + 1));
                data[currentSection][key] = value;
            }
        }
        std::string getString(std::string section, std::string key, std::string defaultValue){
            section = trim(section);
            key = trim(key);
            if(data[section].find(key) == data[section].end()) return defaultValue;
            std::string value = trim(data[section][key]);
            return value;
        }
        bool getBool(std::string section, std::string key, bool defaultValue){
            std::string value = getString(section, key, defaultValue ? "true" : "false");
            return value == "true";
        }
        float getFloat(std::string section, std::string key, float defaultValue){
            std::string value = getString(section, key, std::to_string(defaultValue));
            return std::stof(value);
        }
        int getInt(std::string section, std::string key, int defaultValue){
            std::string value = getString(section, key, std::to_string(defaultValue));
            return std::stoi(value);
        }
};


#endif