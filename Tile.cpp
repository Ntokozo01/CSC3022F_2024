#include "Tile.h"
#include <vector>
#include <iostream>
#include <fstream>

#define Ttile NDLMDU011::Tile
#define TManager NDLMDU011::TileManager

Ttile::Tile() : x_start(0), x_end(0), y_start(0), y_end(0) {}

Ttile::Tile(int width, int height): width(width), height(height){}

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

Ttile::~Tile()
{
    int height = getHeight();

    /*for (int i = 0; i < height; i++)
    {
        delete[] pixels;
    }
    delete[] pixels;*/
}

TManager::TileManager(int grid_length) : grid_length(grid_length)
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