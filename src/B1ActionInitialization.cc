#include "B1ActionInitialization.hh"
#include "B1PrimaryGeneratorAction.hh"
#include "B1RunAction.hh"
#include "B1EventAction.hh"
#include "B1SteppingAction.hh"
#include "HistoManager.hh"


B1ActionInitialization::B1ActionInitialization(HistoManager* histoManager)
 : G4VUserActionInitialization(),histoManager(histoManager)
{}


B1ActionInitialization::~B1ActionInitialization()
{}



void B1ActionInitialization::BuildForMaster() const
{
  B1RunAction* runAction = new B1RunAction(histoManager);
  SetUserAction(runAction);
}

void B1ActionInitialization::Build() const
{
  SetUserAction(new B1PrimaryGeneratorAction);

  B1RunAction* runAction = new B1RunAction(histoManager);
  SetUserAction(runAction);

  B1EventAction* eventAction = new B1EventAction(runAction);
  SetUserAction(eventAction);

  SetUserAction(new B1SteppingAction(eventAction, histoManager));
}
