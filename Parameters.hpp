#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "Vec3.hpp"



struct Parameters{

    double temperature;
    double pressure;
    Vec3 boxSize;
    double volume;
    double oldVolume;
    double oldBoxSize;
    double averageVolume;
    double maxTranslation;
    double maxRotation;
    double maxVolumeChange;
    double temporaryT = 0.0, temporaryAcceptedT = 0.0;
    double temporaryR = 0.0, temporaryAcceptedR = 0.0;
    double temporaryV = 0.0, temporaryAcceptedV = 0.0;
    unsigned long long int nMoves;
    int nParticles;
    int counterPrints = 0;
    int counterUpdating = 0;
    int counterVolumeAverage = 0;
    int disableRotationalMove = 0;
    int turnOnNTV;
    int turnOnUpdateParameters;
    double length;
    double radius;
    double vHSC;
    double deltaLength;
    double deltaRadius;
    double rc = 0;
    int frequencyUpdateAverages;
    int startingOptionLattice = 1;
    int turnOnLinkedCell = 0;
    int allowTurnOnLinkedCells = 1;

    bool ReadParametersFromFile(const std::string & filename){
        
        std::ifstream input(filename);

        if(! input.is_open()){ 
            return false; 
        }

        std::string key;
        while (input >> key) {
            if (key == "temperature") {
                input >> temperature;
            }
            else if (key == "pressure"){
                input >> pressure;
            }
            else if (key == "boxSize"){
                input >> boxSize[0] >> boxSize[1] >> boxSize[2];
            }
            else if(key == "volume"){
                input >> volume;
            }
            else if(key == "averageVolume"){
                input >> averageVolume;
            }
            else if(key == "maxTranslation"){
                input >> maxTranslation;
            }
            else if(key == "maxRotation"){
                input >> maxRotation;
            }
            else if(key =="maxVolumeChange"){
                input >> maxVolumeChange;
            }
            else if(key == "nMoves"){
                input >> nMoves;
            }
            else if(key == "nParticles"){
                input >> nParticles;
            }
            else if(key == "turnOnNTV"){
                input >> turnOnNTV;
            }
            else if(key == "turnOnUpdateParameters" ){
                input >> turnOnUpdateParameters;
            } 
            else if(key == "length") {
                input >> length;
            }
            else if(key == "radius") {
                input >> radius;
            }
            else if(key == "deltaLength") {
                input >> deltaLength;
            }
            else if (key == "deltaRadius") {
                input >> deltaRadius;
            }
            else if (key == "frequencyUpdateAverages") {
                input >> frequencyUpdateAverages;
            }
            else if (key == "startingOptionLattice") {
                input >> startingOptionLattice;
            }
            else if (key == "disableRotationalMove") {
                input >> disableRotationalMove;
            }
            else if (key == "turnOnLinkedCell") {
                input >> turnOnLinkedCell;
            }
            else if (key == "allowTurnOnLinkedCells") {
                input >> allowTurnOnLinkedCells;
            }
            else { return false; }
        }  
        
        return true;
    }

    void UpdateParameters(int move){
        int cyclesForUpdating = 1000;
        
        if(move == counterUpdating * nParticles* cyclesForUpdating && turnOnUpdateParameters !=0){
            
            if(temporaryT !=0){
                double ratio  =  temporaryAcceptedT / temporaryT;
                if( ratio > 0.4){
                    maxTranslation *= 1.10;
                }
                else if( ratio < 0.3){
                    maxTranslation *= 0.90;
                }
            }
            
            if(temporaryR !=0 && turnOnNTV != 1){
                double ratio  =  temporaryAcceptedR / temporaryR;
                if( ratio > 0.4){
                    maxRotation *= 1.10;
                }
                else if( ratio < 0.3){
                    maxRotation *= 0.90;
                }
            }
            if(temporaryV !=0){
                double ratio  =  temporaryAcceptedV / temporaryV;
                if( ratio > 0.4){
                    maxVolumeChange *= 1.10;
                }
                else if( ratio < 0.3){
                    maxVolumeChange *= 0.90;
                }
            }

            counterUpdating++;
            temporaryAcceptedT = 0;
            temporaryT = 0;
            temporaryAcceptedR = 0;
            temporaryR = 0;
            temporaryAcceptedV = 0;
            temporaryV = 0;
        }    
    }
};



#endif //PARAMETERS_HPP