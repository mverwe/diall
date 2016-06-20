#include "UserCode/diall/interface/genParticleProducer.h"
#include "UserCode/diall/interface/hiEventProducer.h"
#include "UserCode/diall/interface/lwMuonProducer.h"
#include "UserCode/diall/interface/pfParticleProducer.h"
#include "UserCode/diall/interface/LWJetProducer.h"
#include "UserCode/diall/interface/lwJetContainer.h"
#include "UserCode/diall/interface/lwJetFromForestProducer.h"
#include "UserCode/diall/interface/anaBaseTask.h"
#include "UserCode/diall/interface/anaJetEnergyScale.h"
#include "UserCode/diall/interface/anaJetMatching.h"
#include "UserCode/diall/interface/anaJetQA.h"
#include "UserCode/diall/interface/anaRhoProducer.h"
#include "UserCode/diall/interface/anaMET.h"
#include "UserCode/diall/interface/anaMuonIsolation.h"
#include "UserCode/diall/interface/anaMuonMatcher.h"
#include "UserCode/diall/interface/anaPuppiProducer.h"
#include "UserCode/diall/interface/anaPuppiParticles.h"
#include "UserCode/diall/interface/anaSubJet.h"
#include "UserCode/diall/interface/anaZToMuMu.h"
#include "UserCode/diall/interface/anaZgHistos.h"

#include <TList.h>
#include <TChain.h>
#include <TFile.h>

#include <iostream>

using namespace std;

void analyzeZgHistos(std::vector<std::string> urls, const char *outname = "eventObjects.root", Long64_t nentries = 20, Int_t firstF = -1, Int_t lastF = -1, Int_t firstEvent = 0, int activateJetShift = 0) {

  /*
    ptminType: minimum raw pt for particles used by puppi
    0 : 0 GeV
    1 : 1 GeV
    2 : 2 GeV

    jetSignalType: jets used to select jetty region in events
    0 : detector-level jets (akPu4PF)
    1 : particle-level jets (gen jets)
  */

  TString jetSDName = "akt4PFSD";
  TString jetTreeSDName = "akSoftDrop4PFJetAnalyzer";
  TString jetSDChName = "akt4PFSDCh";
  TString jetTreeSDChName = "akSoftDropCh4PFJetAnalyzer";
  TString jetName = "akt4PF";
  TString jetTreeName = "ak4PFJetAnalyzer";
  //jetName = "akCs4PFFilter";
  //jetTreeName = "akCs4PFFilterJetAnalyzer";

  bool doJetShift = false;
  if(activateJetShift>0) doJetShift = true;
  float jetShift = 0.02;
  if(activateJetShift==1) jetShift = 0.02;
  if(activateJetShift==2) jetShift = -0.02;


  double minptjet = 30.;
  bool doDijet = false;
  if(!doDijet) minptjet = 40.;

  std::cout << "analyzing subjets for: " << jetName << " tree: " << jetTreeName << std::endl;
  std::cout << "analyzing subjets for: " << jetSDName << " tree: " << jetTreeSDName << std::endl;
   
  std::cout << "nfiles: " << urls.size() << std::endl;
  for (auto i = urls.begin(); i != urls.end(); ++i)
    std::cout << *i << std::endl;

  size_t firstFile = 0;
  size_t lastFile = urls.size();

  if(firstF>-1) {
    firstFile = (size_t)firstF;
    lastFile = std::min((size_t)lastF,lastFile);
  }
  std::cout << "firstFile: " << firstFile << "  lastFile: " << lastFile << std::endl;

  Int_t lastEvent = nentries;
  if(firstEvent>0) {
    lastEvent = firstEvent + nentries;
  }
  std::cout << "firstEvent: " << firstEvent << std::endl;
  
  //add files to chain
  TChain *chain = NULL;
  chain = new TChain("hiEvtAnalyzer/HiTree");
  for(size_t i=firstFile; i<lastFile; i++) chain->Add(urls[i].c_str());
  Printf("hiTree done");
  
  TChain *jetTreeSD = new TChain(Form("%s/t",jetTreeSDName.Data()));
  for(size_t i=firstFile; i<lastFile; i++) jetTreeSD->Add(urls[i].c_str());
  chain->AddFriend(jetTreeSD);
  Printf("jetTreeSD done");

//  TChain *jetTreeSDCh = new TChain(Form("%s/t",jetTreeSDChName.Data()));
//  for(size_t i=firstFile; i<lastFile; i++) jetTreeSDCh->Add(urls[i].c_str());
//  Printf("jetTreeSDCh done");

  TChain *jetTree = new TChain(Form("%s/t",jetTreeName.Data()));
  for(size_t i=firstFile; i<lastFile; i++) jetTree->Add(urls[i].c_str());
  Printf("jetTree done");

  TList *fEventObjects = new TList();

  //---------------------------------------------------------------
  // producers
  //
  hiEventProducer *p_evt = new hiEventProducer("hiEvtProd");
  p_evt->SetInput(chain);
  p_evt->SetHIEventContName("hiEventContainer");
  p_evt->SetEventObjects(fEventObjects);

  lwJetFromForestProducer *p_SDJet = new lwJetFromForestProducer("lwJetForestProdSD");
  p_SDJet->SetInput(chain);
  p_SDJet->SetJetContName(jetSDName);
  p_SDJet->SetGenJetContName("akt4Gen");
  p_SDJet->SetEventObjects(fEventObjects);
  p_SDJet->SetMinJetPt(minptjet);
  p_SDJet->SetRadius(0.4);

/*
  lwJetFromForestProducer *p_SDChJet = new lwJetFromForestProducer("lwJetForestProdSDCh");
  p_SDChJet->SetInput(jetTreeSDCh);
  p_SDChJet->SetJetContName(jetSDChName);
  p_SDChJet->SetGenJetContName("akt4GenCh");
  p_SDChJet->SetEventObjects(fEventObjects);
  p_SDChJet->SetMinJetPt(minptjet);
  p_SDChJet->SetRadius(0.4);
*/

  lwJetFromForestProducer *p_Jet = new lwJetFromForestProducer("lwJetForestProd");
  p_Jet->SetInput(jetTree);
  p_Jet->SetJetContName(jetName);
  p_Jet->SetGenJetContName("");//akt4Gen");
  p_Jet->SetEventObjects(fEventObjects);
  p_Jet->SetMinJetPt(minptjet);
  p_Jet->SetRadius(0.4);
  
  //---------------------------------------------------------------
  //analysis modules
  //

  //handler to which all modules will be added
  anaBaseTask *handler = new anaBaseTask("handler","handler");

  anaJetQA *jetQA = new anaJetQA("anaJetQA","anaJetQA");
  jetQA->ConnectEventObject(fEventObjects);
  jetQA->SetJetsName(jetName);
  handler->Add(jetQA);

  anaJetMatching *match = new anaJetMatching("jetMatching","jetMatching");
  match->ConnectEventObject(fEventObjects);
  match->SetHiEvtName("hiEventContainer");
  match->SetNCentBins(1);
  match->SetJetsNameBase(jetName);
  match->SetJetsNameTag(jetSDName);
  match->SetMatchingType(0);
  handler->Add(match);
  
  anaZgHistos *anazghistos = new anaZgHistos("anaZgHistos","anaZgHistos");
  anazghistos->ConnectEventObject(fEventObjects);
  anazghistos->SetHiEvtName("hiEventContainer");
  anazghistos->SetJetsName(jetSDName);
  anazghistos->SetJetsRefName(jetName);
  anazghistos->SetNCentBins(1);
  anazghistos->SetJetEtaRange(-1.3,1.3);
  anazghistos->SetDeltaRRange(0.1,999.);
  anazghistos->DoJetShift(doJetShift,jetShift);
  handler->Add(anazghistos);

  anaZgHistos *anazghistosdrSmall = new anaZgHistos("anaZgHistosDrSmall","anaZgHistosDrSmall");
  anazghistosdrSmall->ConnectEventObject(fEventObjects);
  anazghistosdrSmall->SetHiEvtName("hiEventContainer");
  anazghistosdrSmall->SetJetsName(jetSDName);
  anazghistosdrSmall->SetJetsRefName(jetName);
  anazghistosdrSmall->SetNCentBins(1);
  anazghistosdrSmall->SetJetEtaRange(-1.3,1.3);
  anazghistosdrSmall->SetDeltaRRange(0.1,0.2);
  anazghistosdrSmall->DoJetShift(doJetShift,jetShift);
  handler->Add(anazghistosdrSmall);

  anaZgHistos *anazghistosdrLarge = new anaZgHistos("anaZgHistosDrLarge","anaZgHistosDrLarge");
  anazghistosdrLarge->ConnectEventObject(fEventObjects);
  anazghistosdrLarge->SetHiEvtName("hiEventContainer");
  anazghistosdrLarge->SetJetsName(jetSDName);
  anazghistosdrLarge->SetJetsRefName(jetName);
  anazghistosdrLarge->SetNCentBins(1);
  anazghistosdrLarge->SetJetEtaRange(-1.3,1.3);
  anazghistosdrLarge->SetDeltaRRange(0.2,999.);
  anazghistosdrLarge->DoJetShift(doJetShift,jetShift);
  handler->Add(anazghistosdrLarge);

  anaZgHistos *anazghistosGen = new anaZgHistos("anazghistosGen","anazghistosGen");
  anazghistosGen->ConnectEventObject(fEventObjects);
  anazghistosGen->SetHiEvtName("hiEventContainer");
  anazghistosGen->SetJetsName("akt4Gen"); //ungroomed jets with groomed subjet variables at gen level
  anazghistosGen->SetJetsRefName(""); //
  anazghistosGen->SetNCentBins(1);
  anazghistosGen->SetJetEtaRange(-1.3,1.3);
  anazghistosGen->SetDeltaRRange(0.1,999.);
  handler->Add(anazghistosGen);

  anaZgHistos *anazghistosGendrSmall = new anaZgHistos("anaZgHistosGenDrSmall","anaZgHistosGenDrSmall");
  anazghistosGendrSmall->ConnectEventObject(fEventObjects);
  anazghistosGendrSmall->SetHiEvtName("hiEventContainer");
  anazghistosGendrSmall->SetJetsName("akt4Gen");
  anazghistosGendrSmall->SetJetsRefName("");
  anazghistosGendrSmall->SetNCentBins(1);
  anazghistosGendrSmall->SetJetEtaRange(-1.3,1.3);
  anazghistosGendrSmall->SetDeltaRRange(0.1,0.2);
  handler->Add(anazghistosGendrSmall);

  anaZgHistos *anazghistosGendrLarge = new anaZgHistos("anaZgHistosGenDrLarge","anaZgHistosGenDrLarge");
  anazghistosGendrLarge->ConnectEventObject(fEventObjects);
  anazghistosGendrLarge->SetHiEvtName("hiEventContainer");
  anazghistosGendrLarge->SetJetsName("akt4Gen");
  anazghistosGendrLarge->SetJetsRefName("");
  anazghistosGendrLarge->SetNCentBins(1);
  anazghistosGendrLarge->SetJetEtaRange(-1.3,1.3);
  anazghistosGendrLarge->SetDeltaRRange(0.2,999.);
  handler->Add(anazghistosGendrLarge);

  //SD with charged hadrons
  anaJetMatching *matchCh = new anaJetMatching("jetMatchingCh","jetMatchingCh");
  matchCh->ConnectEventObject(fEventObjects);
  matchCh->SetHiEvtName("hiEventContainer");
  matchCh->SetNCentBins(1);
  matchCh->SetJetsNameBase(jetName);
  matchCh->SetJetsNameTag(jetSDChName);
  matchCh->SetMatchingType(0);
  //  handler->Add(matchCh);
  
  anaZgHistos *anazghistosCh = new anaZgHistos("anaZgHistosCh","anaZgHistosCh");
  anazghistosCh->ConnectEventObject(fEventObjects);
  anazghistosCh->SetHiEvtName("hiEventContainer");
  anazghistosCh->SetJetsName(jetSDChName);
  anazghistosCh->SetJetsRefName(jetName);
  anazghistosCh->SetNCentBins(1);
  anazghistosCh->SetJetEtaRange(-1.3,1.3);
  //  handler->Add(anazghistosCh);

  anaZgHistos *anazghistosGenCh = new anaZgHistos("anazghistosGenCh","anazghistosGenCh");
  anazghistosGenCh->ConnectEventObject(fEventObjects);
  anazghistosGenCh->SetHiEvtName("hiEventContainer");
  anazghistosGenCh->SetJetsName("akt4GenCh"); //ungroomed jets with groomed subjet variables at gen level
  anazghistosGenCh->SetJetsRefName(""); //
  anazghistosGenCh->SetNCentBins(1);
  anazghistosGenCh->SetJetEtaRange(-1.3,1.3);
  //  handler->Add(anazghistosGenCh);

  
  //---------------------------------------------------------------
  //Event loop
  //---------------------------------------------------------------
  Long64_t entries_tot =  chain->GetEntries(); //93064
  if(nentries<0) lastEvent = chain->GetEntries();
  Printf("nentries: %lld  tot: %lld",nentries,entries_tot);
  for (Long64_t jentry=firstEvent; jentry<lastEvent; ++jentry) {
    if(jentry%10000==0) cout << "entry: "<< jentry << endl;
    //Run producers
    //Printf("produce hiEvent");
    p_evt->Run(jentry);   //hi event properties
    //Printf("produce jets");
    p_SDJet->Run(jentry); //forest SoftDrop jets
    //   p_SDChJet->Run(jentry); //forest SoftDrop jets with charged hadrons
    p_Jet->Run(jentry); //forest jets
	    
    //Execute all analysis tasks
    handler->ExecuteTask();
  }
    
  fEventObjects->Print();

  TFile *out = new TFile(outname,"RECREATE");
  TList *tasks = handler->GetListOfTasks();
  TIter next(tasks);
  anaBaseTask *obj;
  while ((obj = dynamic_cast<anaBaseTask*>(next()) ))
    if(obj->GetOutput()) obj->GetOutput()->Write(obj->GetName(),TObject::kSingleKey);
  
  out->Write();
  out->Close();
  
}
