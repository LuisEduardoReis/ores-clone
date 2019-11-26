//
// Created by luis.reis on 01-04-2018.
//

#include "Coord.h"

const Coord Coord::EMPTY = Coord(INT_MIN, INT_MIN);

void Coord::set(int _x, int _y) {
    x = _x;
    y = _y;
}

void Coord::add(int ax, int ay) {
    x += ax;
    y += ay;
}

bool Coord::operator<(const Coord &o) const {
    return (y < o.y || (y == o.y && x < o.x));
}
bool Coord::operator>(const Coord &rhs) const {
    return rhs < *this;
}
bool Coord::operator<=(const Coord &rhs) const {
    return !(rhs < *this);
}
bool Coord::operator>=(const Coord &rhs) const {
    return !(*this < rhs);
}
bool Coord::operator==(const Coord &o) const {
    return x == o.x && y == o.y;
}
bool Coord::operator!=(const Coord &o) const {
    return !(o == *this);
}


