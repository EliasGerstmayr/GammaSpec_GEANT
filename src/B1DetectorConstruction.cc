

#include "B1DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Polycone.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Material.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4Transform3D.hh"
#include "G4VisAttributes.hh"

#include "MagneticField.hh"
#include "G4FieldManager.hh"
#include "G4EqMagElectricField.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4MagIntegratorDriver.hh"
#include "G4ClassicalRK4.hh"
#include "G4CashKarpRKF45.hh"
#include "G4NystromRK4.hh"
#include "G4ChordFinder.hh"
#include "G4Mag_UsualEqRhs.hh"

#include "G4RotationMatrix.hh"
#include "CADMesh.hh"

B1DetectorConstruction::B1DetectorConstruction(): G4VUserDetectorConstruction(),fScoringVolume(0)
{ }

B1DetectorConstruction::~B1DetectorConstruction()
{ }

G4VPhysicalVolume* B1DetectorConstruction::Construct()
{
  // Useful variables
  G4ThreeVector pos;

  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();

  // World
  G4double world_sizeXY = 500 * cm;
  G4double world_sizeZ  = 500 * cm;

  // World material
  //Vacuum
	G4Material* Vacuum = new G4Material("Vacuum",
                                      1.,
                                      1.01*g/mole,
                                      universe_mean_density,
                                      kStateGas,
                                      2.73*kelvin,
                                      3.e-18*pascal);

  // Elements
  G4Element* Pb = nist->FindOrBuildElement(82);
  G4Element* Cs = nist->FindOrBuildElement(55);
  G4Element* I  = nist->FindOrBuildElement(53);
  G4Element* Tl = nist->FindOrBuildElement(81);
  G4Element* Al = nist->FindOrBuildElement(13);
  G4Element* Ni = nist->FindOrBuildElement(28);
  G4Element* Cr = nist->FindOrBuildElement(24);
  G4Element* Fe = nist->FindOrBuildElement(26);
  G4Element* Mn = nist->FindOrBuildElement(25);
  G4Element* Si = nist->FindOrBuildElement(14);
  G4Element* C  = nist->FindOrBuildElement(12);
  G4Element* H  = nist->FindOrBuildElement(1);
  G4Element* O  = nist->FindOrBuildElement(8);
  G4Element* N  = nist->FindOrBuildElement(7);
  G4Element* Nd  = nist->FindOrBuildElement(60);
  G4Element* B  = nist->FindOrBuildElement(5);
  G4Element* Ti = nist->FindOrBuildElement("Ti");
  G4Element* Bi = nist->FindOrBuildElement("Bi");

  //                                        Density  # components
  G4Material* Lead = new G4Material("Lead", 11.35*g/cm3, 1);
  //              (Element, Natoms)
	Lead->AddElement(Pb, 1);

  G4Material* CsI = new G4Material("CaesiumIodide", 4.51*g/cm3, 2);
	CsI->AddElement(Cs, 1);
	CsI->AddElement(I, 1);

  G4Material* FusedSilica = new G4Material("FusedSilica", 2.2*g/cm3, 2);
	FusedSilica->AddElement(Si, 1);
	FusedSilica->AddElement(O, 2);

  G4Material* CsI_Tl = new G4Material("CaesiumIodideThallium", 4.51*g/cm3, 2);
	CsI_Tl->AddMaterial(CsI, 99.6*perCent);
	CsI_Tl->AddElement(Tl, 0.4*perCent);

  G4Material* Aluminium = new G4Material("Aluminium", 2.7*g/cm3, 1);
  Aluminium->AddElement(Al, 1);

  G4Material* Bismuth = new G4Material("Bismuth", 9.78*g/cm3, 1);
  Bismuth->AddElement(Bi, 1);

  G4Material* SS316 = new G4Material("Stainless Steel 316", 8.06*g/cm3, 6);
  SS316->AddElement(C,  0.001);
	SS316->AddElement(Si, 0.007);
	SS316->AddElement(Cr, 0.18);
	SS316->AddElement(Mn, 0.01);
	SS316->AddElement(Fe, 0.712);
	SS316->AddElement(Ni, 0.09);

  G4Material* Kapton = new G4Material("Kapton", 1.42*g/cm3, 4);
  // Below is from phsyics.nist.gov/cgi-bin/Star/compos.pl?matno=179 aka ASTAR database
  Kapton->AddElement(H, 0.026362);
  Kapton->AddElement(C, 0.691133);
  Kapton->AddElement(O, 0.073270);
  Kapton->AddElement(N, 0.209235);

  G4Material* Iron = new G4Material("Iron", 7.87*g/cm3, 1);
	Iron->AddElement(Fe, 1);

  G4Material* NdMagnet = new G4Material("NdMagnet", 7.4*g/cm3, 3);
	NdMagnet->AddElement(Nd, 2);
	NdMagnet->AddElement(Fe, 14);
  NdMagnet->AddElement(B, 1);

  G4Material* Polycarbonate = new G4Material("Polycarbonate", 1.2*g/cm3, 3);
  // Taken from https://physics.nist.gov/cgi-bin/Star/compos.pl?matno=219 for Polycarbonate MAKROLON Lexan (Elias, Dec 2018)
  Polycarbonate->AddElement(H, 0.055491);
  Polycarbonate->AddElement(C, 0.755751);
  Polycarbonate->AddElement(O, 0.188758);

  G4Material* TiO2 = new G4Material("Titanium Dioxide", 4.26*g/cm3, 2);
  // Taken from https://physics.nist.gov/cgi-bin/Star/compos.pl?matno=265 for Titanium Dioxide (Elias, Dec 2018)
  TiO2->AddElement(O, 0.400592);
  TiO2->AddElement(Ti, 0.599408);

  // Create world
  G4Box* solidWorld = new G4Box("World", 0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);

  G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, Vacuum, "World");

  G4VPhysicalVolume* physWorld =
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking

  // Create solid, logical and physical volumes

  G4VisAttributes* cyanColour = new G4VisAttributes(G4Colour(0.,1.,1.));
  G4VisAttributes* redColour = new G4VisAttributes(G4Colour(1.,0.,0.));
  G4VisAttributes* yellowColour = new G4VisAttributes(G4Colour(1.,1.,0.));


  // Measurement Screens
  G4Box* solidVacScreen = new G4Box("VacScreen", 0.5*screensize_x, 0.5*screensize_y, .1*mm);
  G4LogicalVolume* logicVacScreen = new G4LogicalVolume(solidVacScreen, Vacuum, "VacScreen");
  logicVacScreen->SetVisAttributes(yellowColour);
  G4VPhysicalVolume* physVacScreen;

  // CsI crystals for RAL stack (2015)
  G4Box* solidCrystal = new G4Box("Crystal", 0.5*crystalx, 0.5*crystaly, 0.5*crystalz);
  G4LogicalVolume* logicCrystal = new G4LogicalVolume(solidCrystal, CsI, "Crystal");
  logicCrystal->SetVisAttributes(cyanColour);
  G4VPhysicalVolume* physCrystal;

  // CsI crystals for Jena profile stack (2018)
  G4Box* solidCrystal_Jena = new G4Box("Crystal_Jena", 0.5*crystalx_Jena, 0.5*crystaly_Jena, 0.5*crystalz_Jena);
  G4LogicalVolume* logicCrystal_Jena = new G4LogicalVolume(solidCrystal_Jena, CsI, "Crystal_Jena");
  logicCrystal_Jena->SetVisAttributes(cyanColour);
  G4VPhysicalVolume* physCrystal_Jena;

  // CsI crystals for DESY profile stack (2018)
  G4Box* solidCrystal_DESY = new G4Box("Crystal_DESY", 0.5*crystalx_DESY, 0.5*crystaly_DESY, 0.5*crystalz_DESY);
  G4LogicalVolume* logicCrystal_DESY = new G4LogicalVolume(solidCrystal_DESY, CsI, "Crystal_DESY");
  logicCrystal_DESY->SetVisAttributes(cyanColour);
  G4VPhysicalVolume* physCrystal_DESY;

  // CsI crystals for QUB profile stack (2018 and earlier)
  G4Box* solidCrystal_QUB = new G4Box("Crystal_QUB", 0.5*crystalx_QUB, 0.5*crystaly_QUB, 0.5*crystalz_QUB);
  G4LogicalVolume* logicCrystal_QUB = new G4LogicalVolume(solidCrystal_QUB, CsI, "Crystal_QUB");
  logicCrystal_QUB->SetVisAttributes(cyanColour);
  G4VPhysicalVolume* physCrystal_QUB;


  // CsI crystals for 2 axis spectral stack (2018)
  // Horizontal
  G4Box* solidCrystal_HDual = new G4Box("Crystal_HDual", 0.5*crystalx_Dual, 0.5*crystaly_Dual, 0.5*crystalz_Dual);
  G4LogicalVolume* logicCrystal_HDual = new G4LogicalVolume(solidCrystal_HDual, CsI, "Crystal_HDual");
  logicCrystal_HDual->SetVisAttributes(cyanColour);
  G4VPhysicalVolume* physCrystal_HDual;
  //Vertical, x and y are interchanged
  G4Box* solidCrystal_VDual = new G4Box("Crystal_VDual", 0.5*crystaly_Dual, 0.5*crystalx_Dual, 0.5*crystalz_Dual);
  G4LogicalVolume* logicCrystal_VDual = new G4LogicalVolume(solidCrystal_VDual, CsI, "Crystal_VDual");
  logicCrystal_VDual->SetVisAttributes(cyanColour);
  G4VPhysicalVolume* physCrystal_VDual;

  // TiO2 coating horizontal and vertical layers for profile stacks

  // Jena
  G4double totallengthx_Jena = (Ncrystalsx_Jena - 1)*crystalspacing_Jena + crystalx_Jena;
  G4double totallengthy_Jena = (Ncrystalsy_Jena - 1)*crystalspacing_Jena + crystaly_Jena;
  // horizontal coating
  G4Box* solidHCoating_Jena = new G4Box("HCoating_Jena", 0.5*totallengthx_Jena, 0.5*coatingthickness_Jena, 0.5*crystalz_Jena);
  G4LogicalVolume* logicHCoating_Jena = new G4LogicalVolume(solidHCoating_Jena, TiO2, "HCoating_Jena");
  G4VPhysicalVolume* physHCoating_Jena;
  // vertical coating
  G4Box* solidVCoating_Jena = new G4Box("VCoating_Jena", 0.5*coatingthickness_Jena, 0.5*totallengthy_Jena, 0.5*crystalz_Jena);
  G4LogicalVolume* logicVCoating_Jena = new G4LogicalVolume(solidVCoating_Jena, TiO2, "VCoating_Jena");
  G4VPhysicalVolume* physVCoating_Jena;

  // DESY
  G4double totallengthx_DESY = (Ncrystalsx_DESY - 1)*crystalspacing_DESY + crystalx_DESY;
  G4double totallengthy_DESY = (Ncrystalsy_DESY - 1)*crystalspacing_DESY + crystaly_DESY;
  // horizontal coating
  G4Box* solidHCoating_DESY = new G4Box("HCoating_DESY", 0.5*totallengthx_DESY, 0.5*coatingthickness_DESY, 0.5*crystalz_DESY);
  G4LogicalVolume* logicHCoating_DESY = new G4LogicalVolume(solidHCoating_DESY, TiO2, "HCoating_DESY");
  G4VPhysicalVolume* physHCoating_DESY;
  // vertical coating
  G4Box* solidVCoating_DESY = new G4Box("VCoating_DESY", 0.5*coatingthickness_DESY, 0.5*totallengthy_DESY, 0.5*crystalz_DESY);
  G4LogicalVolume* logicVCoating_DESY = new G4LogicalVolume(solidVCoating_DESY, TiO2, "VCoating_DESY");
  G4VPhysicalVolume* physVCoating_DESY;

  // QUB
  G4double totallengthx_QUB = (Ncrystalsx_QUB - 1)*crystalspacing_QUB + crystalx_QUB;
  G4double totallengthy_QUB = (Ncrystalsy_QUB - 1)*crystalspacing_QUB + crystaly_QUB;
  // horizontal coating
  G4Box* solidHCoating_QUB = new G4Box("HCoating_QUB", 0.5*totallengthx_QUB, 0.5*coatingthickness_QUB, 0.5*crystalz_QUB);
  G4LogicalVolume* logicHCoating_QUB = new G4LogicalVolume(solidHCoating_QUB, Aluminium, "HCoating_QUB");
  G4VPhysicalVolume* physHCoating_QUB;
  // vertical coating
  G4Box* solidVCoating_QUB = new G4Box("VCoating_DESY", 0.5*coatingthickness_QUB, 0.5*totallengthy_QUB, 0.5*crystalz_QUB);
  G4LogicalVolume* logicVCoating_QUB = new G4LogicalVolume(solidVCoating_QUB, Aluminium, "VCoating_QUB");
  G4VPhysicalVolume* physVCoating_QUB;


  // Aluminium dividers, horizontal and vertical for RAL stack (2015)
  G4double totallengthz = (Ncrystalsz - 1)*crystalspacing + crystalz;
  G4double totallengthy = (Ncrystalsy - 1)*crystalspacing + crystaly;
  G4Box* solidHDivider = new G4Box("HDivider", 0.5*crystalx, 0.5*dividerthickness, 0.5*totallengthz);
  G4LogicalVolume* logicHDivider = new G4LogicalVolume(solidHDivider, Aluminium, "HDivider");
  G4VPhysicalVolume* physHDivider;

  G4Box* solidVDivider = new G4Box("VDivider", 0.5*crystalx, 0.5*totallengthy, 0.5*dividerthickness);
  G4LogicalVolume* logicVDivider = new G4LogicalVolume(solidVDivider, Aluminium, "VDivider");
  G4VPhysicalVolume* physVDivider;

  // Dual axis stack - dividers and spacers
  G4double totallengthy_Dual = (Ncrystalsy_Dual - 1)*crystalspacing_Dual + crystaly_Dual;
  G4double totallengthx_Dual = totallengthy_Dual;

  // Rubber spacers in longitudinal direction, for now use Polycarbonate
  G4Box* solidLDivider_Dual = new G4Box("LDivider_Dual", 0.5*totallengthx_Dual, 0.5*totallengthy_Dual, 0.5*dividerthickness_LDual);
  G4LogicalVolume* logicLDivider_Dual = new G4LogicalVolume(solidLDivider_Dual, Polycarbonate, "LDivider_Dual");
  G4VPhysicalVolume* physLDivider_Dual;

  // Nylon spacers in horizontal direction, for now use Polycarbonate
  G4Box* solidHDivider_Dual = new G4Box("HDivider_Dual", 0.5*totallengthx_Dual, 0.5*dividerthickness_TDual, 0.5*crystalz_Dual);
  G4LogicalVolume* logicHDivider_Dual = new G4LogicalVolume(solidHDivider_Dual, Polycarbonate, "HDivider_Dual");
  G4VPhysicalVolume* physHDivider_Dual;

  // Nylon spacers in horizontal direction, for now use Polycarbonate
  G4Box* solidVDivider_Dual = new G4Box("VDivider_Dual", 0.5*dividerthickness_TDual, 0.5*totallengthy_Dual, 0.5*crystalz_Dual);
  G4LogicalVolume* logicVDivider_Dual = new G4LogicalVolume(solidVDivider_Dual, Polycarbonate, "VDivider_Dual");
  G4VPhysicalVolume* physVDivider_Dual;

  // Stainless steel front plate
  // simply change the material here in the code to see what the different front plate would do

  G4Box* solidFrontPlate_plastic = new G4Box("Front plate plastic", 0.5*crystalx, 0.5*totallengthy, 0.5*frontplatethickness);
  G4LogicalVolume* logicFrontPlate_plastic = new G4LogicalVolume(solidFrontPlate_plastic, Polycarbonate, "Front Plate plastic");
  G4VPhysicalVolume* physFrontPlate_plastic;


  G4Box* solidFrontPlate = new G4Box("Front plate", 0.5*crystalx, 0.5*totallengthy, 0.5*frontplatethickness);
  G4LogicalVolume* logicFrontPlate = new G4LogicalVolume(solidFrontPlate, SS316, "Front Plate");
  G4VPhysicalVolume* physFrontPlate;

  G4Box* solidDetFilter = new G4Box("DetFilter", 0.5*crystalx, 0.5*totallengthy, 0.5*detfilter_thickness);
  G4LogicalVolume* logicDetFilter = new G4LogicalVolume(solidDetFilter, SS316, "Det Filter");
  G4VPhysicalVolume* physDetFilter;

  //Titanium Dioxide 'front plate'/reflector coating for the profile stacks (0.5mm TiO2)
  //Jena
  G4Box* solidFrontPlate_Jena = new G4Box("Front plate Jena", 0.5*totallengthx_Jena, 0.5*totallengthy_Jena, 0.5*frontplatethickness_Jena);
  G4LogicalVolume* logicFrontPlate_Jena = new G4LogicalVolume(solidFrontPlate_Jena, TiO2, "Front Plate Jena");
  G4VPhysicalVolume* physFrontPlate_Jena;
  //DESY
  G4Box* solidFrontPlate_DESY = new G4Box("Front plate DESY", 0.5*totallengthx_DESY, 0.5*totallengthy_DESY, 0.5*frontplatethickness_DESY);
  G4LogicalVolume* logicFrontPlate_DESY = new G4LogicalVolume(solidFrontPlate_DESY, TiO2, "Front Plate DESY");
  G4VPhysicalVolume* physFrontPlate_DESY;
  //QUB
  G4Box* solidFrontPlate_QUB = new G4Box("Front plate QUB", 0.5*totallengthx_QUB, 0.5*totallengthy_QUB, 0.5*frontplatethickness_QUB);
  G4LogicalVolume* logicFrontPlate_QUB = new G4LogicalVolume(solidFrontPlate_QUB, Aluminium, "Front Plate QUB");
  G4VPhysicalVolume* physFrontPlate_QUB;

  // Dual axis stack plates
  // Aluminium (2mm) front plate for Dual axis stack
  G4Box* solidFrontPlate_Dual = new G4Box("Front plate Dual", 0.5*totallengthx_Dual, 0.5*totallengthy_Dual, 0.5*frontplatethickness_Dual);
  G4LogicalVolume* logicFrontPlate_Dual = new G4LogicalVolume(solidFrontPlate_Dual, Aluminium, "Front Plate Dual");
  G4VPhysicalVolume* physFrontPlate_Dual;

  // Aluminium (32 mm) back plate for Dual axis stack
  G4Box* solidBackPlate_Dual = new G4Box("Back plate Dual", 0.5*totallengthx_Dual, 0.5*totallengthy_Dual, 0.5*backplatethickness_Dual);
  G4LogicalVolume* logicBackPlate_Dual = new G4LogicalVolume(solidBackPlate_Dual, Aluminium, "Back Plate Dual");
  G4VPhysicalVolume* physBackPlate_Dual;

  // Kapton window
  G4Box* solidKaptonWindow = new G4Box("Kapton Window", 0.51*kaptonwidth, 0.51*kaptonwidth, 0.51*kaptonthickness);
  G4LogicalVolume* logicKaptonWindow = new G4LogicalVolume(solidKaptonWindow, Kapton, "Kapton Window");
  G4VPhysicalVolume* physKaptonWindow;

  // Al foil
  G4Box* solidAluminiumFoil = new G4Box("Aluminium Foil", 0.51*aluminiumwidth, 0.51*aluminiumwidth, 0.51*alumniniumthickness);
  G4LogicalVolume* logicAluminiumFoil = new G4LogicalVolume(solidAluminiumFoil, Aluminium, "Aluminium Foil");
  G4VPhysicalVolume* physAluminiumFoil;

  // Plastic converter
  G4Box* solidPlasticConverter = new G4Box("Plastic Converter", 0.5*plasticconverterwidth, 0.5*plasticconverterwidth, 0.5*plasticconverterthickness);
  G4LogicalVolume* logicPlasticConverter = new G4LogicalVolume(solidPlasticConverter, Polycarbonate, "Plastic Converter");
  G4VPhysicalVolume* physPlasticConverter;

  // Iron converter
  G4Box* solidIronConverter = new G4Box("Iron Converter", 0.5*ironconverterwidth, 0.5*ironconverterwidth, 0.5*ironconverterthickness);
  G4LogicalVolume* logicIronConverter = new G4LogicalVolume(solidIronConverter, Iron, "Iron Converter");
  G4VPhysicalVolume* physIronConverter;

  // Bismuth converter
  G4Box* solidBismuthConverter = new G4Box("Bismuth Converter", 0.5*bismuthconverterwidth, 0.5*bismuthconverterwidth, 0.5*bismuthconverterthickness);
  G4LogicalVolume* logicBismuthConverter = new G4LogicalVolume(solidBismuthConverter, Bismuth, "Bismuth Converter");
  G4VPhysicalVolume* physBismuthConverter;

  // Lead shielding
  G4Box* solidLeadShield = new G4Box("Lead Shield No Hole", 30*cm, 30*cm, 7.5*cm);
  G4Tubs* leadhole = new G4Tubs("Leadhole",   // Name
                                 0,           // Inner radius
                                 2*cm,        // Outer radius
                                 20*cm,       // Z half length
                                 0*deg,       // Starting angle
                                 360*deg);    // Segment angle
  G4SubtractionSolid* solidLeadShieldHole = new G4SubtractionSolid("Lead Shield", solidLeadShield, leadhole);
  G4LogicalVolume* logicLeadShield = new G4LogicalVolume(solidLeadShieldHole, Lead, "Lead Shield");
  G4VPhysicalVolume* physLeadShield;

  // Place all volumes
  G4double x = 0.,y,z;


  if (includespectrometer){

    // Place crystals
    G4int crystalind = 0;
    for (int i = 0; i < Ncrystalsy; i++ ) {
      for (int j = 0; j < Ncrystalsz; j++) {
        y = 0.5*((Ncrystalsy-1) * crystalspacing) - i * crystalspacing;
        z = -(firstcrystalz + j * crystalspacing);
        pos = G4ThreeVector(x, y, z);
        crystalind = i * Ncrystalsz + j;
        physCrystal =   new G4PVPlacement(0,                // Rotation
                                          pos,              // Position
                                          logicCrystal,     // Logical volume
                                          "Crystal",        // Name
                                          logicWorld,       // Mother  volume
                                          false,            // No boolean operation
                                          crystalind,       // Copy number
                                          checkOverlaps);   // Overlaps checking

        crystals[crystalind] = physCrystal;
      }
    }

    // Place dividers
    if (includeDividers) {
      // Place horizontal dividers
      for (int i = 0; i < Ncrystalsy-1; i++ ) {
        x = 0;
        y = 0.5*((Ncrystalsy-1) * crystalspacing) - (i + 0.5) * crystalspacing;
        z = -(firstcrystalz + 0.5*((Ncrystalsz - 1) * crystalspacing));
        pos = G4ThreeVector(x, y, z);
        physHDivider =   new G4PVPlacement(0,                // Rotation
                                           pos,              // Position
                                           logicHDivider,    // Logical volume
                                           "HDivider",       // Name
                                           logicWorld,       // Mother  volume
                                           false,            // No boolean operation
                                           0,                // Copy number
                                           checkOverlaps);   // Overlaps checking
      }

      // Place vertical dividers
      for (int j = 0; j < Ncrystalsz-1; j++ ) {
        x = 0;
        y = 0;
        z = -(firstcrystalz + 0.5*crystalspacing + j*crystalspacing);
        pos = G4ThreeVector(x, y, z);
        physVDivider =   new G4PVPlacement(0,                // Rotation
                                           pos,              // Position
                                           logicVDivider,    // Logical volume
                                           "VDivider",       // Name
                                           logicWorld,       // Mother  volume
                                           false,            // No boolean operation
                                           0,                // Copy number
                                           checkOverlaps);   // Overlaps checking
      }
    }

    // Place front plate
    if (includefrontplate) {

      if(includeplasticfrontplate){
        x = 0;
        y = 0;
        z = -(firstcrystalz - 0.5*crystalz - 0.5*frontplatethickness - 0.1*mm);
        pos = G4ThreeVector(x, y, z);
        physFrontPlate_plastic =   new G4PVPlacement(0,              // Rotation
                                           pos,              // Position
                                           logicFrontPlate_plastic,  // Logical volume
                                           "Front Plate plastic",    // Name
                                           logicWorld,       // Mother  volume
                                           false,            // No boolean operation
                                           0,                // Copy number
                                           checkOverlaps);   // Overlaps checking
      }
      else{

        x = 0;
        y = 0;
        z = -(firstcrystalz - 0.5*crystalz - 0.5*frontplatethickness - 0.1*mm);
        pos = G4ThreeVector(x, y, z);
        physFrontPlate =   new G4PVPlacement(0,              // Rotation
                                           pos,              // Position
                                           logicFrontPlate,  // Logical volume
                                           "Front Plate",    // Name
                                           logicWorld,       // Mother  volume
                                           false,            // No boolean operation
                                           0,                // Copy number
                                           checkOverlaps);   // Overlaps checking
                                       }
    }
  }



  if (include_detfilter){


    x = 0;
    y = 0;
    z = -(detfilter_pos_z);
    pos = G4ThreeVector(x, y, z);
    physDetFilter =   new G4PVPlacement(0,              // Rotation
                                       pos,              // Position
                                       logicDetFilter,  // Logical volume
                                       "DetFilter",    // Name
                                       logicWorld,       // Mother  volume
                                       false,            // No boolean operation
                                       0,                // Copy number
                                       checkOverlaps);   // Overlaps checking



  }

  // Jena CsI stack (2018)
  // Stack is made up of 45 x 45 CsI crystals
  // Each crystal is 1x1 mm and 10 mm deep
  // Each pair of crystals and the outside is coated in 0.2 mm epoxy (?)
  // a 0.5 mm layer on the front side of epoxy
  if(includeprofilestack_Jena){

    // Place crystals
    G4int crystalind_Jena = 0;
    for (int i = 0; i < Ncrystalsy_Jena; i++ ) {
      // this centres our stack in y direction onto the laser axis
      y = 0.5*((Ncrystalsy_Jena-1) * crystalspacing_Jena) - i * crystalspacing_Jena;

      for (int j = 0; j < Ncrystalsx_Jena; j++) {
        // this centres our stack in x direction onto the laser axis
        x = 0.5*((Ncrystalsx_Jena-1) * crystalspacing_Jena) - j * crystalspacing_Jena;
        z = -(firstcrystalz_Jena);
        pos = G4ThreeVector(x, y, z);
        crystalind_Jena = i * Ncrystalsx_Jena + j;
        physCrystal_Jena =   new G4PVPlacement(0,            // Rotation
                                          pos,              // Position
                                          logicCrystal_Jena,// Logical volume
                                          "Crystal_Jena",   // Name
                                          logicWorld,       // Mother  volume
                                          false,            // No boolean operation
                                          crystalind_Jena,  // Copy number
                                          checkOverlaps);   // Overlaps checking

        crystals_Jena[crystalind_Jena] = physCrystal_Jena;
      }
    }

    // Place coatings
    if (includecoating_Jena) {
      // Place horizontal coatings
      for (int i = 0; i < Ncrystalsy_Jena+1; i++ ) {
        x = 0;
        y = 0.5*((Ncrystalsy_Jena+1) * crystalspacing_Jena) - (i + 0.5) * crystalspacing_Jena;
        z = -firstcrystalz_Jena;
        pos = G4ThreeVector(x, y, z);
        physHCoating_Jena =   new G4PVPlacement(0,            // Rotation
                                           pos,               // Position
                                           logicHCoating_Jena,// Logical volume
                                           "HCoating_Jena",  // Name
                                           logicWorld,       // Mother  volume
                                           false,            // No boolean operation
                                           0,                // Copy number
                                           checkOverlaps);   // Overlaps checking
      }

      // Place vertical dividers
      for (int j = 0; j < Ncrystalsx_Jena+1; j++ ) {
        x = 0.5*((Ncrystalsx_Jena+1) * crystalspacing_Jena) - (j + 0.5) * crystalspacing_Jena;
        y = 0;
        z = -firstcrystalz_Jena;
        pos = G4ThreeVector(x, y, z);
        physVCoating_Jena =   new G4PVPlacement(0,                // Rotation
                                           pos,                   // Position
                                           logicVCoating_Jena,    // Logical volume
                                           "VCoating_Jena",       // Name
                                           logicWorld,            // Mother  volume
                                           false,                  // No boolean operation
                                           0,                      // Copy number
                                           checkOverlaps);         // Overlaps checking
      }
    }

    // Place front plate
    if (includefrontplate_Jena) {
      x = 0;
      y = 0;
      z = -(firstcrystalz_Jena - 0.5*crystalz_Jena - 0.5*frontplatethickness_Jena - 0.1*mm);
      pos = G4ThreeVector(x, y, z);
      physFrontPlate_Jena =   new G4PVPlacement(0,              // Rotation
                                         pos,                   // Position
                                         logicFrontPlate_Jena,  // Logical volume
                                         "Front Plate Jena",    // Name
                                         logicWorld,            // Mother  volume
                                         false,                 // No boolean operation
                                         0,                     // Copy number
                                         checkOverlaps);        // Overlaps checking
    }



  }

  // DESY CsI stack (2018)
  // Stack is made up of 30 x 30 CsI crystals
  // Each crystal is 1.5x1.5 mm and 10 mm deep
  // Each pair of crystals and the outside is coated in 0.2 mm epoxy (?)
  // there is a 0.5 mm layer of epoxy as reflector
  if(includeprofilestack_DESY){

    // Place crystals
    G4int crystalind_DESY = 0;
    for (int i = 0; i < Ncrystalsy_DESY; i++ ) {
      // this centres our stack in y direction onto the laser axis
      y = 0.5*((Ncrystalsy_DESY-1) * crystalspacing_DESY) - i * crystalspacing_DESY;

      for (int j = 0; j < Ncrystalsx_DESY; j++) {
        // this centres our stack in x direction onto the laser axis
        x = 0.5*((Ncrystalsx_DESY-1) * crystalspacing_DESY) - j * crystalspacing_DESY;
        z = -(firstcrystalz_DESY);
        pos = G4ThreeVector(x, y, z);
        crystalind_DESY = i * Ncrystalsx_DESY + j;
        physCrystal_DESY =   new G4PVPlacement(0,            // Rotation
                                          pos,              // Position
                                          logicCrystal_DESY,// Logical volume
                                          "Crystal_DESY",   // Name
                                          logicWorld,       // Mother  volume
                                          false,            // No boolean operation
                                          crystalind_DESY,  // Copy number
                                          checkOverlaps);   // Overlaps checking

        crystals_DESY[crystalind_DESY] = physCrystal_DESY;
      }
    }

    // Place coatings
    if (includecoating_DESY) {
      // Place horizontal coatings
      for (int i = 0; i < Ncrystalsy_DESY+1; i++ ) {
        x = 0;
        y = 0.5*((Ncrystalsy_DESY+1) * crystalspacing_DESY) - (i + 0.5) * crystalspacing_DESY;
        z = -firstcrystalz_DESY;
        pos = G4ThreeVector(x, y, z);
        physHCoating_DESY =   new G4PVPlacement(0,            // Rotation
                                           pos,               // Position
                                           logicHCoating_DESY,// Logical volume
                                           "HCoating_DESY",  // Name
                                           logicWorld,       // Mother  volume
                                           false,            // No boolean operation
                                           0,                // Copy number
                                           checkOverlaps);   // Overlaps checking
      }

      // Place vertical dividers
      for (int j = 0; j < Ncrystalsx_DESY+1; j++ ) {
        x = 0.5*((Ncrystalsx_DESY+1) * crystalspacing_DESY) - (j + 0.5) * crystalspacing_DESY;
        y = 0;
        z = -firstcrystalz_DESY;
        pos = G4ThreeVector(x, y, z);
        physVCoating_DESY =   new G4PVPlacement(0,                // Rotation
                                           pos,                   // Position
                                           logicVCoating_DESY,    // Logical volume
                                           "VCoating_DESY",       // Name
                                           logicWorld,            // Mother  volume
                                           false,                  // No boolean operation
                                           0,                      // Copy number
                                           checkOverlaps);         // Overlaps checking
      }
    }


    // Place front plate
    if (includefrontplate_DESY) {
      x = 0;
      y = 0;
      z = -(firstcrystalz_DESY - 0.5*crystalz_DESY - 0.5*frontplatethickness_DESY - 0.1*mm);
      pos = G4ThreeVector(x, y, z);
      physFrontPlate_DESY =   new G4PVPlacement(0,              // Rotation
                                         pos,                   // Position
                                         logicFrontPlate_DESY,  // Logical volume
                                         "Front Plate DESY",    // Name
                                         logicWorld,            // Mother  volume
                                         false,                 // No boolean operation
                                         0,                     // Copy number
                                         checkOverlaps);        // Overlaps checking
    }


  }




  // QUB CsI stack (2018 and earlier)
  // Stack is made up of 20 x 20 CsI crystals
  // Each crystal is 2x2 mm and 20 mm deep
  // Each pair of crystals and the outside is rolled in two layers of Al foil 0.1 mm
  // a 10 mm aluminium plate is in the front
  if(includeprofilestack_QUB){

    // Place crystals
    G4int crystalind_QUB = 0;
    for (int i = 0; i < Ncrystalsy_QUB; i++ ) {
      // this centres our stack in y direction onto the laser axis
      y = 0.5*((Ncrystalsy_QUB-1) * crystalspacing_QUB) - i * crystalspacing_QUB;

      for (int j = 0; j < Ncrystalsx_QUB; j++) {
        // this centres our stack in x direction onto the laser axis
        x = 0.5*((Ncrystalsx_QUB-1) * crystalspacing_QUB) - j * crystalspacing_QUB;
        z = -(firstcrystalz_QUB);
        pos = G4ThreeVector(x, y, z);
        crystalind_QUB = i * Ncrystalsx_QUB + j;
        physCrystal_QUB =   new G4PVPlacement(0,            // Rotation
                                          pos,              // Position
                                          logicCrystal_QUB,// Logical volume
                                          "Crystal_QUB",   // Name
                                          logicWorld,       // Mother  volume
                                          false,            // No boolean operation
                                          crystalind_QUB,  // Copy number
                                          checkOverlaps);   // Overlaps checking

        crystals_QUB[crystalind_QUB] = physCrystal_QUB;
      }
    }

    // Place coatings
    if (includecoating_QUB) {
      // Place horizontal coatings
      for (int i = 0; i < Ncrystalsy_QUB+1; i++ ) {
        x = 0;
        y = 0.5*((Ncrystalsy_QUB+1) * crystalspacing_QUB) - (i + 0.5) * crystalspacing_QUB;
        z = -firstcrystalz_QUB;
        pos = G4ThreeVector(x, y, z);
        physHCoating_QUB =   new G4PVPlacement(0,            // Rotation
                                           pos,               // Position
                                           logicHCoating_QUB,// Logical volume
                                           "HCoating_QUB",  // Name
                                           logicWorld,       // Mother  volume
                                           false,            // No boolean operation
                                           0,                // Copy number
                                           checkOverlaps);   // Overlaps checking
      }

      // Place vertical dividers
      for (int j = 0; j < Ncrystalsx_QUB+1; j++ ) {
        x = 0.5*((Ncrystalsx_QUB+1) * crystalspacing_QUB) - (j + 0.5) * crystalspacing_QUB;
        y = 0;
        z = -firstcrystalz_QUB;
        pos = G4ThreeVector(x, y, z);
        physVCoating_QUB =   new G4PVPlacement(0,                // Rotation
                                           pos,                   // Position
                                           logicVCoating_QUB,    // Logical volume
                                           "VCoating_QUB",       // Name
                                           logicWorld,            // Mother  volume
                                           false,                  // No boolean operation
                                           0,                      // Copy number
                                           checkOverlaps);         // Overlaps checking
      }
    }

    // Place front plate
    if (includefrontplate_QUB) {
      x = 0;
      y = 0;
      z = -(firstcrystalz_QUB - 0.5*crystalz_QUB - 0.5*frontplatethickness_QUB - 0.1*mm);
      pos = G4ThreeVector(x, y, z);
      physFrontPlate_QUB =   new G4PVPlacement(0,              // Rotation
                                         pos,                   // Position
                                         logicFrontPlate_QUB,  // Logical volume
                                         "Front Plate QUB",    // Name
                                         logicWorld,            // Mother  volume
                                         false,                 // No boolean operation
                                         0,                     // Copy number
                                         checkOverlaps);        // Overlaps checking
    }



  }

  // RAL dual axis spectral stack (2018)
  // Stack is made up of 10 CsI crystals per layer
  // Each crystal is 5x5 mm and 50 mm deep
  // spacing, coating, spacers
  // each crystal is spaced by 1.5 mm, 1 mm nylon dividers
  // each alternating set is spaced by 1mm and divided by 1 mm rubber
  if (includedualaxisspectrometer){

    // Place crystals facing one direction (sideways)
    G4int crystalind_HDual = 0;
    x = firstcrystalx_Dual; //on-axis in x or offset if non-zero
    for (int i = 0; i < Ncrystalsz_HDual; i++ ) {
      z = -(firstcrystalz_Dual + 2 * i * crystalspacingz_Dual);
      for (int j = 0; j < Ncrystalsy_Dual; j++) {
        y = 0.5*((Ncrystalsy_Dual-1) * crystalspacing_Dual) - j * crystalspacing_Dual + firstcrystaly_Dual;
        pos = G4ThreeVector(x, y, z);
        crystalind_HDual = i * Ncrystalsy_Dual + j;
        physCrystal_HDual =   new G4PVPlacement(0,           // Rotation
                                          pos,              // Position
                                          logicCrystal_HDual,// Logical volume
                                          "Crystal_HDual",   // Name
                                          logicWorld,       // Mother  volume
                                          false,            // No boolean operation
                                          crystalind_HDual,       // Copy number
                                          checkOverlaps);   // Overlaps checking

        crystals_HDual[crystalind_HDual] = physCrystal_HDual;
      }
    }

    // Place crystals facing the other direction (vertical)
    G4int crystalind_VDual = 0;
    y = firstcrystaly_Dual; //on-axis in y
    for (int i = 0; i < Ncrystalsz_VDual; i++ ) {
      z = -(firstcrystalz_Dual + 2 * i * crystalspacingz_Dual + crystalspacingz_Dual); // first row vertical, then every second
      for (int j = 0; j < Ncrystalsx_Dual; j++) {
        x = 0.5*((Ncrystalsx_Dual-1) * crystalspacing_Dual) - j * crystalspacing_Dual + firstcrystalx_Dual;
        pos = G4ThreeVector(x, y, z);
        crystalind_VDual = i * Ncrystalsx_Dual + j;
        physCrystal_VDual =   new G4PVPlacement(0,           // Rotation
                                          pos,              // Position
                                          logicCrystal_VDual,// Logical volume
                                          "Crystal_VDual",   // Name
                                          logicWorld,       // Mother  volume
                                          false,            // No boolean operation
                                          crystalind_VDual,       // Copy number
                                          checkOverlaps);   // Overlaps checking

        crystals_VDual[crystalind_VDual] = physCrystal_VDual;
      }
    }


    // Place front plate
    if (includefrontplate_Dual) {
      x = firstcrystalx_Dual;
      y = firstcrystaly_Dual;
      z = -(firstcrystalz_Dual - 0.5*crystalz_Dual - 0.5*frontplatethickness_Dual - dividerthickness_LDual - 0.1*mm);
      pos = G4ThreeVector(x, y, z);
      physFrontPlate_Dual =   new G4PVPlacement(0,              // Rotation
                                         pos,                   // Position
                                         logicFrontPlate_Dual,  // Logical volume
                                         "Front Plate Dual",    // Name
                                         logicWorld,            // Mother  volume
                                         false,                 // No boolean operation
                                         0,                     // Copy number
                                         checkOverlaps);        // Overlaps checking
    }

    // Place back plate
    if (includebackplate_Dual) {
      x = firstcrystalx_Dual;
      y = firstcrystaly_Dual;
      z = -(firstcrystalz_Dual + (Ncrystalsz_HDual + Ncrystalsz_VDual - 1) * crystalspacingz_Dual + 0.5 * crystalz_Dual + 0.5*backplatethickness_Dual + dividerthickness_LDual + 0.1*mm);
      pos = G4ThreeVector(x, y, z);
      physBackPlate_Dual =   new G4PVPlacement(0,              // Rotation
                                         pos,                   // Position
                                         logicBackPlate_Dual,  // Logical volume
                                         "Back Plate Dual",    // Name
                                         logicWorld,            // Mother  volume
                                         false,                 // No boolean operation
                                         0,                     // Copy number
                                         checkOverlaps);        // Overlaps checking
    }

    // Include Dividers
    if (includedividers_Dual) {

      // longitudinal dividers (1 mm rubber)
      // if the horizontal and vertical arrays are not filled up from back to front, this might not be true anymore
      for (int j = 0; j < (Ncrystalsz_HDual+Ncrystalsz_VDual); j++ ) {
        x = firstcrystalx_Dual;
        y = firstcrystaly_Dual;
        z = -(firstcrystalz - 0.5*crystalspacingz_Dual + j*crystalspacingz_Dual);
        pos = G4ThreeVector(x, y, z);
        physLDivider_Dual =   new G4PVPlacement(0,           // Rotation
                                           pos,              // Position
                                           logicLDivider_Dual,// Logical volume
                                           "LDivider_Dual",  // Name
                                           logicWorld,       // Mother  volume
                                           false,            // No boolean operation
                                           0,                // Copy number
                                           checkOverlaps);   // Overlaps checking
      }

      // Place horizontal dividers for when crystals face horizontal direction
      for (int j = 0; j < Ncrystalsz_HDual; j++){
        z = -(firstcrystalz_Dual + j * 2 * crystalspacingz_Dual);

        for (int i = 0; i < Ncrystalsy_Dual-1; i++ ) {
              x = firstcrystalx_Dual;
              y = 0.5*((Ncrystalsy_Dual-1) * crystalspacing_Dual) - (i + 0.5) * crystalspacing_Dual + firstcrystaly_Dual;

              pos = G4ThreeVector(x, y, z);
              physHDivider_Dual =   new G4PVPlacement(0,                // Rotation
                                                 pos,              // Position
                                                 logicHDivider_Dual,    // Logical volume
                                                 "HDivider_Dual",       // Name
                                                 logicWorld,       // Mother  volume
                                                 false,            // No boolean operation
                                                 0,                // Copy number
                                                 checkOverlaps);   // Overlaps checking
            }

      }

      // Place vertical dividers for when crystals face vertical direction
      for (int j = 0; j < Ncrystalsz_VDual; j++){
        z = -(firstcrystalz_Dual + (j + 0.5) * 2 * crystalspacingz_Dual);

        for (int i = 0; i < Ncrystalsx_Dual-1; i++ ) {
              y = firstcrystaly_Dual;
              x = 0.5*((Ncrystalsx_Dual-1) * crystalspacing_Dual) - (i + 0.5) * crystalspacing_Dual + firstcrystalx_Dual;

              pos = G4ThreeVector(x, y, z);
              physVDivider_Dual =   new G4PVPlacement(0,                // Rotation
                                                 pos,              // Position
                                                 logicVDivider_Dual,    // Logical volume
                                                 "VDivider_Dual",       // Name
                                                 logicWorld,       // Mother  volume
                                                 false,            // No boolean operation
                                                 0,                // Copy number
                                                 checkOverlaps);   // Overlaps checking
            }

      }


    }

  }


  // Place kapton window
  if (includekaptonwindow) {
    x = 0;
    y = 0;
    z = -1.39*m;
    pos = G4ThreeVector(x, y, z);
    physFrontPlate =   new G4PVPlacement(0,                // Rotation
                                       pos,                // Position
                                       logicKaptonWindow,  // Logical volume
                                       "Kapton Window",    // Name
                                       logicWorld,         // Mother  volume
                                       false,              // No boolean operation
                                       0,                  // Copy number
                                       checkOverlaps);     // Overlaps checking
  }

  // Place aluminium foil
  if (includealuminium) {
    x = 0;
    y = 0;
    z = -1.38*m;
    pos = G4ThreeVector(x, y, z);
    physFrontPlate =   new G4PVPlacement(0,                   // Rotation
                                         pos,                 // Position
                                         logicAluminiumFoil,  // Logical volume
                                         "Aluminium Foil",    // Name
                                         logicWorld,          // Mother  volume
                                         false,               // No boolean operation
                                         0,                   // Copy number
                                         checkOverlaps);      // Overlaps checking
  }

  // Place plastic converter foil
  if (includeplasticconverter) {
    x = 0;
    y = 0;
    z = -zposition_plasticconverter;
    pos = G4ThreeVector(x, y, z);
    physPlasticConverter =   new G4PVPlacement(0,                   // Rotation
                                         pos,                 // Position
                                         logicPlasticConverter,  // Logical volume
                                         "Plastic Converter",    // Name
                                         logicWorld,          // Mother  volume
                                         false,               // No boolean operation
                                         0,                   // Copy number
                                         checkOverlaps);      // Overlaps checking
  }

  // Place Iron converter foil
  if (includeironconverter) {
    x = 0;
    y = 0;
    z = -zposition_ironconverter;
    pos = G4ThreeVector(x, y, z);
    physIronConverter =   new G4PVPlacement(0,                   // Rotation
                                         pos,                 // Position
                                         logicIronConverter,  // Logical volume
                                         "Iron Converter",    // Name
                                         logicWorld,          // Mother  volume
                                         false,               // No boolean operation
                                         0,                   // Copy number
                                         checkOverlaps);      // Overlaps checking
  }

  // Place Bismuth converter foil
  if (includebismuthconverter) {
    x = 0;
    y = 0;
    z = -zposition_bismuthconverter;
    pos = G4ThreeVector(x, y, z);
    physBismuthConverter =   new G4PVPlacement(0,                   // Rotation
                                         pos,                 // Position
                                         logicBismuthConverter,  // Logical volume
                                         "Bismuth Converter",    // Name
                                         logicWorld,          // Mother  volume
                                         false,               // No boolean operation
                                         0,                   // Copy number
                                         checkOverlaps);      // Overlaps checking
  }

  // Gemini breadboard
  if (includebreadboard) {
    G4Box* solidBreadboard = new G4Box("Breadboard", 661.5*mm, 51.5*mm, 1.299*m);
  	G4LogicalVolume* logicBreadboard = new G4LogicalVolume(solidBreadboard, Aluminium, "Breadboard");
  	G4VPhysicalVolume* physBreadboard = new G4PVPlacement(0,
                                                          G4ThreeVector(0., -388.5*mm, 0.),
  			                                                  logicBreadboard,
                                                          "Breadboard",
                                                          logicWorld,
                                                          false,
                                                          0,
                                                          checkOverlaps);
  }

  // Gemini chamber
  if (includechamber) {
  	G4Box* chamberouter = new G4Box("Chamberouter", 0.785*m, 0.685*m, 1.41*m);
  	G4Box* chamberinner = new G4Box("Chamberinner", 0.725*m, 0.625*m, 1.35*m);
    G4Tubs* chamberhole = new G4Tubs("Chamberhole",   // Name
                                     0,               // Inner radius
                                     kaptonwidth/2,   // Outer radius
                                     5*cm,            // Z half length
                                     0*deg,           // Starting angle
                                     360*deg);        // Segment angle
  	G4SubtractionSolid* chambernohole = new G4SubtractionSolid("ChamberNoHole", chamberouter, chamberinner);
    G4SubtractionSolid* chamber = new G4SubtractionSolid("Chamber", chambernohole, chamberhole, 0, G4ThreeVector(0,-76*mm,-1.38*m));
    G4LogicalVolume* logicChamber = new G4LogicalVolume(chamber, Aluminium, "Chamber");
  	G4VPhysicalVolume* physChamber = new G4PVPlacement(0,
                                                       G4ThreeVector(0*mm,76.*mm,0*mm),
  	                                                   logicChamber,
                                                       "Chamber",
                                                       logicWorld,
                                                       false,
                                                       0,
                                                       checkOverlaps);
  }
  // Lead shielding
  if(includeleadshielding) {
    x = 0;
    y = 0;
    z = -(firstcrystalz - 0.5*crystalz - 0.5*frontplatethickness - 20.*cm);
    pos = G4ThreeVector(x, y, z);
    physLeadShield =   new G4PVPlacement(0,               // Rotation
                                         pos,             // Position
                                         logicLeadShield, // Logical volume
                                         "LeadShield",    // Name
                                         logicWorld,      // Mother  volume
                                         false,           // No boolean operation
                                         0,               // Copy number
                                         checkOverlaps);  // Overlaps checking
  }

    CADMesh* mesh;
    G4double centreofsmallmagnet = -341.3*mm; // Distance from centre of breadboard

    /*
    G4VSolid* solidSmallMagnet;
    G4LogicalVolume* logicSmallMagnet;
    G4VPhysicalVolume* physSmallMagnet;
    pos = G4ThreeVector(0, 0, centreofsmallmagnet);
    mesh = new CADMesh("./Meshes/MagnetSmallClean.stl", mm, pos, false);
    solidSmallMagnet = mesh->TessellatedMesh();
    logicSmallMagnet = new G4LogicalVolume(solidSmallMagnet, Iron, "Small magnet", 0, 0, 0);
    logicSmallMagnet->SetVisAttributes(cyanColour);
    physSmallMagnet = new G4PVPlacement(0, G4ThreeVector(), logicSmallMagnet, "Small magnet", logicWorld, false, 0);

    G4VSolid* solidMagnetSpacer;
    G4LogicalVolume* logicMagnetSpacer;
    G4VPhysicalVolume* physMagnetSpacer;
    pos = G4ThreeVector(0, 0, centreofsmallmagnet-6.*cm-1.4*cm);
    mesh = new CADMesh("./Meshes/MagnetSpacerClean.stl", mm, pos, false);
    solidMagnetSpacer = mesh->TessellatedMesh();
    logicMagnetSpacer = new G4LogicalVolume(solidMagnetSpacer, Iron, "Magnet spacer", 0, 0, 0);
    logicMagnetSpacer->SetVisAttributes(cyanColour);
    physMagnetSpacer = new G4PVPlacement(0, G4ThreeVector(), logicMagnetSpacer, "MagnetSpacer", logicWorld, false, 0);

    G4VSolid* solidLargeMagnet;
    G4LogicalVolume* logicLargeMagnet;
    G4VPhysicalVolume* physLargeMagnet;
    pos = G4ThreeVector(0, 0, centreofsmallmagnet-13.5*cm-2.8*cm);
    mesh = new CADMesh("./Meshes/MagnetLargeClean.stl", mm, pos, false);
    solidLargeMagnet = mesh->TessellatedMesh();
    logicLargeMagnet = new G4LogicalVolume(solidLargeMagnet, Iron, "Large magnet", 0, 0, 0);
    logicLargeMagnet->SetVisAttributes(cyanColour);
    physLargeMagnet = new G4PVPlacement(0, G4ThreeVector(), logicLargeMagnet, "Large magnet", logicWorld, false, 0);

    G4VSolid* solidLargeMagnet2;
    G4LogicalVolume* logicLargeMagnet2;
    G4VPhysicalVolume* physLargeMagnet2;
    pos = G4ThreeVector(0, 15*mm, centreofsmallmagnet-13.5*cm-2.8*cm-15.*cm-3.04*cm);
    mesh = new CADMesh("./Meshes/MagnetLargeClean.stl", mm, pos, false);
    solidLargeMagnet2 = mesh->TessellatedMesh();
    logicLargeMagnet2 = new G4LogicalVolume(solidLargeMagnet2, Iron, "Large magnet 2", 0, 0, 0);
    logicLargeMagnet2->SetVisAttributes(cyanColour);
    physLargeMagnet2 = new G4PVPlacement(0, G4ThreeVector(), logicLargeMagnet2, "Large magnet 2", logicWorld, false, 0);
    */


    // Magnetic fields! Stolen wholeheartedly from Kris' code
    G4double xmax, xmin, ymax, ymin, zmax, zmin;
    G4double minEps = 1e-7 * mm;  //   Minimum & value for smallest steps
  	G4double maxEps = 1e-6 * mm;  //   Maximum & value for largest steps
    G4double deltaonestep = 0.5e-6 * mm;

    // Quadrupole parameters
    G4double TCCDist = 425*mm;
    G4double distancetocentreoffirstquad = 358.6*mm;
    G4double distancebetweenquadcentres = 384.7*mm + 5.*mm;
    G4double centreoffirstquad = (TCCDist - distancetocentreoffirstquad);
    G4double L1 = 60*mm;
    G4double L2 = 60*mm;

    // Add 30mm total length for field falloff
    G4double inset = 5.*mm;

    // Quad 1
    if(includequads){
      xmax = 3.*mm + inset; xmin = -3.*mm - inset; ymax = 3*mm + inset; ymin = -3.*mm - inset;
      zmin = centreoffirstquad-L1/2-inset; zmax = centreoffirstquad+L1/2+inset;
    	G4Box* magnet1_box = new G4Box("magnet1_box", 0.5*(xmax-xmin), 0.5*(ymax-ymin), 0.5*(zmax-zmin));
    	G4LogicalVolume* magnet1_log = new G4LogicalVolume(magnet1_box, Vacuum, "magnet1_log");
    	G4VPhysicalVolume* magnet1_phys = new G4PVPlacement(0,
                                                        G4ThreeVector(0.5*(xmax+xmin),0.5*(ymax+ymin),0.5*(zmax+zmin)),
    			                                              magnet1_log,
                                                        "magnet1",
                                                        logicWorld,
                                                        false,
                                                        0,
                                                        checkOverlaps);
      MagneticField* magfield1 = new MagneticField();
      magfield1->SetMagFieldValue(bfieldx, bfieldy);
    	magfield1->SetMagFieldParams(xmin, xmax, ymin, ymax, zmin, zmax, inset, true, true);
    	magfield1->SetEFieldValue(0.);
    	G4FieldManager* fieldmanager1 = new G4FieldManager();
    	fieldmanager1->SetDetectorField(magfield1);
    	G4EqMagElectricField *myEquation1 = new G4EqMagElectricField(magfield1);
    	G4MagIntegratorStepper* myStepper1 = new G4ClassicalRK4(myEquation1, 8);
    	G4MagInt_Driver* myIntgrDriver1 = new G4MagInt_Driver(deltaonestep*2., myStepper1,myStepper1->GetNumberOfVariables());
    	G4ChordFinder* myChordFinder1 = new G4ChordFinder(myIntgrDriver1);
    	fieldmanager1->SetChordFinder(myChordFinder1);
      fieldmanager1->SetMinimumEpsilonStep(minEps);
    	fieldmanager1->SetMaximumEpsilonStep(maxEps);
    	fieldmanager1->SetDeltaOneStep(deltaonestep);
    	magnet1_log->SetFieldManager(fieldmanager1, true);
}
    // Body of quad 1
    if(includequads){
      //                             (Name, Inner r, Outer r, Z half length, theta min, theta max)
      G4Tubs* quad1outer = new G4Tubs("Quad1outer",  0,  17.5*mm, L1/2, 0*deg, 360*deg);
      G4Tubs* quad1inner = new G4Tubs("Quad1inner",  0,  3*mm,    L1/2, 0*deg, 360*deg);
    	G4SubtractionSolid* solidQuad1 = new G4SubtractionSolid("Quad1", quad1outer, quad1inner);
      G4LogicalVolume* logicQuad1= new G4LogicalVolume(solidQuad1, NdMagnet, "Quad1");
      logicQuad1->SetVisAttributes(cyanColour);
    	G4VPhysicalVolume* physQuad1 = new G4PVPlacement(0,
                                                       G4ThreeVector((xmin+xmax)/2,(ymin+ymax)/2,(zmin+zmax)/2),
    	                                                 logicQuad1,
                                                       "Quad1",
                                                       logicWorld,
                                                       false,
                                                       0,
                                                       checkOverlaps);
}
    // Quadrupole 2
    if(includequads){
      xmax = 3.*mm + inset; xmin = -3.*mm - inset; ymax = 3*mm + inset; ymin = -3.*mm - inset;
      zmin = centreoffirstquad-distancebetweenquadcentres-L2/2-inset; zmax = centreoffirstquad-distancebetweenquadcentres+L2/2+inset;

    	G4Box* magnet2_box = new G4Box("magnet2_box", 0.5*(xmax-xmin), 0.5*(ymax-ymin), 0.5*(zmax-zmin));
    	G4LogicalVolume* magnet2_log = new G4LogicalVolume(magnet2_box, Vacuum, "magnet2_log");
    	G4VPhysicalVolume* magnet2_phys = new G4PVPlacement(0,
                                                        G4ThreeVector(0.5*(xmax+xmin),0.5*(ymax+ymin),0.5*(zmax+zmin)),
    			                                              magnet2_log,
                                                        "magnet2",
                                                        logicWorld,
                                                        false,
                                                        0,
                                                        checkOverlaps);
      MagneticField* magfield2 = new MagneticField();
      magfield2->SetMagFieldValue(bfieldx, bfieldy);
    	magfield2->SetMagFieldParams(xmin, xmax, ymin, ymax, zmin, zmax, inset, true, false);
    	magfield2->SetEFieldValue(0.);
    	G4FieldManager* fieldmanager2 = new G4FieldManager();
    	fieldmanager2->SetDetectorField(magfield2);
      G4EqMagElectricField *myEquation2 = new G4EqMagElectricField(magfield2);
    	G4MagIntegratorStepper* myStepper2 = new G4ClassicalRK4(myEquation2, 8);
    	G4MagInt_Driver* myIntgrDriver2 = new G4MagInt_Driver(deltaonestep*2., myStepper2,myStepper2->GetNumberOfVariables());
    	G4ChordFinder* myChordFinder2 = new G4ChordFinder(myIntgrDriver2);
    	fieldmanager2->SetChordFinder(myChordFinder2);
      fieldmanager2->SetMinimumEpsilonStep(minEps);
    	fieldmanager2->SetMaximumEpsilonStep(maxEps);
    	fieldmanager2->SetDeltaOneStep(deltaonestep);
    	magnet2_log->SetFieldManager(fieldmanager2, true);
}
    // Body of quad 2
    if(includequads){
    //                             (Name, Inner r, Outer r, Z half length, theta min, theta max)
      G4Tubs* quad2outer = new G4Tubs("Quad2outer",  0,  17.5*mm, L2/2, 0*deg, 360*deg);
      G4Tubs* quad2inner = new G4Tubs("Quad2inner",  0,  3*mm,    L2/2, 0*deg, 360*deg);
    	G4SubtractionSolid* solidQuad2 = new G4SubtractionSolid("Quad2", quad2outer, quad2inner);
      G4LogicalVolume* logicQuad2= new G4LogicalVolume(solidQuad2, NdMagnet, "Quad2");
      logicQuad2->SetVisAttributes(cyanColour);
    	G4VPhysicalVolume* physQuad2 = new G4PVPlacement(0,G4ThreeVector((xmin+xmax)/2,(ymin+ymax)/2,(zmin+zmax)/2),logicQuad2,"Quad2",logicWorld,false,0,checkOverlaps);
}

    // Lead collimator
  if (includeleadcollimator) {
    //                             (Name, Inner r, Outer r, Z half length, theta min, theta max)
      G4Tubs* collouter = new G4Tubs("Collouter",  0,  25.*mm, 50.*mm, 0*deg, 360*deg);
      G4Tubs* collinner = new G4Tubs("Collinner",  0,  0.5*mm, 50.*mm, 0*deg, 360*deg);
    	G4SubtractionSolid* solidLeadCollimator = new G4SubtractionSolid("Lead collimator", collouter, collinner);
      G4LogicalVolume* logicLeadCollimator = new G4LogicalVolume(solidLeadCollimator, Lead, "Lead collimator");
    	G4VPhysicalVolume* physLeadCollimator = new G4PVPlacement(0,
                                                               G4ThreeVector(0,0,TCCDist - 1.109*m + 50.*mm),
                                                               logicLeadCollimator,
                                                               "Lead collimator",
                                                               logicWorld,
                                                               false,
                                                               0,
                                                               checkOverlaps);
}

   G4Tubs* solidRR = new G4Tubs("RR volume",  0,  0.5*mm, 10.*um, 0*deg, 360*deg);
   G4LogicalVolume* logicRRVolume= new G4LogicalVolume(solidRR, Vacuum, "RR volume");
   rrVolume = new G4PVPlacement(0,
                                G4ThreeVector(0,0,TCCDist - 1.109*m),
                                logicRRVolume,
                                "RR volume",
                                logicWorld,
                                false,
                                0,
                                checkOverlaps);

   // f/2 parabola
   if(includeparabola){

       const G4int Nz = 20; // Number of points
       G4double rmax = 350; // Max outer radius of parent parabola in mm
       G4double router[Nz+1];
       G4double rinner[Nz+1];
       G4double zplanes[Nz+1];
       for (int i = 0; i < Nz+1; i++){
         router[i] = double(i)/double(Nz-1) * rmax; // Still in mm
         zplanes[i] = (1/(300*std::sqrt(3)))*router[i]*router[i]; // Still in mm
         rinner[i] = 0;
         router[i] = router[i] * mm;
         zplanes[i] = zplanes[i] * mm;
       }

       router[Nz-1] = router[Nz-2];
       zplanes[Nz-1] = -5*cm;
       G4Polycone* f2parent = new G4Polycone("f2parent",0*deg,360*deg,Nz,zplanes,rinner,router);
       G4Tubs* f2daughter = new G4Tubs("f2daughter", 0, 10*cm, 100*cm, 0*deg, 360*deg);
       G4Tubs* f2hole = new G4Tubs("f2hole", 0, 1*cm, 100*cm, 0*deg, 360*deg);

       pos = G4ThreeVector(17.5*cm, 0, 0);
       G4SubtractionSolid* f2parentwithhole = new G4SubtractionSolid("f2parentwithhole", f2parent, f2hole, new G4RotationMatrix(pi/2,-pi/6,0), pos);
       pos = G4ThreeVector(15*cm, 0, 0);
       G4IntersectionSolid* f2nohole = new G4IntersectionSolid("f2 no hole", f2parentwithhole, f2daughter, new G4RotationMatrix(0,0,0), pos);
       G4LogicalVolume* logicf2 = new G4LogicalVolume(f2nohole, FusedSilica, "f2");
       G4VPhysicalVolume* physf2 = new G4PVPlacement(new G4RotationMatrix(pi, pi/6, -pi/2), G4ThreeVector(0, -151.*mm, -850.*mm),
                                        logicf2, "f2", logicWorld, false, 0, checkOverlaps);
}


   if(includedipolemagnetfield){
         // Spectrometer dipole
         G4double width = 20*mm, length = 300*mm, height = 100*mm;
         xmax = width/2+inset; xmin = xmax-width-inset; ymax = height/2+inset; ymin = ymax-height-inset;
         zmax = -1.2*m + length/2 + inset;
         zmin = -1.2*m - length/2 - inset;

       	 G4Box* magnet3_box = new G4Box("magnet3_box", 0.5*(xmax-xmin), 0.5*(ymax-ymin), 0.5*(zmax-zmin));
         G4LogicalVolume* magnet3_log = new G4LogicalVolume(magnet3_box, Vacuum, "magnet3_log");
         G4VPhysicalVolume* magnet3_phys = new G4PVPlacement(0,G4ThreeVector(0.5*(xmax+xmin),0.5*(ymax+ymin),0.5*(zmax+zmin)),magnet3_log,"magnet3",logicWorld,false,0,checkOverlaps);
         MagneticField* magfield3 = new MagneticField();
         magfield3->SetMagFieldValue(1.*tesla, 0.);
         magfield3->SetMagFieldParams(xmin, xmax, ymin, ymax, zmin, zmax, inset, false, false);
         magfield3->SetEFieldValue(0.);
         G4FieldManager* fieldmanager3 = new G4FieldManager();
         fieldmanager3->SetDetectorField(magfield3);
         G4EqMagElectricField *myEquation3 = new G4EqMagElectricField(magfield3);
         G4MagIntegratorStepper* myStepper3 = new G4ClassicalRK4(myEquation3, 8);
         G4MagInt_Driver* myIntgrDriver3 = new G4MagInt_Driver(deltaonestep*2., myStepper3,myStepper3->GetNumberOfVariables());
         G4ChordFinder* myChordFinder3 = new G4ChordFinder(myIntgrDriver3);
         fieldmanager3->SetChordFinder(myChordFinder3);
         fieldmanager3->SetMinimumEpsilonStep(minEps);
         fieldmanager3->SetMaximumEpsilonStep(maxEps);
         fieldmanager3->SetDeltaOneStep(deltaonestep);
         magnet3_log->SetFieldManager(fieldmanager3, true);

}

   if(includedipolemagnet){
     // Spectrometer dipole body
     G4VSolid* solidLargeMagnet;
     G4LogicalVolume* logicLargeMagnet;
     G4VPhysicalVolume* physLargeMagnet;
     pos = G4ThreeVector(0, 0, -1.2*m);
     mesh = new CADMesh("./Meshes/MagnetLargeClean.stl", mm, pos, false);
     solidLargeMagnet = mesh->TessellatedMesh();
     logicLargeMagnet = new G4LogicalVolume(solidLargeMagnet, Iron, "Large magnet", 0, 0, 0);
     logicLargeMagnet->SetVisAttributes(cyanColour);
     physLargeMagnet = new G4PVPlacement(0, G4ThreeVector(), logicLargeMagnet, "Large magnet", logicWorld, false, 0);
 }


    // Dipole before first quad
    /*
    G4double width = 20*mm, length = 200*mm, height = 100*mm;
    xmax = width/2+inset; xmin = xmax-width-inset; ymax = height/2+inset; ymin = ymax-height-inset;
    zmax = TCCDist - 0.5*(TCCDist - centreoffirstquad-L1/2) + length/2 + inset;
    zmin = TCCDist - 0.5*(TCCDist - centreoffirstquad-L1/2) - length/2 - inset;

  	G4Box* magnet3_box = new G4Box("magnet3_box", 0.5*(xmax-xmin), 0.5*(ymax-ymin), 0.5*(zmax-zmin));
  	G4LogicalVolume* magnet3_log = new G4LogicalVolume(magnet3_box, Vacuum, "magnet3_log");
  	G4VPhysicalVolume* magnet3_phys = new G4PVPlacement(0,
                                                      G4ThreeVector(0.5*(xmax+xmin),0.5*(ymax+ymin),0.5*(zmax+zmin)),
  			                                              magnet3_log,
                                                      "magnet3",
                                                      logicWorld,
                                                      false,
                                                      0,
                                                      checkOverlaps);
    MagneticField* magfield3 = new MagneticField();
    magfield3->SetMagFieldValue(1.*tesla, 0.);
  	magfield3->SetMagFieldParams(xmin, xmax, ymin, ymax, zmin, zmax, inset, false, false);
  	magfield3->SetEFieldValue(0.);
  	G4FieldManager* fieldmanager3 = new G4FieldManager();
  	fieldmanager3->SetDetectorField(magfield3);
  	G4EqMagElectricField *myEquation3 = new G4EqMagElectricField(magfield3);
  	G4MagIntegratorStepper* myStepper3 = new G4ClassicalRK4(myEquation3, 8);
  	G4MagInt_Driver* myIntgrDriver3 = new G4MagInt_Driver(deltaonestep*2., myStepper3,myStepper3->GetNumberOfVariables());
  	G4ChordFinder* myChordFinder3 = new G4ChordFinder(myIntgrDriver3);
  	fieldmanager3->SetChordFinder(myChordFinder3);
  	fieldmanager3->SetMinimumEpsilonStep(minEps);
  	fieldmanager3->SetMaximumEpsilonStep(maxEps);
  	fieldmanager3->SetDeltaOneStep(deltaonestep);
    magnet3_log->SetFieldManager(fieldmanager3, true);

    // Dipole between quads
    width = 20*mm, length = 200*mm, height = 100*mm;
    xmax = width/2+inset; xmin = xmax-width-inset; ymax = height/2+inset; ymin = ymax-height-inset;
    zmax = centreoffirstquad-distancebetweenquadcentres/2 - L1/4 + L2/4 + length/2 + inset;
    zmin = centreoffirstquad-distancebetweenquadcentres/2 - L1/4 + L2/4 - length/2 - inset;

  	G4Box* magnet4_box = new G4Box("magnet4_box", 0.5*(xmax-xmin), 0.5*(ymax-ymin), 0.5*(zmax-zmin));
  	G4LogicalVolume* magnet4_log = new G4LogicalVolume(magnet4_box, Vacuum, "magnet4_log");
  	G4VPhysicalVolume* magnet4_phys = new G4PVPlacement(0,
                                                      G4ThreeVector(0.5*(xmax+xmin),0.5*(ymax+ymin),0.5*(zmax+zmin)),
  			                                              magnet4_log,
                                                      "magnet4",
                                                      logicWorld,
                                                      false,
                                                      0,
                                                      checkOverlaps);
    MagneticField* magfield4 = new MagneticField();
    magfield4->SetMagFieldValue(-1.*tesla, 0.);
  	magfield4->SetMagFieldParams(xmin, xmax, ymin, ymax, zmin, zmax, inset, false, false);
  	magfield4->SetEFieldValue(0.);
  	G4FieldManager* fieldmanager4 = new G4FieldManager();
  	fieldmanager4->SetDetectorField(magfield4);
  	G4EqMagElectricField *myEquation4 = new G4EqMagElectricField(magfield4);
  	G4MagIntegratorStepper* myStepper4 = new G4ClassicalRK4(myEquation4, 8);
  	G4MagInt_Driver* myIntgrDriver4 = new G4MagInt_Driver(deltaonestep*2., myStepper4,myStepper4->GetNumberOfVariables());
  	G4ChordFinder* myChordFinder4 = new G4ChordFinder(myIntgrDriver4);
  	fieldmanager4->SetChordFinder(myChordFinder4);
  	fieldmanager4->SetMinimumEpsilonStep(minEps);
  	fieldmanager4->SetMaximumEpsilonStep(maxEps);
  	fieldmanager4->SetDeltaOneStep(deltaonestep);
    magnet4_log->SetFieldManager(fieldmanager4, true);


    // Lead collimator
    G4Box* solidLeadCollimator = new G4Box("Lead collimator", 0.5*100.*mm, 0.5*100.*mm, 10.*mm);
    G4LogicalVolume* logicLeadCollimator= new G4LogicalVolume(solidLeadCollimator, Lead, "Lead collimator");

    x = 0;
    y = 56.*mm;
    z =  centreoffirstquad + L1/2+inset+25*mm;
    pos = G4ThreeVector(x, y, z);
    G4VPhysicalVolume* physLeadCollimator =  new G4PVPlacement(0,                   // Rotation
                                                               pos,                 // Position
                                                               logicLeadCollimator,  // Logical volume
                                                               "Lead Collimator",    // Name
                                                               logicWorld,          // Mother  volume
                                                               false,               // No boolean operation
                                                               0,                   // Copy number
                                                               checkOverlaps);      // Overlaps checking
    */

    // Here add some measurement screens made out of Vacuum
    // These were added in the context of the detector development June 2019

    if(includevacscreens){

              G4int vacscreenind = 0;
              G4RotationMatrix* VacScreenRot = new G4RotationMatrix(0, 0, 0);

              x = 0.0;
              y = 0.0;

              for (int i = 0; i<Nvacscreens; i++){

                  z = -(firstscreenpos_z + i*vacscreen_spacing);
                  pos = G4ThreeVector(x, y, z);
                  vacscreenind = i;
                  physVacScreen = new G4PVPlacement(VacScreenRot,
                                    pos,
                                    logicVacScreen,             //its logical volume
                                    "VacScreen",                //its name
                                    logicWorld,                //its mother  volume
                                    false,                   //no boolean operation
                                    vacscreenind,            //copy number
                                    checkOverlaps);          //overlaps checking

                  vacscreens[vacscreenind] = physVacScreen;
                  if (debug) G4cout << "Placing screen " << vacscreenind << G4endl;

            }
      }







    // Measurement plane acting as lanex screen
    //G4RotationMatrix* lanexRot = new G4RotationMatrix(0, 45*deg, 0);
    //G4ThreeVector lanexPos = G4ThreeVector(0., 30.*cm/std::sqrt(2), TCCDist - 2.5*m);
    //... or somewhere else
    G4RotationMatrix* measRot = new G4RotationMatrix(0, 0, 0);
    //G4ThreeVector measPos = G4ThreeVector(0., 0., TCCDist - 0.4*m);
    G4ThreeVector measPos = G4ThreeVector(0., 0., -90.0*cm);

    //G4Box* solidMeasurement = new G4Box("Measurement", 30.*mm, 30.*cm, .1*mm);
    G4Box* solidMeasurement = new G4Box("Measurement", 10.*mm, 10.*mm, .1*mm);

    G4LogicalVolume* logicMeasurement = new G4LogicalVolume(solidMeasurement, Vacuum, "Measurement");

    G4VPhysicalVolume* physMeasurement =
      new G4PVPlacement(measRot,
                        measPos,
                        logicMeasurement,             //its logical volume
                        "Measurement",                //its name
                        logicWorld,                //its mother  volume
                        false,                   //no boolean operation
                        0,                       //copy number
                        checkOverlaps);          //overlaps checking
/* // This is for a lanex screen
                        G4VPhysicalVolume* physMeasurement =
                          new G4PVPlacement(lanexRot,
                                            lanexPos,
                                            logicMeasurement,             //its logical volume
                                            "Measurement",                //its name
                                            logicWorld,                //its mother  volume
                                            false,                   //no boolean operation
                                            0,                       //copy number
                                            checkOverlaps);          //overlaps checking

*/
    fScoringVolume = physMeasurement;





    /*
    // First large magnet
    xmax = 13.5*mm; xmin = -13.5*mm; ymax = 50.*mm; ymin = -50.*mm;
    zmin = centreofsmallmagnet-60*mm-28.*mm-75.*mm   -75.*mm;
    zmax = centreofsmallmagnet-60*mm-28.*mm-75.*mm   +75.*mm;
  	G4Box* magnet2_box = new G4Box("magnet2_box", 0.5*(xmax-xmin), 0.5*(ymax-ymin), 0.5*(zmax-zmin));
  	G4LogicalVolume* magnet2_log = new G4LogicalVolume(magnet2_box, Vacuum, "magnet2_log");
  	G4VPhysicalVolume* magnet2_phys = new G4PVPlacement(0,
                                                      G4ThreeVector(0.5*(xmax+xmin),0.5*(ymax+ymin),0.5*(zmax+zmin)),
  			                                              magnet2_log,
                                                      "magnet2",
                                                      logicWorld,
                                                      false,
                                                      0,
                                                      checkOverlaps);
    MagneticField* magfield2 = new MagneticField();
  	magfield2->SetMagFieldParams(xmin, xmax, ymin, ymax, zmin, zmax);
  	magfield2->SetMagFieldValue(bfieldx, bfieldy);
  	magfield2->SetEFieldValue(0.);
  	G4FieldManager* fieldmanager2 = new G4FieldManager();
  	fieldmanager2->SetDetectorField(magfield2);
  	G4EqMagElectricField *myEquation2 = new G4EqMagElectricField(magfield2);
  	G4MagIntegratorStepper* myStepper2 = new G4ClassicalRK4(myEquation2, 8);
  	G4MagInt_Driver* myIntgrDriver2 = new G4MagInt_Driver(1.0e-3*mm, myStepper2,myStepper2->GetNumberOfVariables());
  	G4ChordFinder* myChordFinder2 = new G4ChordFinder(myIntgrDriver2);
  	fieldmanager2->SetChordFinder(myChordFinder2);
  	magnet2_log->SetFieldManager(fieldmanager2, true);
  	fieldmanager2->SetMinimumEpsilonStep(minEps);
  	fieldmanager2->SetMaximumEpsilonStep(maxEps);
  	fieldmanager2->SetDeltaOneStep(0.5e-3 * mm);

    // Second large magnet
    xmax = 13.5*mm; xmin = -13.5*mm; ymax = 65.*mm; ymin = -35.*mm;
    zmin = centreofsmallmagnet-60*mm-28.*mm-150.*mm-30.4*mm-75.*mm   -75.*mm;
    zmax = centreofsmallmagnet-60*mm-28.*mm-150.*mm-30.4*mm-75.*mm   +75.*mm;
  	G4Box* magnet3_box = new G4Box("magnet3_box", 0.5*(xmax-xmin), 0.5*(ymax-ymin), 0.5*(zmax-zmin));
  	G4LogicalVolume* magnet3_log = new G4LogicalVolume(magnet3_box, Vacuum, "magnet3_log");
  	G4VPhysicalVolume* magnet3_phys = new G4PVPlacement(0,
                                                      G4ThreeVector(0.5*(xmax+xmin),0.5*(ymax+ymin),0.5*(zmax+zmin)),
  			                                              magnet3_log,
                                                      "magnet3",
                                                      logicWorld,
                                                      false,
                                                      0,
                                                      checkOverlaps);
    MagneticField* magfield3 = new MagneticField();
  	magfield3->SetMagFieldParams(xmin, xmax, ymin, ymax, zmin, zmax);
  	magfield3->SetMagFieldValue(bfieldx, bfieldy);
  	magfield3->SetEFieldValue(0.);
  	G4FieldManager* fieldmanager3 = new G4FieldManager();
  	fieldmanager3->SetDetectorField(magfield3);
  	G4EqMagElectricField *myEquation3 = new G4EqMagElectricField(magfield3);
  	G4MagIntegratorStepper* myStepper3 = new G4ClassicalRK4(myEquation3, 8);
  	G4MagInt_Driver* myIntgrDriver3 = new G4MagInt_Driver(1.0e-3*mm, myStepper3,myStepper3->GetNumberOfVariables());
  	G4ChordFinder* myChordFinder3 = new G4ChordFinder(myIntgrDriver3);
  	fieldmanager3->SetChordFinder(myChordFinder3);
  	magnet3_log->SetFieldManager(fieldmanager3, true);
  	fieldmanager3->SetMinimumEpsilonStep(minEps);
  	fieldmanager3->SetMaximumEpsilonStep(maxEps);
  	fieldmanager3->SetDeltaOneStep(0.5e-3 * mm);

    */

  // Always return the physical World
  return physWorld;
}
