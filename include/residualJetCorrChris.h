#ifndef residualJetCorrChris_H
#define residualJetCorrChris_H

#include "TFile.h"
#include "TF1.h"
#include "TMath.h"

#include <string>

//#include "UserCode/diall/include/jecConfigParser.h"
//#include "UserCode/diall/include/checkMakeDir.h"

class residualJetCorrChris
{

 private:
  //Int_t nCentBins = 4;
  std::string centStr[4] = {"Cent50to100", "Cent30to50", "Cent10to30", "Cent0to10"};
  Int_t centBins[4+1] = {100, 50, 30, 10, 0};

  //Int_t nEtaBins = 3;
  std::string etaStr[3] = {"0p0to0p5", "0p5to1p0", "1p0to1p6"};

  TFile* resCorrFile_p;
  TF1* resCorr_p[4][3];

 public:

  residualJetCorrChris() {}

  
  bool initPtEtaJetResidualCorr(const std::string corrFileName, std::string corrForm)
  {
    /* if(!checkFile(corrFileName)){ */
    /*   std::cout << "Input corrFile, \'" << corrFileName << "\', is not valid file. Check. Return false" << std::endl; */
    /*   return false; */
    /* } */

    /* jecConfigParser internalConfig; */
    /* if(!internalConfig.SetConfigParser(corrFileName)){ */
    /*   std::cout << "In file \'" << __FILE__ << "\', init of corrFileName \'" << corrFileName << "\' failed. Check file. return false" << std::endl; */
    /*   return false; */
    /* } */

    /* std::string corrFileCorrForm = internalConfig.GetCorrForm(); */

    /* while(corrFileCorrForm.find(" ") != std::string::npos){ */
    /*   corrFileCorrForm.replace(corrFileCorrForm.find(" "), 1, ""); */
    /* } */

    /* while(corrForm.find(" ") != std::string::npos){ */
    /*   corrForm.replace(corrForm.find(" "), 1, ""); */
    /* } */

    /* if(corrForm.size() != corrFileCorrForm.size() || corrFileCorrForm.find(corrForm) == std::string::npos){ */
    /*   std::cout << "CORRFORM in configFile, \'" << corrForm << "\' does not match CORRFORM in corrFile, \'" << corrFileCorrForm << "\'. Return false" << std::endl; */
    /*   return false; */
    /* } */

    //  resCorrFile_p = new TFile("corrFile_Prev20161212.root", "READ");
    resCorrFile_p = TFile::Open(corrFileName.c_str(), "READ");
    resCorrFile_p->ls();

    for(Int_t centIter = 0; centIter < 4; centIter++){
      for(Int_t etaIter = 0; etaIter < 3; etaIter++){
        const std::string inName = "corr_Eta" + etaStr[etaIter] + "_Inc_FitMean_akPu3PF_" + centStr[centIter] + "_c";
        resCorr_p[centIter][etaIter] = (TF1*)resCorrFile_p->Get(inName.c_str());
        Printf("cent: %d eta: %d",centIter,etaIter);
        if(resCorr_p[centIter][etaIter]) Printf("found corr function");
        else Printf("corr function not found");
      }
    }

    return true;
  }

  float getPtEtaJetResidualCorr(Float_t jtPt, Float_t jtEta, Int_t centBin, std::string corrForm)
  {

    //Printf("Get corr factor for jet with pt: %f, eta: %f and centBin: %d",jtPt,jtEta,centBin);
    
    Float_t jtCorr = 1.;
    if(centBin >= centBins[0]) return jtCorr;

    Float_t par0 = 0;
    Float_t par1 = 0;
    Float_t par2 = 0;

    Float_t resFactor = 1.0;

    for(Int_t centIter = 0; centIter < 4; centIter++){
      if(centBin >= centBins[centIter+1]){
        if(TMath::Abs(jtEta) < .5){
          if(!resCorr_p[centIter][0]) Printf("resCorr function not found. centIter: %d etaBin: %d",centIter,0);
          par0 = resCorr_p[centIter][0]->GetParameter(0);
          par1 = resCorr_p[centIter][0]->GetParameter(1);
          par2 = resCorr_p[centIter][0]->GetParameter(2);
        }
        else if(TMath::Abs(jtEta) < 1.0){
          if(!resCorr_p[centIter][1]) Printf("resCorr function not found. centIter: %d etaBin: %d",centIter,1);
          par0 = resCorr_p[centIter][1]->GetParameter(0);
          par1 = resCorr_p[centIter][1]->GetParameter(1);
          par2 = resCorr_p[centIter][1]->GetParameter(2);
        }
        else{
          if(!resCorr_p[centIter][2]) Printf("resCorr function not found. centIter: %d etaBin: %d",centIter,2);
          par0 = resCorr_p[centIter][2]->GetParameter(0);
          par1 = resCorr_p[centIter][2]->GetParameter(1);
          par2 = resCorr_p[centIter][2]->GetParameter(2);
	 
          //	 resFactor = 0.98;
        }
        //Printf("par0: %f  par1: %f  par2: %f",par0,par1,par2);
        break;
      }
    }

    while(corrForm.find(" ") != std::string::npos){
      corrForm.replace(corrForm.find(" "), 1, "");
    }

    if(corrForm.find("[0]+[1]/TMath::Sqrt(x)+[2]/x") != std::string::npos){
      jtCorr = 1./(par0 + par1/TMath::Sqrt(jtPt) + par2/jtPt);
      //    jtPt *= jtCorr;
      //    jtCorr = 1./(par0 + par1/TMath::Sqrt(jtPt) + par2/jtPt);
    }
    else if(corrForm.find("[0]+[1]/x+[2]/(x*x)") != std::string::npos){
      jtCorr = 1./(par0 + par1/jtPt + par2/(jtPt*jtPt));
      //    jtPt *= jtCorr;
      //    jtCorr = 1./(par0 + par1/jtPt + par2/(jtPt*jtPt));
    }
    else{
      std::cout << "No valid correction form given, return 0" << std::endl;
      jtCorr = 0;
    }

    return resFactor*jtCorr;
  }

};

#endif
