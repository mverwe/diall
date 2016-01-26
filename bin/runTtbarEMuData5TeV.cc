#include "UserCode/diall/analyzeTtbarEMuppData5TeV.C"

//#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
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
  //  AutoLibraryLoader::enable();
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
  Int_t firstEvent = 0;
  int isData = 1;
  
  std::cout << "Have " << argc << " arguments:" << std::endl;
  for (int i = 0; i < argc; ++i) {
    std::cout << argv[i] << std::endl;
    switch(argc)
      {
      case 4: 
	firstFile = atoi(argv[2]);
	lastFile = atoi(argv[3]);
	break;
      case 5:
	firstFile = atoi(argv[2]);
	lastFile = atoi(argv[3]);
	firstEvent = atoi(argv[4]);
	break;
      case 6:
	firstFile = atoi(argv[2]);
	lastFile = atoi(argv[3]);
	firstEvent = atoi(argv[4]);
	isData = atoi(argv[5]);
      }
  }
  // read configuration
  const edm::ParameterSet &runProcess = edm::readPSetsFrom(argv[1])->getParameter<edm::ParameterSet>("config");
  std::vector<std::string> urls=runProcess.getParameter<std::vector<std::string> >("input");

  for (std::vector<std::string>::const_iterator i = urls.begin(); i != urls.end(); ++i)
    std::cout << *i << std::endl;
  
  std::string outname = "AnaResults.root";
  // std::string outname = runProcess.getParameter<std::string>("output");
  int maxEvts = runProcess.getParameter<int>("maxEvents");
  
  std::cout<< maxEvts<< firstFile << lastFile << firstEvent<< isData<< std::endl;//firstEvent << isData << std::endl;

  analyzeTtbarEMuppData5TeV(urls,outname.c_str(),maxEvts,firstFile,lastFile,firstEvent,isData);
  
  cout << "Results have been stored in " << outname << endl;

}
