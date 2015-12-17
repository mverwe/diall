#include "UserCode/diall/interface/anaHadronIsolation.h"

#include "UserCode/diall/interface/pfParticle.h"
#include "UserCode/diall/interface/genParticle.h"

#include "TClass.h"
#include "TMath.h"

ClassImp(anaHadronIsolation)
   
anaHadronIsolation::anaHadronIsolation(const char *name, const char *title) 
:anaBaseTask(name,title),
  fIsolationType(kRaw),
  fConeRadius(0.5),
  fOffset(0.05),
  fPtMinProbe(40.),
  fRandom(new TRandom3(0)),
  fCentBin(-1),
  fPFParticlesName(""),
  fPFParticles(0x0),
  fRhoMapName(""),
  fRhoMap(),
  fRhoMMapName(""),
  fRhoMMap(),
  fJetContRecoName(""),
  fJetContReco(),
  fJetContGenName(""),
  fJetContGen(),
  fh2CentIso(),
  fh2IsoZCone(),
  fh2PtRecoIso(),
  fh3PtProbeIsoPtJet(),
  fh3ResIsoZlead(),
  fh3PtProbeResIso(),
  fh3PtJetResIso(),
  fh3PtProbeResRawIso(),
  fh3PtJetResRawIso()
{

  fh2IsoZCone = new TH2F*[4];
  fh2PtRecoIso = new TH2F*[4];
  for(Int_t i = 0; i<4; i++) {
    fh2IsoZCone[i] = 0;
    fh2PtRecoIso[i] = 0;
  }
}

//________________________________________________________________________
anaHadronIsolation::~anaHadronIsolation() {
  // Destructor
  if(fRandom) { delete fRandom; fRandom = 0x0; }
}

//----------------------------------------------------------
void anaHadronIsolation::Exec(Option_t * /*option*/)
{
   //printf("anaHadronIsolation executing\n");
   if(!fInitOutput) CreateOutputObjects();
   if(!SelectEvent()) return;

   //Get objects from event
   if(!fRhoMap && !fRhoMapName.IsNull())
     fRhoMap = dynamic_cast<rhoMap*>(fEventObjects->FindObject(fRhoMapName.Data()));

   if(!fRhoMMap && !fRhoMMapName.IsNull())
     fRhoMMap = dynamic_cast<rhoMap*>(fEventObjects->FindObject(fRhoMMapName.Data()));
   
   //Get pf particles
   if(!fPFParticles && !fPFParticlesName.IsNull()) {
     fPFParticles = dynamic_cast<TClonesArray*>(fEventObjects->FindObject(fPFParticlesName.Data()));
   }
   if(!fPFParticles) return;

   //Get det-level jets
   if(!fJetContReco && !fJetContRecoName.IsNull()) {
     fJetContReco = dynamic_cast<lwJetContainer*>(fEventObjects->FindObject(fJetContRecoName.Data()));
   }

   //Get particle-level jets
   if(!fJetContGen && !fJetContGenName.IsNull()) {
     fJetContGen = dynamic_cast<lwJetContainer*>(fEventObjects->FindObject(fJetContGenName.Data()));
   }
   
   //Determine centrality bin
   Double_t cent = 1;
   fCentBin = 0;
   if(fHiEvent) {
     cent = fHiEvent->GetCentrality();
     if(cent>=0. && cent<10.)       fCentBin = 0;
     else if(cent>=10. && cent<30.) fCentBin = 1;
     else if(cent>=30. && cent<50.) fCentBin = 2;
     else if(cent>=50. && cent<80.) fCentBin = 3;
     else fCentBin = -1;
   }

   std::vector<pfParticle> partInCone;
   for (int i = 0; i < fPFParticles->GetEntriesFast(); i++) {
     pfParticle *pfProbe = static_cast<pfParticle*>(fPFParticles->At(i));
     if(!pfProbe) continue;
     if(pfProbe->Pt()<fPtMinProbe || abs(pfProbe->Eta())>2. || pfProbe->GetId()!=1) continue;

     partInCone.clear();
     Double_t conePt = 0.;
     Double_t ptlead = -1;
     for (int j = 0; j < fPFParticles->GetEntriesFast(); j++) {
       if(i==j) continue;
       
       pfParticle *pfp = static_cast<pfParticle*>(fPFParticles->At(j));
       Double_t dr = pfProbe->DeltaR(pfp);
       if(fIsolationType==kCS && dr<(fConeRadius+fOffset)) { //collect particles in cone
         partInCone.push_back(*pfp);
       }
       if(dr>fConeRadius) continue;
       if(fIsolationType==kRaw || fIsolationType==kArea) {
         conePt+=pfp->Pt();
         if(pfp->Pt()>ptlead) ptlead = pfp->Pt();
       }
       else if(fIsolationType==kVS)  {
         conePt+=pfp->PtVS();
         if(pfp->PtVS()>ptlead) ptlead = pfp->PtVS();
       }
       else if(fIsolationType==kPuppi || fIsolationType==kPuppi2)  {
         Double_t weight = pfp->GetPuppiWeight();
         if(fIsolationType==kPuppi2) weight = pfp->GetPuppiWeight2();
         Double_t pt = pfp->Pt()*weight;
         conePt+=pt;
         if(pt>ptlead) ptlead = pt;
       }
     }//pfParticle in cone loop

     if(fIsolationType==kArea) {
       if(fRhoMap)
         conePt-=fRhoMap->GetValue(pfProbe->Eta())*TMath::Pi()*fConeRadius*fConeRadius;
       else
         conePt = 999.;
     }

     if(fIsolationType==kCS) {
       conePt = -1;//DoConstituentSubtraction(partInCone,muon->Eta(),muon->Phi(),ptlead);
       //   Printf("conePt: %f  ptlead: %f",conePt,ptlead);
     }
     Double_t iso = 999.;
     if(pfProbe->Pt()>0.) iso = conePt/pfProbe->Pt();
     fh2CentIso->Fill(cent,iso);
     if(fCentBin>=0) {
       Double_t zlead = ptlead/conePt;
       fh2IsoZCone[fCentBin]->Fill(iso,zlead);
       fh2PtRecoIso[fCentBin]->Fill(pfProbe->Pt(),iso);

       //correlate to det-level jet
       Double_t drmin = 999.;
       int jetId = -1;
       for (int k = 0; k < fJetContReco->GetNJets(); k++) {
         lwJet *jet = fJetContReco->GetJet(k);
         if(!jet) continue;
         Double_t dr = pfProbe->DeltaR(jet);
         if(dr<drmin) {
           drmin = dr;
           jetId = k;
         }
       }

       //fill histograms
       lwJet *jet = fJetContReco->GetJet(jetId);
       //int idm = jet->GetMatchId1();
       //lwJet *jetp = jet->GetJet(idm);
       double res = -999.;
       double resRaw = -999.;
       //if(jetp->Pt()>0.) res = jet->Pt()/jetp->Pt();
       //if(jet->GetRefPt()>0.) res = jet->Pt()/jet->GetRefPt();
       if(jet->Pt()>0.) res = pfProbe->Pt()/jet->Pt();
       if(jet->GetRawPt()>0.) resRaw = pfProbe->Pt()/jet->GetRawPt();
       fh3PtProbeIsoPtJet->Fill(pfProbe->Pt(),iso,jet->Pt());
       fh3ResIsoZlead->Fill(res,iso,zlead);
       fh3PtProbeResIso->Fill(pfProbe->Pt(),res,iso);
       fh3PtJetResIso->Fill(jet->Pt(),res,iso);

       fh3PtProbeResRawIso->Fill(pfProbe->Pt(),resRaw,iso);
       fh3PtJetResRawIso->Fill(jet->Pt(),resRaw,iso);
     }
   }//probe hadron loop
}

//----------------------------------------------------------
Double_t anaHadronIsolation::DoConstituentSubtraction(std::vector<pfParticle> particles, const Double_t muEta, const Double_t muPhi, Double_t &ptlead) {

  //apply constituent subtraction on particles in cone around muon
  //Code copied and adjusted from ConstituentSubtractor of fastjet contrib package
  if(!fRandom) fRandom = new TRandom3(0); //should already exist

  //--------------------------------------------------
  //construct vector with ghosts in grid around muon axis in eta-phi plane
  //grid size: fConeRadius vs fConeRadius + some extra room (fIOffset)
  Double_t ghostArea = 0.005;
  std::vector<pfParticle> ghosts;
  Double_t gridLength = 2.*(fConeRadius+fOffset);
  Int_t nEta = TMath::FloorNint(gridLength/ghostArea);
  Int_t nPhi = nEta;
  ghostArea = 0.005*0.005;

  Double_t etaMin = muEta - gridLength/2.;
  Double_t phiMin = muPhi - gridLength/2.;
  Double_t etaWidth = gridLength/(Double_t)nEta;
  Double_t phiWidth = gridLength/(Double_t)nPhi;

  Double_t rho = 1e-9;
  if(fRhoMap) rho = fRhoMap->GetValue(muEta);
  Double_t rhom = 0.;
  if(fRhoMMap) rhom = fRhoMMap->GetValue(muEta);

  //create ghost particles (only with fConeRadius from probe particle position)
  for(Int_t ieta= 0; ieta<nEta; ++ieta) {
    for(Int_t iphi= 0; iphi<nPhi; ++iphi) {
      //pick random eta and phi within grid cell
      Double_t geta = etaMin + (Double_t)ieta*etaWidth + fRandom->Uniform(1.)*etaWidth;
      Double_t gphi = phiMin + (Double_t)iphi*phiWidth + fRandom->Uniform(1.)*phiWidth;

      Double_t dPhi = muPhi - gphi;
      Double_t dEta = muEta - geta;
      dPhi = TVector2::Phi_mpi_pi(dPhi);
      Double_t dr2 = dPhi * dPhi + dEta * dEta;
      Double_t r = TMath::Sqrt(dr2);
      if(r>(fConeRadius+fOffset)) continue;
      
      pfParticle ghost(rho*ghostArea,geta,gphi,rhom*ghostArea);
      ghosts.push_back(ghost);
    }
  }

  //--------------------------------------------------
  // computing and sorting the distances, deltaR
  Double_t alpha = 0.;
  Double_t alpha_times_two=alpha*2.;
  Double_t maxDeltaR = -1.;
  bool use_max_deltaR=false;
  if (maxDeltaR>0.) use_max_deltaR=true;
  Double_t maxDeltaR_squared=pow(maxDeltaR,2);
    
  std::vector<std::pair<double,int> > deltaRs;  // the first element is deltaR, the second element is only the index in the vector used for sorting
  std::vector<int> particle_indices_unsorted;
  std::vector<int> ghost_indices_unsorted;

  for (unsigned int i=0;i<particles.size(); i++) {
    double pt_factor=1.;
    if (fabs(alpha_times_two)>1e-5) pt_factor=pow(particles[i].Pt(),alpha_times_two);
  
    for (unsigned int j=0;j<ghosts.size(); j++) {
      double deltaR_squared = ghosts[j].DeltaRSquared(particles[i])*pt_factor;
      if (!use_max_deltaR || deltaR_squared<=maxDeltaR_squared) {
        particle_indices_unsorted.push_back(i);
        ghost_indices_unsorted.push_back(j);
        int deltaRs_size=deltaRs.size();  // current position
        deltaRs.push_back(std::make_pair(deltaR_squared,deltaRs_size));
      }
    }
  }
  std::sort(deltaRs.begin(),deltaRs.end(),anaHadronIsolation::SortingFunction);
  unsigned long nStoredPairs=deltaRs.size();
  
  //--------------------------------------------------
  // the iterative process. Here, only finding the fractions of pt to be corrected.
  // The actual correction of particles is done later.
  std::vector<double> ghosts_fraction_of_pt(ghosts.size(),1.);
  std::vector<double> particles_fraction_of_pt(particles.size(),1.);
  for (unsigned long iindices=0;iindices<nStoredPairs;++iindices) {
    int particle_index=particle_indices_unsorted[deltaRs[iindices].second];
    int ghost_index=ghost_indices_unsorted[deltaRs[iindices].second];
    
    if (ghosts_fraction_of_pt[ghost_index]>0 && particles_fraction_of_pt[particle_index]>0) {
      double ratio_pt = particles[particle_index].Pt()*particles_fraction_of_pt[particle_index]/ghosts[ghost_index].Pt()/ghosts_fraction_of_pt[ghost_index];
      if (ratio_pt>1) {
        particles_fraction_of_pt[particle_index]*=1-1./ratio_pt;
        ghosts_fraction_of_pt[ghost_index]=-1;
      }
      else {
        ghosts_fraction_of_pt[ghost_index]*=1-ratio_pt;
        particles_fraction_of_pt[particle_index]=-1;
      }
    }
  }

  //--------------------------------------------------
  //Subtract particles and return sum pt
  std::vector<pfParticle> subtracted_particles;
  Double_t sumPt = 0.;
  for (unsigned int i=0;i<particles_fraction_of_pt.size(); i++) {
    if (particles_fraction_of_pt[i]<=0) continue;  // particles with zero pt are not used
    double subtracted_pt=0;
    if (particles_fraction_of_pt[i]>0) subtracted_pt=particles[i].Pt()*particles_fraction_of_pt[i];

    //only take particles in cone (excluding offset)
    Double_t dPhi = muPhi - particles[i].Phi();
    Double_t dEta = muEta - particles[i].Eta();
    dPhi = TVector2::Phi_mpi_pi(dPhi);
    Double_t dr2 = dPhi * dPhi + dEta * dEta;
    if(dr2<(fConeRadius*fConeRadius)) {
      if(subtracted_pt>ptlead) ptlead = subtracted_pt;
      sumPt+=subtracted_pt;
    }
  }
  return sumPt;
}

//----------------------------------------------------------
Bool_t anaHadronIsolation::SortingFunction(std::pair<double,int> i,std::pair<double, int> j){
    return (i.first < j.first);
  }

//----------------------------------------------------------
void anaHadronIsolation::CreateOutputObjects() {

  anaBaseTask::CreateOutputObjects();

  if(!fOutput) {
    Printf("anaHadronIsolation: fOutput not present");
    return;
  }

  fh2CentIso = new TH2F("fh2CentIso","fh2CentIso;centrality (%);p_{T,cone}/p_{T,probe}",100,0,100,400,-3.,5.);
  fOutput->Add(fh2CentIso);

  TString histName = "";
  TString histTitle = "";
  for(Int_t i = 0; i<4; i++) {
    histName = Form("fh2IsoZCone_%d",i);
    histTitle = Form("%s;p_{T,cone}/p_{T,probe};z_{lead,cone}",histName.Data());
    fh2IsoZCone[i] = new TH2F(histName.Data(),histTitle.Data(),250,0,5,100,0,1.);
    fOutput->Add(fh2IsoZCone[i]);

    histName = Form("fh2PtRecoIso_%d",i);
    histTitle = Form("%s;p_{T,probe,reco};iso",histName.Data());
    fh2PtRecoIso[i] = new TH2F(histName.Data(),histTitle.Data(),100,0.,100.,250,0,5);
    fOutput->Add(fh2PtRecoIso[i]);
  }

  histName = Form("fh3PtProbeIsoPtJet");
  histTitle = Form("%s;p_{T,probe};p_{T,cone}/p_{T,probe};p_{T,jet}",histName.Data());
  fh3PtProbeIsoPtJet = new TH3F(histName.Data(),histTitle.Data(),100,0.,200.,250,0,5,50,0.,500.);
  fOutput->Add(fh3PtProbeIsoPtJet);
  
  histName = Form("fh3ResIsoZlead");
  histTitle = Form("%s;p_{T,probe}/p_{T,jet};p_{T,cone}/p_{T,probe};z_{lead}",histName.Data());
  fh3ResIsoZlead = new TH3F(histName.Data(),histTitle.Data(),300,0.,3.,125,0,5,100,0.,1.);
  fOutput->Add(fh3ResIsoZlead);

  histName = Form("fh3PtProbeResIso");
  histTitle = Form("%s;p_{T,probe};p_{T,probe}/p_{T,jet};p_{T,cone}/p_{T,probe}",histName.Data());
  fh3PtProbeResIso = new TH3F(histName.Data(),histTitle.Data(),100,0.,200.,300,0,3,125,0.,5.);
  fOutput->Add(fh3PtProbeResIso);

  histName = Form("fh3PtJetResIso");
  histTitle = Form("%s;p_{T,jet};p_{T,probe}/p_{T,jet};p_{T,cone}/p_{T,probe}",histName.Data());
  fh3PtJetResIso = new TH3F(histName.Data(),histTitle.Data(),50,0.,500.,300,0,3,125,0.,5.);
  fOutput->Add(fh3PtJetResIso);

  histName = Form("fh3PtProbeResRawIso");
  histTitle = Form("%s;p_{T,probe};p_{T,probe}/p_{T,jet};p_{T,cone}/p_{T,probe}",histName.Data());
  fh3PtProbeResRawIso = new TH3F(histName.Data(),histTitle.Data(),100,0.,200.,300,0,3,125,0.,5.);
  fOutput->Add(fh3PtProbeResRawIso);

  histName = Form("fh3PtJetResRawIso");
  histTitle = Form("%s;p_{T,jet};p_{T,probe}/p_{T,jet};p_{T,cone}/p_{T,probe}",histName.Data());
  fh3PtJetResRawIso = new TH3F(histName.Data(),histTitle.Data(),50,0.,500.,300,0,3,125,0.,5.);
  fOutput->Add(fh3PtJetResRawIso);
  
  
}
