#ifndef REGION_H
#define REGION_H

#include "vec.h"
#include "mylist.h"
#include <algorithm>
#include <cmath>

class Rect {
    public:
    Vec vert1;
    Vec vert2;

    explicit Rect();

    explicit Rect (const Vec& v1, const Vec& v2);

    double GetWidth() const;

    double GetHeight() const;

    Vec GetCenter() const;

    bool Contains (const Rect& rect) const;

    void Print() const;

    void Move(const Vec& vec);
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

    const RegionSet& operator+= (const RegionSet& regset2);

    const RegionSet& operator-= (const RegionSet& regset2);

    void Move (const Vec& vec);
};

void IntersectRegsets (const RegionSet& regset1, const RegionSet& regset2, RegionSet& result);

#endif