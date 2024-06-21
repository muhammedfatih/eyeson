#ifndef INI_CONFIGURATION_MANAGER_HPP
#define INI_CONFIGURATION_MANAGER_HPP

#include <string>
#include <map>
#include <fstream>
#include <iostream>

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
                    currentSection = line.substr(1, line.size() - 2);
                    continue;
                }
                auto delimiter = line.find('=');
                if(delimiter == std::string::npos) continue;
                auto key = line.substr(0, delimiter);
                auto value = line.substr(delimiter + 1);
                data[currentSection][key] = value;
            }
        }
        std::string getString(std::string section, std::string key, std::string defaultValue){
            if(data[section].find(key) == data[section].end()) return defaultValue;
            return data[section][key];
        }
        bool getBool(std::string section, std::string key, bool defaultValue){
            std::string value = getString(section, key, defaultValue ? "true" : "false");
            return value == "true";
        }
};


#endif