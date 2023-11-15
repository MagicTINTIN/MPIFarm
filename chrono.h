#ifndef CHRONO_H_INCLUDED
#define CHRONO_H_INCLUDED

#include <chrono>
#include <string>

class execchrono
{
private:
    std::chrono::duration<double> duration;
    std::chrono::time_point<std::chrono::steady_clock> beginTime;
    std::chrono::time_point<std::chrono::steady_clock> lapTime;

public:
    execchrono();
    /**
     * Output time since last lap
     * @param msg Message to print with (optionnal)
     */
    void lap(std::string msg = "");
    /**
     * Output time since last start
     * @param msg Message to print with (optionnal)
     */
    void total(std::string msg = "");
};

#endif // CHRONO_H_INCLUDED