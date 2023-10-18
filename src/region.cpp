#include "region.h"

Rect::Rect():
    vert1 (),
    vert2 ()
    {}

Rect::Rect (const Vec& v1, const Vec& v2) {
    vert1 = v1;
    vert2 = v2;
    if (v2.x < v1.x) {
        vert1.x = v2.x;
        vert2.x = v1.x;
    }
    if (v2.y < v1.y) {
        vert1.y = v2.y;
        vert2.y = v1.y;
    }
}

double Rect::GetWidth() const {
    return vert2.x - vert1.x;
}

double Rect::GetHeight() const {
    return vert2.y - vert1.y;
}

Vec Rect::GetCenter() const {
    return (vert1 + vert2) / 2;
}

bool Rect::Contains (const Rect& rect) const {
    return (rect.vert1.x >= vert1.x &&
            rect.vert2.x <= vert2.x &&
            rect.vert1.y >= vert1.y &&
            rect.vert2.y <= vert2.y);
}

bool Rect::Contains (const Vec& vec) const {
    return (vec.x >= vert1.x &&
            vec.x <= vert2.x &&
            vec.y >= vert1.y &&
            vec.y <= vert2.y);
}

void Rect::Print () const {
    std::cout << " Rect((" << vert1.x << ", " << vert1.y << "), "
                       "(" << vert2.x << ", " << vert2.y << ")) ";
}


void Rect::Move (const Vec& vec) {
    vert1 += vec;
    vert2 += vec;
}

bool Intersect (const Rect& rect1, const Rect& rect2) {
    return (rect1.vert1.x < rect2.vert2.x &&
            rect2.vert1.x < rect1.vert2.x &&
            rect1.vert1.y < rect2.vert2.y &&
            rect2.vert1.y < rect1.vert2.y);
}

bool HaveCommonSide (const Rect& rect1, const Rect& rect2) {
    return ((rect1.vert1.x == rect2.vert1.x && rect1.vert2.x == rect2.vert2.x &&
            (rect1.vert1.y == rect2.vert2.y || rect1.vert2.y == rect2.vert1.y)) ||
            (rect1.vert1.y == rect2.vert1.y && rect1.vert2.y == rect2.vert2.y &&
            (rect1.vert1.x == rect2.vert2.x || rect1.vert2.x == rect2.vert1.x)));
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
                Vec v1 (std::min(r1.vert1.x, r2.vert1.x), std::min(r1.vert1.y, r2.vert1.y));
                Vec v2 (std::max(r1.vert2.x, r2.vert2.x), std::max(r1.vert2.y, r2.vert2.y));
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
    ListNode<Rect>* end_of_list = regions.EndOfList();
    ListNode<Rect>* node = regions.GetHead();

    while (node != end_of_list) {
        if (Intersect (region, node -> val)) {
            Rect r1 = node -> val;
            Rect r2 = region;

            RegionSet new_regions;

            double X[4] = {r1.vert1.x, r2.vert1.x, r1.vert2.x, r2.vert2.x};
            double Y[4] = {r1.vert1.y, r2.vert1.y, r1.vert2.y, r2.vert2.y};

            if (r2.vert1.x < r1.vert1.x) {
                X[0] = r2.vert1.x;
                X[1] = r1.vert1.x;
            }
            if (r2.vert2.x < r1.vert2.x) {
                X[2] = r2.vert2.x;
                X[3] = r1.vert2.x;
            }
            if (r2.vert1.y < r1.vert1.y) {
                Y[0] = r2.vert1.y;
                Y[1] = r1.vert1.y;
            }
            if (r2.vert2.y < r1.vert2.y) {
                Y[2] = r2.vert2.y;
                Y[3] = r1.vert2.y;
            }

            for (int x_index = 0; x_index < 3; x_index++) {
                if (X[x_index] < r1.vert1.x || X[x_index] >= r1.vert2.x) continue;
                if (X[x_index] == X[x_index + 1]) continue;
                for (int y_index = 0; y_index < 3; y_index++) {
                    if (Y[y_index] < r1.vert1.y || Y[y_index] >= r1.vert2.y) continue;
                    if (Y[y_index] == Y[y_index + 1]) continue;
                    Rect reg (Vec(X[x_index], Y[y_index]), Vec(X[x_index + 1], Y[y_index + 1]));
                    if (!Intersect(reg, r2)) new_regions.AddRegion(reg);
                }
            }
            node = node -> next;
            regions.Remove (node -> prev);
            (*this) += new_regions;
            
        }
        else node = node -> next;
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
    ListNode<Rect>* node = regions.GetHead();
    while (node != regions.EndOfList()){
        if (node -> val.Contains(vec)) return true;
        node = node -> next;
    }
    return false;
}

void RegionSet::Print() const {
    std::cout << "regset:\n";
    ListNode<Rect>* node = regions.GetHead();
    while (node != regions.EndOfList()){
        std::cout << "\t";
        node -> val.Print();
        std::cout << "\n";
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