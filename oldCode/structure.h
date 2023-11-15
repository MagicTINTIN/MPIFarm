#ifndef STRUCTURE_H_INCLUDED
#define STRUCTURE_H_INCLUDED

#include <iostream>
#include <string>
#include <algorithm>
#include <mutex>
#include <condition_variable>

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

class DataMonitor
{
public:
	DataMonitor();

	void addItem(imageStruct item);

	imageStruct removeItem();

	void displayItems();

private:
	std::mutex lock;
	std::condition_variable cv;
	int itemNb;
	imageStruct itemArr[DATA_ARR_SIZE];
};

class ResMonitor
{
public:
	ResMonitor();

	void addItem(imageStruct item);

	imageStruct removeItem();

	void displayItems();

	void exportItems(std::string path);

private:
	std::mutex lock;
	std::condition_variable cv;
	int itemNb;
	imageStruct itemArr[RES_ARR_SIZE];
};

#endif // STRUCTURE_H_INCLUDED