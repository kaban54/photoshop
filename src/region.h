#ifndef REGION_H
#define REGION_H

#include "vec.h"
#include "mylist.h"
#include <algorithm>
#include <cmath>

struct Rect {
    double x;
    double y;
    double w;
    double h;

    explicit Rect();

    explicit Rect (double x_, double y_, double w_, double h_);

    explicit Rect (const Vec& p1, const Vec& p2);

    Vec GetPos() const {return Vec (x, y);}

    Vec GetSize() const {return Vec (w, h);}

    Vec GetCenter() const {return Vec (x + w / 2, y + h / 2);}

    double Left() const {return x;}

    double Right() const {return x + w;}

    double Top() const {return y;}

    double Bot() const {return y + h;}

    bool Contains (const Rect& rect) const;

    bool Contains (const Vec& vec) const;

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

    void operator+= (const RegionSet& regset2);

    void operator-= (const RegionSet& regset2);

    void operator^= (const RegionSet& regset2);

    void Move (const Vec& vec);

    bool Contains (const Vec& vec) const;

    void Print() const;
};

void IntersectRegsets (const RegionSet& regset1, const RegionSet& regset2, RegionSet& result);

#endif