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
// $Id: B1SteppingAction.cc 74483 2013-10-09 13:37:06Z gcosmo $
//
/// \file B1SteppingAction.cc
/// \brief Implementation of the B1SteppingAction class

#include "B1SteppingAction.hh"
#include "B1EventAction.hh"
#include "B1DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include <random>

#ifndef USERVARIABLES_HH
#define USERVARIABLES_HH
#include "UserVariables.hh"
#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1SteppingAction::B1SteppingAction(B1EventAction* eventAction, HistoManager* histoManager)
: G4UserSteppingAction(),
  fEventAction(eventAction),
  histoManager(histoManager),
  fScoringVolume(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1SteppingAction::~B1SteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1SteppingAction::UserSteppingAction(const G4Step* step)
{
  // Get the detector construction from the run manager
  const B1DetectorConstruction* detectorConstruction
    = static_cast<const B1DetectorConstruction*>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());

  // This gets the histogram of gun energies
  G4double energy = 0;
  // If particle is on its first step and it didn't come from anything else
  if (step->GetTrack()->GetCurrentStepNumber() == 1 && step->GetTrack()->GetParentID() == 0) {
    energy = step->GetPreStepPoint()->GetKineticEnergy();
    //G4cout << "Particle energy " << energy << std::endl;
    histoManager->FillHisto(histoManager->gunEnHist, energy, 1.);
  }

  // This gets the energies of particles as they pass through a plane
  G4double x, y, z, e, px, py, pz, theta;
  if (step->GetPostStepPoint()->GetTouchableHandle()->GetVolume() == detectorConstruction->GetScoringVolume()){
    if (step->GetTrack()->GetKineticEnergy() > 1.*MeV & step->GetTrack()->GetMomentum().z() < 0) {
			x = step->GetTrack()->GetPosition().x();
			y = step->GetTrack()->GetPosition().y();
			z = step->GetTrack()->GetPosition().z();
      px = step->GetTrack()->GetMomentum().x();
      py = step->GetTrack()->GetMomentum().y();
      pz = step->GetTrack()->GetMomentum().z();
      e = step->GetTrack()->GetKineticEnergy();

      theta = -1.*std::asin(std::sqrt(px*px + py*py)/pz);

			histoManager->FillTrackHit(x, y, z, px, py, pz, e, step->GetTrack()->GetDefinition()->GetParticleName());
    }
	}

  // This modifies the energy of particles passing through collimator
  if (RRon & step->GetTrack()->GetDefinition()->GetParticleName() == "e-") {
    if (step->GetPostStepPoint()->GetTouchableHandle()->GetVolume() == detectorConstruction->GetRRVolume()){

      G4int ID = step->GetTrack()->GetTrackID();

      std::map<G4int, int> RR = fEventAction->GetRRMap();

      B1RunAction* RA = fEventAction->GetRunAction();

      // If track ID not in hashtable...
      if(RR.find(ID) == RR.end()){
        e = step->GetTrack()->GetKineticEnergy();

        //G4double r = (double)rand()/RAND_MAX;
        G4double r = RA->drawNorm();
        r = r < 0.0 ? 0.0 : r;
        r = r > 1.0 ? 1.0 : r;

        x = step->GetTrack()->GetPosition().x();
  			y = step->GetTrack()->GetPosition().y();
        z = step->GetTrack()->GetPosition().z();

        G4double fspotrad = 5*um;

        // If inside focal spot of size ~ 15 microns
        if( x*x + y*y < fspotrad*fspotrad){
          RR[ID] = 1;
          px = step->GetTrack()->GetMomentum().x();
          py = step->GetTrack()->GetMomentum().y();
          pz = step->GetTrack()->GetMomentum().z()*(1 - r);
          G4double pnorm = std::sqrt(px*px + py*py + pz*pz);
          step->GetTrack()->SetKineticEnergy(e*(1 - r));
          step->GetTrack()->SetMomentumDirection(G4ThreeVector(px/pnorm, py/pnorm, pz/pnorm));
          if(debug) {std::cout << "RR modified energy" << std::endl;}
        }
      } else {
        // Particle has already experienced radiation reaction so do nothing
        if(debug) {std::cout << "RR already modified energy of " << ID << std::endl;}
      }

    }
  }

  // Get energy deposited in crystal (record deposited energy by particles in crystal)
  // crystals in the RAL 2015 stack
  if (step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName() == "Crystal") {
    G4int crystalind = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetCopyNo();
    G4int j = crystalind % Ncrystalsz;
    G4float i = ((float)crystalind - (float)j)/(float)Ncrystalsz;
    if (debug) G4cout << "Hit crystal " << crystalind << " = (" << i << ", " << j << ")" << G4endl;
    G4double edepStep = step->GetTotalEnergyDeposit();
    histoManager->FillHisto(histoManager->csiHist, i, j, edepStep);
    fEventAction->AddEdep(edepStep);
  }
  // crystals in the Jena profile stack
  if (step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName() == "Crystal_Jena") {
    G4int crystalind_Jena = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetCopyNo();
    G4int j = crystalind_Jena % Ncrystalsx_Jena;
    G4float i = ((float)crystalind_Jena - (float)j)/(float)Ncrystalsx_Jena;
    if (debug) G4cout << "Hit Jena profile crystal " << crystalind_Jena << " = (" << i << ", " << j << ")" << G4endl;
    G4double edepStep_Jena = step->GetTotalEnergyDeposit();
    histoManager->FillHisto(histoManager->csiHist_Jena, i, j, edepStep_Jena);
    fEventAction->AddEdep_Jena(edepStep_Jena);
  }
  // crystals in the DESY profile stack
  if (step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName() == "Crystal_DESY") {
    G4int crystalind_DESY = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetCopyNo();
    G4int j = crystalind_DESY % Ncrystalsx_DESY;
    G4float i = ((float)crystalind_DESY - (float)j)/(float)Ncrystalsx_DESY;
    if (debug) G4cout << "Hit DESY profile crystal " << crystalind_DESY << " = (" << i << ", " << j << ")" << G4endl;
    G4double edepStep_DESY = step->GetTotalEnergyDeposit();
    histoManager->FillHisto(histoManager->csiHist_DESY, i, j, edepStep_DESY);
    fEventAction->AddEdep_DESY(edepStep_DESY);
  }
  // horizontal facing crystals in the dual stack
  if (step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName() == "Crystal_HDual") {
    G4int crystalind_HDual = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetCopyNo();
    G4int j = crystalind_HDual % Ncrystalsy_Dual;
    G4float i = ((float)crystalind_HDual - (float)j)/(float)Ncrystalsy_Dual;
    if (debug) G4cout << "Hit horizontal dual stack crystal " << crystalind_HDual << " = (" << i << ", " << j << ")" << G4endl;
    G4double edepStep_HDual = step->GetTotalEnergyDeposit();
    histoManager->FillHisto(histoManager->csiHist_HDual, i, j, edepStep_HDual);
    fEventAction->AddEdep_HDual(edepStep_HDual);
  }
  // vertical facing crystals in the dual stack
  if (step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName() == "Crystal_VDual") {
    G4int crystalind_VDual = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetCopyNo();
    G4int j = crystalind_VDual % Ncrystalsx_Dual;
    G4float i = ((float)crystalind_VDual - (float)j)/(float)Ncrystalsx_Dual;
    if (debug) G4cout << "Hit vertical dual stack crystal " << crystalind_VDual << " = (" << i << ", " << j << ")" << G4endl;
    G4double edepStep_VDual = step->GetTotalEnergyDeposit();
    histoManager->FillHisto(histoManager->csiHist_VDual, i, j, edepStep_VDual);
    fEventAction->AddEdep_VDual(edepStep_VDual);
  }

}
