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
        u_char **pixels;
        int num_pixels;
        int height, width; // tile image dimensions

    public:
        Tile();
        Tile(int width, int height);

        /**
         * @brief assigns the pixels data to the Tile object
         * @param tile_pixels 2D unsigned char pixel array which is sub-image of the input image 
         */
        void assignPixels(u_char **tile_pixels);

        /**
         * @brief Set the pixel values of the tile to black (0) to indicate an empty tile
         */
        void setTileEmpty(void);
    
        void setHeight(int height);
        void setWidth(int width);
        int getHeight();
        int getWidth();

        /**
         * @brief returns the pixels of this Tile image
         * @return 2D array
         */
        u_char **getTilePixels(void);
        ~Tile();
    };

    class TileManager
    {
    private:
        std::vector<std::vector<NDLMDU011::Tile *>> tile_board;
        int grid_length, grid_size;
        int tile_width, tile_height; // the pixel dimensions of a tile image
        int x_empty, y_empty;        // index position of the empty tile in the tile_board

    public:
        TileManager();
        TileManager(int grid_length, int tile_width, int tile_height);

        /**
         * @brief Adds a reference to the Tile object into the tile_board at the provided coordinates
         * @param tile_image a reference or pointer to the Tile object dynamically instantiated
         * @param x_index the x/horizontal coordinate of the tile to the tile_board
         * @param y_index the y/vertical coordinate of the tile to the tile_board
         */
        void addTile(Tile &tile_image, int x_index, int y_index);

        /**
         * @brief Swaps the empty tile with an adjacent tile in the position given by direction in the tile_board
         * @param direction the position of the tile to swap with in the empty board
         * @return 1, to indicate 1 single swap meaning swap was successful
         */
        int swapWith(Directions direction);

        /**
         * @brief Creates/extract the Tiles (pixels) from the input image based on the grid_size
         * @param pixels 2D unsigned char array which contains the pixels of the input image
         * @return the number of Tiles extracted
         */
        int extractSubTiles(u_char **pixels);

        /**
         * @brief does the reverse of extracting Tiles by combining them to 2D unsigned char pixels array for output image
         * @return 2D unsigned char array which contains the pixels of the output image
         */
        u_char **retrieveTileImage(void);

        /**
         * @brief get the pointer to the Tile object on the tile_board in the given coordinate values
         * @param x_index the x/horizontal coordinate of the tile to the tile_board
         * @param y_index the y/vertical coordinate of the tile to the tile_board
         * @return a pointer to the Tile object 
         */
        NDLMDU011::Tile *getTile(int x_index, int y_index);

        ~TileManager();
    };

    /**
     * @brief moves the 2D array 
     * @param width number horizontal colums of the array
     * @param height number vertical rows of the array
     * @return 2D array of the same size  
     */
    u_char **transferArray(u_char **arrOrig, int height, int width);
    void readPGMImage(std::string filename, u_char **pixels, int &pixel_width, int &pixel_height);

    /**
     * @brief writes out the image of the given pixels array
     * @param width width of the output image
     * @param height height of the output imag
     * @param array 2D array for pixels data of the image of dimensions height and width for rows and columns
     */
    void writeImage(int width, int height, std::string filename, u_char **array);

    /**
     * @brief writes out the SummaryImage by compiling the Images pixels for moves 0 to the given number of moves
     * @param filename the output file name of the Summary image
     * @param moves the number of moves that were applied to move the empty tile which indicate number of images contained
     * @param image_width width of each image to be contained
     * @param image_height height of each image to be contained
     * @param boarder_size size of the margin/space between images on the Summary image to be added
     */
    void writeSummaryImage(std::string filename, int moves, int image_width, int image_height, int border_size);
}

#endif