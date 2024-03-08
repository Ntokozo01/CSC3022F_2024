#include "Tile.h"
#include <vector>
#include <iostream>
#include <fstream>

#define Ttile NDLMDU011::Tile
#define TManager NDLMDU011::TileManager

Ttile::Tile() { Tile(0, 0); }

Ttile::Tile(int width, int height) : width(width), height(height)
{
    // std::cout << ++NDLMDU011::count << ". Tile constructor called" << std::endl;
}

void Ttile::assignPixels(u_char **tile_pixels)
{
    pixels = tile_pixels;
}

int Ttile::getHeight()
{
    return height;
}

int Ttile::getWidth()
{
    return width;
}

u_char **Ttile::getTilePixels(void)
{
    /*u_char **arrCopy = new u_char *[height];

    for (int i = 0; i < height; i++)
    {
        arrCopy[i] = new u_char[width];

        for (int j = 0; j < width; j++)
        {
            arrCopy[i][j] = pixels[i][j];
        }
    }

    return arrCopy;*/
    return pixels;
}

void Ttile::setHeight(int height)
{
    this->height = height;
}
void Ttile::setWidth(int width)
{
    this->width = width;
}

/**
 * Set the tiles the pixel values to be 0 for black color
 */
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
    // std::cout << "Tile destructor called" << std::endl;
    int height = getHeight();
    /*for (int i = 0; i < height; i++)
    {
        delete[] pixels;
    }
    delete[] pixels;*/
}

// Sets the attributes values and resize the tile_board to store the Tile pointers
TManager::TileManager(int grid_length, int tile_width, int tile_height)
    : grid_length(grid_length), tile_width(tile_width), tile_height(tile_height)
{
    tile_board.resize(grid_length, std::vector<Tile *>(grid_length));
}

/**
 * Add the pointer to Tile object to the tile_board at position of indices x_index and y_index
 * @param tile_image : a reference object for the dynamically allocated Tile
 * @param x_index : horizontal/x-coordinate of the tile in tile_board
 * @param y_index : vertical/y-coordinate of the tile in tile_board
 */
void TManager::addTile(Tile &tile_image, int x_index, int y_index)
{
    tile_board[y_index][x_index] = &tile_image;
}

/**
 * Get the tile object from tile_board at indices specified by x_index and y_index
 * @param x_index : horizontal/x-coordinate of the tile in tile_board
 * @param y_index : vertical/y-coordinate of the tile in tile_board
 */
NDLMDU011::Tile *TManager::getTile(int x_index, int y_index)
{
    return tile_board[y_index][x_index];
}

/**
 * Swaps the empty tile with the adjacent tile in the direction specified if it is a valid swap
 * @param direction : Direction enum object of the 4 possible adjacent tile directions
 * (Top, Bottom, Left or Right) to the empty tile
 * @return 0 if no swap was made (direction invalid), 1 when the swap was successful (direction is valid)
 */
int TManager::swapWith(Directions direction)
{
    int x_swap, y_swap; // coordinates of the Tile to swap with in the tile board

    switch (direction)
    {
    case top: // Top is towards y-coordinate of 0
        x_swap = x_empty;
        y_swap = y_empty - 1;
        break;
    case bottom: // Bottom is towards y-coordinate of grid_length
        x_swap = x_empty;
        y_swap = y_empty + 1;
        break;
    case left: // Left is towards x-coordinate of 0
        x_swap = x_empty - 1;
        y_swap = y_empty;
        break;
    case right: // Right is towards x-coordinate of grid_length
        x_swap = x_empty + 1;
        y_swap = y_empty;
        break;

    default:
        break;
    }

    // Check if the position of the tile to swap on the direction is valid (in-bounds of tile_board),
    // return 0 (false) if the move direction is invalid, indication no move made
    if ((x_swap < 0) || (x_swap >= grid_length) || (y_swap < 0) || (y_swap >= grid_length))
    {
        return 0;
    }

    // Swap the empty tile and the one in the position of the direction in the tile_board
    Tile *temp = tile_board[y_empty][x_empty];
    tile_board[y_empty][x_empty] = tile_board[y_swap][x_swap];
    tile_board[y_swap][x_swap] = temp;

    // Change the coordinates/position of the empty tile in tile_board to the one that was swapped with
    x_empty = x_swap;
    y_empty = y_swap;
    return 1;
}

int TManager::extractSubTiles(u_char **pixels)
{
    // std::cout << "Extracting subTiles" << std::endl;
    int x_index = 0, y_index = 0; // Tile coordinates in TileManager tile_board
    int x_coord = 0, y_coord = 0; // start coordinates of the pixels for the sub image/tile from the original image

    while (y_index < grid_length)
    {
        u_char **tile_pixels = new u_char *[tile_height];
        for (int row = y_coord; row < y_coord + tile_height; ++row)
        {
            tile_pixels[row - y_coord] = new u_char[tile_width];
            for (int col = x_coord; col < x_coord + tile_width; ++col)
            {
                tile_pixels[row - y_coord][col - x_coord] = pixels[row][col];
            }
        }

        // Create a new tile, add the pixels for the sub-tile and save it to the tile_board
        NDLMDU011::Tile *tile_image = new Tile(tile_width, tile_height);
        tile_image->assignPixels(NDLMDU011::transferArray(tile_pixels, tile_height, tile_width));
        tile_board[y_index][x_index] = tile_image;

        x_index++;             // Moves to the right index for the next tile in the tile board
        x_coord += tile_width; // advance to the leftmost pixel for the next tile in the row

        // When done with tiles in the one row, advance to the next row and first leftmost index for a new tile
        if (x_index >= grid_length)
        {
            x_index = 0;            // leftmost tile in the tile_board for row y_index
            y_index++;              // move to the next row in the tile board
            y_coord += tile_height; // Advance to read the tiles for the new row images
            x_coord = 0;            // leftmost pixel for the new tile from the original image
        }
    }

    //  Set the last (bottom-right) tile on the tile_board as empty tile and its pixel value as 0 for black
    x_empty = grid_length - 1;
    y_empty = grid_length - 1;
    tile_board[y_empty][x_empty]->setTileEmpty();

    return tile_board.size();
}

u_char **TManager::retrieveTileImage(void)
{
    int image_width = tile_width * grid_length;
    int image_height = tile_height * grid_length;

    // Declare a new array of the same size as pixels read from the input image to compile all tile pixels together
    u_char **image_pixels = new u_char *[image_height];
    for (int k = 0; k < image_height; ++k)
    {
        image_pixels[k] = new u_char[image_width];
    }

    int x_index = 0, y_index = 0; // Tile coordinates in TileManager tile_board
    int x_coord = 0, y_coord = 0; // start coordinates of the pixels for  the sub image/tile to the outputimage

    while (y_index < grid_length)
    {
        Tile *tile_image = getTile(x_index, y_index);       // get the pointer to the tile from the tile_board
        u_char **tile_pixels = tile_image->getTilePixels(); // get the array of pixels for the tile

        for (int row = y_coord; row < y_coord + tile_height; ++row)
        {
            for (int col = x_coord; col < x_coord + tile_width; ++col)
            {
                // Copy the pixels from the tile to the output pixel array
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
    }

    return image_pixels;
}

TManager::~TileManager()
{
    // std::cout << "Tile manager destructor called" << std::endl;
    // Delete the dynamically allocated Tile objects from the tile_board
    for (int i = 0; i < grid_length; ++i)
    {
        for (int j = 0; j < grid_length; ++j)
        {
            delete tile_board[i][j];
        }
    }
}

// Transfer/Move the array to another array
u_char **NDLMDU011::transferArray(u_char **arrOrig, int height, int width)
{
    u_char **arrCopy = new u_char *[height];

    for (int i = 0; i < height; i++)
    {
        arrCopy[i] = new u_char[width];

        for (int j = 0; j < width; j++)
        {
            arrCopy[i][j] = arrOrig[i][j];
        }
        delete[] arrOrig[i];
    }
    delete[] arrOrig;

    return arrCopy;
}