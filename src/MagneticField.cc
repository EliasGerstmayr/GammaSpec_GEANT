// Brief implementation based on G4MagneticField for local detector.
// Field location is determined by method SetMagFieldParams and is essentially
// a field by a strong permanent magnet.

#include "MagneticField.hh"

MagneticField::MagneticField()
{
	IsInitialised = false;
	Bx = 0.;
	By = 0.;
	Ey = 0.;//2e6 *volt/m;
	xmax = ymax = 0.05*m;
	xmin = ymin = -0.01*m;
	zmax = 0.3*m;
	zmin = 0.05*m;
}

MagneticField::~MagneticField()
{}

// Set the value of the Global Field to fieldValue along X

void MagneticField::SetMagFieldValue(G4double fieldValue)
{
	// If quad, set Bx magnitude at outer edge, i.e. at xmax/ymax/xmin/ymin
	Bx = fieldValue;
}

void MagneticField::SetMagFieldValue(G4double fieldValuex, G4double fieldValuey)
{
	// If quad, set Bx magnitude at outer edge, i.e. at xmax/ymax/xmin/ymin
	Bx = fieldValuex;
	By = fieldValuey;
}

void MagneticField::SetEFieldValue(G4double fieldValue)
{
	Ey = fieldValue;
}

void MagneticField::SetMagFieldParams(G4double xmi, G4double xma, G4double ymi, G4double yma, G4double zmi, G4double zma)
{
	IsInitialised = true;
	xmin = xmi;
	xmax = xma;
	ymin = ymi;
	ymax = yma;
	zmin = zmi;
	zmax = zma;
	// If not specified, assume dipole
	IsQuad = false;
}

// Inset is the distance the real magnet is inset from the edge of the box. I.e. logical box size is zmax - zmin, real magnet size is zmax-zmin-2*inset
void MagneticField::SetMagFieldParams(G4double xmi, G4double xma, G4double ymi, G4double yma, G4double zmi, G4double zma, G4double ins, G4bool isquad, G4bool isfocussing)
{
	IsInitialised = true;
	xmin = xmi;
	xmax = xma;
	ymin = ymi;
	ymax = yma;
	zmin = zmi;
	zmax = zma;
	inset = ins;
	IsQuad = isquad;
	IsFocussing = isfocussing;

	if(IsQuad){
		g = 2. * Bx/(xmax - xmin - 2*inset);

		if (!IsFocussing) {
			g *= -1.;
		}

	}
}

// Set the value of the Global Field

void MagneticField::GetFieldValue(const double Point[3], double *Bfield) const
{
	//  Return as Bfield[0], [1], [2] the magnetic field x, y & z components
	//   and   as Bfield[3], [4], [5] the electric field x, y & z components
	Bfield[1] = 0.; //By
	Bfield[2] = 0.; //Bz
	Bfield[4] = 0.; //Ey
	Bfield[5] = 0.; //Ez

	if (!IsInitialised){ Bfield[0] = 0.; return;}

	if(Point[0]<xmax && Point[0]>xmin && Point[1]<ymax && Point[1]>ymin && Point[2]>zmin && Point[2]<zmax) {

		G4double x, y, z, r, Lx, Ly, Lz, Lxmag, Lymag, Lzmag, qx, qy, qz;

		Lx = xmax - xmin;
		Ly = ymax - ymin;
		Lz = zmax - zmin;

		Lxmag = Lx - 2*inset;
		Lymag = Ly - 2*inset;
		Lzmag = Lz - 2*inset;

		qx = 0.5*(xmax+xmin);
		qy = 0.5*(ymax+ymin);
		qz = 0.5*(zmax+zmin);

		x = Point[0] - qx;
		y = Point[1] - qy;
		z = Point[2] - qz;
		r = std::sqrt(x*x + y*y);

		// Spatial scale of smoothing at edge of magnet
		// Make sure inset > delta*3
		G4double delta = 1.*mm;

		G4double smooth = 0.5 * ( std::tanh((r+0.5*Lxmag)/delta) + std::tanh((0.5*Lxmag-r)/delta) )
										*	0.5	* ( std::tanh((z+0.5*Lzmag)/delta) + std::tanh((0.5*Lzmag-z)/delta) );

		if (IsQuad) {
			// Assign quadrupole fields

			Bfield[0] = g * smooth * y;
			Bfield[1] = g * smooth * x;
			Bfield[2] = 0.;
			Bfield[3] = 0.;
			Bfield[4] = 0.;
			Bfield[5] = 0.;
		} else {
		  // Assign dipole fields
			Bfield[0] = Bx;
			Bfield[1] = By;
			Bfield[2] = 0.;
			Bfield[3] = 0.;
			Bfield[4] = 0.;
			Bfield[5] = 0.;

			//std::cout << "In dipole: " <<  smooth << std::endl;

			/*
			// Do some field falloff
			if (xmax-Point[0]<1.*mm) Bfield[0] = Bfield[0] * 0.5*(1. + std::tanh(4*(xmax-Point[0])/mm-8.));
			if (ymax-Point[1]<1.*mm) Bfield[0] = Bfield[0] * 0.5*(1. + std::tanh(4*(ymax-Point[1])/mm-8.));
			if (zmax-Point[2]<1.*mm) Bfield[0] = Bfield[0] * 0.5*(1. + std::tanh(4*(zmax-Point[2])/mm-8.));

			if (Point[0]-xmin<1.*mm) Bfield[0] = Bfield[0] * 0.5*(1. + std::tanh(4*(Point[0]-xmin)/mm-8.));
			if (Point[1]-ymin<1.*mm) Bfield[0] = Bfield[0] * 0.5*(1. + std::tanh(4*(Point[1]-ymin)/mm-8.));
			if (Point[2]-zmin<1.*mm) Bfield[0] = Bfield[0] * 0.5*(1. + std::tanh(4*(Point[2]-zmin)/mm-8.));

			if (xmax-Point[0]<1.*mm) Bfield[1] = Bfield[1] * 0.5*(1. + std::tanh(4*(xmax-Point[0])/mm-8.));
			if (ymax-Point[1]<1.*mm) Bfield[1] = Bfield[1] * 0.5*(1. + std::tanh(4*(ymax-Point[1])/mm-8.));
			if (zmax-Point[2]<1.*mm) Bfield[1] = Bfield[1] * 0.5*(1. + std::tanh(4*(zmax-Point[2])/mm-8.));

			if (Point[0]-xmin<1.*mm) Bfield[1] = Bfield[1] * 0.5*(1. + std::tanh(4*(Point[0]-xmin)/mm-8.));
			if (Point[1]-ymin<1.*mm) Bfield[1] = Bfield[1] * 0.5*(1. + std::tanh(4*(Point[1]-ymin)/mm-8.));
			if (Point[2]-zmin<1.*mm) Bfield[1] = Bfield[1] * 0.5*(1. + std::tanh(4*(Point[2]-zmin)/mm-8.));
			*/
		}

	} else {
		Bfield[0] = 0.;
		Bfield[1] = 0.;
		Bfield[2] = 0.;
		Bfield[3] = 0.;
		Bfield[4] = 0.;
		Bfield[5] = 0.;
	}

}
