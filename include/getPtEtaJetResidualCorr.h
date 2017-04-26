#ifndef GETPTETAJETRESIDUALCORR_H
#define GETPTETAJETRESIDUALCORR_H

#include "TFile.h"
#include "TF1.h"
#include "TMath.h"

#include <string>

#include "UserCode/diall/include/jecConfigParser.h"
#include "UserCode/diall/include/checkMakeDir.h"

class residualJetCorrChris
{

 private:
  const Int_t nCentBins = 4;
  const std::string centStr[nCentBins] = {"Cent50to100", "Cent30to50", "Cent10to30", "Cent0to10"};
  const Int_t centBins[nCentBins+1] = {100, 50, 30, 10, 0};

  const Int_t nEtaBins = 3;
  const std::string etaStr[nEtaBins] = {"0p0to0p5", "0p5to1p0", "1p0to1p6"};

  TFile* resCorrFile_p;
  TF1* resCorr_p[nCentBins][nEtaBins];

 public:

  residualJetCorrChris() {}

  
  bool initPtEtaJetResidualCorr(const std::string corrFileName, std::string corrForm)
  {
    if(!checkFile(corrFileName)){
      std::cout << "Input corrFile, \'" << corrFileName << "\', is not valid file. Check. Return false" << std::endl;
      return false;
    }

    jecConfigParser internalConfig;
    if(!internalConfig.SetConfigParser(corrFileName)){
      std::cout << "In file \'" << __FILE__ << "\', init of corrFileName \'" << corrFileName << "\' failed. Check file. return false" << std::endl;
      return false;
    }

    std::string corrFileCorrForm = internalConfig.GetCorrForm();

    while(corrFileCorrForm.find(" ") != std::string::npos){
      corrFileCorrForm.replace(corrFileCorrForm.find(" "), 1, "");
    }

    while(corrForm.find(" ") != std::string::npos){
      corrForm.replace(corrForm.find(" "), 1, "");
    }

    if(corrForm.size() != corrFileCorrForm.size() || corrFileCorrForm.find(corrForm) == std::string::npos){
      std::cout << "CORRFORM in configFile, \'" << corrForm << "\' does not match CORRFORM in corrFile, \'" << corrFileCorrForm << "\'. Return false" << std::endl;
      return false;
    }

    //  resCorrFile_p = new TFile("corrFile_Prev20161212.root", "READ");
    resCorrFile_p = new TFile(corrFileName.c_str(), "READ");    

    for(Int_t centIter = 0; centIter < nCentBins; centIter++){
      for(Int_t etaIter = 0; etaIter < nEtaBins; etaIter++){
        const std::string inName = "corr_Eta" + etaStr[etaIter] + "_Inc_Mean_akPu3PF_" + centStr[centIter] + "_c";

        resCorr_p[centIter][etaIter] = (TF1*)resCorrFile_p->Get(inName.c_str());
      }
    }

    return true;
  }

  float getPtEtaJetResidualCorr(Float_t jtPt, Float_t jtEta, Int_t centBin, std::string corrForm)
  {

    Float_t jtCorr = 1.;
    if(centBin >= centBins[0]) return jtCorr;

    Float_t par0 = 0;
    Float_t par1 = 0;
    Float_t par2 = 0;

    Float_t resFactor = 1.0;

    for(Int_t centIter = 0; centIter < nCentBins; centIter++){
      if(centBin >= centBins[centIter+1]){
        if(TMath::Abs(jtEta) < .5){
          par0 = resCorr_p[centIter][0]->GetParameter(0);
          par1 = resCorr_p[centIter][0]->GetParameter(1);
          par2 = resCorr_p[centIter][0]->GetParameter(2);
        }
        else if(TMath::Abs(jtEta) < 1.0){
          par0 = resCorr_p[centIter][1]->GetParameter(0);
          par1 = resCorr_p[centIter][1]->GetParameter(1);
          par2 = resCorr_p[centIter][1]->GetParameter(2);
        }
        else{
          par0 = resCorr_p[centIter][2]->GetParameter(0);
          par1 = resCorr_p[centIter][2]->GetParameter(1);
          par2 = resCorr_p[centIter][2]->GetParameter(2);
	 
          //	 resFactor = 0.98;
        }
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
