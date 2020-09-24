#ifndef UTILE_H
#define UTILE_H

#include <SFML/graphics.hpp>

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <cmath>
#include <random>
#include <iomanip>
#include <sstream>
#include <fstream>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

enum TerrainEnum{dirt,mud,grass,stone,water};
enum BuildingEnum{CHEST1,ASSEMBLINGMACHINE1};
enum TransportSystemEnum{BELT1};

enum Direction{
        EAST,
        WEST,
        NORD,
        SOUTH,
    };

int rand_a_b(int a,int b);

#endif //UTILE_H
