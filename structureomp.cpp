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