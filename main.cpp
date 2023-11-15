#include <iostream>
#include <mpi.h>
#include <random>
#include <iostream>
#include "structure.h"
#include "imageprocess.h"

using namespace std;
using namespace MPI;

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

int main() {
    Init();
    auto rank = COMM_WORLD.Get_rank();
    if (rank == 0) {
        calculate_full_sum();
    } else {
        calculate_partial_sum();
    }
    Finalize();
    return 0;
}