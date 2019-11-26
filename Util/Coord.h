//
// Created by luis.reis on 31-03-2018.
//

#ifndef ORESCLONE_POINT_H
#define ORESCLONE_POINT_H

#include <cstdio>
#include <functional>
#include <climits>

class Coord {

public:
    int x,y;

    Coord() : x(-1), y(-1) {};
    Coord(int x, int y) : x(x), y(y) {}
    Coord(const Coord& o) = default;;

    void set(int x, int y);
    void add(int ax, int ay);
    bool operator==(const Coord &o) const;
    bool operator!=(const Coord &o) const;
    bool operator<(const Coord &o) const;
    bool operator>(const Coord &rhs) const;
    bool operator<=(const Coord &rhs) const;
    bool operator>=(const Coord &rhs) const;

    static const Coord EMPTY;
};

namespace std {
    template<> struct hash<Coord>
    {
        std::size_t operator()(Coord const& c) const noexcept
        {
            std::size_t const h1 ( std::hash<int>{}(c.x) );
            std::size_t const h2 ( std::hash<int>{}(c.y) );
            return h1 ^ (h2 << 1);
        }
    };
}

#endif //ORESCLONE_POINT_H
