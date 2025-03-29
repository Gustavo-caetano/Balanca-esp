#ifndef StringUtils_HPP
#define StringUtils_HPP

#include <Arduino.h>
#include <vector>

class StringUtils
{
private:
public:
    static std::vector <std::string> split(std::string text, char separator = ' ');
    static std::string join(const std::vector<std::string>& vec, char separator = ' ');
    static std::string trim(const std::string& str);
};

#endif // StringUtils_HPP