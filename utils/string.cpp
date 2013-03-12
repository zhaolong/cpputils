#include "config.h"
#include "string.h"

namespace Utils {

void String::LeftShift(char* buf, size_t num)
{
    if(String::IsEmpty(buf) || num == 0)
        return;
    size_t len = strlen(buf) + 1;
    if(num >= len)
        buf[0] = '\0';
    for (size_t i = 0; i < (len - num); i++)
        buf[i] = buf[i+num];

    return;
}

void String::Replace(std::string& str, const char* bereplaced, const char* replaceto)
{
    if(String::IsEmpty(bereplaced) || String::IsEmpty(replaceto))
        return;

    size_t pos = 0;
    size_t bereplaced_len = strlen(bereplaced);
    size_t replaceto_len = strlen(replaceto);

    while ((pos = str.find(bereplaced, pos)) != std::string::npos) 
    {
        str.replace(pos, bereplaced_len, replaceto);
        pos += replaceto_len;
    }

    return;
}

void String::Replace(char* str, const char* bereplaced, const char* replaceto)
{
    if(String::IsEmpty(str) || String::IsEmpty(bereplaced) || String::IsEmpty(replaceto))
        return;

    size_t bereplaced_len = strlen(bereplaced);
    size_t replaceto_len = strlen(replaceto);
    if(bereplaced_len < replaceto_len)
        return;

    char* pos = str;
    while ((pos = strstr(pos, bereplaced)) != NULL)
    {
        memcpy(pos, replaceto, replaceto_len);
        pos += replaceto_len;
        String::LeftShift(pos, bereplaced_len - replaceto_len);
    }

    return;
}

void String::Split(String::Vector& result, const char* input, const char* delimiter)
{
    if(String::IsEmpty(input) || IsEmpty(delimiter) || strstr(input, delimiter) == NULL)
    {
        result.push_back(std::string(input));
        return;
    }

    size_t delimiterLen = strlen(delimiter);

    const char *pos0 = input, *pos1 = input;
    while ((pos1 = strstr(pos1, delimiter)) != NULL)
    {
        result.push_back(std::string(pos0,pos1));
        pos1 += delimiterLen;
        pos0 = pos1;
    }

    result.push_back(std::string(pos0, input + strlen(input)));

    return;
}

bool String::Equal(const char* str1, const char* str2, bool icase /* = true */, size_t len /* = 0 */)
{
    if(!str1 || !str2 || len > strlen(str1) || len > strlen(str2))
        return false;

    if(0 == len)
    {
        if(strlen(str1) != strlen(str2))
            return false;
        len = strlen(str1);
    }

    char a = *str1, b = *str2;
    if(icase)
    {
        while((len--) && (a = *str1++) && (b = *str2++))
        {
            if(a != b)
            {
                if(a >= 'A' && a <= 'Z' && b == (a + 0x20))
                    continue;
                if(b >= 'A' && b <= 'Z' && a == (b + 0x20))
                    continue;
                else
                    break;
            }
        }
    }
    else
        while((len--) && (a = *str1++) && (b = *str2++) && a == b)
            ;

    len++;

    return len == 0;
}

void String::RemoveBlankSuffix(std::string& str)
{
    for (size_t i = str.length() - 1; i >= 0; i--)
    {
        if(str[i] == ' ' || str[i] == '\r' || str[i] == '\n' || str[i] == '\t')
            str.erase(i);
        else
            break;
    }
}

void String::RemoveBlanks(std::string& str)
{
    for (std::string::iterator iter = str.begin(); iter != str.end();)
    {
        if(*iter == ' ' || *iter == '\r' || *iter == '\n' || *iter == '\t')
            iter = str.erase(iter);
        else
            iter++;
    }
}

const char* String::Find(const char* str, const char* sub, bool icase)
{
    if(IsEmpty(str) || IsEmpty(sub))
        return NULL;

    size_t strLen = strlen(str);
    size_t subLen = strlen(sub);

    if(subLen > strLen)
        return NULL;

    const char *start = str;
    const char *end = start + (strLen - subLen);

    if(icase)
    {
        while(start <= end)
        {
            if(_memicmp(start, sub, subLen) == 0)
                return start;
            start++;
        }
    }
    else
    {
        while(start <= end)
        {
            if(memcmp(start, sub, subLen) == 0)
                return start;
            start++;
        }
    }
    
    return NULL;
}

} /* namespace Utils */