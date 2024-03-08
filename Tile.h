#include <vector>
#include <string>

#ifndef _TILE_H
#define _TILE_H
#define u_char unsigned char

namespace NDLMDU011
{
    enum Directions
    {
        top = 1,
        bottom,
        left,
        right
    };
    static int count = 0;


    class Tile
    {
    private:
        unsigned char **pixels;
        int num_pixels, height, width;

    public:
        Tile();
        Tile(int width, int height);
        void assignPixels(u_char **tile_pixels);
        void setTileEmpty(void);
        void setHeight(int height);
        void setWidth(int width);
        int getHeight();
        int getWidth();
        u_char **getTilePixels(void);
        ~Tile();
    };

    class TileManager
    {
    private:
        std::vector<std::vector<NDLMDU011::Tile *>> tile_board;
        int grid_length, grid_size;
        int tile_width, tile_height;
        int x_empty, y_empty; // index position of the empty tile in the tile_board

    public:
        TileManager();
        TileManager(int grid_length, int tile_width, int tile_height);
        void addTile(Tile &tile_image, int x_index, int y_index);
        int swapWith(Directions direction);
        int extractSubTiles(unsigned char **pixels);
        u_char **retrieveTileImage(void);
        NDLMDU011::Tile *getTile(int x_index, int y_index);
        ~TileManager();
    };

    unsigned char **transferArray(unsigned char **arrOrig, int height, int width);
    void readPGMImage(std::string filename, unsigned char **pixels, int &pixel_width, int &pixel_height);
    void writeImage(int width, int height, std::string filename, unsigned char **array);

    void writeSummaryImage(std::string filename, int moves, int image_width, int image_height, int border_size);
}

#endif