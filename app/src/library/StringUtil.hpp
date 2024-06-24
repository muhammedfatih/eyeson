#ifndef STRING_UTIL_HPP
#define STRING_UTIL_HPP
#include <string>

std::string trim(const std::string& section) {
    std::string trimmedSection = section;

    if (!trimmedSection.empty() && trimmedSection[0] == ' ') {
        trimmedSection = trimmedSection.substr(1);
    }

    if (!trimmedSection.empty() && trimmedSection[trimmedSection.size() - 1] == ' ') {
        trimmedSection = trimmedSection.substr(0, trimmedSection.size() - 1);
    }

    return trimmedSection;
}
#endif