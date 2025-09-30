#include "VersionUtils.hpp"
#include "../stringutils/StringUtils.hpp"

// v1 >= v2 -> true, v1 < v2 -> false
bool VersionUtils::isVersionGreaterOrEqual(const std::string& v1, const std::string& v2) {
    auto removeV = [](const std::string& v) -> std::string {
        if (!v.empty() && v[0] == 'v') return v.substr(1);
        return v;
    };

    auto splitVersion = [](const std::string& v, std::vector<int>& nums, std::string& pre) {
        std::string core = v;
        size_t dashPos = v.find('-');
        if (dashPos != std::string::npos) {
            core = v.substr(0, dashPos);
            pre = v.substr(dashPos + 1);
        }

        auto parts = StringUtils::split(core, '.');
        for (const auto& p : parts) {
            try {
                nums.push_back(std::stoi(p));
            } catch (...) {
                nums.push_back(0);
            }
        }
    };

    std::vector<int> n1, n2;
    std::string pre1, pre2;
    splitVersion(removeV(v1), n1, pre1);
    splitVersion(removeV(v2), n2, pre2);

    // compara partes numéricas
    for (size_t i = 0; i < 3; ++i) {
        int a = i < n1.size() ? n1[i] : 0;
        int b = i < n2.size() ? n2[i] : 0;
        if (a > b) return true;
        if (a < b) return false;
    }

    // se chegou aqui, partes numéricas são iguais
    if (pre1.empty() && !pre2.empty()) return true;
    if (!pre1.empty() && pre2.empty()) return false;

    if (!pre1.empty() && !pre2.empty()) {
        return pre1 >= pre2;  // aqui usamos >= para tratar igualdade
    }

    // se não houver pré-release ou ambos iguais, retorna true
    return true;
}
