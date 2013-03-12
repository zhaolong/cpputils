#ifndef _utils_STRING_H_
#define _utils_STRING_H_

#include <string>
#include <vector>
#include <sstream>

namespace Utils {

class String {
public:
    typedef std::vector<std::string> Vector;

    static void LeftShift(char* buf, size_t num);

    static void Replace(std::string& str, const char* bereplaced, const char* replaceto);
    static void Replace(char* str, const char* bereplaced, const char* replaceto);

    static void Split(Vector& result, const char* input, const char* delimiter);
    static bool Equal(const char* str1, const char* str2, bool icase = true, size_t len = 0);

    static bool StartsWith(const char* str, const char* that, bool icase = true) {
        if(!str || !that)
            return false;
        return String::Equal(str, that, icase, strlen(that));
    }
    static bool EndsWith(const char* str, const char* that, bool icase = true) {
        if(!str || !that)
            return false;
        int len = strlen(str) - strlen(that);
        return len >= 0 ? String::Equal(str + len, that, icase, strlen(that)) : false;
    }

    static bool IsEmpty(const char* str) {
        return (str == NULL) || (*str == '\0');
    }
    
    static void RemoveBlanks(std::string& str);
    static void RemoveBlankSuffix(std::string& str);

    static const char* Find(const char* str, const char* sub, bool icase);

};


template<typename outype, typename intype>
static outype Convert(intype in) {
    std::stringstream ss;
    ss.imbue(std::locale("C"));
    ss<<in;
    outype out;
    ss>>out;
    return out;
}
static std::string ToString(int num) {
    return Convert<std::string>(num);
}

} /* namespace Utils */

#endif /* _utils_STRING_H_ */