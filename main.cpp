#include <iostream>
#include <string>
#include <mpi.h>
#include <random>
#include <iostream>
#include <fstream>
#include "structure.h"
#include "imageprocess.h"
#include "includes/nlohmann/json.hpp"

using json = nlohmann::json;
// using namespace std;
// using namespace MPI;

void calculate_full_sum();
void calculate_partial_sum();

const int MAIN_PROCESS = 0;
const int TAG_PARTIAL_ARRAY = 2;
const int TAG_PARTIAL_SUM = 1;

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

int getAverageColor(int *imageSet, int const nbImages)
{
	return 0;
}

int main(int argc, char const *argv[])
{
	if (argc < 2)
	{
		std::cout << "Please enter json file as argument" << std::endl
				  << "For instance : ./main P170B328_ServieresV_L3_small.json" << std::endl;
		return 0;
	}
	else if (argc > 2)
	{
		std::cout << "Please enter only one json file as argument and optionnaly the number of threads to start as a second argument" << std::endl
				  << "For instance : ./main P170B328_ServieresV_L3_small.json" << std::endl;
		return 0;
	}

	int *imageSet = nullptr;
	int elementsPerProcess(0);
	int elementsLastProcess(0);
	int totalElements(0);

	MPI::Init();
	int rank = MPI::COMM_WORLD.Get_rank();
	int totalProcesses = MPI::COMM_WORLD.Get_size();
	if (rank == 0)
	{
		std::ifstream f(argv[1]);
		json jsonArray = json::parse(f);

		imageSet = new int[jsonArray.size()];

		for (std::size_t i = 0; i < jsonArray.size(); ++i)
		{
			imageSet[i] = jsonArray[i];
		}

		totalElements = jsonArray.size();
		elementsPerProcess = totalElements / (totalProcesses - 1);
		elementsLastProcess = totalElements % (totalProcesses - 1);
	}
	double partialImageSet[elementsPerProcess];
	MPI::COMM_WORLD.Scatter(imageSet, elementsPerProcess, MPI::DOUBLE, partialImageSet, elementsPerProcess, MPI::DOUBLE, 0);
	double average = getAverageColor(imageSet, elementsPerProcess);
	double *averagesPartsImagesColor = nullptr;
	if (rank == 0)
	{
		delete[] imageSet;
		averagesPartsImagesColor = new double[totalProcesses];
	}
	MPI::COMM_WORLD.Gather(&average, 1, MPI::DOUBLE, averagesPartsImagesColor, 1, MPI::DOUBLE, 0);
	MPI::Finalize();
	return 0;
}