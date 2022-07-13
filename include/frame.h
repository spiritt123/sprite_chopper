#pragma once

struct Frame
{
public:
    Frame()
    {
        this->position_x = 0;
        this->position_y = 0;
        this->size_x = 0;
        this->size_y = 0;
    }

    Frame(float position_x, float position_y, float size_x, float size_y)
    {
        this->position_x = position_x;
        this->position_y = position_y;
        this->size_x = size_x;
        this->size_y = size_y;
    }

    ~Frame()
    {}

public:
    float position_x;
    float position_y;
    float size_x;
    float size_y;
};

