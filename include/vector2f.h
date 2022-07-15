#pragma once

struct Vector2f
{
    float x, y;

    Vector2f(float X = 0, float Y = 0) : x(X), y(Y) {}
    Vector2f(const Vector2f &vector) 
    {
        x = vector.x;
        y = vector.y;
    }
    
    Vector2f& operator=(const Vector2f &vector)
    {
        this->x = vector.x;
        this->y = vector.y;
        return *this;
    }

    friend const Vector2f operator+(const Vector2f &left, const Vector2f &right)
    {
        Vector2f out;
        out.x = left.x + right.x;
        out.y = left.y + right.y;
        return out;
    }

    friend const Vector2f operator/(const Vector2f &left, float value)
    {
        Vector2f out;
        out.x = left.x / value;
        out.y = left.y / value;
        return out;
    }

    friend Vector2f operator+=(Vector2f &left, const Vector2f &right)
    {
        left.x += right.x;
        left.y += right.y;
        return left;
    }

    friend const Vector2f operator-(const Vector2f &left, const Vector2f &right)
    {
        Vector2f out;
        out.x = left.x - right.x;
        out.y = left.y - right.y;
        return out;
    }

    friend Vector2f operator-=(Vector2f &left, const Vector2f &right)
    {
        left.x -= right.x;
        left.y -= right.y;
        return left;
    }
};
