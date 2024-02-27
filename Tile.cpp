#include "Tile.h"
#include <vector>
#include <iostream>
#include <fstream>

#define Ttile NDLMDU011::Tile
#define TManager NDLMDU011::TileManager

Ttile::Tile() : x_start(0), x_end(0), y_start(0), y_end(0) {}

Ttile::Tile(int width, int height) : width(width), height(height) {}

Ttile::Tile(int x_start, int x_end, int y_start, int y_end) : x_start(x_start), x_end(x_end), y_start(y_start), y_end(y_end) {}

void Ttile::assignPixels(unsigned char **tile_pixels)
{
    pixels = tile_pixels;
}

int Ttile::getHeight()
{
    return y_end - y_start;
}

int Ttile::getWidth()
{
    return x_end - x_start;
}

unsigned char **Ttile::getTilePixels(void)
{
    return pixels;
}

void Ttile::setTileEmpty(void)
{
    for (int row = 0; row < height; ++row)
    {
        for (int col = 0; col < width; ++col)
        {
            pixels[row][col] = 0;
        }
    }
}

Ttile::~Tile()
{
    int height = getHeight();

    /*for (int i = 0; i < height; i++)
    {
        delete[] pixels;
    }
    delete[] pixels;*/
}

TManager::TileManager(int grid_length, int tile_width, int tile_height)
    : grid_length(grid_length), tile_width(tile_width), tile_height(tile_height)
{
    tile_board.resize(grid_length, std::vector<Tile>(grid_length));
}

void TManager::addTile(Tile tile_image, int x_index, int y_index)
{
    tile_board[y_index][x_index] = tile_image;
}

NDLMDU011::Tile TManager::getTile(int x_index, int y_index)
{
    return tile_board[y_index][x_index];
}

int TManager::extractSubTiles(unsigned char **pixels)
{
    int x_index = 0, y_index = 0; // Tile coordinates in TileManager tile_board
    int x_coord = 0, y_coord = 0; // start coordinates of the sub image/tile from the original image

    while (y_index < grid_length)
    {
        // std::cout << "y_index = " << y_index << " x_index = " << x_index << std::endl;
        unsigned char **tile_pixels = new unsigned char *[tile_height];
        for (int row = y_coord; row < y_coord + tile_height; ++row)
        {
            tile_pixels[row - y_coord] = new unsigned char[tile_width];
            // std::cout << "row = " << row << " y_coord = " << y_coord << std::endl;
            for (int col = x_coord; col < x_coord + tile_width; ++col)
            {
                // std::cout << "row = " << row << " y_coord = " << y_coord << " col = " << col << " x_coord = " << x_coord<< std::endl;

                tile_pixels[row - y_coord][col - x_coord] = pixels[row][col];
            }
        }
        // std::string fName = "out_image-" + std::to_string(y_index) + std::to_string(x_index) + ".pgm";
        // NDLMDU011::writeImage(tile_width, tile_height, fName, tile_pixels);

        NDLMDU011::Tile tile_image(tile_width, tile_height);
        tile_image.assignPixels(NDLMDU011::deepCopy(tile_pixels, tile_height, tile_width));
        addTile(tile_image, x_index, y_index);

        x_index++;
        x_coord += tile_width;

        if (x_index >= grid_length)
        {
            x_index = 0;
            y_index++;
            y_coord += tile_height;
            x_coord = 0;
        }

        // std::cout << "Next tile" << std::endl;
    }
    tile_board[grid_length - 1][grid_length - 1].setTileEmpty();

    return tile_board.size();
}

unsigned char **TManager::retrieveTileImage(void)
{
    int image_width = tile_width * grid_length;
    int image_height = tile_height * grid_length;

    unsigned char **image_pixels = new unsigned char *[image_height];
    for (int k = 0; k < image_height; ++k)
    {
        image_pixels[k] = new unsigned char[image_width];
    }

    int x_index = 0, y_index = 0; // Tile coordinates in TileManager tile_board
    int x_coord = 0, y_coord = 0; // start coordinates of the sub image/tile from the original image

    while (y_index < grid_length)
    {
        // std::cout << "y_index = " << y_index << " x_index = " << x_index << std::endl;
        Tile tile_image = getTile(x_index, y_index);
        unsigned char **tile_pixels = tile_image.getTilePixels();

        for (int row = y_coord; row < y_coord + tile_height; ++row)
        {
            // std::cout << "row = " << row << " y_coord = " << y_coord << std::endl;
            for (int col = x_coord; col < x_coord + tile_width; ++col)
            {
                // std::cout << "row = " << row << " y_coord = " << y_coord << " col = " << col << " x_coord = " << x_coord<< std::endl;

                image_pixels[row][col] = tile_pixels[row - y_coord][col - x_coord];
            }
        }

        x_index++;
        x_coord += tile_width;

        if (x_index >= grid_length)
        {
            x_index = 0;
            y_index++;
            y_coord += tile_height;
            x_coord = 0;
        }

        // std::cout << "Next tile" << std::endl;
    }

    return image_pixels;
}

TManager::~TileManager()
{
}

unsigned char **NDLMDU011::deepCopy(unsigned char **arrOrig, int height, int width)
{
    unsigned char **arrCopy = new unsigned char *[height];

    for (int i = 0; i < height; i++)
    {
        arrCopy[i] = new unsigned char[width];

        for (int j = 0; j < width; j++)
        {
            arrCopy[i][j] = arrOrig[i][j];
        }
        delete[] arrOrig[i];
    }
    delete[] arrOrig;

    return arrCopy;
}