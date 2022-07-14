#include "sprite_chopper.h"

SpriteChopper::SpriteChopper()
    :_filename("sprite_chopper.coords")
{
    _is_select_area = false;
}

SpriteChopper::SpriteChopper(std::string filename)
    : _filename(filename)
{
    _is_select_area = false;
}

SpriteChopper::~SpriteChopper()
{}

void SpriteChopper::save()
{
    std::ofstream ofs(_filename, std::ios::out | std::ios::binary);
    for (const auto &s : _frames)
    {
        ofs.write((const char*) &s.position.x, sizeof(float));
        ofs.write((const char*) &s.position.y, sizeof(float));
        ofs.write((const char*) &s.size.x, sizeof(float));
        ofs.write((const char*) &s.size.y, sizeof(float));
    }
    ofs.close();
}

void SpriteChopper::load()
{
    std::ifstream ifs(_filename, std::ios::binary | std::ios::in);
    _frames.clear();
    Vector2f position;
    Vector2f size;
    while (
        ifs.read(reinterpret_cast<char*>(&position.x), sizeof(float)) &&
        ifs.read(reinterpret_cast<char*>(&position.y), sizeof(float)) &&
        ifs.read(reinterpret_cast<char*>(&size.x), sizeof(float)) &&
        ifs.read(reinterpret_cast<char*>(&size.y), sizeof(float)) )
    {
        _frames.push_back({position, size});
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

void SpriteChopper::moveSelectedFrames(Vector2f offset)
{
    for (auto &f : _frames)
    {
        if(f.status == STATUS::ACTIVE)
        {
            f.move(offset);
        }
    }
}

const Frame& SpriteChopper::getCurrentFrame()
{
    if (_is_select_area)
        return _current_frame;
    else
        return empty_frame;
}

bool SpriteChopper::isSelectStatus()
{
    return _is_select_area;
}

void SpriteChopper::updateMousePosition(Vector2f mouse_position)
{
    _mouse_position = mouse_position;
    if (_is_select_area)
        _current_frame.size = _mouse_position - _current_frame.position;
}

void SpriteChopper::mousePressed()
{
    Frame *fr = getFrameWithConsistThisTarget(_mouse_position);
    if (fr == nullptr)
    {
        _current_frame.position = _mouse_position;
        _is_select_area = true;
    }
    else
    {
        fr->changeStatus();
    }
}

void SpriteChopper::mouseReleased()
{
    Vector2f rect_size = _mouse_position - _current_frame.position;
    if (!_is_select_area)
        return;
    if (rect_size.x < 0)
    {
        _current_frame.position.x += rect_size.x;
        rect_size.x *= -1;
    }
    if (rect_size.y < 0)
    {
        _current_frame.position.y += rect_size.y;
        rect_size.y *= -1;
    }
    _current_frame.size = rect_size;
    addNewFrame({_current_frame.position, _current_frame.size});
    _current_frame.size = {0, 0};
    _is_select_area = false;
}

Frame* SpriteChopper::getFrameWithConsistThisTarget(Vector2f target)
{
    for (int i = 0; i < _frames.size(); ++i)
    {
        Frame fr = _frames[i];
        if (!isOutOfBound(fr.position, 
                          fr.position + fr.size, 
                          target))
            return &_frames[i];
    }
    return nullptr;
}

bool SpriteChopper::isOutOfBound(Vector2f lr, Vector2f rd, Vector2f target)
{
    if (target.x > rd.x || target.x < lr.x)
        return true;
    if (target.y > rd.y || target.y < lr.y)
        return true;
    return false;
}

