#include "Tile.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

/*
// reads the PGM image binary data from the file and stores it into pixels array
void readPGMImage(std::string filename)
{
    std::ifstream fread(filename, std::fstream::binary);

    std::cout << "Reading file: " << filename << std::endl;
    std::string line;
    std::getline(fread, line); // get the Magic number of input image which is "P5"

    // std::cout << line << std::endl;
    int pixelWidth, pixelHeight;
    fread >> pixelWidth >> pixelHeight;

    imageHeight = pixelHeight;
    imageWidth = pixelWidth;

    std::getline(fread, line); // get the maximum value of the pixels in this image data usually 255

    char p, p1;
    fread.read(&p1, 1);
    fread.read(&p1, 1);
    fread.read(&p1, 1);
    fread.read(&p1, 1);

    for (int i = 0; i < pixelHeight; ++i)
    {
        for (int j = 0; j < pixelWidth; ++j)
        {
            fread.read(&p, 1);
            pixels[i][j] = static_cast<unsigned char>(p);
            //  std::cout << (0 + pixels[i][j]) << " ";
        }
        // std::cout << std::endl;
    }
    // fread.read(reinterpret_cast<char *>(pixels[0]), pixelWidth * pixelHeight);
    std::cout << "Pixels read successful " << std::endl;
}*/

int main(int argc, char *argv[])
{
    int size_grid = 0, numberOfMoves = 0;
    std::string inputImage;

    if (argc >= 7)
    {
        int i = 0;
        while (i < argc)
        {
            // std::cout << i << " " << argv[i] << std::endl;

            if (std::string(argv[i]) == "-s")
            {
                // std::cout << i << " " << argv[i] << std::endl;
                size_grid = std::stoi(argv[++i]);
            }
            else if (std::string(argv[i]) == "-i")
            {
                // std::cout << i << " " << argv[i] << std::endl;
                inputImage = argv[++i];
            }
            else if (std::string(argv[i]) == "-n")
            {
                // std::cout << i << " " << argv[i] << std::endl;
                numberOfMoves = std::stoi(argv[++i]);
            }
            i++;
        }
    }
    else
    {
        std::cerr << "Provide the required number of argumants" << std::endl;
        return 1;
    }

    /*std::cout << "Size of grid " << size_grid << " x " << size_grid << std::endl;
    std::cout << "PGM Image " << inputImage << std::endl;
    std::cout << "Number of Moves " << numberOfMoves << std::endl;*/

    unsigned char **pixels;
    std::ifstream fread(inputImage, std::fstream::binary);

    std::cout << "Reading file: " << inputImage << std::endl;
    std::string line;
    std::getline(fread, line); // get the Magic number of input image which is "P5"

    std::getline(fread, line);
    while (line[0] == '#')
    {
        std::getline(fread, line);
    }

    std::istringstream iss(line);
    // std::cout << line << std::endl;
    int pixelWidth, pixelHeight;
    iss >> pixelWidth >> pixelHeight;

    pixels = new unsigned char *[pixelHeight];

    // imageHeight = pixelHeight;
    // imageWidth = pixelWidth;

    std::getline(fread, line); // get the maximum value of the pixels in this image data usually 255

    char p, p1;
    /*fread.read(&p1, 1);
    fread.read(&p1, 1);
    fread.read(&p1, 1);
    fread.read(&p1, 1);*/

    //char * array = new char[pixelHeight * pixelWidth];
    //fread.read(array, pixelHeight*pixelWidth);

    for (int i = 0; i < pixelHeight; ++i)
    {
        pixels[i] = new unsigned char[pixelWidth];
        for (int j = 0; j < pixelWidth; ++j)
        {
            fread.read(&p, 1);
            pixels[i][j] = static_cast<unsigned char>(p);
            //std::cout << (0 + pixels[i][j]) << " ";
             //std::cout << (0 + array[i*pixelWidth + j]) << " ";
        }
        //std::cout << std::endl;
    }
    // fread.read(reinterpret_cast<char *>(pixels[0]), pixelWidth * pixelHeight);
    std::cout << "Pixels read successful " << std::endl;

    std::string outName = "output.pgm";
    std::ofstream outfile(outName, std::ios::binary);

    // Write the header information of the .pgm file
    outfile << "P5" << std::endl;
    outfile << pixelWidth << " " << pixelHeight << std::endl;
    outfile << 255 << std::endl;

    // Write the binary data of the image the frames to image file
    //outfile.write(reinterpret_cast<char *>(pixels), pixelHeight * pixelWidth);
     for (int i = 0; i < pixelHeight; ++i)
    {
        for (int j = 0; j < pixelWidth; ++j)
        {
            outfile.write((char *)&pixels[i][j], 1);
            // std::cout << (0 + pixels[i][j]) << " ";
        }
        // std::cout << std::endl;
    }


    outfile.close();

    for (int i = 0; i < pixelHeight; ++i)
    {
        delete[] pixels[i];
    }
    delete[] pixels;
    return 0;
}