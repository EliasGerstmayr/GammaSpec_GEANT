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
// $Id: B1RunAction.hh 93886 2015-11-03 08:28:26Z gcosmo $
//
/// \file B1RunAction.hh
/// \brief Definition of the B1RunAction class

#ifndef B1RunAction_h
#define B1RunAction_h 1

#include "G4UserRunAction.hh"
#include "G4Parameter.hh"
#include "globals.hh"
#include "HistoManager.hh"
#include <random>
#include <functional>

class G4Run;
class HistoManager;

/// Run action class
///
/// In EndOfRunAction(), it calculates the dose in the selected volume
/// from the energy deposit accumulated via stepping and event actions.
/// The computed dose is then printed on the screen.

class B1RunAction : public G4UserRunAction
{
  public:
    B1RunAction(HistoManager*);
    virtual ~B1RunAction();

    // virtual G4Run* GenerateRun();
    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);
    void AddEdep (G4double edep);
    void AddEdep_Jena (G4double edep_Jena);
    void AddEdep_QUB (G4double edep_QUB);
    void AddEdep_DESY (G4double edep_DESY);
    void AddEdep_HDual (G4double edep_HDual);
    void AddEdep_VDual (G4double edep_VDual);
    double drawNorm() { return normrnd(); };

  private:
    G4Parameter<G4double> fEdep;
    G4Parameter<G4double> fEdep2;
    G4Parameter<G4double> fEdep_Jena;
    G4Parameter<G4double> fEdep2_Jena;
    G4Parameter<G4double> fEdep_QUB;
    G4Parameter<G4double> fEdep2_QUB;
    G4Parameter<G4double> fEdep_DESY;
    G4Parameter<G4double> fEdep2_DESY;
    G4Parameter<G4double> fEdep_HDual;
    G4Parameter<G4double> fEdep2_HDual;
    G4Parameter<G4double> fEdep_VDual;
    G4Parameter<G4double> fEdep2_VDual;
    HistoManager* histoManager;

    // Global random number generator
    std::default_random_engine generator;
    std::normal_distribution<double> distribution;
    std::function<double()> normrnd;
};

#endif
