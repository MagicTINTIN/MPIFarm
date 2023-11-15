#ifndef STRUCTUREOMP_H_INCLUDED
#define STRUCTUREOMP_H_INCLUDED

#include <iostream>
#include <string>
#include <algorithm>

int const DATA_ARR_SIZE(12);
int const RES_ARR_SIZE(25);

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

class ResMonitor
{
public:
	ResMonitor();

	void addItem(imageStruct item);

	imageStruct removeItem();

	void displayItems();

	void exportItems(std::string path);

private:
	int itemNb;
	imageStruct itemArr[RES_ARR_SIZE];
};

#endif // STRUCTUREOMP_H_INCLUDED