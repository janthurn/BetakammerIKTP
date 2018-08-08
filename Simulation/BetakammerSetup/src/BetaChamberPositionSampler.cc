
//---------------------------------------------------------------------------//
//              BetaChamber Simulation for IKTP TU Dresden
//
//                              Position Sampler
//---------------------------------------------------------------------------

//system headers
#include <iostream>

//Geant4 headers
#include "Randomize.hh"
#include "G4LogicalVolume.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4SubtractionSolid.hh"
#include "G4VisExtent.hh"
#include "G4Navigator.hh"
#include "G4TransportationManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

//MaGe includes
#include "BetaChamberPositionSampler.hh"


using namespace std;

BetaChamberPositionSampler::BetaChamberPositionSampler() :
  fMyVolume(0)
{
  fVolumeName="NULL";
  fMyNavigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
 // fGeneratorUtil = new MGGeneratorUtil();
  fCopyNumber = 0;
  fSolid_type = "NULL";
 // fSolidDaughter1_type = "NULL";
 // fSolidDaughter2_type = "NULL";
//  fRadius = 0;
 // fMessenger = new MGGeneratorPositionSamplingMessenger(this);

  //default values for changable variables through messenger
  //fInnerCylinderRadius   = 0*cm;
 // fInnerSphereRadius     = 0*cm;
  //fInnerDiskRadius       = 0*cm;
  //fCylinderStartingAngle = 0 * deg;
  //fCylinderSpanningAngle = 360 * deg;
  //fXCenter = 0.;
  //fYCenter = 0.;
  //fZCenter = 0.;
  //fUseCenterManually = false;
  //fNCalls = 0;
  //fNTries = 0;

  //half-size of the world volume, used for the dummy vertex
 // fWorldVolumeHalfLength = -1;





}
//================================================================================
//================================================================================
//================================================================================

BetaChamberPositionSampler::~BetaChamberPositionSampler()
{

}

//================================================================================
//================================================================================
//================================================================================

G4ThreeVector BetaChamberPositionSampler::SampleUniformlyInVolume(G4String volName,G4int copyN){

  G4ThreeVector vertex_world(0.,0.,0.); //vertex in world coordinates

  //G4ThreeVector vertex_dummy(-0.999*fWorldVolumeHalfLength,
  //			     -0.999*fWorldVolumeHalfLength,
  //			     -0.999*fWorldVolumeHalfLength); //dummy vertex in world coordinates if no point
  //beloning to sampled volume could be found

  if (volName == "NULL")    {
      G4cout <<"The confinement is set but the volume was not specified" << "\n";
      G4cout << "Return default position" << "\n";
       return vertex_world;
  }
  // ===========================================================================
  //Step 1: check if the volume changed since the last call
  G4bool ifound = true;
  if (volName != fVolumeName || copyN != fCopyNumber) {  //volume changed
      fVolumeName = volName;
      fCopyNumber = copyN;
      ifound = InitializeSamplingVolume();
      if (!ifound)	{
	         G4cout <<"The confinement is set but the volume was not specified" << "\n";
	         G4cout << "Return default position" << "\n";
	         return vertex_world;
	  }
      else	{	  G4cout << "Volume initialized" << "\n"; }
  }
  // ===========================================================================
  //Step 2: sample the point in the three cases (Box,Tube,Orb)
  G4ThreeVector VolumePosition = fMyVolume->GetObjectTranslation();
  // G4cout << "VolumePosition = ("<<VolumePosition.x()<<","<<VolumePosition.y()<<","<<VolumePosition.z() <<")\n";
  G4ThreeVector vertex;       // vertex in local volume's coordinates
  // G4ThreeVector vertex_Zshift(0.,0.,zshift) ;
  // G4cout << "vertex_Zshift = ("<<vertex_Zshift.x()<<","<<vertex_Zshift.y()<<","<<vertex_Zshift.z() <<")\n";
  //G4cout << "Solid type: " << fSolid_type << "\n";

  if (fSolid_type == "G4Box") {
	G4double x_lo = fSolid_par_arr[0];
	G4double x_hi = fSolid_par_arr[1];
	G4double y_lo = fSolid_par_arr[2];
	G4double y_hi = fSolid_par_arr[3];
	G4double z_lo = fSolid_par_arr[4];
	G4double z_hi = fSolid_par_arr[5];
    vertex = pick_point_in_box(x_lo, x_hi, y_lo, y_hi,z_lo, z_hi);
	vertex_world = vertex+VolumePosition;
  }
  else if (fSolid_type == "G4Tubs"  ){
      G4double r1      = fSolid_par_arr[0];
      G4double r2      = fSolid_par_arr[1];
      G4double h       = fSolid_par_arr[2] * 2.0;
      vertex = pick_point_in_annulus(r1, r2, h);
    //   G4cout << "vertex = ("<<vertex.x()<<","<<vertex.y()<<","<<vertex.z() <<")\n";
      vertex_world = vertex+VolumePosition;
    //   G4cout << "vertex_world = ("<<vertex_world.x()<<","<<vertex_world.y()<<","<<vertex_world.z() <<")\n";
  }
  else if (fSolid_type == "G4Orb"){
      G4double r      = fSolid_par_arr[0];
      vertex = pick_point_in_orb(r);
  	  vertex_world = vertex+VolumePosition;
    //   G4double VertexRadius = sqrt(vertex.x()*vertex.x()+vertex.y()*vertex.y()+vertex.z()*vertex.z());
  }
  else if (fSolid_type == "G4Sphere"){
      G4double r      = fSolid_par_arr[0];
      vertex = pick_point_in_sphere(r);
  	  vertex_world = vertex+VolumePosition;
  }
//  G4cout << "The point " << vertex_world/mm << " is in ";
//  G4cout << fMyNavigator->LocateGlobalPointAndSetup(vertex_world)->GetName() << "\n";


  if (fMyNavigator->LocateGlobalPointAndSetup(vertex_world) != fMyVolume){
    //   G4cout << "The point " << vertex_world/mm << " is in ";
    //   G4cout << fMyNavigator->LocateGlobalPointAndSetup(vertex_world)->GetName() << "\n";
    //   G4cout << "The sampled point is not in the right volume!?!" << "\n";
    // G4cout << "Volume center (x,y,z)= ( "  << fMyVolume->GetObjectTranslation().x()/mm<<" , ";
    // G4cout <<  fMyVolume->GetObjectTranslation().y()/mm<<" , ";
    // G4cout   << fMyVolume->GetObjectTranslation().z()/mm<<" )\n ";




    G4cout << "Returning dummy vertex" << "\n";
    vertex_world = VolumePosition;
  }

  return vertex_world;
}

//================================================================================
//================================================================================
//================================================================================

G4bool BetaChamberPositionSampler::InitializeSamplingVolume()
{
  G4cout << "Starting InitializeSamplingVolume" << "\n";

  //------------------------------------------------------------------------------------
  // ----- Trying to find the physical volume in the store -----------------------------
  //------------------------------------------------------------------------------------

  G4cout << "Volume to be found: " << fVolumeName << "\n";

  //For now it works if there are no copies
  G4PhysicalVolumeStore* volumeStore = G4PhysicalVolumeStore::GetInstance();
  G4int nVolumes = (G4int) volumeStore->size();
  G4bool ifound = false;
  G4int nfound=0;
  G4int i;
  for (i=0; i<nVolumes; i++) {
    if ((*volumeStore)[i]->GetName() ==fVolumeName ) {
      ifound = true;
      nfound++;
      fMyVolume = (*volumeStore)[i];
    }
  }
  //--------------------------------
  if (nfound > 1) {
    ifound = false;
    G4cout << "The volume name is not unique: retrieve copy number" << "\n";
    for (i=0; i<nVolumes; i++) {
      if ((*volumeStore)[i]->GetName() == fVolumeName &&
	  (*volumeStore)[i]->GetCopyNo() == fCopyNumber && !ifound){
	ifound = true;
	fMyVolume = (*volumeStore)[i];
      }
    }
  }
  //--------------------------------
  if (!ifound) return ifound;

  G4cout << "Found volume: " << fMyVolume->GetName() << ", copy " << fMyVolume->GetCopyNo() << "\n";

  //------------------------------------------------------------------------------------
  // --- Get solid type and  store the dimensions of solids for Tubs and Boxes ---------
  //------------------------------------------------------------------------------------

  G4VSolid* pSolid = fMyVolume->GetLogicalVolume()->GetSolid();
  fHasDaughters = (fMyVolume->GetLogicalVolume()->GetNoDaughters() != 0);
  fSolid_type = pSolid->GetEntityType();

  G4cout << "pSolid->GetEntityType()= " << fSolid_type << "\n";
  G4cout << "Implemented Solid types: Box,Tube,Orb \n" ;
  //--------------------------------

  if (fSolid_type == "G4Box"){
	G4cout << "Identified source solid type as G4Box." << "\n";
	G4double x_half_length = ((G4Box*)pSolid)->GetXHalfLength();
	G4double y_half_length = ((G4Box*)pSolid)->GetYHalfLength();
	G4double z_half_length = ((G4Box*)pSolid)->GetZHalfLength();
    //      G4cout << "X half-length (mm): " << x_half_length << "\n";
    //      G4cout << "Y half-length (mm): " << y_half_length << "\n";
    //      G4cout << "Z half-length (mm): " << z_half_length << "\n";

	fSolid_par_arr[0] = -x_half_length;
	fSolid_par_arr[1] =  x_half_length;
	fSolid_par_arr[2] = -y_half_length;
	fSolid_par_arr[3] =  y_half_length;
	fSolid_par_arr[4] = -z_half_length;
	fSolid_par_arr[5] =  z_half_length;
  }
  //--------------------------------
  else if (fSolid_type == "G4Tubs"){
	G4cout << "Identified source solid type as G4Tubs." << "\n";
	fSolid_par_arr[0] = ((G4Tubs*)pSolid)->GetInnerRadius();
	fSolid_par_arr[1] = ((G4Tubs*)pSolid)->GetOuterRadius();
	fSolid_par_arr[2] = ((G4Tubs*)pSolid)->GetZHalfLength();
	fSolid_par_arr[3] = ((G4Tubs*)pSolid)->GetStartPhiAngle();
	fSolid_par_arr[4] = ((G4Tubs*)pSolid)->GetDeltaPhiAngle();
	//G4cout << "Inner radius (mm):        " << fSolid_par_arr[0] << "\n";
	//G4cout << "Outer radius (mm):        " << fSolid_par_arr[1] << "\n";
	//G4cout << "Z half-length (mm):       " << fSolid_par_arr[2] << "\n";
	//G4cout << "Starting phi angle (rad): " << fSolid_par_arr[3] << "\n";
	//G4cout << "Delta phi angle (rad):    " << fSolid_par_arr[4] << "\n";
    }
    else if (fSolid_type == "G4Orb"){
        G4cout << "Identified source solid type as G4Orb." << "\n";
        fSolid_par_arr[0] = ((G4Orb*)pSolid)->GetRadius();
    }
    else if (fSolid_type == "G4Sphere"){
        G4cout << "Identified source solid type as G4Sphere." << "\n";
        fSolid_par_arr[0] = ((G4Sphere*)pSolid)->GetOuterRadius();
    }
   return ifound;
}

//====================================================================================
//====================================================================================
//====================================================================================

G4ThreeVector BetaChamberPositionSampler::pick_point_in_box(G4double x_lo, G4double x_hi,
                                G4double y_lo, G4double y_hi,
                                G4double z_lo, G4double z_hi) {
  G4double dx = x_hi - x_lo;
  G4double dy = y_hi - y_lo;
  G4double dz = z_hi - z_lo;

  G4double x = x_lo + dx*G4UniformRand();
  G4double y = y_lo + dy*G4UniformRand();
  G4double z = z_lo + dz*G4UniformRand();
  //G4cout <<"z= "<< z << " mm"<<endl;
  G4ThreeVector pos(x,y,z);

  return pos;
}

//====================================================================================
//====================================================================================
//====================================================================================

G4ThreeVector BetaChamberPositionSampler::pick_point_in_annulus(G4double r1, G4double r2, G4double h)
{
  G4double a = r1*r1;
  G4double b = r2*r2 - a;

  G4double x_rnd = G4UniformRand();
  G4double r = sqrt(b*x_rnd + a);

  G4double phi = twopi * G4UniformRand();

  G4double x = r * cos(phi);
  G4double y = r * sin(phi);

  G4double z = h * (-0.5 + G4UniformRand());

  G4ThreeVector pos(x,y,z);

  return pos;

}

//====================================================================================
//====================================================================================
//====================================================================================

G4ThreeVector BetaChamberPositionSampler::pick_point_in_orb(G4double r){

    G4double radial = r * G4UniformRand();
    G4double phi    = twopi * G4UniformRand();
    G4double theta  = pi * G4UniformRand();

    // G4cout << "Random point in Orb of Radius: "<< r/mm*1e3<< " um" << "\n";
    // G4cout << "("<< radial/mm <<"mm , "<< (phi/twopi*360)<<" , "<< (theta/pi*180) <<")";
    G4double x = radial *sin(theta)*cos(phi);
    G4double y = radial *sin(theta)*sin(phi);
    G4double z = radial *cos(theta);
    // G4cout << " -> ("<< x/mm <<"mm , "<< y/mm <<" mm , "<< z/mm <<" mm)" << "\n";

      G4ThreeVector pos(x,y,z);
      return pos;

}

//====================================================================================
//====================================================================================
//====================================================================================

G4ThreeVector BetaChamberPositionSampler::pick_point_in_sphere(G4double r){

    G4double radial = r * G4UniformRand();
    G4double phi    = twopi * G4UniformRand();
    G4double theta  = 0.5*pi * G4UniformRand();

    // G4cout << "Random point in Orb of Radius: "<< r/mm*1e3<< " um" << "\n";
    // G4cout << "("<< radial/mm <<"mm , "<< (phi/twopi*360)<<" , "<< (theta/pi*180) <<")";
    G4double x = radial *sin(theta)*cos(phi);
    G4double y = radial *sin(theta)*sin(phi);
    G4double z = radial *cos(theta);
    // G4cout << " -> ("<< x/mm <<"mm , "<< y/mm <<" mm , "<< z/mm <<" mm)" << "\n";

      G4ThreeVector pos(x,y,z);
      return pos;

}
