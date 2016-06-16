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

#include <TList.h>
#include <TChain.h>
#include <TFile.h>

#include <iostream>

using namespace std;

void analyzeSubJets(std::vector<std::string> urls, const char *outname = "eventObjects.root", Long64_t nentries = 20, Int_t firstF = -1, Int_t lastF = -1, Int_t firstEvent = 0) {

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
  TString jetSD000Name = "akt4PFSDZCut000";
  TString jetTreeSD000Name = "akSoftDropZCut0004PFJetAnalyzer";
  TString jetName = "akt4PF";
  TString jetTreeName = "ak4PFJetAnalyzer";
  //jetName = "akCs4PFFilter";
  //jetTreeName = "akCs4PFFilterJetAnalyzer";

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

  TChain *jetTreeSD000 = new TChain(Form("%s/t",jetTreeSD000Name.Data()));
  for(size_t i=firstFile; i<lastFile; i++) jetTreeSD000->Add(urls[i].c_str());
  Printf("jetTreeSD000 done");


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

  lwJetFromForestProducer *p_SD000Jet = new lwJetFromForestProducer("lwJetForestProdSD000");
  p_SD000Jet->SetInput(jetTreeSD000);
  p_SD000Jet->SetJetContName(jetSD000Name);
  p_SD000Jet->SetGenJetContName("akt4Gen000");
  p_SD000Jet->SetEventObjects(fEventObjects);
  p_SD000Jet->SetMinJetPt(minptjet);
  p_SD000Jet->SetRadius(0.4);

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

  anaJetMatching *match000 = new anaJetMatching("jetMatching000","jetMatching000");
  match000->ConnectEventObject(fEventObjects);
  match000->SetHiEvtName("hiEventContainer");
  match000->SetNCentBins(1);
  match000->SetJetsNameBase(jetName);
  match000->SetJetsNameTag(jetSD000Name);
  match000->SetMatchingType(0);
  match000->SetMatchId(2);
  handler->Add(match000);
  
  anaSubJet *anasubjets = new anaSubJet("anaSubJets","anaSubJets");
  anasubjets->ConnectEventObject(fEventObjects);
  anasubjets->SetHiEvtName("hiEventContainer");
  anasubjets->SetJetsName(jetSDName);
  anasubjets->SetJetsRefName(jetName);
  anasubjets->SetJets000Name(jetSD000Name);
  anasubjets->SetNCentBins(1);
  //anasubjets->SetJetEtaRange(-2.,2.);
  anasubjets->SetJetEtaRange(-1.3,1.3);
  anasubjets->SetDoDijets(doDijet);
  anasubjets->AddLeadingJetPtBin(120.,150.);
  anasubjets->AddLeadingJetPtBin(150.,180.);
  anasubjets->AddLeadingJetPtBin(180.,220.);
  anasubjets->AddLeadingJetPtBin(220.,260.);
  anasubjets->AddLeadingJetPtBin(260.,300.);
  anasubjets->AddLeadingJetPtBin(300.,500.);
  anasubjets->SetPtMinSubleading(30.);
  anasubjets->SetStoreTree(true);
  anasubjets->SetStoreTreeRef(true);//false);
  anasubjets->SetMinPtJetTree(80.);
  handler->Add(anasubjets);

  anaJetMatching *matchRev = new anaJetMatching("jetMatchingReversed","jetMatchingReversed");
  matchRev->ConnectEventObject(fEventObjects);
  matchRev->SetHiEvtName("hiEventContainer");
  matchRev->SetNCentBins(1);
  matchRev->SetJetsNameBase(jetName);
  matchRev->SetJetsNameTag(jetSD000Name);
  matchRev->SetMatchingType(0);
  handler->Add(matchRev);

  anaJetMatching *match000Rev = new anaJetMatching("jetMatching000Reversed","jetMatching000Reversed");
  match000Rev->ConnectEventObject(fEventObjects);
  match000Rev->SetHiEvtName("hiEventContainer");
  match000Rev->SetNCentBins(1);
  match000Rev->SetJetsNameBase(jetName);
  match000Rev->SetJetsNameTag(jetSDName);
  match000Rev->SetMatchingType(0);
  match000Rev->SetMatchId(2);
  handler->Add(match000Rev);
  
  anaSubJet *anasubjetsRev = new anaSubJet("anaSubJetsReversed","anaSubJetsReversed");
  anasubjetsRev->ConnectEventObject(fEventObjects);
  anasubjetsRev->SetHiEvtName("hiEventContainer");
  anasubjetsRev->SetJetsName(jetSD000Name);
  anasubjetsRev->SetJetsRefName(jetName);
  anasubjetsRev->SetJets000Name(jetSDName);
  anasubjetsRev->SetNCentBins(1);
  //anasubjetsRev->SetJetEtaRange(-2.,2.);
  anasubjetsRev->SetJetEtaRange(-1.3,1.3);
  anasubjetsRev->SetDoDijets(doDijet);
  anasubjetsRev->AddLeadingJetPtBin(120.,150.);
  anasubjetsRev->AddLeadingJetPtBin(150.,180.);
  anasubjetsRev->AddLeadingJetPtBin(180.,220.);
  anasubjetsRev->AddLeadingJetPtBin(220.,260.);
  anasubjetsRev->AddLeadingJetPtBin(260.,300.);
  anasubjetsRev->AddLeadingJetPtBin(300.,500.);
  anasubjetsRev->SetPtMinSubleading(30.);
  anasubjetsRev->SetStoreTree(true);
  anasubjetsRev->SetStoreTreeRef(true);//false);
  anasubjetsRev->SetMinPtJetTree(80.);
  handler->Add(anasubjetsRev);

  anaSubJet *anasubjetsGen = new anaSubJet("anasubjetsGen","anasubjetsGen");
  anasubjetsGen->ConnectEventObject(fEventObjects);
  anasubjetsGen->SetHiEvtName("hiEventContainer");
  anasubjetsGen->SetJetsName("akt4Gen"); //ungroomed jets with groomed subjet variables at gen level
  anasubjetsGen->SetJetsRefName(""); //
  anasubjetsGen->SetNCentBins(1);
  anasubjetsGen->SetJetEtaRange(-1.3,1.3);
  //anasubjetsGen->SetJetEtaRange(-2.,2.);
  anasubjetsGen->SetDoDijets(doDijet);
  anasubjetsGen->AddLeadingJetPtBin(120.,150.);
  anasubjetsGen->AddLeadingJetPtBin(150.,180.);
  anasubjetsGen->AddLeadingJetPtBin(180.,220.);
  anasubjetsGen->AddLeadingJetPtBin(220.,260.);
  anasubjetsGen->AddLeadingJetPtBin(260.,300.);
  anasubjetsGen->AddLeadingJetPtBin(300.,500.);
  anasubjetsGen->SetPtMinSubleading(30.);
  anasubjetsGen->SetStoreTree(true);
  anasubjetsGen->SetStoreTreeRef(false);
  anasubjetsGen->SetMinPtJetTree(80.);
  handler->Add(anasubjetsGen);

  anaSubJet *anasubjetsGen000 = new anaSubJet("anasubjetsGen000","anasubjetsGen000");
  anasubjetsGen000->ConnectEventObject(fEventObjects);
  anasubjetsGen000->SetHiEvtName("hiEventContainer");
  anasubjetsGen000->SetJetsName("akt4Gen000"); //ungroomed jets with groomed subjet variables at gen level
  anasubjetsGen000->SetJetsRefName(""); //
  anasubjetsGen000->SetNCentBins(1);
  anasubjetsGen000->SetJetEtaRange(-1.3,1.3);
  //anasubjetsGen000->SetJetEtaRange(-2.,2.);
  anasubjetsGen000->SetDoDijets(doDijet);
  anasubjetsGen000->AddLeadingJetPtBin(120.,150.);
  anasubjetsGen000->AddLeadingJetPtBin(150.,180.);
  anasubjetsGen000->AddLeadingJetPtBin(180.,220.);
  anasubjetsGen000->AddLeadingJetPtBin(220.,260.);
  anasubjetsGen000->AddLeadingJetPtBin(260.,300.);
  anasubjetsGen000->AddLeadingJetPtBin(300.,500.);
  anasubjetsGen000->SetPtMinSubleading(30.);
  anasubjetsGen000->SetStoreTree(true);
  anasubjetsGen000->SetStoreTreeRef(false);
  anasubjetsGen000->SetMinPtJetTree(80.);
  handler->Add(anasubjetsGen000);

  anaSubJet *anasubjetsNoFakes = new anaSubJet("anaSubJetsNoFakes","anaSubJetsNoFakes");
  anasubjetsNoFakes->ConnectEventObject(fEventObjects);
  anasubjetsNoFakes->SetHiEvtName("hiEventContainer");
  anasubjetsNoFakes->SetJetsName(jetSDName);
  anasubjetsNoFakes->SetJetsRefName(jetName);
  anasubjetsNoFakes->SetNCentBins(4);
  anasubjetsNoFakes->SetJetEtaRange(-2.,2.);
  anasubjetsNoFakes->SetMinRefPt(10.);
  //handler->Add(anasubjetsNoFakes);

  //SD with charged hadrons
  anaJetMatching *matchCh = new anaJetMatching("jetMatchingCh","jetMatchingCh");
  matchCh->ConnectEventObject(fEventObjects);
  matchCh->SetHiEvtName("hiEventContainer");
  matchCh->SetNCentBins(1);
  matchCh->SetJetsNameBase(jetName);
  matchCh->SetJetsNameTag(jetSDChName);
  matchCh->SetMatchingType(0);
//  handler->Add(matchCh);
  
  anaSubJet *anasubjetsCh = new anaSubJet("anaSubJetsCh","anaSubJetsCh");
  anasubjetsCh->ConnectEventObject(fEventObjects);
  anasubjetsCh->SetHiEvtName("hiEventContainer");
  anasubjetsCh->SetJetsName(jetSDChName);
  anasubjetsCh->SetJetsRefName(jetName);
  anasubjetsCh->SetNCentBins(1);
  //anasubjetsCh->SetJetEtaRange(-2.,2.);
  anasubjetsCh->SetJetEtaRange(-1.3,1.3);
  anasubjetsCh->SetDoDijets(doDijet);
  anasubjetsCh->AddLeadingJetPtBin(120.,150.);
  anasubjetsCh->AddLeadingJetPtBin(150.,180.);
  anasubjetsCh->AddLeadingJetPtBin(180.,220.);
  anasubjetsCh->AddLeadingJetPtBin(220.,260.);
  anasubjetsCh->AddLeadingJetPtBin(260.,300.);
  anasubjetsCh->AddLeadingJetPtBin(300.,500.);
  anasubjetsCh->SetPtMinSubleading(30.);
  anasubjetsCh->SetStoreTree(true);
  anasubjetsCh->SetStoreTreeRef(true);//false);
  anasubjetsCh->SetMinPtJetTree(80.);
//  handler->Add(anasubjetsCh);

  anaSubJet *anasubjetsGenCh = new anaSubJet("anasubjetsGenCh","anasubjetsGenCh");
  anasubjetsGenCh->ConnectEventObject(fEventObjects);
  anasubjetsGenCh->SetHiEvtName("hiEventContainer");
  anasubjetsGenCh->SetJetsName("akt4GenCh"); //ungroomed jets with groomed subjet variables at gen level
  anasubjetsGenCh->SetJetsRefName(""); //
  anasubjetsGenCh->SetNCentBins(1);
  anasubjetsGenCh->SetJetEtaRange(-1.3,1.3);
  //anasubjetsGenCh->SetJetEtaRange(-2.,2.);
  anasubjetsGenCh->SetDoDijets(doDijet);
  anasubjetsGenCh->AddLeadingJetPtBin(120.,150.);
  anasubjetsGenCh->AddLeadingJetPtBin(150.,180.);
  anasubjetsGenCh->AddLeadingJetPtBin(180.,220.);
  anasubjetsGenCh->AddLeadingJetPtBin(220.,260.);
  anasubjetsGenCh->AddLeadingJetPtBin(260.,300.);
  anasubjetsGenCh->AddLeadingJetPtBin(300.,500.);
  anasubjetsGenCh->SetPtMinSubleading(30.);
  anasubjetsGenCh->SetStoreTree(true);
  anasubjetsGenCh->SetStoreTreeRef(false);
  anasubjetsGenCh->SetMinPtJetTree(80.);
//  handler->Add(anasubjetsGenCh);

  
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
    p_SD000Jet->Run(jentry); //forest SoftDrop jets with zcut=0
    //   p_SDChJet->Run(jentry); //forest SoftDrop jets with charged hadrons
    p_Jet->Run(jentry); //forest jets
	    
    //Execute all analysis tasks
    handler->ExecuteTask();
  }
    
  fEventObjects->Print();

  TFile *out = new TFile(outname,"RECREATE","",8);
  TList *tasks = handler->GetListOfTasks();
  TIter next(tasks);
  anaBaseTask *obj;
  while ((obj = dynamic_cast<anaBaseTask*>(next()) ))
    if(obj->GetOutput()) obj->GetOutput()->Write(obj->GetName(),TObject::kSingleKey);
  
  out->Write();
  out->Close();
  
}
