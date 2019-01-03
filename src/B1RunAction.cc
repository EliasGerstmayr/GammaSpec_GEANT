//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: B1RunAction.cc 93886 2015-11-03 08:28:26Z gcosmo $
//
/// \file B1RunAction.cc
/// \brief Implementation of the B1RunAction class

#include "B1RunAction.hh"
#include "B1PrimaryGeneratorAction.hh"
#include "B1DetectorConstruction.hh"
// #include "B1Run.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4ParameterManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1RunAction::B1RunAction(HistoManager* histoManager):
G4UserRunAction(),
fEdep("Edep", 0.),
fEdep2("Edep2", 0.),
fEdep_Jena("Edep_Jena", 0.),
fEdep2_Jena("Edep2_Jena", 0.),
fEdep_DESY("Edep_DESY", 0.),
fEdep2_DESY("Edep2_DESY", 0.),
fEdep_HDual("Edep_HDual", 0.),
fEdep2_HDual("Edep2_HDual", 0.),
fEdep_VDual("Edep_VDual", 0.),
fEdep2_VDual("Edep2_VDual", 0.),
histoManager(histoManager)
{
  // add new units for dose
  //
  const G4double milligray = 1.e-3*gray;
  const G4double microgray = 1.e-6*gray;
  const G4double nanogray  = 1.e-9*gray;
  const G4double picogray  = 1.e-12*gray;

  new G4UnitDefinition("milligray", "milliGy" , "Dose", milligray);
  new G4UnitDefinition("microgray", "microGy" , "Dose", microgray);
  new G4UnitDefinition("nanogray" , "nanoGy"  , "Dose", nanogray);
  new G4UnitDefinition("picogray" , "picoGy"  , "Dose", picogray);

  // Register parameter to the parameter manager
  G4ParameterManager* parameterManager = G4ParameterManager::Instance();
  parameterManager->RegisterParameter(fEdep);
  parameterManager->RegisterParameter(fEdep2);
  parameterManager->RegisterParameter(fEdep_Jena);
  parameterManager->RegisterParameter(fEdep2_Jena);
  parameterManager->RegisterParameter(fEdep_DESY);
  parameterManager->RegisterParameter(fEdep2_DESY);
  parameterManager->RegisterParameter(fEdep_HDual);
  parameterManager->RegisterParameter(fEdep2_HDual);
  parameterManager->RegisterParameter(fEdep_VDual);
  parameterManager->RegisterParameter(fEdep2_VDual);

  histoManager->book();

  // Set up random engine
  generator.seed(rand());
  // Fraction of energy lost, e.g. (0.1, 0.05) means 10% energy loss +- 5%
  distribution = std::normal_distribution<double>(0.3, 0.1);
  normrnd = std::bind(distribution, generator);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1RunAction::~B1RunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1RunAction::BeginOfRunAction(const G4Run*)
{
  // inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  // reset parameters to their initial values
  G4ParameterManager* parameterManager = G4ParameterManager::Instance();
  parameterManager->Reset();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1RunAction::EndOfRunAction(const G4Run* run)
{
  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;

  // Merge parameters
  G4ParameterManager* parameterManager = G4ParameterManager::Instance();
  parameterManager->Merge();

  // Compute dose = total energy deposit in a run and its variance
  //
  G4double edep  = fEdep.GetValue();
  G4double edep2 = fEdep2.GetValue();

  G4double edep_Jena  = fEdep_Jena.GetValue();
  G4double edep2_Jena = fEdep2_Jena.GetValue();

  G4double edep_DESY  = fEdep_DESY.GetValue();
  G4double edep2_DESY = fEdep2_DESY.GetValue();

  G4double edep_HDual  = fEdep_HDual.GetValue();
  G4double edep2_HDual = fEdep2_HDual.GetValue();

  G4double edep_VDual  = fEdep_VDual.GetValue();
  G4double edep2_VDual = fEdep2_VDual.GetValue();

  G4double rms = edep2 - edep*edep/nofEvents;
  if (rms > 0.) rms = std::sqrt(rms); else rms = 0.;

  G4double rms_Jena = edep2_Jena - edep_Jena*edep_Jena/nofEvents;
  if (rms_Jena > 0.) rms_Jena = std::sqrt(rms_Jena); else rms_Jena = 0.;

  G4double rms_DESY = edep2_DESY - edep_DESY*edep_DESY/nofEvents;
  if (rms_DESY > 0.) rms_DESY = std::sqrt(rms_DESY); else rms_DESY = 0.;

  G4double rms_HDual = edep2_HDual - edep_HDual*edep_HDual/nofEvents;
  if (rms_HDual > 0.) rms_HDual = std::sqrt(rms_HDual); else rms_HDual = 0.;

  G4double rms_VDual = edep2_VDual - edep_VDual*edep_VDual/nofEvents;
  if (rms_VDual > 0.) rms_VDual = std::sqrt(rms_VDual); else rms_VDual = 0.;

  const B1DetectorConstruction* detectorConstruction
   = static_cast<const B1DetectorConstruction*>
     (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  G4double dose = edep;
  G4double rmsDose = rms;

  G4double dose_Jena = edep_Jena;
  G4double rmsDose_Jena = rms_Jena;

  G4double dose_DESY = edep_DESY;
  G4double rmsDose_DESY = rms_DESY;

  G4double dose_HDual = edep_HDual;
  G4double rmsDose_HDual = rms_HDual;

  G4double dose_VDual = edep_VDual;
  G4double rmsDose_VDual = rms_VDual;

  // Run conditions
  //  note: There is no primary generator action object for "master"
  //        run manager for multi-threaded mode.
  const B1PrimaryGeneratorAction* generatorAction
   = static_cast<const B1PrimaryGeneratorAction*>
     (G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());

  // Print
  //
  if (IsMaster()) {
    G4cout
     << G4endl
     << "--------------------End of Global Run-----------------------";
  }
  else {
    G4cout
     << G4endl
     << "--------------------End of Local Run------------------------";
  }

  G4cout
     << G4endl
     << " The run consists of " << nofEvents << " electrons."
     << G4endl
     << " Energy deposited in RAL crystal = "
     << G4BestUnit(dose,"Energy") << " +- " << G4BestUnit(rmsDose,"Energy")
     << G4endl
     << " Energy deposited in Jena crystals = "
     << G4BestUnit(dose_Jena,"Energy") << " +- " << G4BestUnit(rmsDose_Jena,"Energy")
     << G4endl
     << " Energy deposited in DESY crystals = "
     << G4BestUnit(dose_DESY,"Energy") << " +- " << G4BestUnit(rmsDose_DESY,"Energy")
     << G4endl
     << " Energy deposited in horizontal Dual crystals = "
     << G4BestUnit(dose_HDual,"Energy") << " +- " << G4BestUnit(rmsDose_HDual,"Energy")
     << G4endl
     << " Energy deposited in vertical Dual crystals = "
     << G4BestUnit(dose_VDual,"Energy") << " +- " << G4BestUnit(rmsDose_VDual,"Energy")
     << G4endl
     << "------------------------------------------------------------"
     << G4endl
     << G4endl;

  histoManager->save();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1RunAction::AddEdep(G4double edep)
{
  fEdep  += edep;
  fEdep2 += edep*edep;
}

void B1RunAction::AddEdep_Jena(G4double edep_Jena)
{
  fEdep_Jena  += edep_Jena;
  fEdep2_Jena += edep_Jena*edep_Jena;
}

void B1RunAction::AddEdep_DESY(G4double edep_DESY)
{
  fEdep_DESY  += edep_DESY;
  fEdep2_DESY += edep_DESY*edep_DESY;
}

void B1RunAction::AddEdep_HDual(G4double edep_HDual)
{
  fEdep_HDual  += edep_HDual;
  fEdep2_HDual += edep_HDual*edep_HDual;
}

void B1RunAction::AddEdep_VDual(G4double edep_VDual)
{
  fEdep_VDual  += edep_VDual;
  fEdep2_VDual += edep_VDual*edep_VDual;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
