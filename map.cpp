#include "map.h"

int map_width = 0;
int map_height = 0;

void LoadMapFromFile(std::string path, std::vector<std::vector<int> >& map_data) 
{
    std::ifstream file(path);
    if (!file) 
    {
        std::cerr << "Cannot open map file: " << path << "\n";
        return;
    }

    file>>map_width>>map_height;
    map_data.resize(map_height, std::vector<int>(map_width));

    for (int y = 0; y < map_height/24; y++)
    {
        for (int x = 0; x < map_width/16; x++)
        {
            file >> map_data[y][x];
        }
    }
}
