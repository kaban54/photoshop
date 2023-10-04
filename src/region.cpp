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

bool Intersect (const Rect& rect1, const Rect& rect2) {
    return (rect1.vert1.x < rect2.vert2.x &&
            rect2.vert1.x < rect1.vert2.x &&
            rect1.vert1.y < rect2.vert2.y &&
            rect2.vert1.y < rect1.vert2.y);
}


RegionSet::RegionSet():
    regions () {}

void RegionSet::MergeRegions() {
    //std::cout << ":)\n";
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
            (*this) += new_regions;
            node = node -> next;
            regions.Remove (node -> prev);
        }
        else node = node -> next;
    }
    
}

const RegionSet& RegionSet::operator+= (const RegionSet& regset2) {
    ListNode<Rect>* end_of_list = regset2.regions.EndOfList();
    ListNode<Rect>* node = regset2.regions.GetHead();

    while (node != end_of_list) {
        AddRegion (node -> val);
        node = node -> next;
    }
    return *this;
}

const RegionSet& RegionSet::operator-= (const RegionSet& regset2) {
    ListNode<Rect>* end_of_list = regset2.regions.EndOfList();
    ListNode<Rect>* node = regset2.regions.GetHead();

    while (node != end_of_list) {
        SubtractRegion (node -> val);
        node = node -> next;
    }
    return *this;
}