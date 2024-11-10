#ifndef ATOM_HPP
#define ATOM_HPP

#include "Vec3.hpp"

struct Atom{

    double x, y, z;
    double length;
    double radius;
    Vec3   orientation;

    double &operator[](size_t i){
        switch(i){
            case 0:
                return x;
                break;
            case 1:
                return y;
                break;
            case 2:
                return z;
                break;
            
            default:
                break;        
        }
    }
};


#endif //ATOM_HPP