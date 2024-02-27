#include "Tile.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

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

    // Write the binary data of the image the frames to image file
    // outfile.write(reinterpret_cast<char *>(pixels), pixelHeight * pixelWidth);
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            outfile.write((char *)&array[i][j], 1);
            // std::cout << (0 + pixels[i][j]) << " ";
        }
        // std::cout << std::endl;
    }

    outfile.close();
    std::cout <<filename <<", image written successfully" << std::endl;
}
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
    int grid_length = 0, numberOfMoves = 0;
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
                grid_length = std::stoi(argv[++i]);
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

    /*std::cout << "Size of grid " << grid_length << " x " << size_grid << std::endl;
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

    char p;//, p1;
    /*fread.read(&p1, 1);
    fread.read(&p1, 1);
    fread.read(&p1, 1);
    fread.read(&p1, 1);*/

    // char * array = new char[pixelHeight * pixelWidth];
    // fread.read(array, pixelHeight*pixelWidth);

    for (int i = 0; i < pixelHeight; ++i)
    {
        pixels[i] = new unsigned char[pixelWidth];
        for (int j = 0; j < pixelWidth; ++j)
        {
            fread.read(&p, 1);
            pixels[i][j] = static_cast<unsigned char>(p);
            // std::cout << (0 + pixels[i][j]) << " ";
            // std::cout << (0 + array[i*pixelWidth + j]) << " ";
        }
        // std::cout << std::endl;
    }
    // fread.read(reinterpret_cast<char *>(pixels[0]), pixelWidth * pixelHeight);
    // std::cout << "Pixels read successful " << std::endl;
    // std::string outName = "output.pgm";
    // NDLMDU011::writeImage(pixelWidth, pixelHeight, outName, pixels);

    int grid_size = grid_length * grid_length; // number of subdivided image tiles
    int tile_width = (pixelWidth / grid_length); // the width of a tile from integer division
    int tile_height = pixelHeight / grid_length; // the height of a tile from integer division

    pixelWidth = tile_width * grid_length; // updated/reduced width of the image
    pixelHeight = tile_height * grid_length; // updated/reduced height of the image

    NDLMDU011::TileManager tile_manager(grid_length);
    int x_index = 0, y_index = 0; // Tile coordinates in TileManager tile_board
    int x_coord = 0, y_coord = 0; // start coordinates of the sub image/tile from the original image 

    while (y_index < grid_length)
    {
       // std::cout << "y_index = " << y_index << " x_index = " << x_index << std::endl;
        unsigned char **tile_pixels = new unsigned char *[tile_height];
        for (int row = y_coord; row < y_coord + tile_height; ++row)
        {
            tile_pixels[row-y_coord] = new unsigned char[tile_width];
           // std::cout << "row = " << row << " y_coord = " << y_coord << std::endl;
            for (int col = x_coord; col < x_coord + tile_width; ++col)
            {
                //std::cout << "row = " << row << " y_coord = " << y_coord << " col = " << col << " x_coord = " << x_coord<< std::endl;
            
                tile_pixels[row - y_coord][col - x_coord] = pixels[row][col];
            }
        }
        std::string fName = "out_image-" + std::to_string(y_index) + std::to_string(x_index) + ".pgm";
        NDLMDU011::writeImage(tile_width, tile_height, fName, tile_pixels);

        NDLMDU011::Tile tile_image(tile_width, tile_height);
        tile_image.assignPixels(NDLMDU011::deepCopy(tile_pixels, tile_height, tile_width));
        tile_manager.addTile(tile_image, x_index, y_index);

        /*for (int i = 0; i < tile_height; ++i)
        {
            delete[] tile_pixels[i];
        }
        delete[] tile_pixels;*/

        x_index++;
        x_coord += tile_width;
        
        if (x_index >= grid_length)
        {
            x_index = 0;
            y_index++;
            y_coord += tile_height;
            x_coord = 0;
        }

        //std::cout << "Next tile" << std::endl;
    }

    for (int i = 0; i < pixelHeight; ++i)
    {
        delete[] pixels[i];
    }
    delete[] pixels;
    return 0;
}