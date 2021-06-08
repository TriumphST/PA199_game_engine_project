#pragma once
#include <string>

using namespace std;

class Texture
{
public:
    Texture() {};
    Texture(const char* filename);
    int width, height, nrChannels;
    unsigned char* data;
};

