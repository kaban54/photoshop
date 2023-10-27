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
    ListNode<Rect>* end_of_list = regions.EndOfList();
    ListNode<Rect>* node1 = regions.GetHead();

    while (node1 != end_of_list) {
        ListNode<Rect>* node2 = node1 -> next;
        while (node2 != end_of_list) {
            Rect r1 = node1 -> val;
            Rect r2 = node2 -> val;
            if (r1.Contains(r2)) {
                node2 = node2 -> next;
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
                node1 = end_of_list;
                break;
            }
            node2 = node2 -> next;
        }
        node1 = node1 -> next;
    }
}

void RegionSet::AddRegion (const Rect& region) {
    regions.InsertHead(region);
    MergeRegions();
}

void RegionSet::SubtractRegion (const Rect& region) {
    bool restart = true;
    while (restart) {
        restart = false;

        ListNode<Rect>* end_of_list = regions.EndOfList();
        ListNode<Rect>* node = regions.GetHead();

        while (node != end_of_list) {
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
            else node = node -> next;
        }
    }
}

void RegionSet::operator+= (const RegionSet& regset2) {
    ListNode<Rect>* end_of_list = regset2.regions.EndOfList();
    ListNode<Rect>* node = regset2.regions.GetHead();

    while (node != end_of_list) {
        AddRegion (node -> val);
        node = node -> next;
    }
}

void RegionSet::operator-= (const RegionSet& regset2) {
    ListNode<Rect>* end_of_list = regset2.regions.EndOfList();
    ListNode<Rect>* node = regset2.regions.GetHead();
    while (node != end_of_list) {
        SubtractRegion (node -> val);
        node = node -> next;
    }
}

void RegionSet::operator^= (const RegionSet& regset2) {
    RegionSet tmp;
    tmp += *this;
    tmp -= regset2;
    *this -= tmp;
}

void RegionSet::Move (const Vec& vec) {
    ListNode<Rect>* end_of_list = regions.EndOfList();
    ListNode<Rect>* node = regions.GetHead();

    while (node != end_of_list) {
        node -> val.Move(vec);
        node = node -> next;
    }
}

bool RegionSet::Contains (const Vec& vec) const {
    ListNode<Rect>* end_of_list = regions.EndOfList();
    ListNode<Rect>* node = regions.GetHead();
    while (node != end_of_list) {
        if (node -> val.Contains(vec)) return true;
        node = node -> next;
    }
    return false;
}

void RegionSet::Print() const {
    ListNode<Rect>* node = regions.GetHead();
    while (node != regions.EndOfList()){
        std::cerr << "\t";
        node -> val.Print();
        std::cerr << "\n";
        node = node -> next;
    }
}

void IntersectRegsets (const RegionSet& regset1, const RegionSet& regset2, RegionSet& result) {
    RegionSet tmp;
    tmp.regions.InsertElems (regset1.regions);
    tmp -= regset2;
    result.regions.InsertElems (regset1.regions);
    result -= tmp;
}