#include "HistoMessenger.hh"
#include <sstream>
#include "HistoManager.hh"
#include "G4UIdirectory.hh"
#include "G4UIcommand.hh"
#include "G4UIparameter.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithABool.hh"

HistoMessenger::HistoMessenger(HistoManager* manager):histoManager(manager)
{
  histoDir = new G4UIdirectory("/B1/histo/");
  histoDir->SetGuidance("Histograms control");
  filePathCmd = new G4UIcmdWithAString("/B1/histo/setOutputFile", this);
}

HistoMessenger::~HistoMessenger()
{
  delete filePathCmd;
  delete histoDir;
}

void HistoMessenger::SetNewValue(G4UIcommand* command, G4String newValues)
{
    if (command == filePathCmd){
      histoManager->UpdateFileName(newValues);
    }
}
