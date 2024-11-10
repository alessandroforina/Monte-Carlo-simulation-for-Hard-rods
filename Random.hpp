#ifndef RANDOM_HPP
#define RANDOM_HPP
#include "./randnumgen.hpp"
#include "Vec3.hpp"

template < typename T >
T Random ( T start, T end)
{
    return ( end - start ) * rng.ranf() + start;
}

Vec3 Marsaglia(){

    
    double S{1.0}, V1{0.0}, V2{0.0};
    while(S >= 1.0){

        V1 = Random(-1.0,1.0);
        V2 = Random(-1.0,1.0);
        S = V1*V1 + V2*V2;
        }

    double factor = 2 * sqrt(1.0 - S);
        
    return {V1* factor, V2*factor, 1.0 - 2.0*S};


}

#endif //RANDOM_HPP
