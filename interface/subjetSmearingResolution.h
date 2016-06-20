#ifndef subjetSmearingResolution_h
#define subjetSmearingResolution_h

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
#include "TF2.h"
#include "TMath.h"

#include <fstream>

using namespace std;

class subjetSmearingResolution
{
 private:
  TString fInputFile = "PtZgScale2DGausFit.root";
  TFile *fFile;

  TF2 *fit[8];
  TF1 *fgpt[8];
  TF1 *fgzg[8];
  
 public:
  
  subjetSmearingResolution() {
    //
  }

  void setInputFileList(TString str) {
    fInputFile = str;
    fFile = TFile::Open(str.Data());

    for(int i = 0; i<8; ++i) {
      fit[i] = dynamic_cast<TF2*>(fFile->Get(Form("fit_%d",i)));
      double sigmapt = fit[i]->GetParameter(2);
      fgpt[i] = new TF1(Form("fgpt_%d",i),"gaus",0.,3.);
      fgpt[i]->SetParameters(1.,1.,sigmapt*0.1);
      
      double sigmazg = fit[i]->GetParameter(4);
      fgzg[i] = new TF1(Form("fgzg_%d",i),"gaus",0.,3.);
      fgzg[i]->SetParameters(1.,1.,sigmazg*0.1);
    }
  }
  
  bool smearJet(double pt, double zg, double &ptrec, double &zgrec) {

    const int nPtBins = 8;
    double ptmin[nPtBins] = {100.,110.,120.,130.,140.,150.,160.,170.};
    double ptmax[nPtBins] = {110.,120.,130.,140.,150.,160.,170.,180.};

    int ptbin = -1;
    for(int ipt = 0; ipt<nPtBins; ++ipt) {
      if(pt>ptmin[ipt] && pt<ptmax[ipt]) {
        ptbin = ipt;
        break;
      }
    }
    if(ptbin<0) return false;
    
    double dpt = fgpt[ptbin]->GetRandom();
    double dzg = fgzg[ptbin]->GetRandom();

    ptrec = pt*dpt;
    zgrec = zg*dzg;
    
    return true; 
  }

};
#endif
