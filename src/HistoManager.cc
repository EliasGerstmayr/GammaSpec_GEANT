#include "HistoManager.hh"
#include "HistoMessenger.hh"
#include "G4UnitsTable.hh"
#include "G4ios.hh"
#include <ctime>
#include <vector>

#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TProfile.h"
#include "TTree.h"
#include "TObjArray.h"
#include "TTree.h"

#ifndef USERVARIABLES_HH
#define USERVARIABLES_HH
#include "UserVariables.hh"
#endif

//Initialising the static data types
SH HistoManager::tracking;
EN HistoManager::targetenergy;
SH HistoManager::directions;

HistoManager::HistoManager():factoryOn(false)
{
	// histograms initialising, setting everything to zero
	gunEnHist = 0;
	histoMessenger = new HistoMessenger(this);
	fileName = "";
}

HistoManager::~HistoManager()
{
	delete histoMessenger;
}

G4String HistoManager::MakeFileName(){
	time_t rawtime; //Setting filenames with time and date.
	char date[40], name[50];
	date[0] = '\0';
	rawtime = time(NULL);
	strftime(date, 40, "%H%M%S_%d%b%Y", gmtime(&rawtime));
	savef = date;
	// Temporary constant datafile name
	strcpy(name, "data//");
	strcat(name, date);
	strcat(name, ".root");
	return name;
}

void HistoManager::UpdateFileName(G4String newname) {
	time_t rawtime;
	char date[40], name[50];
	date[0] = '\0';
	rawtime = time(NULL);
	strftime(date, 40, "%H%M%S_%d%b%Y", gmtime(&rawtime));
	strcpy(date, newname);
	strcpy(name, "data//");
	strcat(name, date);
	strcat(name, ".root");
	fileName = name;

	if(debug) {
		G4cout << "Filename updated to " << newname << G4endl;
	}

}

void HistoManager::book()
{
	if (fileName.length() < 1){
		fileName = MakeFileName();//filename with time and date, saved into folder \data
	}


	factoryOn = true;
	DataList = new TObjArray; //create a collection of root objects

	// Particle gun energies
	//										Name					Title			Nbins Lower   Upper
	gunEnHist = new TH1D("GunEnergy", "GunEnergy", 100, 0.*MeV, 2200.*MeV);
	DataList->Add(gunEnHist);
	// CsI crystal energies
	// Bins like linspace(x0, x1, Nx) etc				 Nx          x0   x1               Ny           y0   y1
	csiHist = new TH2D("CsIEnergy", "CsIEnergy", Ncrystalsy, -0.5, Ncrystalsy-0.5, Ncrystalsz ,-0.5, Ncrystalsz-0.5);
	if(includespectrometer){
		DataList->Add(csiHist);
}

	csiHist_Jena = new TH2D("CsIEnergy_Jena", "CsIEnergy_Jena", Ncrystalsy_Jena, -0.5, Ncrystalsy_Jena-0.5, Ncrystalsx_Jena ,-0.5, Ncrystalsx_Jena-0.5);
	if(includeprofilestack_Jena){
		DataList->Add(csiHist_Jena);
}

	csiHist_DESY = new TH2D("CsIEnergy_DESY", "CsIEnergy_DESY", Ncrystalsy_DESY, -0.5, Ncrystalsy_DESY-0.5, Ncrystalsx_DESY ,-0.5, Ncrystalsx_DESY-0.5);
	if(includeprofilestack_DESY){
		DataList->Add(csiHist_DESY);
}

	csiHist_HDual = new TH2D("CsIEnergy_HDual", "CsIEnergy_HDual", Ncrystalsy_Dual, -0.5, Ncrystalsy_Dual-0.5, Ncrystalsz_HDual ,-0.5, Ncrystalsz_HDual-0.5);
	csiHist_VDual = new TH2D("CsIEnergy_VDual", "CsIEnergy_VDual", Ncrystalsx_Dual, -0.5, Ncrystalsx_Dual-0.5, Ncrystalsz_VDual ,-0.5, Ncrystalsz_VDual-0.5);

	if(includedualaxisspectrometer){
		DataList->Add(csiHist_HDual)
		DataList->Add(csiHist_VDual)
	}
	//Tracker data tree - gammas
	trackergamma = new TTree("TrackerGamma","Tracker hit positions");
	trackergamma->Branch("Tracker", &tracking,"x/D:y:z:px:py:pz:e");
	//Tracker data tree - electrons
	trackerelec = new TTree("TrackerElec","Tracker hit positions");
	trackerelec->Branch("Tracker", &tracking,"x/D:y:z:px:py:pz:e");
	//Energy saved in target tree
	taren = new TTree("A","Energy deposited in the target");
	taren->Branch("Absorption", &targetenergy, "Energy/D:Event/I");
	//Tree to measure the shower profile.
	dirs = new TTree("D","Shower energy depisition locations");
  dirs->Branch("Deposit", &directions, "px/D:py:pz:Edep:xP/I:yP");
}

void HistoManager::save()
{
	if (factoryOn) {
		TFile* OutPutfile = new TFile(fileName,"RECREATE"); //open .root file
		DataList->Write();
		trackergamma->Write();
		trackerelec->Write();
		OutPutfile->Close();

	}
	factoryOn = false;
	G4cout << "Saved to " << fileName << G4endl;
}

void HistoManager::FillHisto(TH1D* hist, G4double e, G4double weight)
{
		hist->Fill(e, weight);
}

void HistoManager::FillHisto(TH2D* hist, G4int i, G4int j, G4double weight)
{
		hist->Fill(i, j, weight);
}

void HistoManager::FillTrackHit(G4double x, G4double y, G4double z, G4double px, G4double py, G4double pz, G4double e, G4String partname){

	tracking.x = x;
	tracking.y = y;
	tracking.z = z;
	tracking.px = px;
	tracking.py = py;
	tracking.pz = pz;
	tracking.e = e;

	if (partname == "gamma"){
			trackergamma->Fill();
	} else if (partname == "e-"){
			trackerelec->Fill();
	}

	if(debug) {std::cout << partname << " passing into CsI stack." << std::endl;}

}

void HistoManager::FillEnergy(G4double En, G4int evt){
	targetenergy.Energy = En;
	targetenergy.event = evt;
	taren->Fill();
}

void HistoManager::FillDirections(G4ThreeVector dir, G4double edep, G4int xp, G4int yp){
	directions.x = dir.x();
	directions.y = dir.y();
	directions.z = dir.z();
	directions.e = edep;
	dirs->Fill();
}
