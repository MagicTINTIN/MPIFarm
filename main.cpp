#include <iostream>
#include <string>
#include <cmath>
#include <mpi.h>
#include <iomanip>
#include <fstream>
#include <filesystem>
#include "imageprocess.h"
#include "includes/nlohmann/json.hpp"

using json = nlohmann::json;
// namespace fs = std::filesystem;
// using namespace std;
// using namespace MPI;

const int MAIN_PROCESS = 0;
const int TAG_PARTIAL_ARRAY = 2;
const int TAG_PARTIAL_SUM = 1;

std::string getName(std::string const &name, int const &val, int const &max)
{
	int nb0max = std::floor(std::log10(std::max(1, max)));
	int nb0val = std::floor(std::log10(std::max(1, val)));
	std::string output = name;
	for (size_t i = 0; i < (nb0max - nb0val); i++)
	{
		output += "0";
	}
	output += std::to_string(val) + ".jpg";
	return output;
}

long averageMultipleImages(int *imageSet, int const &nbImages, int const &processNb, int const &totalSize, std::string const &prefix)
{
	// std::cout << processNb << " is currently in " << fs::current_path() << std::endl;
	// std::ofstream exportFlux("/home/user/Documents/filefromMPIFarm", std::ios::app);
	// exportFlux << "Process nb " << processNb << " is currently in " << fs::current_path() << std::endl;
	// exportFlux.close();

	rgb values(0, 0, 0);
	for (size_t i = 0; i < nbImages; i++)
	{
		std::string filename = getName(prefix, imageSet[i], totalSize);
		values += averageColorImg(filename);
	}

	if (nbImages > 0)
		values /= nbImages;

	return combineColors(values);
}

int main(int argc, char const *argv[])
{
	if (argc < 2)
	{
		std::cout << "\nPlease enter json file as argument" << std::endl
				  << "For instance : build/MPIFarm P170B328_ServieresV_L3_small.json" << std::endl;
		return 0;
	}
	else if (argc > 3)
	{
		std::cout << "\nPlease enter only one json file as argument and optionnaly --benchmark as second argument" << std::endl
				  << "For instance : build/MPIFarm P170B328_ServieresV_L3_small.json" << std::endl;
		return 0;
	}

	int *imageSet = nullptr;
	int elementsPerProcess(0);
	int elementsNotProcessed(0);
	int totalElements(0);

	std::ifstream f(argv[1]);
	json dataSetJSON = json::parse(f);
	f.close();

	bool benchmark = false;
	if (argc == 3 && std::string(argv[2]) == "--benchmark")
		benchmark = true;

	MPI::Init();
	int rank = MPI::COMM_WORLD.Get_rank();
	int totalProcesses = MPI::COMM_WORLD.Get_size();

	double startTime, stopTime;

	totalElements = dataSetJSON["sequenceNumbers"].size();
	elementsPerProcess = totalElements / totalProcesses;

	if (rank == 0)
	{
		imageSet = new int[dataSetJSON["sequenceNumbers"].size()];
		for (std::size_t i = 0; i < dataSetJSON["sequenceNumbers"].size(); ++i)
		{
			imageSet[i] = dataSetJSON["sequenceNumbers"][i];
		}

		elementsNotProcessed = totalElements % totalProcesses;
		std::cout << "\nGetting average color of image sequence: " << imageSet[0] << "-" << imageSet[elementsPerProcess * totalProcesses - 1] << " (" << elementsNotProcessed << " images ignored)" << std::endl;
		startTime = MPI::Wtime();
	}

	int partialImageSet[elementsPerProcess];
	MPI::COMM_WORLD.Scatter(imageSet, elementsPerProcess, MPI::INT, partialImageSet, elementsPerProcess, MPI::INT, 0);
	long average = averageMultipleImages(partialImageSet, elementsPerProcess, rank, dataSetJSON["maxSize"].get<int>(), dataSetJSON["sequenceName"].get<std::string>());

	long *averagesPartsImagesColor = nullptr;
	if (rank == 0)
	{
		delete[] imageSet;
		averagesPartsImagesColor = new long[totalProcesses];
	}

	MPI::COMM_WORLD.Gather(&average, 1, MPI::LONG, averagesPartsImagesColor, 1, MPI::LONG, 0);
	MPI::Finalize();

	if (rank == 0)
	{
		rgb values(0, 0, 0);

		stopTime = MPI::Wtime();
		for (size_t i = 0; i < totalProcesses; i++)
		{

			rgb vals = splitColors(averagesPartsImagesColor[i]);
			std::cout << "Process n°" << i << " : #" << std::setw(6) << std::setfill('0') << std::right << std::hex << averagesPartsImagesColor[i] << std::dec << ", R:" << std::setfill(' ') << std::setw(3) << vals.R << " G:" << std::setw(3) << vals.G << " B:" << std::setw(3) << vals.B << std::endl;

			values += vals;
		}
		if (totalProcesses > 0)
			values /= totalProcesses;
		long averageHex = combineColors(values);
		std::cout << "Global average color: #" << std::setw(6) << std::setfill('0') << std::right << std::hex << averageHex << std::dec << ", R:" << std::setfill(' ') << std::setw(3) << values.R << " G:" << std::setw(3) << values.G << " B:" << std::setw(3) << values.B << std::endl;

		std::cout << "Time elapsed: " << stopTime - startTime << "s" << std::endl;

		if (benchmark)
		{
			std::ofstream exportFlux("benchmark.csv", std::ios::app);
			exportFlux << totalProcesses << "," << stopTime - startTime << std::endl;
			exportFlux.close();
		}
	}

	delete[] averagesPartsImagesColor;
	return 0;
}