#ifndef B1SteppingAction_h
#define B1SteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"
#include "HistoManager.hh"
#include <map>

class B1EventAction;

class G4VPhysicalVolume;

/// Stepping action class

class B1SteppingAction : public G4UserSteppingAction
{
  public:
    B1SteppingAction(B1EventAction* eventAction, HistoManager* histoManager);
    virtual ~B1SteppingAction();

    // method from the base class
    virtual void UserSteppingAction(const G4Step*);

  private:
    B1EventAction*  fEventAction;
    G4VPhysicalVolume* fScoringVolume;
    HistoManager*   histoManager;
};

#endif
