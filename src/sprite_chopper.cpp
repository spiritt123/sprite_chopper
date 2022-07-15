#include "sprite_chopper.h"
#include <vector>
#include <algorithm>

SpriteChopper::SpriteChopper()
    :_filename("sprite_chopper")
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
    std::ofstream ofs(_filename + ".coords", std::ios::out | std::ios::binary);
    for (const auto &f : _frames)
    {
        ofs.write((const char*) &f.position.x, sizeof(float));
        ofs.write((const char*) &f.position.y, sizeof(float));
        ofs.write((const char*) &f.size.x, sizeof(float));
        ofs.write((const char*) &f.size.y, sizeof(float));
    }
    ofs.close();
}

void SpriteChopper::saveToXml()
{
    std::ofstream ofs(_filename + ".xml", std::ios::out);
    std::string out = "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n";
    out += "<array>\n";
    for (const auto &f : _frames)
    {
        out += "<frame>\n";
        out += "\t<position>\n";
        out += "\t\t<x>" + std::to_string(f.position.x) + "<\\x>\n";
        out += "\t\t<y>" + std::to_string(f.position.y) + "<\\y>\n";
        out += "\t<\\position>\n";
        out += "\t<size>\n";
        out += "\t\t<x>" + std::to_string(f.size.x) + "<\\x>\n";
        out += "\t\t<y>" + std::to_string(f.size.y) + "<\\y>\n";
        out += "\t<\\size>\n";
        out += "<\\frame>\n";
    }
    out += "<\\array>";
    ofs << out;
    ofs.close();
}

void SpriteChopper::saveToJson()
{
    std::ofstream ofs(_filename + ".json", std::ios::out);
    std::string out = "[";
    for (const auto &f : _frames)
    {
        out += "{\n";
        out += "\t\"position\" : {\n";
        out += "\t\t\"x\" : " + std::to_string(f.position.x) + ",\n";
        out += "\t\t\"y\" : " + std::to_string(f.position.y) + "\n";
        out += "\t},\n";
        out += "\t\"size\" : {\n";
        out += "\t\t\"x\" : " + std::to_string(f.size.x) + ",\n";
        out += "\t\t\"y\" : " + std::to_string(f.size.y) + "\n";
        out += "\t}\n";
        out += "},\n";
    }
    out.pop_back();
    out.pop_back();
    out += "]";
    ofs << out;
    ofs.close();
}

void SpriteChopper::load()
{
    std::ifstream ifs(_filename + ".coords", std::ios::binary | std::ios::in);
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
    
void SpriteChopper::deleteSelectedFrames()
{
    _frames.erase(
        std::remove_if(_frames.begin(), 
                       _frames.end(),
                       [](const Frame &f) { return f.status == STATUS::ACTIVE; }
                      ), _frames.end()
    );
}

void SpriteChopper::copyFrame()
{
    auto it = std::find_if(_frames.begin(), _frames.end(), [](const Frame &f) {return f.status == STATUS::ACTIVE;});
    if (it == _frames.end())
        return;
    Frame copy_frame = *it;
    copy_frame.status = STATUS::ACTIVE;
    _frames.push_back(copy_frame);
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
    
void SpriteChopper::movePositionPointInSelectedFrames(Vector2f offset)
{
    for (auto &f : _frames)
    {
        if(f.status == STATUS::ACTIVE)
        {
            f.position += offset;
            f.size     -= offset;
        }
    }
}

void SpriteChopper::moveSizePointInSelectedFrames(Vector2f offset)
{
    for (auto &f : _frames)
    {
        if(f.status == STATUS::ACTIVE)
        {
            f.size += offset;
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
    auto it = std::find_if(_frames.begin(), _frames.end(), [](const Frame &f) {return f.status == STATUS::MOVED;});
    if (it != _frames.end())
    {
        it->position = _mouse_position - it->size / 2;
    }

}

void SpriteChopper::mousePressed()
{
    Frame *fr = getFrameWithConsistThisTarget(_mouse_position);
    if (fr == nullptr)
    {
        _current_frame.position = _mouse_position;
        _is_select_area = true;
    }
    else if (fr->status == STATUS::ACTIVE)
    {
        fr->status = STATUS::MOVED;
    }
}

void SpriteChopper::mousePressedForMove()
{
    Frame *fr = getFrameWithConsistThisTarget(_mouse_position);
    if (fr != nullptr)
    {
        fr->status = STATUS::MOVED;
    }
}

void SpriteChopper::mouseReleased()
{
    Frame *fr = getFrameWithConsistThisTarget(_mouse_position);
    if (fr != nullptr)
    {
        _current_frame.size = {0, 0};
        _is_select_area = false;
        fr->changeStatus();
        return;
    }

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

