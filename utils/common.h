#ifndef _utils_COMMON_H_
#define _utils_COMMON_H_

#include <ctime>

namespace Utils {

void Sleep(unsigned long miliseconds);

class Timer
{
public:
    Timer()
    {
        m_start=clock();
    }
    size_t Count() /* miliseconds */
    {
        m_end=clock(); 
        size_t count = (size_t)(float(m_end - m_start)/CLOCKS_PER_SEC*1000);
        return count;
    }
    ~Timer()
    {
    }
private:
    clock_t   m_start;
    clock_t   m_end;
};

} /* namespace Utils */

#endif /* _utils_COMMON_H_ */
