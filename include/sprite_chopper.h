#pragma once

#include <fstream>
#include <vector>
#include <string>
#include "frame.h"

class SpriteChopper
{
public:
    SpriteChopper();
    SpriteChopper(std::string filename);
    ~SpriteChopper();

    void save();
    void load();

    const std::vector<Frame>& getFrames();
    void addNewFrame(const Frame &frame);
    
private:
    std::vector<Frame> _frames;
    std::string _filename;
};


