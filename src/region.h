#ifndef REGION_H
#define REGION_H

#include "vec2.h"
#include "mylist.h"
#include <algorithm>
#include <cmath>

using namespace plugin;

struct Rect {
    double x;
    double y;
    double w;
    double h;

    explicit Rect();

    explicit Rect (double x_, double y_, double w_, double h_);

    explicit Rect (const Vec2& p1, const Vec2& p2);

    Vec2 GetPos() const {return Vec2 (x, y);}

    Vec2 GetSize() const {return Vec2 (w, h);}

    Vec2 GetCenter() const {return Vec2 (x + w / 2, y + h / 2);}

    double Left() const {return x;}

    double Right() const {return x + w;}

    double Top() const {return y;}

    double Bot() const {return y + h;}

    bool Contains (const Rect& rect) const;

    bool Contains (const Vec2& vec) const;

    void Print() const;

    void Move(const Vec2& vec);
};

bool Intersect (const Rect& rect1, const Rect& rect2);

bool HaveCommonSide (const Rect& rect1, const Rect& rect2);

class RegionSet {
    public:
    MyList<Rect> regions;

    explicit RegionSet();

    void MergeRegions();

    void AddRegion (const Rect& region);

    void SubtractRegion (const Rect& region);

    void operator+= (const RegionSet& regset2);

    void operator-= (const RegionSet& regset2);

    void operator^= (const RegionSet& regset2);

    void Move (const Vec2& vec);

    bool Contains (const Vec2& vec) const;

    void Print() const;
};

void IntersectRegsets (const RegionSet& regset1, const RegionSet& regset2, RegionSet& result);

#endif