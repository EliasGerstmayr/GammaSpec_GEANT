#include <random>

// CsI spectrometer

// RAL Stack (2015)
static const G4bool includespectrometer = true;
static const int Ncrystalsz = 33;
static const int Ncrystalsy = 21;
static const G4double firstcrystalz = 180.0*cm;

static const G4double crystalz = 4.5*mm;
static const G4double crystaly = 4.5*mm;
static const G4double crystalx = 50.0*mm;
static const G4double crystalspacing = 5.0*mm;

static const G4double dividerthickness = 0.49*mm;
static const G4bool includeDividers = true;

static const G4double frontplatethickness = 10.0*mm;
static const G4bool includeplasticfrontplate = true; //if true replaces steel frontplate with plastic
static const G4bool includefrontplate = true;

// Dual spectrometer

static const G4bool includedualaxisspectrometer = false;

static const G4double firstcrystalz_Dual = 180.0*cm;
static const int Ncrystalsx_Dual = 10;
static const int Ncrystalsy_Dual = 10;
static const int Ncrystalsz_Dual = 20; //?

static const G4double crystalz_Dual = 5.0*mm;
static const G4double crystaly_Dual = 5.0*mm;
static const G4double crystalx_Dual = 50.0*mm;
static const G4double crystalspacing_Dual = 5.0*mm;
static const G4double crystalspacingz_Dual = 10.0*mm;

// CsI profile stacks

// Jena 45x45 (1 mm x 1 mm x 10 mm)

static const G4bool includeprofilestack_Jena = true;
static const int Ncrystalsx_Jena = 45;
static const int Ncrystalsy_Jena = 45;
static const G4double firstcrystalz_Jena = 160.0*cm;

static const G4double crystalx_Jena = 1.0*mm;
static const G4double crystaly_Jena = 1.0*mm;
static const G4double crystalz_Jena = 10.0*mm;
static const G4double crystalspacing_Jena = 1.2*mm; // crystals are spaced by 0.2 mm thick layers of coating

static const G4double coatingthickness_Jena = 0.2*mm;
static const G4bool includecoating_Jena = false;

static const G4double frontplatethickness_Jena = 0.5*mm; //TiO2 layer
static const G4bool includefrontplate_Jena = false;

// DESY 30x30 (1.5 mm x 1.5 mm x 10 mm)

static const G4bool includeprofilestack_DESY = false;
static const int Ncrystalsx_DESY = 30;
static const int Ncrystalsy_DESY = 30;
static const G4double firstcrystalz_DESY = 160.0*cm;

static const G4double crystalx_DESY = 1.5*mm;
static const G4double crystaly_DESY = 1.5*mm;
static const G4double crystalz_DESY = 10.0*mm;
static const G4double crystalspacing_DESY = 1.7*mm;

static const G4double coatingthickness_DESY = 0.2*mm;// crystals are spaced by 0.2 mm thick layers of coating
static const G4bool includecoating_DESY = false;

static const G4double frontplatethickness_DESY = 0.5*mm; //TiO2 layer
static const G4bool includefrontplate_DESY = false;

// converters, windows etc.

static const G4double converterthickness = 5.0*mm;
static const G4bool includeconverter = false;

static const G4double kaptonthickness = 0.1*mm;
static const G4double kaptonwidth = 100.*mm;
static const G4bool includekaptonwindow = false;

static const G4double alumniniumthickness = 0.1*mm;
static const G4double aluminiumwidth = 100.*mm;
static const G4bool includealuminium = false;

static const G4bool includeleadshielding = false;
static const G4bool includebreadboard = false;
static const G4bool includechamber = false;

static const G4bool includeleadcollimator = false;

// Magnet parameters
static const G4bool includedipolemagnet = false;

// For quadrupoles (dipole magnet strength is fixed to 1 tesla)
static const G4bool includequads = false;
static const G4double bfieldx = 1.25*tesla;
static const G4double bfieldy = 0.*tesla;

// Other flags
static const G4bool checkOverlaps = false;
static const G4bool debug = false;
static const G4bool plot = false;
static const G4bool RRon = false;
