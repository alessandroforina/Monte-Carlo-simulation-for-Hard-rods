 #include <iostream>

#include "Simulation.hpp"

 int main() {

   rng.rseed();
   Simulation sim;
   sim.Initialization("assets.txt");
   sim.Run();

   return 0;
 }