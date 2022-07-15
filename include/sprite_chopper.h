#pragma once

#include <fstream>
#include <vector>
#include <string>
#include "frame.h"
#include "vector2f.h"

class SpriteChopper
{
public:
    SpriteChopper();
    SpriteChopper(std::string filename);
    ~SpriteChopper();

    void save();
    void saveToXml();
    void saveToJson();
    void load();
    void deleteSelectedFrames();

    const std::vector<Frame>& getFrames();
    void addNewFrame(const Frame &frame);
    void copyFrame();

    void moveSelectedFrames(Vector2f offset);
    void movePositionPointInSelectedFrames(Vector2f offset);
    void moveSizePointInSelectedFrames(Vector2f offset);

    const Frame& getCurrentFrame();
    bool isSelectStatus();

    void updateMousePosition(Vector2f mouse_position);
    void mousePressed();
    void mousePressedForMove();
    void mouseReleased();

    Frame* getFrameWithConsistThisTarget(Vector2f target);

private:
    bool isOutOfBound(Vector2f lr, Vector2f rd, Vector2f target);
    
private:
    std::vector<Frame> _frames;
    Frame _current_frame;
    std::string _filename;
    Vector2f _mouse_position;
    bool _is_select_area;
};


