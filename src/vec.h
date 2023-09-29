#ifndef VEC_H
#define VEC_H

#include <cmath>
#include <cstdlib>

class Vec {
    public:
    double x;
    double y;
    double z;

    explicit Vec (double x_, double y_, double z_ = 0);

    void SetPos (double x_, double y_, double z_ = 0);

    Vec operator-() const;

    double GetLen() const;

    void Normalize();

    Vec operator!() const;

    void RotateAroundZ (const double angle);
};

Vec operator+ (const Vec& vec1, const Vec& vec2);

Vec operator- (const Vec& vec1, const Vec& vec2);

Vec operator* (const Vec& vec, const double scalar);

Vec operator* (const double scalar, const Vec& vec);

Vec operator/ (const Vec& vec, const double scalar);

Vec operator+= (Vec& vec1, const Vec& vec2);

Vec operator-= (Vec& vec1, const Vec& vec2);

Vec operator*= (Vec& vec, const double scalar);

Vec operator/= (Vec& vec, const double scalar);

double operator, (const Vec& vec1, const Vec& vec2);

Vec operator^ (const Vec& vec1, const Vec& vec2);

Vec operator* (const Vec& vec1, const Vec& vec2);


class Ray {
    public:
    Vec start;
    Vec dir;

    explicit Ray (const Vec& start_, const Vec& dir_);
};

double Distance (Ray ray, Vec point);

double GetRandAngle ();

#endif