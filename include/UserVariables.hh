#include <random>

// CsI spectrometer

// RAL Stack (2015)
static const G4bool includespectrometer = true;
static const int Ncrystalsz = 12; // used to be 45!
static const int Ncrystalsy = 21;
static const G4double firstcrystalz = 180.0*cm;

static const G4double crystalz = 4.5*mm;
static const G4double crystaly = 4.5*mm;
static const G4double crystalx = 50.0*mm;
static const G4double crystalspacing = 5.0*mm;

static const G4double dividerthickness = 0.49*mm;
static const G4bool includeDividers = true;

static const G4double frontplatethickness = 1.0*mm; // 10.0*mm for steel, 1.0*mm for plastic
static const G4bool includeplasticfrontplate = true; //if true replaces steel frontplate with plastic
static const G4bool includefrontplate = true;

// Dual spectrometer

static const G4bool includedualaxisspectrometer = false;

static const G4double firstcrystalz_Dual = 180.0*cm;
static const G4double firstcrystalx_Dual = 3.25*mm; // this is to avoid placing a gap in the centre
static const G4double firstcrystaly_Dual = 3.25*mm; // this is to avoid placing a gap in the centre
static const int Ncrystalsx_Dual = 10; // how many crystals in x (this is for vertical facing crystals)
static const int Ncrystalsy_Dual = 10; // how many crystals in y (this is for horizontal facing crystals)
static const int Ncrystalsz_HDual = 35; // 70 layers in total, 35 each direction
static const int Ncrystalsz_VDual = 35; // H means crystals are facing in x direction, V in vertical

static const G4double crystalz_Dual = 5.0*mm;
static const G4double crystaly_Dual = 5.0*mm;
static const G4double crystalx_Dual = 50.0*mm; // in vertical, x and y are simply interchanged
static const G4double crystalspacing_Dual = 6.5*mm; // crystals are spaced by 1.5 mm, 1 mm is nylon divider
static const G4double crystalspacingz_Dual = 6.0*mm; // crystal columns are spaced by 1 mm rubber

static const G4bool includefrontplate_Dual = false; // 2 mm aluminium front plate
static const G4bool includebackplate_Dual = false; // 32 mm aluminium
static const G4double frontplatethickness_Dual = 2.0*mm;
static const G4double backplatethickness_Dual = 32.0*mm;

static const G4bool includedividers_Dual = false; //1 mm Nylon in transversal and 1 mm rubber in longitudinal direction
static const G4bool includeframe_Dual = false; // outside aluminium frame

static const G4double dividerthickness_TDual = 1.0*mm; // transversal divider (nylon)
static const G4double dividerthickness_LDual = 0.99*mm; // longitudinal divider (rubber)

// CsI profile stacks

// Jena 45x45 (1 mm x 1 mm x 10 mm)

static const G4bool includeprofilestack_Jena = false;
static const int Ncrystalsx_Jena = 45;
static const int Ncrystalsy_Jena = 45;
static const G4double firstcrystalz_Jena = 160.0*cm;

static const G4double crystalx_Jena = 1.0*mm;
static const G4double crystaly_Jena = 1.0*mm;
static const G4double crystalz_Jena = 10.0*mm;
static const G4double crystalspacing_Jena = 1.2*mm; // crystals are spaced by 0.2 mm thick layers of coating

static const G4double coatingthickness_Jena = 0.19*mm;
static const G4bool includecoating_Jena = false;

static const G4double frontplatethickness_Jena = 0.49*mm; //TiO2 layer
static const G4bool includefrontplate_Jena = false;

// DESY 30x30 (1.5 mm x 1.5 mm x 10 mm)

static const G4bool includeprofilestack_DESY = false;
static const int Ncrystalsx_DESY = 30;
static const int Ncrystalsy_DESY = 30;
static const G4double firstcrystalz_DESY = 170.0*cm;

static const G4double crystalx_DESY = 1.5*mm;
static const G4double crystaly_DESY = 1.5*mm;
static const G4double crystalz_DESY = 10.0*mm;
static const G4double crystalspacing_DESY = 1.7*mm;

static const G4double coatingthickness_DESY = 0.19*mm;// crystals are spaced by 0.2 mm thick layers of coating
static const G4bool includecoating_DESY = false;

static const G4double frontplatethickness_DESY = 0.49*mm; //TiO2 layer
static const G4bool includefrontplate_DESY = false;


// QUB 20x20 (2 mm x 2 mm x 20 mm)

static const G4bool includeprofilestack_QUB = true;
static const int Ncrystalsx_QUB = 20;
static const int Ncrystalsy_QUB = 20;
static const G4double firstcrystalz_QUB = 159.0*cm;

static const G4double crystalx_QUB = 2.0*mm;
static const G4double crystaly_QUB = 2.0*mm;
static const G4double crystalz_QUB = 20.0*mm;
static const G4double crystalspacing_QUB = 2.1*mm;

static const G4double coatingthickness_QUB = 0.09*mm;// crystals are spaced by 0.1 mm thick layers of aluminium foil
static const G4bool includecoating_QUB = true;

static const G4double frontplatethickness_QUB = 10.00*mm; //Aluminium frame
static const G4bool includefrontplate_QUB = true;


////////////////// DETECTOR DEVELOPMENT AREA //////////////////////

static const G4bool includevacscreens = true;

static const G4double screensize_x = 100.0*mm;
static const G4double screensize_y = 100.0*mm;

static const G4double firstscreenpos_z = 155.0*cm;

static const int Nvacscreens = 3;

static const G4double vacscreen_spacing = 20.0*cm;

static const G4bool include_detfilter = true;
static const G4double detfilter_pos_z =  firstcrystalz + Ncrystalsz*crystalspacing + 5.0*mm;
static const G4double detfilter_thickness = 2.0*mm;

/////////////////////////////////////////////////////////////////
// converters, windows etc.

static const G4double converterthickness = 5.0*mm;
static const G4bool includeconverter = false;

static const G4double plasticconverterthickness = 1.6*mm;
static const G4double plasticconverterwidth = 10.0*mm;
static const G4bool includeplasticconverter = false;
static const G4double zposition_plasticconverter = 80.0*cm;

static const G4double ironconverterthickness = 2.0*mm;
static const G4double ironconverterwidth = 10.0*mm;
static const G4bool includeironconverter = false;
static const G4double zposition_ironconverter = 80.0*cm;

static const G4double bismuthconverterthickness = 1.0*mm;
static const G4double bismuthconverterwidth = 10.0*mm;
static const G4bool includebismuthconverter = false;
static const G4double zposition_bismuthconverter = 80.0*cm;


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

static const G4bool includeparabola = false;

// Magnet parameters
static const G4bool includedipolemagnet = false;
static const G4bool includedipolemagnetfield = false;

// For quadrupoles (dipole magnet strength is fixed to 1 tesla)
static const G4bool includequads = false;
static const G4double bfieldx = 1.25*tesla;
static const G4double bfieldy = 0.*tesla;

// Other flags
static const G4bool checkOverlaps = false;
static const G4bool debug = false;
static const G4bool plot = false;
static const G4bool RRon = false;
