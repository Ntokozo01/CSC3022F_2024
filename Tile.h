#include <vector>

#ifndef _TILE_H
#define _TILE_H

namespace NDLMDU011
{
    class TileManager
    {
    private:
        std::vector<unsigned char**> tile_board; 
        int number_of_tiles;
    
    public:
        TileManager();
        void readPGMImage();
        ~TileManager();
    };

        class Tile
    {
    private:
        unsigned char **tile;

    public:
        Tile();
        ~Tile();
    };
}

#endif