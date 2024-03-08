#include "Tile.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <math.h>

#define Ttile NDLMDU011::Tile
#define TManager NDLMDU011::TileManager

std::vector<unsigned char **> summary_pixels; // Container for the Generated Images pixels after each move

// Write out the pgm image char-by-char
void NDLMDU011::writeImage(int width, int height, std::string filename, unsigned char **array)
{
    std::ofstream outfile(filename, std::ios::binary);

    // Write the header information of the .pgm file
    outfile << "P5" << std::endl;
    outfile << width << " " << height << std::endl;
    outfile << 255 << std::endl;

    // Write the binary data of the image 1-bit at a time
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            outfile.write((char *)&array[i][j], 1);
            // std::cout << (0 + array[i][j]) << " ";
        }
        // std::cout << std::endl;
    }
    outfile.close();
    std::cout << filename << ", image written successfully" << std::endl;
}

void writeSummaryImage(std::string filename, int moves, int image_width, int image_height, int border_size)
{
    int width_scaling = ceil(sqrt(moves)); // Calculates the number of images to be aligned vertically in a box manner
    int height_scaling = round(sqrt(moves)); // Calculates the number of images to be aligned horizontally in a box manner

    //Calculate the new summary image dimensions based on number of images to be aligned and border/margin space between them
    // (scaling + 1) to include the top or left margin before images
    int summary_width = width_scaling * image_width + (width_scaling + 1) * border_size; 
    int summary_height = height_scaling * image_height + (height_scaling + 1) * border_size;

    std::ofstream outfile(filename, std::ios::binary);

    // Write the header information of the .pgm file
    outfile << "P5" << std::endl;
    outfile << summary_width << " " << summary_height << std::endl;
    outfile << 255 << std::endl;

    char white = 255; // represents a white character or pixel
    char white_arr[border_size]; // = {white};
    std::fill_n(white_arr, border_size, white); // Fill the array with char 255 for white pixels

    // Adds the white border between images vertically
    for (int j = 0; j < border_size; ++j)
    {
        for (int k = 0; k < summary_width; ++k)
        {
            outfile.write(&white, 1);
        }
    }

    // loops through the images in summary_pixels vector for next vertical (row) alignment 
    for (int summary_row = 0; summary_row < height_scaling; ++summary_row)
    {
        // loops through the rows of the horizontal images to write their pixels to the file
        for (int row = 0; row < image_height; ++row)
        {
            // Adds the whiter border between images horizontally
            outfile.write(white_arr, border_size);

            // loops through the images in summary_pixels vector for horizontal images
            for (int col = 0; col < width_scaling; ++col)
            {
                int image_iterator = summary_row * width_scaling + col; // switching index between the images in the vector
                // Writes the row pixels of a single image
                for (int pix = 0; pix < image_width; ++pix)
                {
                    if (image_iterator < moves)
                    {
                        outfile.write((char *)&summary_pixels[image_iterator][row][pix], 1);
                    }
                    else
                    {
                        // Writes a white pixel to fill up the box image
                        outfile.write(&white, 1);
                    }
                }
                // Adds the whiter border between images horizontally
                outfile.write(white_arr, border_size);
            }
        }
        // Adds the white border between images vertically
        for (int j = 0; j < border_size; ++j)
        {
            for (int k = 0; k < summary_width; ++k)
            {
                outfile.write(&white, 1);
            }
        }
    }
    outfile.close();
    std::cout << filename << ", image written successfully" << std::endl;
}

int main(int argc, char *argv[])
{
    int grid_length = 0, numberOfMoves = 0;
    std::string inputImage, output_image;
    std::string summary_image = "Summary_image.pgm"; //set a default Summary Image name
    int border_size = 10; // set a default value for images margin value in summary image

    // Extract the input values from the command line
    if (argc >= 8)
    {
        int i = 0;
        while (i < argc)
        {
            if (std::string(argv[i]) == "-s")
            {
                grid_length = std::stoi(argv[++i]);
                grid_length = std::max(grid_length, 2); // Disallow grid length less than 2
            }
            else if (std::string(argv[i]) == "-i")
            {
                // The base name of the output images
                output_image = argv[++i];

                int period = output_image.find_last_of(".");
                if (period > 0){
                    output_image = output_image.substr(0,period);
                }
            }
            else if (std::string(argv[i]) == "-n")
            {
                numberOfMoves = std::stoi(argv[++i]);
                numberOfMoves = std::max(numberOfMoves, 0); // Discard moves less than 0

            }
            else if (std::string(argv[i]) == "-x")
            {
                // The summary image name of the
                summary_image = argv[++i];
            }
            else if (std::string(argv[i]) == "-m")
            {
                border_size = std::stoi(argv[++i]);
                border_size = std::max(border_size, 0); // Don't accept negative values
            }
            i++;
        }
        // Extract the input image name with .pgm extension as the last argument from command line parameters
        inputImage = argv[argc - 1];
    }
    else
    {
        std::cerr << "Provide the required number of argumants" << std::endl;
        return 1;
    }

    unsigned char **pixels; // represent a 2D array for the image pixels of the input image
    int image_width, image_height;

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

    // Set the random number generator starting value (seed) based on with the current time
    srand(static_cast<unsigned int>(time(nullptr)));

    int success_swaps = 0;
    unsigned char **image_pixels = tile_manager.retrieveTileImage();
    std::string outName = output_image + "-" + std::to_string(success_swaps) + ".pgm";
    NDLMDU011::writeImage(image_width, image_height, outName, image_pixels);

    summary_pixels.push_back(image_pixels);

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
            unsigned char **image_pixels = tile_manager.retrieveTileImage();
            std::string outName = output_image + "-" + std::to_string(success_swaps) + ".pgm";
            NDLMDU011::writeImage(image_width, image_height, outName, image_pixels);

            summary_pixels.push_back(image_pixels);
        }
    }

    writeSummaryImage(summary_image, ++numberOfMoves, image_width, image_height, border_size);
    for (int i = 0; i < numberOfMoves; i++)
    {
        for (int j = 0; j < image_height; ++j)
        {
            delete[] summary_pixels[i][j];
        }
        delete[] summary_pixels[i];
    }

    return 0;
}