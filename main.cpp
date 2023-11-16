#include <iostream>
#include <mpi.h>
#include <random>
#include <iostream>
#include "structure.h"
#include "imageprocess.h"

using namespace std;
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

int main(int argc, char const *argv[])
{
	if (argc < 2)
	{
		cout << "Please enter json file as argument" << endl
			 << "For instance : ./main P170B328_ServieresV_L3_small.json" << endl;
		return 0;
	}
	else if (argc > 2)
	{
		cout << "Please enter only one json file as argument and optionnaly the number of threads to start as a second argument" << endl
			 << "For instance : ./main P170B328_ServieresV_L3_small.json" << endl;
		return 0;
	}

	int *imageSet = nullptr;

	MPI::Init();
	auto rank = MPI::COMM_WORLD.Get_rank();
	auto total_processes = MPI::COMM_WORLD.Get_size();
	if (rank == 0)
	{
		auto total_elements = ELEMENTS_PER_PROCESS * total_processes;
		random_numbers = new double[total_elements];
		generate_random_numbers(random_numbers, total_elements);
	}
	double random_numbers_chunk[ELEMENTS_PER_PROCESS];
	MPI::COMM_WORLD.Scatter(random_numbers, ELEMENTS_PER_PROCESS, MPI::DOUBLE, random_numbers_chunk, ELEMENTS_PER_PROCESS, MPI::DOUBLE, 0);
	double average = get_average(random_numbers_chunk, ELEMENTS_PER_PROCESS);
	double *averages_of_chunks = nullptr;
	if (rank == 0)
	{
		delete[] random_numbers;
		averages_of_chunks = new double[total_processes];
	}
	COMM_WORLD.Gather(&average, 1, MPI::DOUBLE, averages_of_chunks, 1, MPI::DOUBLE, 0);
	MPI::Finalize();
	return 0;
}