#include "Tile.h"
#include <vector>
#include <iostream>
#include <fstream>

#define Ttile NDLMDU011::Tile
#define TManager NDLMDU011::TileManager


Ttile::~Tile(){
    delete[] tile;
}