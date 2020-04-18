#pragma once

#include<utility>
#include<cmath>
#include<iostream>

struct Vec2 {
    double x;
    double y;

    Vec2();
    Vec2(double _x, double _y);
    Vec2(const Vec2 & other);
    Vec2(Vec2 && other);
    Vec2& operator=(Vec2 && other);
    Vec2& operator+=(const Vec2 &other);
    Vec2& operator-=(const Vec2 &other);
    const Vec2 operator+(const Vec2 &other) const;
    const Vec2 operator-(const Vec2 &other) const;
    Vec2& operator/=(double div);
    friend std::ostream &operator<<(std::ostream & output, const Vec2 &vec);
    bool operator==(const Vec2 & other);
    bool operator!=(const Vec2 & other);

    double X();
    double Y();
    int GetXInt();
    int GetYInt();
    double Distance(const Vec2 & other);
};