#include "vec2.h"


Vec2::Vec2():
    x (0),
    y (0) {}

Vec2::Vec2(double x_, double y_):
    x (x_),
    y (y_) {}

inline Vec2 Vec2::operator-() const {
    return Vec2(-x, -y);
}

inline double Vec2::GetLen() const {
    return std::sqrt(x * x + y * y);
}

inline void Vec2::Normalize() {
    *this /= GetLen();
}

inline Vec2 Vec2::operator!() const {
    Vec2 ret = *this;
    ret.Normalize();
    return ret;
}

void Vec2::Rotate (const double angle) {
    double sin_ = std::sin (angle);
    double cos_ = std::cos (angle);

    double new_x = x * cos_ - y * sin_;
    y = x * sin_ + y * cos_;
    x = new_x;
}

inline Vec2 operator+= (Vec2& vec1, const Vec2& vec2) {
    vec1.x += vec2.x;
    vec1.y += vec2.y;
    return vec1;
}

inline Vec2 operator-= (Vec2& vec1, const Vec2& vec2) {
    vec1.x -= vec2.x;
    vec1.y -= vec2.y;
    return vec1;
}

inline Vec2 operator*= (Vec2& vec, const double scalar) {
    vec.x *= scalar;
    vec.y *= scalar;
    return vec;
}

inline Vec2 operator/= (Vec2& vec, const double scalar) {
    vec.x /= scalar;
    vec.y /= scalar;
    return vec;
}

inline Vec2 operator+ (Vec2 vec1, const Vec2& vec2) {
    return vec1 += vec2;
}

inline Vec2 operator- (Vec2 vec1, const Vec2& vec2) {
    return vec1 -= vec2;
}

inline Vec2 operator* (Vec2 vec, const double scalar) {
    return vec *= scalar;
}

inline Vec2 operator* (const double scalar, Vec2 vec) {
    return vec *= scalar;
}

inline Vec2 operator/ (Vec2 vec, const double scalar) {
    return vec /= scalar;
}

inline double operator, (const Vec2& vec1, const Vec2& vec2) {
    return vec1.x * vec2.x + vec1.y * vec2.y;
}

inline Vec2 operator^ (const Vec2& vec1, const Vec2& vec2) {
    return Vec2(vec1.x * vec2.x, vec1.y * vec2.y);
}

double GetRandAngle () {
    return std::rand() / (RAND_MAX / (M_PI * 2));
}