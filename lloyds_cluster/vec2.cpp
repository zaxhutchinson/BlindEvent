#include"vec2.hpp"

Vec2::Vec2()
    : x(0.0f), y(0.0f)
{}

Vec2::Vec2(double _x, double _y)
    : x(_x), y(_y)
{}

Vec2::Vec2(const Vec2 &other) 
    : x(other.x), y(other.y)
{}

Vec2::Vec2(Vec2 && other) {
    *this = std::move(other);
}

Vec2& Vec2::operator=(Vec2&&other) {
    if(this != &other) {
        x = other.x;
        y = other.y;
    }
    return *this;
}

Vec2& Vec2::operator+=(const Vec2 &other) {
    this->x += other.x;
    this->y += other.y;
    return *this;
}
Vec2& Vec2::operator-=(const Vec2 &other) {
    this->x -= other.x;
    this->y -= other.y;
    return *this;
}
const Vec2 Vec2::operator+(const Vec2 &other) const {
    return Vec2(*this) += other;
}
const Vec2 Vec2::operator-(const Vec2 &other) const {
    return Vec2(*this) -= other;
}
Vec2& Vec2::operator/=(double div) {
    this->x /= div;
    this->y /= div;
    return *this;
}
std::ostream &operator<<(std::ostream & output, const Vec2 &vec) {
    output << vec.x<< "," << vec.y;
    return output;
}
bool Vec2::operator==(const Vec2 & other) {
    return x==other.x && y==other.y;
}
bool Vec2::operator!=(const Vec2 & other) {
    return !(*this==other);
}


double Vec2::X() {
    return x;
}
double Vec2::Y() {
    return y;
}
int Vec2::GetXInt() {
    return static_cast<int>(x);
}
int Vec2::GetYInt() {
    return static_cast<int>(y);
}

double Vec2::Distance(const Vec2 & other) {
    return std::sqrt(
        std::pow(other.x-x,2.0) +
        std::pow(other.y-y,2.0)
    );
}