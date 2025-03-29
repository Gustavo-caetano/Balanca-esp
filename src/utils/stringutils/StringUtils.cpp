#include "StringUtils.hpp"

std::vector<std::string> StringUtils::split(std::string text, char separator) {
    std::vector<std::string> result;
    size_t startIndex = 0;
    size_t separatorIndex = text.find(separator);

    while (separatorIndex != std::string::npos) {
        result.push_back(text.substr(startIndex, separatorIndex - startIndex));
        startIndex = separatorIndex + 1;
        separatorIndex = text.find(separator, startIndex);
    }

    result.push_back(text.substr(startIndex));

    return result;
}

std::string StringUtils::join(const std::vector<std::string>& vec, char separator) {
    std::string result = "";
    for (size_t i = 0; i < vec.size(); ++i) {
        result += vec[i];
        if (i < vec.size() - 1) {
            result += separator;
        }
    }
    return result;
}

std::string StringUtils::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, last - first + 1);
}
