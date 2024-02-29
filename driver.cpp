#include "Tile.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>

#define Ttile NDLMDU011::Tile
#define TManager NDLMDU011::TileManager

void print2DArray(unsigned char **pixels, int x_start, int x_end, int y_start, int y_end)
{
    for (int i = x_start; i < y_end; ++i)
    {
        for (int j = y_start; j < y_end; ++j)
        {
            // outfile.write((char *)&pixels[i][j], 1);
            std::cout << (0 + pixels[i][j]) << " ";
        }
        std::cout << std::endl;
    }
}

void NDLMDU011::writeImage(int width, int height, std::string filename, unsigned char **array)
{
    std::ofstream outfile(filename, std::ios::binary);

    // Write the header information of the .pgm file
    outfile << "P5" << std::endl;
    outfile << width << " " << height << std::endl;
    outfile << 255 << std::endl;

    // Write the binary data of the image 1-bit at a time
    // outfile.write(reinterpret_cast<char *>(pixels), image_height * image_width);
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            outfile.write((char *)&array[i][j], 1);
        }
    }

    outfile.close();
    std::cout << filename << ", image written successfully" << std::endl;
}

int main(int argc, char *argv[])
{
    int grid_length = 0, numberOfMoves = 0;
    std::string inputImage;

    // Extract the input values from the command line
    if (argc >= 7)
    {
        int i = 0;
        while (i < argc)
        {
            if (std::string(argv[i]) == "-s")
            {
                grid_length = std::stoi(argv[++i]);
            }
            else if (std::string(argv[i]) == "-i")
            {
                inputImage = argv[++i];
            }
            else if (std::string(argv[i]) == "-n")
            {
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

    /*std::cout << "Size of grid " << grid_length << " x " << size_grid << std::endl;
    std::cout << "PGM Image " << inputImage << std::endl;
    std::cout << "Number of Moves " << numberOfMoves << std::endl;*/

    unsigned char **pixels; // represent a 2D array for the image pixels of the input image

    // Read the PGM input image and save the data in the appropriate variables
    std::ifstream file_reader(inputImage, std::fstream::binary);
    std::cout << "Reading file: " << inputImage << std::endl;
    std::string line;
    std::getline(file_reader, line); // get the Magic number of input image which is "P5"

    // Read the next line and check if it is a comment line, repeats until the line read is not a comment
    std::getline(file_reader, line);
    while (line[0] == '#') // comment lines start with the character '#'
    {
        std::getline(file_reader, line);
    }

    // Next line after comments is the image dimensions line which is width and height, extracts it from the line
    std::istringstream iss(line);
    int image_width, image_height;
    iss >> image_width >> image_height;

    std::getline(file_reader, line); // get the maximum value of the pixels in this image data usually 255

    pixels = new unsigned char *[image_height];
    // read each pixel value as 1 byte char and cast it into the unsigned char value to store it in the pixels array
    char p;
    for (int i = 0; i < image_height; ++i)
    {
        pixels[i] = new unsigned char[image_width];
        for (int j = 0; j < image_width; ++j)
        {
            file_reader.read(&p, 1);
            pixels[i][j] = static_cast<unsigned char>(p);
        }
    }

    //print2DArray(pixels, 0,image_width, 0, image_height);

    int grid_size = grid_length * grid_length;    // number of subdivided image tiles
    int tile_width = (image_width / grid_length); // the width of a tile from integer division
    int tile_height = image_height / grid_length; // the height of a tile from integer division

    image_width = tile_width * grid_length;   // updated/reduced width of the image
    image_height = tile_height * grid_length; // updated/reduced height of the image

    // Create a TileManager object for Tile objects of the image based on the grid length
    NDLMDU011::TileManager tile_manager(grid_length, tile_width, tile_height);
    // Sub-divide the Image pixels by grid_size times into Tile objects and store the tiles in TileManager
    tile_manager.extractSubTiles(pixels);

    // Free the memory that was allocated to the pixels array
    for (int i = 0; i < image_height; ++i)
    {
        delete[] pixels[i];
    }
    delete[] pixels;

    /*tile_manager.swapWith(NDLMDU011::top);
    tile_manager.swapWith(NDLMDU011::left);
    tile_manager.swapWith(NDLMDU011::bottom);
    tile_manager.swapWith(NDLMDU011::right);*/

    // Set the random number generator starting value (seed) based on with the current time
    srand(static_cast<unsigned int>(time(nullptr)));

    int success_swaps = 0;
    while (success_swaps < numberOfMoves)
    {
        // Generate a random number between 1 and 4 (inclusive)
        int randomNumber = 1 + rand() % 4;
        // Cast the generated random number into a corresponding Direction enum object
        NDLMDU011::Directions randomDirection = (NDLMDU011::Directions)randomNumber;

        // Swap the empty tile with that in the specified direction and check whether if swap was successful to write out an image and make another move
        if (tile_manager.swapWith(randomDirection))
        {
            success_swaps++;
            //std::cout << "Writing tiles" << std::endl;
            unsigned char **image_pixels = tile_manager.retrieveTileImage();
            std::string outName = "outputImage-" + std::to_string(success_swaps) + ".pgm";
            NDLMDU011::writeImage(image_width, image_height, outName, image_pixels);

            // Free the memory space for image_pixels representing the image state after a move 
            for (int i = 0; i < image_height; ++i)
            {
                delete[] image_pixels[i];
            }
            delete[] image_pixels;
        }
    }

    return 0;
}