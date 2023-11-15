#include <iostream>
#include <string>
#include "imageprocess.h"
#include "chrono.h"
#include <algorithm>
#include <fstream>
#include "includes/nlohmann/json.hpp"
#include <thread>
#include <functional>
#include "structure.h"

using json = nlohmann::json;
using namespace std;

int MAX_THREADS(5);

void processImg(imageStruct &img, ResMonitor &res)
{
	if (isImgGray(img.path))
	{
		rgb tmp = averageColorImg(img.path);
		img.averageValue = tmp.R;
		res.addItem(img);
	}
}

void worker(DataMonitor &monitor, ResMonitor &res)
{
	imageStruct img;
	img = monitor.removeItem();
	while (img != END_WORKERS)
	{
		processImg(img, res);
		img = monitor.removeItem();
	}
}
void addImage(json &data, DataMonitor &monitor)
{
	imageStruct img;
	for (const auto &item : data.items())
	{
		img = imageStruct(item.value()["rank"].get<int>(), item.value()["name"].get<std::string>(), item.value()["averageValue"].get<float>());
		monitor.addItem(img);
	}
	for (size_t i = 0; i < MAX_THREADS - 1; i++)
	{
		monitor.addItem(END_WORKERS);
	}
}

int main(int argc, char const *argv[])
{
	if (argc < 2)
	{
		cout << "Please enter json file as argument" << endl
			 << "For instance : ./main P170B328_ServieresV_L1_dat_1.json" << endl;
		return 0;
	}
	else if (argc == 3)
		MAX_THREADS = max(2,atoi(argv[2]));
	else if (argc > 3)
	{
		cout << "Please enter only one json file as argument and optionnaly the number of threads to start as a second argument" << endl
			 << "For instance : ./main P170B328_ServieresV_L1_dat_1.json" << endl;
		return 0;
	}
	cout << "Starting to sort images" << endl;
	execchrono c;

	std::ifstream f(argv[1]);
	json data = json::parse(f);

	DataMonitor monitor;
	ResMonitor resMonitor;

	c.lap("Starting threads : ");

	thread threads[MAX_THREADS];

	threads[0] = thread(addImage, ref(data), ref(monitor));
	for (size_t i = 1; i < MAX_THREADS; i++)
	{
		threads[i] = thread(worker, ref(monitor), ref(resMonitor));
	}
	for (size_t i = 0; i < MAX_THREADS; i++)
	{
		threads[i].join();
	}

	resMonitor.displayItems();
	c.total("END : ");
	resMonitor.exportItems(argv[1]);
	return 0;
}