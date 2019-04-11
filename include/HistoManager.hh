#ifndef HISTOMANAGER_HH
#define HISTOMANAGER_HH

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "HistoMessenger.hh"

#ifndef USERVARIABLES_HH
#define USERVARIABLES_HH
#include "UserVariables.hh"
#endif

class TProfile;
class TH1D;
class TH2D;
class TFile;
class TObjArray;
class TTree;
class HistoMessenger;

typedef struct {double x,y,z,px,py,pz,e;} SH;
typedef struct {double Energy; int event;} EN;

class HistoManager
{
  public:

    HistoManager();
   ~HistoManager();

    void SetFileName   (const G4String name) { fileName = name; G4cout << "Output file set to " << fileName << G4endl;};
    G4String GetFileName() {return fileName;};
    void book();
    void save();
    void FillHisto(TH1D* hist, G4double e, G4double weight = 1.0);
    void FillHisto(TH2D* hist, G4int i, G4int j, G4double weight = 1.0);
   	void FillTrackHit(G4double, G4double, G4double, G4double, G4double, G4double, G4double, G4String);
    void FillDirections(G4ThreeVector dir, G4double edep, G4int xp, G4int yp);
    void FillEnergy(G4double En, G4int evt);
    G4String MakeFileName();
    void UpdateFileName(G4String newname);
    TH1D*   gunEnHist;
    TH2D*   csiHist;
    TH2D*   csiHist_Jena;
    TH2D*   csiHist_QUB;
    TH2D*   csiHist_DESY;
    TH2D*   csiHist_HDual;
    TH2D*   csiHist_VDual;
    HistoMessenger* histoMessenger;

  private:

	  static SH tracking;
	  static EN targetenergy;
	  static SH directions;

    G4String    fileName;
    G4String	  savef;

    TFile*		  OutPutFile;
    TObjArray*  DataList;

    TTree*			trackergamma;
    TTree*			trackerelec;
    TTree*      trackerpositron;
    TTree*			exit;
    TTree*			taren;
    TTree*			dirs;

    G4bool      factoryOn;

};

#endif // HISTOMANAGER_HH
