#ifndef OVERLAP_HPP
#define OVERLAP_HPP

#include "./Atom.hpp"
#include "Parameters.hpp"
#include "./Random.hpp"
#include "Vec3.hpp"
#include <cmath>
#include "./VectorManipulation.hpp"
#include "./randnumgen.hpp"


const double EPSILON  = 0.000001;

double sign(double a,double b) { 
    
    a= abs(a);
    if(b<0){a = -a;} else {a=a;}
    return a;
}

double CS(const Atom &atoms1,const Atom &atoms2, Vec3 boxSize){

    double xmu,xla;
    double lh1 = atoms1.length/2;
    double lh2 = atoms2.length/2;
    Vec3 rij;
    rij[0] = atoms1.x - atoms2.x - boxSize[0]*rint((atoms1.x - atoms2.x)/boxSize[0]);
    rij[1] = atoms1.y - atoms2.y - boxSize[1]*rint((atoms1.y - atoms2.y)/boxSize[1]);
    rij[2] = atoms1.z - atoms2.z - boxSize[2]*rint((atoms1.z - atoms2.z)/boxSize[2]);
    double rr = Dot(rij,rij);
    double rw1  = Dot(rij, atoms1.orientation);
    double rw2  = Dot(rij, atoms2.orientation);
    double w1w2 = Dot(atoms1.orientation, atoms2.orientation);
    double cc = 1 - w1w2*w1w2;

    if(cc<1e-6) {
      if(rw1 && rw2) {
        xla= rw1/2;
        xmu= -rw2/2;
      }
      else return rr;
    }

    else {

      // Step 1

      xla= (rw1-w1w2*rw2)/cc;
      xmu= (-rw2+w1w2*rw1)/cc;
    }

    // Step 2

    if( abs(xla)>lh1 || abs(xmu)>lh2 ) {

      // Step 3 - 7

      if(abs(xla)-lh1>abs(xmu)-lh2) {
        xla= sign(lh1,xla);
        xmu= xla*w1w2-rw2;
        if( abs(xmu)>lh2 ) xmu= sign(lh2,xmu);
      }
      else {
        xmu= sign(lh2,xmu);
        xla= xmu*w1w2+rw1;
        if( abs(xla)>lh1 ) xla= sign(lh1,xla);
      }
    }

    // Step 8

    return rr+(xla)*(xla)+ (xmu)*(xmu) + 2*(xmu*rw2 -xla*(rw1+xmu*w1w2));

}

bool CheckOverlap(const Atom & atoms1, const Atom & atoms2, const double distance2){

    double radius  = atoms1.radius + atoms2.radius;
    return distance2 <= (radius*radius);
}




#endif //OVERLAP_HPP