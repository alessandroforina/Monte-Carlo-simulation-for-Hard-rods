#ifndef VECTORMANIPULATION_HPP
#define VECTORMANIPULATION_HPP

#include <array>
#include "./Atom.hpp"
#include <cmath>
#include "Vec3.hpp"

void Normalization(Vec3  & orientation){

    double norm2 = 0;
    
    for(int i = 0; i < 3; i++){
        
        norm2 += orientation[i] * orientation[i];
    }

    for(int i = 0; i < 3; i++){

        orientation[i] /= sqrt(norm2);
    }
}

Vec3  SumArray(Vec3  array1, Vec3  array2){

    Vec3  arraySum;
    for(int i =0; i < 3; i++){

        arraySum[i] = array1[i] + array2[i];
    }
    return arraySum;
}

Vec3  DifferenceArray(Vec3  array1, Vec3  array2){

    Vec3  arrayDifference;
    for(int i =0; i < 3; i++){

        arrayDifference[i] = array1[i] - array2[i];
    }
    return arrayDifference;
}

Vec3  ArrayPerScalar(Vec3  array1, double scalar){

    Vec3  arrayMultiplied;
    for(int i =0; i < 3; i++){

        arrayMultiplied[i] = array1[i] * scalar;
    }
    return arrayMultiplied;
}

double Dot(const Vec3  & array1, const Vec3  & array2){

    double scalarProduct = 0.0;
    
    for(int i = 0; i < 3; i++){

        scalarProduct += array1[i] * array2[i];
    }

    return scalarProduct;

}






#endif //VECTORMANIPULATION_HPP