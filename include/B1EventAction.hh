#ifndef B1EventAction_h
#define B1EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "B1RunAction.hh"
#include <map>


class B1EventAction : public G4UserEventAction
{
  public:
    B1EventAction(B1RunAction* runAction);
    virtual ~B1EventAction();

    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);

    void AddEdep(G4double edep) { fEdep += edep; }
    void AddEdep_Jena(G4double edep_Jena) { fEdep_Jena += edep_Jena; }
    void AddEdep_DESY(G4double edep_DESY) { fEdep_DESY += edep_DESY; }

    std::map<G4int, int> GetRRMap(){ return RR; };
    B1RunAction* GetRunAction(){ return fRunAction; };

  private:
    B1RunAction* fRunAction;
    G4double     fEdep;
    G4double     fEdep_Jena;
    G4double     fEdep_DESY;

    // Here is a hashtable storing the track IDs which have been affected by radiation reaction
    std::map<G4int, int> RR;
};

#endif
