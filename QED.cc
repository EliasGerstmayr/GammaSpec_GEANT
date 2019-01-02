//#define G4MULTITHREADED

#include "B1DetectorConstruction.hh"
#include "B1ActionInitialization.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "FTFP_BERT.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "Randomize.hh"
#include "time.h"

#ifndef USERVARIABLES_HH
#define USERVARIABLES_HH
#include "UserVariables.hh"
#endif

#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TTree.h"
#include "TObject.h"
#include "TCanvas.h"
#include "TApplication.h"

int main(int argc,char** argv)
{
  // Detect interactive mode (if no arguments) and define UI session
  //
  G4UIExecutive* ui = 0;
  if ( argc == 1 ) {
    ui = new G4UIExecutive(argc, argv);
  }

  // Choose the Random engine
  G4Random::setTheEngine(new CLHEP::RanecuEngine);
  G4long seed = time(NULL);
  CLHEP::HepRandom::setTheSeed(seed);

  // Construct the default run manager
  //
#ifdef G4MULTITHREADED
  G4MTRunManager* runManager = new G4MTRunManager;
#else
  G4RunManager* runManager = new G4RunManager;
#endif

  // Set mandatory initialization classes
  //
  // Detector construction
  runManager->SetUserInitialization(new B1DetectorConstruction());

  // Physics list
  G4VModularPhysicsList* physicsList = new FTFP_BERT;
  physicsList->SetVerboseLevel(100);
  runManager->SetUserInitialization(physicsList);

  // Copying Kris' histomanager
  HistoManager* histoManager = new HistoManager();

  // User action initialization
  runManager->SetUserInitialization(new B1ActionInitialization(histoManager));

  // Initialize visualization
  //
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  // Time application
  clock_t t1,t2;
  t1 = clock();

  // Process macro or start UI session
  //
  if ( ! ui ) {
    // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  }
  else {
    // interactive mode
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
  }

  t2 = clock();
  float diff ((float)t2-(float)t1);
  G4cout << "Time elapsed = " << diff/1e6 << "seconds." << G4endl;

  delete visManager;
  delete runManager;

  if (plot){
    TApplication rootApp("App", 0, 0);
    TFile* f = new TFile(histoManager->GetFileName());
    TObject* H = f->Get("GunEnergy");
    TH2D* CsI = (TH2D*)f->Get("CsIEnergy");
    TTree *TrackerGamma = (TTree*)f->Get("TrackerGamma");
    TTree *TrackerElec = (TTree*)f->Get("TrackerElec");

    TCanvas *c1 = new TCanvas("c1","Electron initial energies",200,10,700,500);
    H->Draw();
    c1->Update();

    TCanvas *c2 = new TCanvas("c2","Electron positions",900,10,700,500);
    TrackerElec->Draw("x:y:e", "", "colz");
    c2->Update();

    TCanvas *c3 = new TCanvas("c3", "Electron spectrum",900,510,700,500);
    TrackerElec->Draw("e");
    c3->Update();

    TCanvas *c4 = new TCanvas("c4","Gamma spectrum",1600,510,700,500);
    TrackerGamma->Draw("e");
    c4->SetLogy();
    c4->Update();

    TCanvas *c5 = new TCanvas("c5","Gamma positions",1600,10,700,500);
    TrackerGamma->Draw("x:y:e", "", "colz");
    c5->Update();


    rootApp.Run();
  }

}
