#include "region.h"

Rect::Rect():
    x (0),
    y (0),
    w (0),
    h (0)
    {}

Rect::Rect (double x_, double y_, double w_, double h_):
    x (x_),
    y (y_),
    w (w_),
    h (h_)
    {}

Rect::Rect (const Vec& p1, const Vec& p2):
    x (p1.x < p2.x ? p1.x : p2.x),
    y (p1.y < p2.y ? p1.y : p2.y),
    w (fabs (p2.x - p1.x)),
    h (fabs (p2.y - p1.y))
    {}

bool Rect::Contains (const Rect& rect) const {
    return (rect.Left()  >= Left()  &&
            rect.Right() <= Right() &&
            rect.Top()   >= Top()   &&
            rect.Bot()   <= Bot()   );
}

bool Rect::Contains (const Vec& vec) const {
    return (vec.x >= Left()  &&
            vec.x <= Right() &&
            vec.y >= Top()   &&
            vec.y <= Bot()   );
}

void Rect::Print () const {
    std::cerr << " Rect((" << x << ", " << y << "), "
                       "(" << w << ", " << h << ")) ";
}


void Rect::Move (const Vec& vec) {
    x += vec.x;
    y += vec.y;
}

bool Intersect (const Rect& rect1, const Rect& rect2) {
    return (rect1.Left() < rect2.Right() &&
            rect2.Left() < rect1.Right() &&
            rect1.Top()  < rect2.Bot()   &&
            rect2.Top()  < rect1.Bot()   );
}

bool HaveCommonSide (const Rect& rect1, const Rect& rect2) {
    return ((rect1.Left() == rect2.Left()  && rect1.Right() == rect2.Right() &&
            (rect1.Top()  == rect2.Bot()   || rect1.Bot()   == rect2.Top())) ||
            (rect1.Top()  == rect2.Top()   && rect1.Bot()   == rect2.Bot()   &&
            (rect1.Left() == rect2.Right() || rect1.Right() == rect2.Left())));
}


RegionSet::RegionSet():
    regions () {}

void RegionSet::MergeRegions() {
    ListNode<Rect>* node1 = nullptr;
    regions.Iterate(node1);

    while (node1 != nullptr) {
        ListNode<Rect>* node2 = node1;
        regions.Iterate(node2);
        while (node2 != nullptr) {
            Rect r1 = node1 -> val;
            Rect r2 = node2 -> val;
            if (r1.Contains(r2)) {
                regions.Iterate(node2);
                regions.Remove (node2 -> prev);
                continue;
            }
            if (r2.Contains(r1)) {
                node1 = node1 -> prev;
                regions.Remove (node1 -> next);
                break;
            }
            if (HaveCommonSide(r1, r2)) {
                Vec v1 (std::min(r1.Left(), r2.Left()), std::min(r1.Top(), r2.Top()));
                Vec v2 (std::max(r1.Right(), r2.Right()), std::max(r1.Bot(), r2.Bot()));
                node1 -> val = Rect (v1, v2);
                regions.Remove (node2);
                node1 = nullptr;
                break;
            }
            regions.Iterate(node2);
        }
        regions.Iterate(node1);
    }
}

void RegionSet::AddRegion (const Rect& region) {
    if (region.w * region.h == 0) return;
    regions.InsertHead(region);
    MergeRegions();
}

void RegionSet::SubtractRegion (const Rect& region) {
    bool restart = true;
    while (restart) {
        restart = false;

        ListNode<Rect>* node = nullptr;
        regions.Iterate(node);

        while (node != nullptr) {
            if (Intersect (region, node -> val)) {
                Rect r1 = node -> val;
                Rect r2 = region;

                RegionSet new_regions;

                double X[4] = {r1.Left(), r2.Left(), r1.Right(), r2.Right()};
                double Y[4] = {r1.Top() , r2.Top() , r1.Bot()  , r2.Bot()  };

                if (r2.Left() < r1.Left()) {
                    X[0] = r2.Left();
                    X[1] = r1.Left();
                }
                if (r2.Right() < r1.Right()) {
                    X[2] = r2.Right();
                    X[3] = r1.Right();
                }
                if (r2.Top() < r1.Top()) {
                    Y[0] = r2.Top();
                    Y[1] = r1.Top();
                }
                if (r2.Bot() < r1.Bot()) {
                    Y[2] = r2.Bot();
                    Y[3] = r1.Bot();
                }

                for (int x_index = 0; x_index < 3; x_index++) {
                    if (X[x_index] < r1.Left() || X[x_index] >= r1.Right()) continue;
                    if (X[x_index] == X[x_index + 1]) continue;
                    for (int y_index = 0; y_index < 3; y_index++) {
                        if (Y[y_index] < r1.Top() || Y[y_index] >= r1.Bot()) continue;
                        if (Y[y_index] == Y[y_index + 1]) continue;
                        Rect reg (Vec(X[x_index], Y[y_index]), Vec(X[x_index + 1], Y[y_index + 1]));
                        if (!Intersect(reg, r2)) new_regions.AddRegion(reg);
                    }
                }
                regions.Remove (node);
                (*this) += new_regions;
                restart = true;
                break;
            }
            else regions.Iterate(node);
        }
    }
}

void RegionSet::operator+= (const RegionSet& regset2) {
    ListNode<Rect>* node = nullptr;
    regset2.regions.Iterate(node);
    while (node != nullptr) {
        AddRegion (node -> val);
        regset2.regions.Iterate(node);
    }
}

void RegionSet::operator-= (const RegionSet& regset2) {
    ListNode<Rect>* node = nullptr;
    regset2.regions.Iterate(node);
    while (node != nullptr) {
        SubtractRegion (node -> val);
        regset2.regions.Iterate(node);
    }
}

void RegionSet::operator^= (const RegionSet& regset2) {
    RegionSet tmp;
    tmp += *this;
    tmp -= regset2;
    *this -= tmp;
}

void RegionSet::Move (const Vec& vec) {
    ListNode<Rect>* node = nullptr;
    regions.Iterate(node);

    while (node != nullptr) {
        node -> val.Move(vec);
        regions.Iterate(node);
    }
}

bool RegionSet::Contains (const Vec& vec) const {
    ListNode<Rect>* node = nullptr;
    regions.Iterate(node);
    while (node != nullptr) {
        if (node -> val.Contains(vec)) return true;
        regions.Iterate(node);
    }
    return false;
}

void RegionSet::Print() const {
    ListNode<Rect>* node = nullptr;
    regions.Iterate(node);
    while (node != nullptr){
        std::cerr << "\t";
        node -> val.Print();
        std::cerr << "\n";
        regions.Iterate(node);
    }
}

void IntersectRegsets (const RegionSet& regset1, const RegionSet& regset2, RegionSet& result) {
    RegionSet tmp;
    tmp.regions.InsertElems (regset1.regions);
    tmp -= regset2;
    result.regions.InsertElems (regset1.regions);
    result -= tmp;
}