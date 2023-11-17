#include <iostream>
#include <string>
#include <mpi.h>
#include <random>
#include <iostream>
#include <fstream>
#include "imageprocess.h"
#include "includes/nlohmann/json.hpp"

using json = nlohmann::json;
// using namespace std;
// using namespace MPI;

const int MAIN_PROCESS = 0;
const int TAG_PARTIAL_ARRAY = 2;
const int TAG_PARTIAL_SUM = 1;

long processImg(std::string &imgPath)
{
	return combineColors(averageColorImg(imgPath));
}

long averageMultipleImages(int *imageSet, int const &nbImages)
{
	std::cout << "Process received" << std::endl;
	for (size_t i = 0; i < nbImages; i++)
	{
		std::cout << i << " ";
	}
	std::cout << std::endl;

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
	int elementsNotProcessed(0);
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
		elementsPerProcess = totalElements / totalProcesses;
		elementsNotProcessed = totalElements % totalProcesses;
		std::cout << "Getting average color of image sequence: " << imageSet[0] << "-" << imageSet[elementsPerProcess * totalProcesses - 1] << " (" << elementsNotProcessed << " images ignored)" << std::endl;
	}
	long partialImageSet[elementsPerProcess];
	MPI::COMM_WORLD.Scatter(imageSet, elementsPerProcess, MPI::LONG, partialImageSet, elementsPerProcess, MPI::LONG, 0);
	long average = averageMultipleImages(imageSet, elementsPerProcess);
	long *averagesPartsImagesColor = nullptr;
	if (rank == 0)
	{
		delete[] imageSet;
		averagesPartsImagesColor = new long[totalProcesses];
	}
	MPI::COMM_WORLD.Gather(&average, 1, MPI::LONG, averagesPartsImagesColor, 1, MPI::LONG, 0);
	MPI::Finalize();
	return 0;
}