#include <vector>
#include <string>

#ifndef _TILE_H
#define _TILE_H

namespace NDLMDU011
{
    enum Directions {top = 1, bottom, left, right};

    class Tile
    {
    private:
        unsigned char **pixels;
        int x_start, x_end;
        int y_start, y_end;
        int num_pixels, height, width;

    public:
        Tile();
        Tile(int width, int height);
        Tile(int x_start, int x_end, int y_start, int y_end);
        void assignPixels(unsigned char **tile_pixels);
        void setTileEmpty(void);
        int getHeight();
        int getWidth();
        unsigned char **getTilePixels(void);
        ~Tile();
    };

    class TileManager
    {
    private:
        std::vector<std::vector<NDLMDU011::Tile>> tile_board;
        int grid_length, grid_size;
        int tile_width, tile_height;
        int x_empty, y_empty; // index position of the empty tile in the tile_board

    public:
        TileManager();
        TileManager(int grid_length, int tile_width, int tile_height);
        void addTile(Tile tile_image, int x_index, int y_index);
        void swapWith(Directions direction);
        int extractSubTiles(unsigned char **pixels);
        unsigned char **retrieveTileImage(void);
        NDLMDU011::Tile getTile(int x_index, int y_index);
        ~TileManager();
    };

    unsigned char **deepCopy(unsigned char **arrOrig, int height, int width);
    void print2DArray(int x_start, int x_end, int y_start, int y_end);
    void writeImage(int width, int height, std::string filename, unsigned char **array);
}

#endif