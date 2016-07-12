#include "UserCode/diall/analyzeZgHistosDataPbPbCalo.C"

#include "FWCore/FWLite/interface/FWLiteEnabler.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TSystem.h"

#include <iostream>

using namespace std;

//
// MAIN METHOD
//
int main(int argc, char* argv[])
{
  // load framework libraries
  gSystem->Load( "libFWCoreFWLite" );
  FWLiteEnabler::enable();
  
  //check arguments
  if ( argc < 2 ) 
    {
      std::cout << "Usage : " << argv[0] << " parameters_cfg.py" << std::endl;
      return 0;
    }

  // int anaFile = 1;
  int firstFile = 0;
  int lastFile  = 1;
  if(argc>2) {
    firstFile = atoi(argv[2]);
    lastFile = atoi(argv[3]);
  }

  Int_t firstEvent = 0;
  if(argc>3)
    firstEvent = atoi(argv[4]);

  int doJetShift = 0;
  if(argc>4)
    doJetShift = atoi(argv[5]);

  std::cout << "Have " << argc << " arguments:" << std::endl;
  for (int i = 0; i < argc; ++i) {
    std::cout << argv[i] << std::endl;
  }
  
  // read configuration
  const edm::ParameterSet &runProcess = edm::readPSetsFrom(argv[1])->getParameter<edm::ParameterSet>("config");
  std::vector<std::string> urls=runProcess.getParameter<std::vector<std::string> >("input");

  std::string outname = "AnaResultsZgCalo.root";
  int maxEvts = runProcess.getParameter<int>("maxEvents");
    
  analyzeZgHistos(urls,outname.c_str(),maxEvts,firstFile,lastFile,firstEvent,doJetShift);
  
  std::cout << "Results have been stored in " << outname << std::endl;
}
