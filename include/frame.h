#pragma once
#include <cmath>
#include "enums.h"
#include "vector2f.h"

struct Frame
{
public:
    Frame()
    {
        this->position = Vector2f(0, 0);
        this->size = Vector2f(10.f, 10.f);
        status = STATUS::PASSIVE;
    }

    Frame(Vector2f position, Vector2f size)
    {
        if (size.x < 10) size.x = 10.f;
        if (size.y < 10) size.y = 10.f;
        this->position = position;
        this->size = size;
        status = STATUS::PASSIVE;
    }

    ~Frame() {}

    void move(Vector2f offset)
    {
        position += offset;
    }

    void changeStatus()
    {
        if (status == STATUS::ACTIVE)
            status = STATUS::PASSIVE;
        else if (status == STATUS::PASSIVE)
            status = STATUS::ACTIVE;
    }

public:
    Vector2f position;
    Vector2f size;
    STATUS status;
};

inline Frame empty_frame = {{0, 0}, {0, 0}};
