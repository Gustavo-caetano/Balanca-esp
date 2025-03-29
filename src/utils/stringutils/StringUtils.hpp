#ifndef StringUtils_HPP
#define StringUtils_HPP

#include <Arduino.h>
#include <vector>

class StringUtils
{
private:
public:
    static std::vector <String> split(String text, char separator = ' ');
    static String join(const std::vector<String>& vec, char separator = ' ');
};

#endif // StringUtils_HPP