#ifndef VEC2_H
#define VEC2_H

#include <cmath>
#include <cstdlib>

namespace plugin {
    struct Vec2 {
        double x;
        double y;

        explicit Vec2 ();

        explicit Vec2 (double x_, double y_);

        Vec2 operator-() const;

        double GetLen() const;

        void Normalize();

        Vec2 operator!() const;

        void Rotate (const double angle);
    };

    Vec2 operator+= (Vec2& vec1, const Vec2& vec2);

    Vec2 operator-= (Vec2& vec1, const Vec2& vec2);

    Vec2 operator*= (Vec2& vec, const double scalar);

    Vec2 operator/= (Vec2& vec, const double scalar);

    Vec2 operator+ (Vec2 vec1, const Vec2& vec2);

    Vec2 operator- (Vec2 vec1, const Vec2& vec2);

    Vec2 operator* (Vec2 vec, const double scalar);

    Vec2 operator* (const double scalar, Vec2 vec);

    Vec2 operator/ (Vec2 vec, const double scalar);

    double operator, (const Vec2& vec1, const Vec2& vec2);

    Vec2 operator^ (const Vec2& vec1, const Vec2& vec2);


    double GetRandAngle ();
}

#endif