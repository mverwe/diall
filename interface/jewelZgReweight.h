#ifndef jewelZgReweight_h
#define jewelZgReweight_h

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TString.h"
#include "TRandom3.h"
#include "TVector.h"
#include "TMatrixD.h"
#include "TF1.h"
#include "TMath.h"

#include <fstream>

using namespace std;

class jewelZgReweight
{
 private:
  TString fInputFile = "/afs/cern.ch/user/m/mverweij/work/PbPb5TeV/mc/subjet/jewelZgReweight/FitResultsRatioPt.root";
  TFile *fFile;

  int fMethod = 0;
  TF1 *fFit[4][9];
  
 public:
  
  jewelZgReweight() {
    //
  }

  void setMethod(int m) {fMethod = m;}
  
  void setInputFileList(TString str) {
    fInputFile = str;
    fFile = TFile::Open(str.Data());

    for(int i = 0; i<4; ++i) { //cent bins
      for(int j = 0; j<9; ++j) { //ptbins
        fFit[i][j] = dynamic_cast<TF1*>(fFile->Get(Form("fitRatioPol1_Cent%d_Pt%d_M%d",i,j,fMethod)));
      }
    }
  }

  int getPtBin(double pt) {

    const int nPtBins = 9;
    double ptmin[nPtBins] = {80.,100.,120.,140.,160.,180.,200.,250.,300.};
    double ptmax[nPtBins] = {100.,120.,140.,160.,180.,200.,250.,300.,500.};

    int ptbin = -1;
    for(int ipt = 0; ipt<nPtBins; ++ipt) {
      if(pt>ptmin[ipt] && pt<ptmax[ipt]) {
        ptbin = ipt;
        break;
      }
    }
    if(ptbin<0) return -1;
    return ptbin;
  }

  int getCentBin(double cent) {

    const int nCent = 4;
    double centMin[nCent] = {0. ,10.,30.,50.};
    double centMax[nCent] = {10.,30.,50.,80.};
    
    int centbin = -1;
    for(int i = 0; i<nCent; ++i) {
      if(cent>centMin[i] && cent<centMax[i]) {
        centbin = i;
        break;
      }
    }
    if(centbin<0) return -1;
    return centbin;
  }

  double getWeight(double pt, double zg, double cent) {
    int ptbin = getPtBin(pt);
    if(ptbin<0 || ptbin>8) return 0.;
    int centbin = getCentBin(cent);
    if(centbin<0 || centbin>3) return 0.;
    return fFit[centbin][ptbin]->Eval(zg);
  }
};
#endif
