#include "sprite_chopper.h"

SpriteChopper::SpriteChopper()
    :_filename("sprite_chopper.coords")
{
}

SpriteChopper::SpriteChopper(std::string filename)
    : _filename(filename)
{

}

SpriteChopper::~SpriteChopper()
{}

void SpriteChopper::save()
{
    std::ofstream ofs(_filename, std::ios::out | std::ios::binary | std::ios::app);
    for (const auto &s : _frames)
    {
        ofs.write((const char*) &s.position_x, sizeof(float));
        ofs.write((const char*) &s.position_y, sizeof(float));
        ofs.write((const char*) &s.size_x, sizeof(float));
        ofs.write((const char*) &s.size_y, sizeof(float));
    }
    ofs.close();
}

void SpriteChopper::load()
{
    std::ifstream ifs(_filename, std::ios::binary | std::ios::in);
    _frames.clear();
    float position_x;
    float position_y;
    float size_x;
    float size_y;
    while (
        ifs.read(reinterpret_cast<char*>(&position_x), sizeof(float)) &&
        ifs.read(reinterpret_cast<char*>(&position_y), sizeof(float)) &&
        ifs.read(reinterpret_cast<char*>(&size_x), sizeof(float)) &&
        ifs.read(reinterpret_cast<char*>(&size_y), sizeof(float)) )
    {
        _frames.push_back({position_x, position_y, size_x, size_y});
    }
}

const std::vector<Frame>& SpriteChopper::getFrames()
{
    return _frames;
}

void SpriteChopper::addNewFrame(const Frame &frame)
{
    _frames.push_back(frame);
}





