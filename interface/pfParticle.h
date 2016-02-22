#ifndef pfParticle_h
#define pfParticle_h

//
// light-weight particle flow candidate
//

#include "UserCode/diall/interface/particleBase.h"

class pfParticle : public particleBase {
 public:
  pfParticle();
  pfParticle(double pt, double eta, double phi, double m, int id = -1, int c = 0);
  virtual ~pfParticle() {;}
  pfParticle(const pfParticle& obj); // copy constructor
  pfParticle& operator=(const pfParticle& other); // assignment
  
  
  void           SetPuppiId(int id)         { fPupId = id;    }

  void           SetPuppiAlpha(double a)    { fPupAlpha = a;  }
  void           SetPuppiMetric2(double a)  { fPupMetric2 = a;}
  void           SetPuppiSumPt(double a)    { fPupSumPt = a;  }
  void           SetPuppiAlpha2(double a)   { fPupAlpha2= a;  }
  void           SetPuppiMeanPt(double a)   { fPupMeanPt = a; }
  void           SetPuppiMeanMd(double a)   { fPupMeanMd = a; }

  void           SetPuppiWeight(double w)   { fPupWeight = w; }
  void           SetPuppiWeight2(double w)  { fPupWeight2= w; }
  void           SetPuppiWeight3(double w)  { fPupWeight3= w; }
  void           SetPtVS(double pt)         { fPtVS  = pt;    }
  
  int            GetPuppiId() const      { return fPupId;     }

  double         GetPuppiAlpha() const   { return fPupAlpha;  }
  double         GetPuppiMetric2() const { return fPupMetric2;}
  double         GetPuppiSumPt() const   { return fPupSumPt;  }
  double         GetPuppiAlpha2() const  { return fPupAlpha2; }
  double         GetPuppiMeanPt() const  { return fPupMeanPt; }
  double         GetPuppiMeanMd() const  { return fPupMeanMd; }

  double         GetPuppiWeight() const  { return fPupWeight; }
  double         GetPuppiWeight2() const { return fPupWeight2;}
  double         GetPuppiWeight3() const { return fPupWeight3;}
  double         PtVS()       const      { return fPtVS;      } 

 protected:
  int          fPupId;       //puppi id
  double       fPupAlpha;    //puppi alpha = pt/r/r
  double       fPupMetric2;  //puppi metric2
  double       fPupSumPt;    //puppi sum pt
  double       fPupAlpha2;   //pt/r
  double       fPupMeanPt;   //mean pt
  double       fPupMeanMd;   //mean md
  double       fPupWeight;   //puppi weight
  double       fPupWeight2;  //puppi weight2
  double       fPupWeight3;  //puppi weight3
  double       fPtVS;        //subtracted pt from HF-Voronoi algo
  
  ClassDef(pfParticle,4)
};
#endif
