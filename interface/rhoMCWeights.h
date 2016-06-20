#ifndef rhoMCWeights_h
#define rhoMCWeights_h

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TString.h"

#include <fstream>

using namespace std;

class rhoMCWeights
{
 private:
  TString fNameWeightFileCent0 = "/afs/cern.ch/user/m/mverweij/work/PbPb5TeV/mc/subjet/rhoWeightFiles/rhoWeightsCent0.txt";
  TString fNameWeightFileCent1 = "/afs/cern.ch/user/m/mverweij/work/PbPb5TeV/mc/subjet/rhoWeightFiles/rhoWeightsCent1.txt";
  TString fNameWeightFileCent2 = "/afs/cern.ch/user/m/mverweij/work/PbPb5TeV/mc/subjet/rhoWeightFiles/rhoWeightsCent2.txt";
  TString fNameWeightFileCent3 = "/afs/cern.ch/user/m/mverweij/work/PbPb5TeV/mc/subjet/rhoWeightFiles/rhoWeightsCent3.txt";

 public:
  
  rhoMCWeights() {}

  void set_file_names(TString cent0, TString cent1, TString cent2, TString cent3) {
    fNameWeightFileCent0 = cent0;
    fNameWeightFileCent1 = cent1;
    fNameWeightFileCent2 = cent2;
    fNameWeightFileCent3 = cent3;
  }
  
  double getWeights(double cent, double rhoCur) {

    TString nameFile = "";
    int centBin = getCentBin(cent);
    if(centBin==0) nameFile = fNameWeightFileCent0;
    else if(centBin==1) nameFile = fNameWeightFileCent1;
    else if(centBin==2) nameFile = fNameWeightFileCent2;
    else if(centBin==3) nameFile = fNameWeightFileCent3;
    else {return 0.;}

    double weightOld = 0.;
    double rhoOld = -1.;
    std::string line;
    ifstream inputFile(nameFile.Data());
    if(inputFile.is_open()) {
      while(getline(inputFile,line)) {
        std::stringstream ss(line);
        float rho, weight;
        if(ss >> rho >> weight) {
          //Printf("rho: %f weight: %f",rho,weight);
          if(rhoCur>rhoOld && rhoCur<=rho) {
            //Printf("Looking for this weight rhoCur: %f rho: %f weight: %f",rhoCur,rho,weight);
            double slope = (weight-weightOld)/(rho-rhoOld);
            double interpolated_weight = weightOld + (rhoCur-rhoOld)*slope;
            //Printf("interpolated_weight: %f",interpolated_weight);
            inputFile.close();
            return interpolated_weight;
          }
          weightOld = weight;
          rhoOld = rho;
        }
      }
    }
    inputFile.close();
    return 0.;
  }

  int getCentBin(double cent) {
    int fCentBin = 0;
    double offset = 2.;
    double centMin[4] = {0.+offset ,10.+offset,30.+offset,50.+offset};
    double centMax[4] = {10.+offset,30.+offset,50.+offset,80.+offset};
    for(int i = 0; i<4; ++i) {
    	if(cent>=centMin[i] && cent<centMax[i]) fCentBin = i;
    }
    return fCentBin;
  }

};
#endif
