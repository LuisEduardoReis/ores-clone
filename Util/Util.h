//
// Created by luis.reis on 25-03-2018.
//

#ifndef ORESCLONE_UTIL_H
#define ORESCLONE_UTIL_H

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <random>
#include "Coord.h"

using namespace std;

inline void logSDLError(std::ostream &os, const std::string &msg) {
    os << msg << " Error: " << SDL_GetError() << std::endl;
}
inline void logSDLImageError(std::ostream &os, const std::string &msg) {
    os << msg << " Error: " << IMG_GetError() << std::endl;
}

template<typename T>
inline bool between(T x, T a, T b) { return a <= x && x <= b; }

template<typename T>
inline bool between2d(T x, T y,  T x1, T y1,  T x2, T y2) { return between(x, x1,x2) && between(y, y1,y2); }

inline int random(int a, int b = 0) {
    int lowerBound = min(a,b), highBound = max(a,b);

    static std::random_device r;
    static std::default_random_engine gen(r());

    return std::uniform_int_distribution<>(lowerBound, highBound-1)(gen);
}

inline float random(float a, float b = 0) {
    float lowerBound = min(a,b), highBound = max(a,b);

    static std::random_device r;
    static std::default_random_engine gen(r());

    return (float) std::uniform_real_distribution<>(lowerBound, highBound)(gen);
}

template <typename T> inline int sign(T val) {
    return (T(0) < val) - (val < T(0));
}

template<typename T> inline void stepTo(T& a, const T& b, const T& x) {
    if (abs(b - a) < x)
        a = b;
    else
        a += sign(b - a) * x;
}

template<typename T> inline T clamp(const T& a, const T& x, const T& b) {
    return max(a, min(x, b));
}

template<typename T> inline T lerp(const T& a, const T& b, float x) {
    return a + (b-a)*clamp<float>(0,x,1);
}

template<typename T> inline T distance(T x1, T y1, const T& x2, const T& y2) {
    x1 -= x2; y1 -= y2;
    return sqrt(x1*x1 + y1*y1);
}

inline float distance(const Coord& a, const Coord& b) {
    return distance<float>(a.x,a.y, b.x,b.y);
}


template<typename ... Args>
string stringFormat( const std::string& format, Args ... args )
{
    size_t size = (size_t) snprintf( nullptr, 0, format.c_str(), args ... ) + 1;
    unique_ptr<char[]> buf( new char[ size ] );
    snprintf( buf.get(), size, format.c_str(), args ... );
    return string( buf.get(), buf.get() + size - 1 );
}

#define PI ((float) acos(-1))
#define TWOPI (2*PI)
#define DEGTORAD (PI/180)
#define RADTODEG (180/PI)


#endif
