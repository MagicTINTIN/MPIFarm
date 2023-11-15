#include <omp.h>
#include <iostream>
#include <string>
#include "imageprocess.h"
#include "chrono.h"
#include <algorithm>
#include <fstream>
#include "includes/nlohmann/json.hpp"
#include <functional>
#include "structureomp.h"

using json = nlohmann::json;
using namespace std;

int MAX_THREADS(5);

float processImg(imageStruct &img)
{
    if (isImgGray(img.path))
    {
        rgb tmp = averageColorImg(img.path);
        return tmp.R;
    }
    return 0;
}

void addImages(json &data, imageStruct *imgArr)
{
    int i(0);
    for (const auto &item : data.items())
    {
        imgArr[i] = imageStruct(item.value()["rank"].get<int>(), item.value()["name"].get<std::string>(), item.value()["averageValue"].get<float>());
        i++;
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
        MAX_THREADS = max(2, atoi(argv[2]));
    else if (argc > 3)
    {
        cout << "Please enter only one json file as argument and optionnaly the number of threads to start as a second argument" << endl
             << "For instance : ./main P170B328_ServieresV_L1_dat_1.json" << endl;
        return 0;
    }

    ResMonitor resmonitor;
    cout << "Starting to sort images" << endl;
    execchrono c;

    std::ifstream f(argv[1]);
    json data = json::parse(f);

    imageStruct imgArr[25];
    addImages(data, imgArr);

    c.lap("Starting threads : ");

    int totalIndexes(0);
    float totalLightness(0);
    const int numberThreads(5);
    const int dataLength(25);
    const int dataLengthPerThread(dataLength / numberThreads);
    
#pragma omp parallel num_threads(numberThreads) default(none) reduction(+ : totalLightness, totalIndexes) shared(imgArr, numberThreads, dataLength, dataLengthPerThread, resmonitor)
    {
        const int threadnb = omp_get_thread_num();
        const int startnb(threadnb * dataLengthPerThread);
        const int endnb = threadnb == numberThreads - 1 ? dataLength : ((threadnb + 1) * dataLengthPerThread);

        for (int i = startnb; i < endnb; i++)
        {
            float light = processImg(imgArr[i]);
            totalLightness += light;
            totalIndexes += i;
            
            imgArr[i].averageValue = light;
            imgArr[i].rank = i;
#pragma omp critical(count_critical)
            {
                resmonitor.addItem(imgArr[i]);
            }
        }
    }

    resmonitor.displayItems();
    resmonitor.exportItems(argv[1]);
    cout << "Total lightness : " << totalLightness << endl
         << "Total Indexes : " << totalIndexes << endl;
    c.total("END : ");
    return 0;
}