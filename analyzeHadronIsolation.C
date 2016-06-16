#include "UserCode/diall/interface/genParticleProducer.h"
#include "UserCode/diall/interface/hiEventProducer.h"
#include "UserCode/diall/interface/lwMuonProducer.h"
#include "UserCode/diall/interface/pfParticleProducer.h"
#include "UserCode/diall/interface/LWJetProducer.h"
#include "UserCode/diall/interface/lwJetContainer.h"
#include "UserCode/diall/interface/lwJetFromForestProducer.h"
#include "UserCode/diall/interface/anaBaseTask.h"
#include "UserCode/diall/interface/anaHadronIsolation.h"
#include "UserCode/diall/interface/anaJetQA.h"
#include "UserCode/diall/interface/anaRhoProducer.h"
#include "UserCode/diall/interface/anaMET.h"
#include "UserCode/diall/interface/anaMuonIsolation.h"
#include "UserCode/diall/interface/anaMuonMatcher.h"
#include "UserCode/diall/interface/anaPuppiProducer.h"
#include "UserCode/diall/interface/anaZToMuMu.h"

#include <TList.h>
#include <TChain.h>
#include <TFile.h>

//#include "CollectFiles.C"

#include <iostream>

using namespace std;

void analyzeHadronIsolation(std::vector<std::string> urls, const char *outname = "eventObjects.root", Long64_t nentries = 20, Int_t firstF = -1, Int_t lastF = -1, Int_t firstEvent = 0, int isData = 1) {

  std::cout << "nfiles: " << urls.size() << std::endl;
  // for (auto i = urls.begin(); i != urls.end(); ++i)
  //   std::cout << *i << std::endl;

  size_t firstFile = 0;
  size_t lastFile = urls.size();

  if(firstF>-1) {
    firstFile = (size_t)firstF;
    lastFile = (size_t)lastF;
  }
  std::cout << "firstFile: " << firstFile << "  lastFile: " << lastFile << std::endl;

  Int_t lastEvent = nentries;
  if(firstEvent>0) {
    lastEvent = firstEvent + nentries;
  }
  std::cout << "firstEvent: " << firstEvent << std::endl;
  
  //add files to chain
  TChain *chain = NULL;
  // chain = new TChain("hiEvtAnalyzer/HiTree");
  // for(size_t i=firstFile; i<lastFile; i++) chain->Add(urls[i].c_str());
  // Printf("hiTree done");
  
  //TChain *pfTree = new TChain("pfcandAnalyzer/pfTree");
  //  for(size_t i=firstFile; i<lastFile; i++) pfTree->Add(urls[i].c_str());
  //chain->AddFriend(pfTree);
  chain = new TChain("pfcandAnalyzer/pfTree");
  for(size_t i=firstFile; i<lastFile; i++) chain->Add(urls[i].c_str());
  Printf("pfTree done");

  TChain *jetTree = new TChain("ak4PFJetAnalyzer/t");//akPu4CaloJetAnalyzer/t");
  for(size_t i=firstFile; i<lastFile; i++) jetTree->Add(urls[i].c_str());
  chain->AddFriend(jetTree);
  Printf("jetTree done");

  TChain *caloJetTree = new TChain("ak4CaloJetAnalyzer/t");//akPu4CaloJetAnalyzer/t");
  for(size_t i=firstFile; i<lastFile; i++) caloJetTree->Add(urls[i].c_str());
  Printf("caloJetTree done");

  TChain *hltTree = new TChain("hltanalysis/HltTree");
  for(size_t i=firstFile; i<lastFile; i++) hltTree->Add(urls[i].c_str());
  chain->AddFriend(hltTree);
  Printf("hltTree done");

  TChain *skimTree = new TChain("skimanalysis/HltTree");
  for(size_t i=firstFile; i<lastFile; i++) skimTree->Add(urls[i].c_str());
  chain->AddFriend(skimTree);
  Printf("skimTree done");
  
  TList *fEventObjects = new TList();

  //---------------------------------------------------------------
  // producers
  //
  hiEventProducer *p_evt = new hiEventProducer("hiEvtProd");
  p_evt->SetInput(chain);
  p_evt->SetHIEventContName("hiEventContainer");
  p_evt->SetEventObjects(fEventObjects);

  pfParticleProducer *p_pf = new pfParticleProducer("pfPartProd");
  p_pf->SetInput(chain);
  p_pf->SetpfParticlesName("pfParticles");
  p_pf->SetEventObjects(fEventObjects);

  lwJetFromForestProducer *p_PFJet = new lwJetFromForestProducer("lwJetForestProdPFJet");
  p_PFJet->SetInput(chain);
  p_PFJet->SetJetContName("akt4PF");
  p_PFJet->SetGenJetContName("akt4Gen");
  p_PFJet->SetEventObjects(fEventObjects);
  p_PFJet->SetRadius(0.4);

  lwJetFromForestProducer *p_CaloJet = new lwJetFromForestProducer("lwJetForestProdCaloJet");
  p_CaloJet->SetInput(caloJetTree);
  p_CaloJet->SetJetContName("akt4Calo");
  p_CaloJet->SetGenJetContName("");
  p_CaloJet->SetEventObjects(fEventObjects);
  p_CaloJet->SetRadius(0.4);

  
  //---------------------------------------------------------------
  //analysis modules
  //

  //handler to which all modules will be added
  anaBaseTask *handler = new anaBaseTask("handler","handler");
  
  //Hadron isolation
  anaHadronIsolation *hadronIsoRaw = new anaHadronIsolation("hadronIsoRaw","hadronIsoRaw");
  hadronIsoRaw->ConnectEventObject(fEventObjects);
  hadronIsoRaw->SetHiEvtName("hiEventContainer");
  hadronIsoRaw->SetPFPartName("pfParticles");
  hadronIsoRaw->SetIsolationType(anaHadronIsolation::kRaw);
  hadronIsoRaw->SetJetRecoName("akt4PF");
  if(isData) {
    hadronIsoRaw->DoPFJet80(true);
    hadronIsoRaw->DoCollisionEventSel(true);
    hadronIsoRaw->DoHBHENoiseFilter(true);
  }
  handler->Add(hadronIsoRaw);

  anaHadronIsolation *hadronIsoRawCaloJet = new anaHadronIsolation("hadronIsoRawCaloJet","hadronIsoRawCaloJet");
  hadronIsoRawCaloJet->ConnectEventObject(fEventObjects);
  hadronIsoRawCaloJet->SetHiEvtName("hiEventContainer");
  hadronIsoRawCaloJet->SetPFPartName("pfParticles");
  hadronIsoRawCaloJet->SetIsolationType(anaHadronIsolation::kRaw);
  hadronIsoRawCaloJet->SetJetRecoName("akt4Calo");
  if(isData) {
    hadronIsoRawCaloJet->DoPFJet80(true);
    hadronIsoRawCaloJet->DoCollisionEventSel(true);
    hadronIsoRawCaloJet->DoHBHENoiseFilter(true);
  }
  handler->Add(hadronIsoRawCaloJet);
	  
	  
  //---------------------------------------------------------------
  //output tree
  Long64_t entries_tot =  chain->GetEntries(); //93064
  if(nentries<0) lastEvent = chain->GetEntries();
  // Long64_t nentries = 20;//chain->GetEntriesFast();
  Printf("nentries: %lld  tot: %lld",nentries,entries_tot);
  for (Long64_t jentry=firstEvent; jentry<lastEvent; ++jentry) {
   if(jentry%10000==0) cout << "entry: "<< jentry << endl;
    //Run producers
    //    p_evt->Run(jentry);   //hi event properties
    p_pf->Run(jentry);    //pf particles
    p_PFJet->Run(jentry); //forest PF jets
    p_CaloJet->Run(jentry); //forest Calo jets
	    
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
