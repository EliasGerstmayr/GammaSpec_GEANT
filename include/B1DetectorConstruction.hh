#ifndef B1DetectorConstruction_h
#define B1DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

#ifndef USERVARIABLES_HH
#define USERVARIABLES_HH
#include "UserVariables.hh"
#endif

class G4VPhysicalVolume;

class B1DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    B1DetectorConstruction();
    virtual ~B1DetectorConstruction();

    virtual G4VPhysicalVolume* Construct();

    G4VPhysicalVolume* GetScoringVolume() const { return fScoringVolume; }
    G4VPhysicalVolume* GetRRVolume() const { return rrVolume; }
    G4VPhysicalVolume*  crystals[Ncrystalsy * Ncrystalsz]; // stack oriented in z, crystals long side in x
    G4VPhysicalVolume*  crystals_Jena[Ncrystalsx_Jena * Ncrystalsy_Jena]; // profile, so long side in z, rows in x and y
    G4VPhysicalVolume*  crystals_QUB[Ncrystalsx_QUB * Ncrystalsy_QUB];
    G4VPhysicalVolume*  crystals_DESY[Ncrystalsx_DESY * Ncrystalsy_DESY];
    G4VPhysicalVolume*  crystals_HDual[Ncrystalsy_Dual * Ncrystalsz_HDual];
    G4VPhysicalVolume*  crystals_VDual[Ncrystalsx_Dual * Ncrystalsz_VDual];

    G4VPhysicalVolume*  vacscreens[Nvacscreens];

  protected:
    G4VPhysicalVolume*  fScoringVolume;
    G4VPhysicalVolume*  rrVolume;

};

#endif
