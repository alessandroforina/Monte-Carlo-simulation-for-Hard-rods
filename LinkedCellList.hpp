#ifndef LINKEDCELLLIST_HPP
#define LINKEDCELLLIST_HPP

const int EMPTY = -1;
#include <array>
#include <cmath>
#include <cstdlib>
#include <vector>

#include "Atom.hpp"
#include "Parameters.hpp"
#include "Overlap.hpp"
#include "Vec3.hpp"

class LinkedCellList
{

public:

    LinkedCellList() = default;
    ~LinkedCellList() = default;

    int ComputeCellIndex(const Atom &atom, Vec3 boxSize)
    {
        double rcx = boxSize[0]/Lc[0];
        double rcy = boxSize[1]/Lc[1];
        double rcz = boxSize[2]/Lc[2];

        int cx = static_cast<int>((atom.x + boxSize[0]/2)/rcx);
        int cy = static_cast<int>((atom.y + boxSize[1]/2)/rcy);
        int cz = static_cast<int>((atom.z + boxSize[2]/2)/rcz);

        int c = cx *(Lc[1] * Lc[2]) + cy*(Lc[2]) + cz; 

        return c;
    }

    void BuildCellList(const std::vector<Atom> &atoms, Vec3 boxSize, double rc)
    {
        for(int i = 0; i < 3; i++){
            Lc[i]  = floor(boxSize[i]/rc);
        }
        //Resize of cells vectors    
        int Lcxyz = Lc[0] * Lc[1] *Lc[2];
        head.resize(Lcxyz);
        lscl.resize(atoms.size());
        lsclPrevious.resize(atoms.size());
        ICP.resize(atoms.size());

        //Initialize the vector of headers

        for(int i =0; i < Lcxyz; i++)
        {
            head[i] = EMPTY;
        }
        for(int i = 0; i < lscl.size(); i++)
        {
            lscl[i] = EMPTY;
            lsclPrevious[i] = EMPTY;
        }

        //Scan of atoms to construct the Linked-Cell-List and the Header

        for(int particle = 0; particle < lscl.size(); particle++)
        {
            int c  = ComputeCellIndex(atoms[particle], boxSize);
            ICP[particle] = c;
            lscl[particle] = head[c];
            if(head[c] != EMPTY){
                lsclPrevious[head[c]] = particle;
            } 
            head[c] = particle;
        }


    }

    bool CheckOverlapLocalLCL(int particle, std::vector <Atom> & newAtoms, Vec3 boxSize){
        
        int newCellIndex = ComputeCellIndex(newAtoms[particle], boxSize);
        bool result = false;

        int cx = newCellIndex / (Lc[1] * Lc[2]);
        int cy = (newCellIndex / Lc[2]) % Lc[1];
        int cz = (newCellIndex) % Lc[2];

               

        for (int auCX = cx - 1; auCX <= cx + 1; auCX++){
            for (int auCY = cy - 1; auCY <= cy + 1; auCY++){
                for (int auCZ = cz - 1; auCZ <= cz + 1; auCZ++){
        
                int auxiliarC = ( (auCX + Lc[0]) % Lc[0]) * Lc[1] * Lc[2] 
                + ( (auCY + Lc[1]) % Lc[1]) *Lc[2]
                +( (auCZ + Lc[2]) % Lc[2] );

                int j = head[auxiliarC];
                while( j != -1){
                    if(j != particle){
                        double distance2 = CS(newAtoms[particle], newAtoms[j], boxSize);
                        result  = CheckOverlap(newAtoms[particle], newAtoms[j], distance2);
                        if(result == true ) { return result;}
                    }
                    j = lscl[j];
                }
            }
            }   
        }
        return result;
    }

    bool OverlapCheckTotalLCL(const std::vector <Atom> &newAtoms, const Vec3 boxSize){
        bool result = false;
        for(int cx = 0; cx < Lc[0]; cx++){
            for(int cy = 0; cy <Lc[1]; cy++){
                for(int cz = 0; cz < Lc[2]; cz++){

                    int c  = cx *(Lc[1]*Lc[2]) + cy*Lc[2] + cz;

                    for(int auCX = cx -1; auCX <= cx +1; auCX++){
                        for(int auCY = cy -1; auCY <= cy +1; auCY++){
                            for(int auCZ = cz -1; auCZ <= cz +1;auCZ++){

                                int auxiliarC = ((auCX +Lc[0]) %Lc[0])*Lc[1]*Lc[2]
                                + ((auCY + Lc[1]) % Lc[1])*Lc[2]
                                + ((auCZ + Lc[2]) % Lc[2]);
                                    
                                int i = head[c];

                                while(i != -1){
                                    int j = head[auxiliarC];
                                    while(j != -1){

                                        if( i < j){

                                            double distance2 = CS(newAtoms[i], newAtoms[j], boxSize);
                                            result  = CheckOverlap(newAtoms[i], newAtoms[j], distance2);
                                            if(result == true) { return result; }
                                        }
                                        j = lscl[j];
                                    }
                                    i = lscl[i];
                                }
                            }
                        }
                    }
                }
            }
        }
        return result;
    }

    void UpdateListTranslational(int particle, int oldCellIndex, int newCellIndex){
        RemoveFromOldCell(particle, oldCellIndex);
        InsertInNewCell(particle, newCellIndex);
    }
    

    void InsertInNewCell(int particle, int newCellIndex)
    {
      
      lscl[particle] = head[newCellIndex];
      head[newCellIndex] = particle;

    }



    void RemoveFromOldCell(int particle, int oldCellIndex){
        
        int n;
        if(head[oldCellIndex] == particle){
            
            head[oldCellIndex] = lscl[particle];

        } else {
            
            n = head[oldCellIndex];
            while (lscl[n] != particle){ 
                n = lscl[n];
            }    
            lscl[n] = lscl[particle];
        }    
    }

    int ReturnHeadSize(){
        return head.size();
    }
    
    int ReturnLcComponent(const int i, Vec3 boxSize, double rc){
        if( i > 3 || i < 0){
            std::cout << "\nÈ stata estratta una componente diversa da x,y e z!\n";
            return -10000;
        } else {
            return floor(boxSize[i]/rc);;
        }
    }

    int ReturnHeadHypothetical(Vec3 boxSize, double rc){
        std::array <int , 3> Lcfake{0};
        
        for(int i = 0; i < 3; i++){
            Lcfake[i]  = floor(boxSize[i]/rc);
        }
        //Resize of cells vectors    
        int Lcxyz = Lcfake[0] * Lcfake[1] * Lcfake[2];
        return Lcxyz;
    }

private:
    
    std::array <int, 3> Lc;
    std::vector <int> lscl;
    std::vector <int> lsclPrevious;
    std::vector <int> head;
    std::vector <int> ICP;
};




#endif //LINKEDCELLLIST_HPP