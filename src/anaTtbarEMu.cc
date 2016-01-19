
#include "UserCode/diall/interface/anaTtbarEMu.h"
#include "UserCode/diall/interface/diParticle.h"
#include "UserCode/diall/interface/genParticle.h"
#include "UserCode/diall/interface/lwJetContainer.h"
#include "UserCode/diall/interface/particleBase.h"
#include "UserCode/diall/interface/pfParticle.h"


#include "TLorentzVector.h"
#include "TMath.h"

#include "TClass.h"

anaTtbarEMu::anaTtbarEMu(const char *name, const char *title) 
:anaBaseTask(name,title),
  fCheckPid(kFALSE),
  fMetType(),      
  fMinPt(0.), 
  fTriggerMapName(""),
  fElectronsName(""),
  fMuonsName(""),  
  fGenJetsName(""),
  fRecoJetsName(""),
  fParticlesName(""),
  fEMusName(""),
  fTopsName(""),
  fTriggerMap(0),
  fElectrons(0x0), 
  fMuons(0x0),     
  fGenJets(0x0),
  fRecoJets(0x0),
  fParticles(0x0),
  fEMus(0x0),
  fTops(0x0),
  fEMuCands(0),
  fh1NEvents(),
  fAnaTtbarEMuInfo(0x0)

{

  //for(Int_t j = 0; j<10; ++j)
  // fh2MetCentPtMin[j] = 0;
  
}


//----------------------------------------------------------
void anaTtbarEMu::Exec(Option_t * /*option*/)
{

  //printf("anaTtbarEMu executing\n");
   //if(!SelectEvent()) return;

   diParticle *pPart(0);
   TLorentzVector met;
   fEMuCands = 0;
   if(!fInitOutput) CreateOutputObjects();

   fh1NEvents->Fill(1);

   //Get objects from event

   //Get electrons
   if(!fTriggerMap && !fTriggerMapName.IsNull()) {
     fTriggerMap = dynamic_cast<triggerMap*>(fEventObjects->FindObject(fTriggerMapName.Data()));
   }
   //if(!fTriggerMap) { Printf("No %s TriggerMap found", fTriggerMapName.Data());  }

   //if(!fTriggerMap->TriggerFired("HLT_HIL3Mu15_v1")) return;
   //std::cout<< fTriggerMap->TriggerFired("HLT_HIL3Mu15_v1") <<std::endl;
   //fTriggerMap->PrintTriggers();
      
   fh1NEvents->Fill(2); //only for now!
   fh1NEvents->Fill(3); //only for now!

   //Get electrons
   if(!fElectrons && !fElectronsName.IsNull()) {
     fElectrons = dynamic_cast<TClonesArray*>(fEventObjects->FindObject(fElectronsName.Data()));
   }
   if(!fElectrons) { Printf("No %s Electrons found", fElectronsName.Data()); return; }

   const Int_t nelectrons = fElectrons->GetEntriesFast();
   fh1NElectrons.push_back(fElectrons->GetEntriesFast());

   //Get muons
   if(!fMuons && !fMuonsName.IsNull()) {
     fMuons = dynamic_cast<TClonesArray*>(fEventObjects->FindObject(fMuonsName.Data()));
   }
   if(!fMuons) { Printf("No %s Muons found", fMuonsName.Data()); return; } 

   const Int_t nmuons = fMuons->GetEntriesFast();
   fh1NMuons.push_back(nmuons);
   
   //Make array for emu candidates
   if(!fEventObjects->FindObject(fEMusName) && !fEMus) {
      fEMus = new TClonesArray("diParticle");
      fEMus->SetName(fEMusName);
      fEventObjects->Add(fEMus);
    }
   if(fEMus) fEMus->Delete();

   //Make array for top quark candidates
   //if(!fEventObjects->FindObject(fTopsName) && !fTops) {
   //   fTops = new TClonesArray("diParticle");
   //   fTops->SetName(fTopsName);
   //   fEventObjects->Add(fTops);
   // }
   //if(fTops) fTops->Delete();
   
   //Get jets
   if(!fRecoJets && !fRecoJetsName.IsNull())
     fRecoJets = dynamic_cast<lwJetContainer*>(fEventObjects->FindObject(fRecoJetsName.Data()));
   if(!fRecoJets) { Printf("No %s RecoJets found", fRecoJetsName.Data()); return; }
   const Int_t njets= fRecoJets->GetNJets();
   fh1NJetsIncl.push_back(njets);

 
   for (int i = 0; i < nelectrons; i++) {
     particleBase *ele = static_cast<particleBase*>(fElectrons->At(i));
     if(!ele) {
       Printf("%s ERROR: couldn't get electron",GetName());
       continue;
     }
     //if(fCheckPid)
     //if(!CheckPid(mu1)) continue;
     for (int j = 0; j < nmuons; j++) {
       particleBase *mu = static_cast<particleBase*>(fMuons->At(j));
       if(!mu) {
         Printf("%s ERROR: couldn't get muon",GetName());
         continue;
       }

       TLorentzVector l1 = ele->GetLorentzVector();
       TLorentzVector l2 = mu->GetLorentzVector();
       TLorentzVector emu = l1 + l2;
       
       //emu pair should be of opposite sign
       if(ele->GetCharge()*mu->GetCharge()<0) {

         //if(fCheckPid)
	 //if(!CheckPid(mu2)) continue;
                   
         //fh3CentPtInvMass->Fill(cent,dimu.Pt(),dimu.M());
         
         //Store emu pair candidates in event
         if(fEMus) {
           pPart = new ((*fEMus)[fEMuCands])
             diParticle(emu.Pt(),
                        emu.Eta(),
                        emu.Phi(),
                        emu.M(),
                        11);
           pPart->SetCharge(0);
           pPart->AddParticle(ele);
           pPart->AddParticle(mu);
           fEMuCands++;
	 }
       }              
     }//muon  loop
   }//electron loop

   Int_t nbjets = 0;
   for (int i = 0; i < fRecoJets->GetNJets(); i++) {
     lwJet * jet = fRecoJets->GetJet(i);
     if(jet->GetCsvSimpleDiscr()>0.9)nbjets++;
     //std::cout<<"disc" <<jet->GetCsvSimpleDiscr()<<std::endl;
   }
   fh1NBJetsIncl.push_back(nbjets);
   
   //Get particles from which MET will be calculated
   if(!fParticles && !fParticlesName.IsNull()) {
     //fEventObjects->Print();
     fParticles = dynamic_cast<TClonesArray*>(fEventObjects->FindObject(fParticlesName.Data()));
     //if(!fParticles) {
       //check if in jet branch
       //lwJetContainer *jetsCont = dynamic_cast<lwJetContainer*>(fEventObjects->FindObject(fParticlesName.Data()));
       //if(jetsCont) fParticles = jetsCont->GetJets();
     //}
   }
   
   if(!fParticles) {
     Printf("%s: WARNING: Couldn't locate %s branch",GetName(),fParticlesName.Data());
     return;
   }
   
   /*   
   //Double_t cent = 5.;//fHiEvent->GetCentrality();
   TLorentzVector p4(0.,0.,0.,0.);
   //Double_t sumEt = 0.;

   const Int_t nptmin = 10;
   //Double_t ptarr[nptmin] {0.,1.,2.,3.,10.,20.,30.,40.,50.,60.};
   TLorentzVector r4[nptmin];
   for(Int_t j = 0; j<nptmin; ++j)
     r4[j].SetPtEtaPhiM(0.,0.,0.,0.);
   for (int i = 0; i < fParticles->GetEntriesFast(); i++) {
     particleBase *p = static_cast<particleBase*>(fParticles->At(i));
     if(!p) {
       Printf("%s ERROR: couldn't get particle",GetName());
       continue;
     }
     
     TLorentzVector l;
     if(fMetType==kGen || fMetType==kPFRaw) {
     //  if(p->Pt() < fMinPt)
       //  continue;
       l = p->GetLorentzVector();
       printf("!! mpika edp %f\n", fMinPt);

     }
     else if(fMetType==kVS) {
       pfParticle *pf = dynamic_cast<pfParticle*>(p);
       if(!pf) {
         Printf("%s ERROR: couldn't cast particle to pfParticle",GetName());
         return;
       }
       l.SetPtEtaPhiM(pf->PtVS(),pf->Eta(),pf->Phi(),pf->M());
     }
     else if(fMetType==kPuppi) {
       pfParticle *pf = dynamic_cast<pfParticle*>(p);
       if(!pf) {
         Printf("%s ERROR: couldn't cast particle to pfParticle",GetName());
         return;
       }
       if (pf->GetPuppiWeight()!=0) printf("weight %f\n", pf->GetPuppiWeight());
       l = pf->GetPuppiWeight()*p->GetLorentzVector();
     }
     
     for(Int_t j = 0; j<nptmin; ++j) {
       if(l.Pt()>ptarr[j])
         r4[j]+=l;
     }
    
     if(l.Pt() < fMinPt) continue;
     fh3PtEtaPhi->Fill(l.Pt(),l.Eta(),l.Phi());
     p4+=l;
     sumEt+=l.Et();
 
   }//particle loop

   met = -p4;
   //fh2MetCent->Fill(cent,met.Pt());
   //fh2SumEtCent->Fill(cent,sumEt);

   //Int_t nhists = TMath::Min(nptmin,10);
   //for(Int_t j = 0; j<nhists; ++j) {
   //  TLorentzVector met2 = -r4[j];
   //  fh2MetCentPtMin[j]->Fill(cent,met2.Pt());
   //}
   Printf("muon loop %d\n", fMuons->GetEntriesFast()-1);
     */
   //int error;
   //fh1uParaZllPt->Fill(fParticles->GetEntriesFast());
   Bool_t filled = false; Bool_t filled_Rec2jets = false;
   for(int i = 0; i<fEMuCands; ++i) 
   { 
       diParticle *pPart = (diParticle*)fEMus->At(i);
       particleBase *pPart1 = dynamic_cast<particleBase*>(pPart->GetDecayParticles()->At(0));
       particleBase *pPart2 = dynamic_cast<particleBase*>(pPart->GetDecayParticles()->At(1));
	  
       //std::cout<<"first "<< pPart1->Pt()<<" second "<< pPart2->Pt()<< fEMuCands<<std::endl;
       float ht = 0;
       std::cout<< " print "<< nbjets<<std::endl;
       if(pPart->M()>20)
       {
	   std::cout<< " meta "<< nbjets<<std::endl;
	   fh1MassEMu.push_back(pPart->M());
	   if (pPart1->Pt()>pPart2->Pt()) 
	   {
	       fh1LeadLeptonPt.push_back(pPart1->Pt());
	       fh1LeadLeptonAbsEta.push_back(TMath::Abs(pPart1->Eta()));
	       fh1LeadLeptonEta.push_back(pPart1->Eta());
	   }
	   else 
	   {
	       fh1LeadLeptonPt.push_back(pPart2->Pt());
	       fh1LeadLeptonAbsEta.push_back(TMath::Abs(pPart2->Eta()));
	       fh1LeadLeptonEta.push_back(pPart2->Eta());
	   }
	   if(!filled)
	   {
	       fh1NEvents->Fill(4);
	       for (int jet = 0; jet < fRecoJets->GetNJets(); jet++) 
	       {
		   float pt = fRecoJets->GetJet(jet)->Pt();
		   if (jet==0)
		   {
		       fh1LeadJetPt.push_back(pt);
		   }
		     ht += fRecoJets->GetJet(jet)->Pt();
	       }
	       fh1HT.push_back(ht);
	       fh1NJets.push_back(njets);
	       if (njets>=2)fh1NEvents->Fill(5);
	       if (nbjets>=1)fh1NEvents->Fill(6);
	       switch(nbjets)
	       {
	       case 1:
		 fh1NEvents->Fill(7);
		 break;
	       case 2:
		 fh1NEvents->Fill(8);
		 break;
	       }
	       filled=true;
	   }
	   if(!filled_Rec2jets && njets>=2)
	   {
	       fh1LeadJetPt_Rec2jets.push_back(fRecoJets->GetJet(0)->Pt());
	       fh1LeadJetAbsEta_Rec2jets.push_back(TMath::Abs(fRecoJets->GetJet(0)->Eta()));
	       fh1LeadJetEta_Rec2jets.push_back(fRecoJets->GetJet(0)->Eta());
	       filled_Rec2jets=true;
	   }
	   if(njets>=2)
	   {
	       fh1DeltaPhi_Rec2jets.push_back(TMath::Abs(pPart1->DeltaPhi(pPart2))/TMath::Pi());
	       //std::cout<<TMath::Abs(pPart1->DeltaPhi(pPart2))/TMath::Pi()<<std::endl;
	       fh1MassEMu_Rec2jets.push_back(pPart->M());
	       if (pPart1->Pt()>pPart2->Pt()) 
	       {
		   fh1LeadLeptonPt_Rec2jets.push_back(pPart1->Pt());
		   fh1LeadLeptonAbsEta_Rec2jets.push_back(TMath::Abs(pPart1->Eta()));
		   fh1LeadLeptonEta_Rec2jets.push_back(pPart1->Eta());
	       }
	       else 
	       {
		   fh1LeadLeptonPt_Rec2jets.push_back(pPart2->Pt());
		   fh1LeadLeptonAbsEta_Rec2jets.push_back(TMath::Abs(pPart2->Eta()));
		   fh1LeadLeptonEta_Rec2jets.push_back(pPart2->Eta());
	       }
	   }
       }
   }
     
     fAnaTtbarEMuInfo->Fill();
     fh1NElectrons.clear();       
     fh1NMuons.clear();             
     fh1NJetsIncl.clear();          
     fh1NBJetsIncl.clear();  
     fh1HT.clear();                 
     fh1MassEMu.clear();            
     fh1LeadJetPt.clear();          
     fh1LeadLeptonPt.clear();       
     fh1LeadLeptonAbsEta.clear();   
     fh1LeadLeptonEta.clear();      
     fh1NJets.clear();              
   
     fh1DeltaPhi_Rec2jets.clear();  
     fh1MassEMu_Rec2jets.clear();   
     fh1LeadJetPt_Rec2jets.clear(); 
     fh1LeadJetAbsEta_Rec2jets.clear();
     fh1LeadJetEta_Rec2jets.clear();   
     fh1LeadLeptonPt_Rec2jets.clear();
     fh1LeadLeptonAbsEta_Rec2jets.clear();
     fh1LeadLeptonEta_Rec2jets.clear();   

     //}
     //if (pPart->Pt()>20 && pPart->M()>60 && pPart->M()<120)
     //std::pair<double, double> u = compHadronicRecoilProjU(pPart,met, error, count);
     //if (pPart->Pt()>48 && pPart->Pt()<60)
     //fh1uParaZllPt->Fill(std::get<1>(u)+pPart->Pt());
     //for (int k = 0; k < fRecoJets->GetNJets(); k++) {
     // acos(cos(muPhi[0]-jtphi[0]))>0.2 && acos(cos(muPhi[0]-jtphi[1]))>0.2 && acos(cos(elePhi[0]-jtphi[0]))>0.2 && acos(cos(elePhi[0]-jtphi[1]))>0.2
     //	 }

     //if(pPart->M()>20)
     
     //printf("%f", pPart->Pt());
       
}
 

//----------------------------------------------------------                                                                         
bool anaTtbarEMu::CheckPid(particleBase *p) {
  //check if generated particle is muon                                                                                              
  genParticle *gp = dynamic_cast<genParticle*>(p);
  if(!gp) return kFALSE;
  if(abs(gp->GetPID())==13) return kTRUE;
  return kFALSE;
}

std::pair<double, double> 
anaTtbarEMu::compMETProjU(diParticle* zP4, double metPx, double metPy, int& errorFlag, int count)
{
  if ( zP4->Pt() == 0. ) {
    Warning ("compMEtProjU", " Failed to compute projection, because Z0 candidate has zero Pt --> returning dummy solution !!");
    errorFlag = 1;
    return std::pair<double, double>(0., 0.);
  }
  
  double qX = zP4->Px();
  double qY = zP4->Py();
  double qT = TMath::Sqrt(qX*qX + qY*qY);
  
  double uX = -metPx;
  double uY = -metPy;
  uX -= qX;
  uY -= qY;
  
  
  double u1 = (uX*qX + uY*qY)/qT;
  double u2 = (uX*qY - uY*qX)/qT;

  return std::pair<double, double>(u1,u2);
}

std::pair<double, double> 
anaTtbarEMu::compHadronicRecoilProjU(diParticle* zP4, TLorentzVector MET, int& errorFlag, int count)
{
  if ( zP4->Pt() == 0. ) {
    Warning ("compMEtProjU", " Failed to compute projection, because Z0 candidate has zero Pt --> returning dummy solution !!");
    errorFlag = 1;
    return std::pair<double, double>(0., 0.);
  }
  
  double qX = zP4->Px();
  double qY = zP4->Py();
  double qT = TMath::Sqrt(qX*qX + qY*qY);
  
  TLorentzVector zBoson = zP4->GetLorentzVector();
  TLorentzVector hadronicRecoil = -(MET+zBoson);

  double uX = hadronicRecoil.Px();
  double uY = hadronicRecoil.Py();
  uX -= qX;
  uY -= qY;
  
  double u1 = (uX*qX + uY*qY)/qT;
  double u2 = (uX*qY - uY*qX)/qT;

  return std::pair<double, double>(u1,u2);
}

//----------------------------------------------------------
void 
anaTtbarEMu::CreateOutputObjects() {

  anaBaseTask::CreateOutputObjects();

  if(!fOutput) {
    Printf("anaTtbarEMu: fOutput not present");
    return;
  }
  
  fh1NEvents = new TH1F("fh1NEvents", "Entries per selection step", 8, 1,9);
  TString  step[8] = {"Initial", "Trigger", "PVfilter", "Dilepton", "#req 2jets","#req 1 b-tags","= 1 b-tag", "=2btag"};
  for (int i=1; i < fh1NEvents->GetNbinsX()+1; i++) 
    fh1NEvents->GetXaxis()->SetBinLabel(i,step[i-1]);

  fOutput->Add(fh1NEvents);

  fAnaTtbarEMuInfo = new TTree("anaTtbarEMuInfo","Tree with info related to ttbar emu analysis");
  fAnaTtbarEMuInfo->Branch("fh1NElectrons", &fh1NElectrons);
  fAnaTtbarEMuInfo->Branch("fh1NMuons",&fh1NMuons);
  fAnaTtbarEMuInfo->Branch("fh1NJetsIncl", &fh1NJetsIncl);
  fAnaTtbarEMuInfo->Branch("fh1NBJetsIncl",&fh1NBJetsIncl);

  fAnaTtbarEMuInfo->Branch("fh1HT", &fh1HT);          
  fAnaTtbarEMuInfo->Branch("fh1MassEMu",&fh1MassEMu);
  fAnaTtbarEMuInfo->Branch("fh1LeadJetPt",&fh1LeadJetPt);
  fAnaTtbarEMuInfo->Branch("fh1LeadLeptonPt",&fh1LeadLeptonPt);
  fAnaTtbarEMuInfo->Branch("fh1LeadLeptonAbsEta",&fh1LeadLeptonAbsEta);
  fAnaTtbarEMuInfo->Branch("fh1LeadLeptonEta",&fh1LeadLeptonEta);     
  fAnaTtbarEMuInfo->Branch("fh1NJets",&fh1NJets);

  fAnaTtbarEMuInfo->Branch("fh1DeltaPhi_Rec2jets", &fh1DeltaPhi_Rec2jets);          
  fAnaTtbarEMuInfo->Branch("fh1MassEMu_Rec2jets",&fh1MassEMu_Rec2jets);
  fAnaTtbarEMuInfo->Branch("fh1LeadJetPt_Rec2jets",&fh1LeadJetPt_Rec2jets);
  fAnaTtbarEMuInfo->Branch("fh1LeadJetAbsEta_Rec2jets",&fh1LeadJetAbsEta_Rec2jets);
  fAnaTtbarEMuInfo->Branch("fh1LeadLeptonPt_Rec2jets",&fh1LeadLeptonPt_Rec2jets);
  fAnaTtbarEMuInfo->Branch("fh1LeadLeptonAbsEta_Rec2jets",&fh1LeadLeptonAbsEta_Rec2jets);     
  fAnaTtbarEMuInfo->Branch("fh1LeadLeptonEta_Rec2jets",&fh1LeadLeptonEta_Rec2jets);
  
  fOutput->Add(fAnaTtbarEMuInfo);


  //for(Int_t j = 0; j<10; ++j) {
  //fh2MetCentPtMin[j] = new TH2F(Form("fh2MetCentPtMin%d",j),"fh2MetCent;centrality;MET",100,0,100,500,0,1000.);
  //fOutput->Add(fh2MetCentPtMin[j]);
  //}
    
  
}


