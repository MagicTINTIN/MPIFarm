#include "chrono.h"
#include <iostream>
#include <chrono>
#include <string>

execchrono::execchrono() : beginTime(std::chrono::steady_clock::now()), lapTime(std::chrono::steady_clock::now())
{
}

void execchrono::lap(std::string msg)
{
    duration = std::chrono::steady_clock::now() - lapTime;
    std::cout << msg << duration.count() << "s" << std::endl;
    lapTime = std::chrono::steady_clock::now();
}

void execchrono::total(std::string msg)
{
    duration = std::chrono::steady_clock::now() - beginTime;
    std::cout << msg << duration.count() << "s" << std::endl;
}