#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <iomanip>
#include "structureomp.h"

/* STRUCTURE MONITOR METHODS */

imageStruct::imageStruct() : rank(0), path(""), averageValue(0.f)
{
}
imageStruct::imageStruct(int imageRank, std::string imagePath, float imageAverageValue) : rank(imageRank), path(imagePath), averageValue(imageAverageValue)
{
}

void replace_first(
    std::string &s,
    std::string const &toReplace,
    std::string const &replaceWith)
{
    std::size_t pos = s.find(toReplace);
    if (pos == std::string::npos)
        return;
    s.replace(pos, toReplace.length(), replaceWith);
}

bool compareImgStruct(imageStruct const &a, imageStruct const &b)
{
    return a.averageValue < b.averageValue;
}

bool imageStruct::operator==(imageStruct const &cmp)
{
    return (this->averageValue == cmp.averageValue && this->path == cmp.path && this->rank == cmp.rank);
}
bool imageStruct::operator!=(imageStruct const &cmp)
{
    return (this->averageValue != cmp.averageValue || this->path != cmp.path || this->rank != cmp.rank);
}

/* RES MONITOR METHODS */

ResMonitor::ResMonitor() : itemNb(0)
{
}

void ResMonitor::addItem(imageStruct item)
{
    if (itemNb < RES_ARR_SIZE)
    {
        // insert in sorted array
        int i = itemNb;
        for (; i > 0 && itemArr[i - 1].averageValue > item.averageValue; --i)
        {
            itemArr[i] = itemArr[i - 1];
            itemArr[i].rank = i;
        }
        itemArr[i] = item;
        itemArr[i].rank = i;
        itemNb++;
    }
    else
    {
        std::cout << "ERROR: Result monitor is already full !";
    }

}

imageStruct ResMonitor::removeItem()
{

    imageStruct rtnval;

    if (itemNb > 0)
    {
        itemNb--;
        rtnval = itemArr[itemNb];
    }
    else
        rtnval = EMPTY_IMAGE;

    return rtnval;
}

void ResMonitor::displayItems()
{
    std::cout << "RES MONITOR : " << std::endl
              << std::setw(4) << "RANK"
              << " | " << std::setw(20) << "Image path"
              << " | " << std::setw(15) << "Average value" << std::endl;
    for (size_t i = 0; i < itemNb; i++)
    {
        std::cout
            << std::setw(4) << itemArr[i].rank
            << " | " << std::setw(20) << itemArr[i].path
            << " | " << std::setw(15) << itemArr[i].averageValue << std::endl;
    }

    std::cout << std::endl;
}

void ResMonitor::exportItems(std::string path)
{
    replace_first(path, ".json", "_rez.txt");
    std::ofstream exportFlux(path, std::ios::app);
    exportFlux << "RES MONITOR : " << std::endl
              << std::setw(4) << "RANK"
              << " | " << std::setw(20) << "Image path"
              << " | " << std::setw(15) << "Average value" << std::endl;
    for (size_t i = 0; i < itemNb; i++)
    {
        exportFlux
            << std::setw(4) << itemArr[i].rank
            << " | " << std::setw(20) << itemArr[i].path
            << " | " << std::setw(15) << itemArr[i].averageValue << std::endl;
    }

    exportFlux << std::endl;
}