#ifndef STRUCTURE_H_INCLUDED
#define STRUCTURE_H_INCLUDED

#include <iostream>
#include <string>
#include <algorithm>

struct imageStruct
{
	int rank;
	std::string path;
	float averageValue;
	imageStruct();
	imageStruct(int imageRank, std::string imagePath, float imageAverageValue);
	bool operator==(imageStruct const &cmp);
	bool operator!=(imageStruct const &cmp);
};

static const imageStruct EMPTY_IMAGE = imageStruct(-1, "", 0);
static const imageStruct END_WORKERS = imageStruct(-2, "!END!", 0);

bool compareImgStruct(imageStruct const &a, imageStruct const &b);

#endif // STRUCTURE_H_INCLUDED