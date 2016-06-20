#ifndef subjetSmearing_h
#define subjetSmearing_h

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

class subjetSmearing
{
 private:
  TString fInputFiles = "inputCent0.list";
  TF1 *f1Res = 0x0;
  TRandom3 *fRandom = 0x0;
  bool fDoScaleVariation = true;
  
  std::vector<TMatrixD*> v_mat;
  std::vector<TVectorT<double>*> v_val;
  std::vector<TVectorT<double>*> v_vec;
  std::vector<TVectorT<double>*> v_mean;
  
 public:
  
  subjetSmearing() {
    
    if(!fRandom) fRandom = new TRandom3();
  }

  void setResolutionFunction(TF1 *f1) {f1Res = f1;}

  void setInputFileList(TString str) { fInputFiles = str; }

  void setDoScaleVariation(bool b)   {fDoScaleVariation = b;}
  
  void smearJet(double ptgen, double ptgensj1, double ptgensj2, double &ptrec, double &ptrecsj1, double &ptrecsj2) {
    
    if(!f1Res) {
      Printf("no resolution function available");
      return;
    }

    if(!fRandom) fRandom = new TRandom3();
        
    //smear pt of big jet
    double smear = f1Res->Eval(ptgen);
    //double sigma = ptgen*smear;
    //ptrec = fRandom->Gaus(ptgen,sigma);

    double ptdiff = 0.;
    
    if(fDoScaleVariation) {
      ptrec = ptgen + smear*ptgen;
      ptdiff = ptrec - ptgen;
    }
    
    //Find eigenvector for this pt bin
    int bin = -1;
    for(unsigned int iv = 0; iv<v_vec.size(); ++iv) {
      if(v_vec[iv]) {
        TVectorT<double> ptVecTmp = *v_vec[iv];
        if(ptgen>ptVecTmp[0] && ptgen<=ptVecTmp[1])
          bin = (int)iv;
      } else
        Printf("cannot find v_vec[%d]",(int)iv);
    }
    if(bin<0) return;
    
    if(!v_mat[bin]) return;
    
    if(!v_mean[bin]) {
      Printf("mean vector not found");
      return;
    }
    
    //double ptdiffMinMean = ptdiff - (*v_mean[bin])[0];
    
    //choose which eigenvector to use
    if(!fRandom) Printf("fRandom does not exist");
    double rnd = fRandom->Uniform();
    int useEigenVec = 0;
    double sumprob = 0.;
    for(int iv = 0; iv<v_val[bin]->GetNoElements(); ++iv) {
      sumprob += (*v_val[bin])[iv];
      if(rnd<sumprob) {
        useEigenVec = iv;
        break;
      }
    }

    //do correlated subjet smearing
    double sjptrec[2] = {0.};
    double sjptgen[2] = {ptgensj1,ptgensj2};
    double corr[2] = {1.};
    for(int k = 0; k<2; ++k) {
      if(v_mat[bin]) {
        double a =  (*v_mat[bin])[0][useEigenVec] / (*v_mat[bin])[k+1][useEigenVec];
        //corr[k] = a*(ptdiff + (*v_mean[bin])[0]) + (*v_mean[bin])[k+1];
        corr[k] = a*ptdiff;
        sjptrec[k] = TMath::Max(0.,sjptgen[k] + corr[k]);
        //Printf("pt rec subjet %d = %f",k,sjptrec[k]);
      }
    }
    ptrecsj1 = sjptrec[0];
    ptrecsj2 = sjptrec[1];
  }

  void initFileList() {
    //Get all eigenvectors
    ifstream inputFile;
    inputFile.open(fInputFiles.Data());
    TString line;
    while( inputFile>>line) {
      // Printf("%s", line.Data());
      TFile *fcov = new TFile(line.Data());
      
      TVectorT<double> *eigenval = (TVectorT<double>*)(fcov->Get("eigenval"));
      
      //normalize eigenvalues
      int ndim = 1; //only using dominant eigenvector
      double sum = 0.;
      //Printf("size eigenval: %d",eigenval->GetNoElements());
      int in = eigenval->GetNoElements();
      if(ndim>0) in = ndim; 
            
      for(int i = 0; i<in; ++i)
        sum+=(*eigenval)[i];

      for(int i = 0; i<eigenval->GetNoElements(); ++i) {
        if(i<in) (*eigenval)[i]=(*eigenval)[i]/sum;
        else     (*eigenval)[i]=0.;
      }
      eigenval->Print();
      v_val.push_back(eigenval);
    
      TMatrixD *eigenvec = (TMatrixD*)(fcov->Get("eigenvec"));
      v_mat.push_back(eigenvec);
      
      TVectorT<double> *ptVec = (TVectorT<double>*)(fcov->Get("ptVec"));
      v_vec.push_back(ptVec);
      
      TVectorT<double> *meanVec = (TVectorT<double>*)(fcov->Get("meanVec"));
      v_mean.push_back(meanVec);
      
      delete fcov;
    }
  }

};
#endif
