#ifndef MagneticField_h
#define MagneticField_h 1

#include "globals.hh"
#include "G4ElectroMagneticField.hh"

class MagneticField : public G4ElectroMagneticField
{
  public:
    MagneticField();
    ~MagneticField();

    //Set the field (fieldValue,0,0)
    void SetMagFieldValue(G4double fieldValue);
    void SetMagFieldValue(G4double fieldValuex, G4double fieldValuey);
    void SetEFieldValue(G4double fieldValue);
    void SetMagFieldParams(G4double xmi, G4double xma, G4double ymi, G4double yma, G4double zmi, G4double zma);
    void SetMagFieldParams(G4double xmi, G4double xma, G4double ymi, G4double yma, G4double zmi, G4double zma, G4double inset, G4bool isquad, G4bool isfocussing);
    void GetFieldValue( const double Point[3], double *Bfield) const;
    // This needs to be implemented as it is a virtual method in G4ElectroMagneticField
    G4bool   DoesFieldChangeEnergy() const {return false;};

  protected:
    G4double 			xmax, xmin, ymax, ymin, zmin, zmax, g, inset;
    G4double 			Bx;
    G4double 			By;
    G4double 			Ey;
    G4bool				IsInitialised, IsQuad, IsFocussing;
};


#endif
