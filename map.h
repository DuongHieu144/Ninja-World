#ifndef MAP_H
#define MAP_H

#include<vector>
#include<iostream>
#include <fstream>
#include <iostream>

void LoadMapFromFile(std::string path, std::vector<std::vector<int> >& map_data);

#endif