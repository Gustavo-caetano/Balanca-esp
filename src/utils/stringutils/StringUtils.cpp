#include "StringUtils.hpp"

std::vector <String> StringUtils::split(String text, char separator){
    
    if(text.isEmpty())
    {
        return {""};
    }

    std::vector<String> result;
    int startIndex = 0;
    int separatorIndex = text.indexOf(separator);

    while (separatorIndex != -1) {
        result.push_back(text.substring(startIndex, separatorIndex));
        startIndex = separatorIndex + 1;
        separatorIndex = text.indexOf(separator, startIndex);
    }

    result.push_back(text.substring(startIndex));

    return result;
}

String StringUtils::join(const std::vector<String>& vec, char separator) {
    String result = "";
    for (size_t i = 0; i < vec.size(); ++i) {
        result += vec[i];
        if (i < vec.size() - 1) {
            result += separator;
        }
    }
    return result;
}
