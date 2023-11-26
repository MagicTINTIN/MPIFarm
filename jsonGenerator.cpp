#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <cmath>

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

int main(int argc, char const *argv[])
{
    std::string fileNameBase = "P170B328_ServieresV_L3_";
    std::string sequenceName = "rainbow/image";
    std::string line;
    std::cout << "Choose a file name base (default: P170B328_ServieresV_L3_): ";
    getline(std::cin, line);
    if (!line.empty())
        fileNameBase = line;
    std::cout << "Define image sequence name (default: rainbow/image): ";
    getline(std::cin, line);
    if (!line.empty())
        sequenceName = line;
    std::cout << sequenceName << std::endl;
    int maxSize;
    std::cout << "What would be theorical maximum index of the image sequence (ex: 9999): ";
    std::cin >> maxSize;
    int begin;
    int end;
    std::cout << "First image number of the sequence: ";
    std::cin >> begin;
    std::cout << "Last image number of the sequence: ";
    std::cin >> end;

    std::ofstream exportFlux(fileNameBase + std::to_string(begin) + ((end == begin) ? ".json" : "-" + std::to_string(end) + ".json"), std::ios::app);

    exportFlux << "{" << std::endl
    << "    \"sequenceName\": \"" << sequenceName << "\","  << std::endl
    << "    \"maxSize\": " << maxSize << "," << std::endl
    << "    \"sequenceNumbers\": [";

    for (size_t i = begin; i < end + 1; i++)
    {
        if (i == begin)
            exportFlux << i;
        else
            exportFlux << "," << i;
    }
    
    exportFlux << "]" << std::endl
    << "}";
    exportFlux.close();
    
    return 0;
}
