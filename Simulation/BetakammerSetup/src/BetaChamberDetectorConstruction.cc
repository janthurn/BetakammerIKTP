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
// $Id: BetaChamberDetectorConstruction.cc 94307 2015-11-11 13:42:46Z gcosmo $
//
/// \file BetaChamberDetectorConstruction.cc
/// \brief Implementation of the BetaChamberDetectorConstruction class

#include "BetaChamberDetectorConstruction.hh"
#include "BetaChamberDetectorConstructionMessenger.hh"
#include "BetaChamberSiliconSD.hh"
#include "BetaChamberAbsorberSD.hh"

#include <globals.hh>
#include <G4Tubs.hh>
#include <G4Box.hh>
#include <G4Cons.hh>
#include <G4Paraboloid.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4Material.hh>
// #include <G4Element.hh>
#include <G4VisAttributes.hh>
#include <G4Sphere.hh>
#include <G4PVPlacement.hh>
#include <G4OpticalSurface.hh>
#include <G4LogicalBorderSurface.hh>
#include <G4LogicalSkinSurface.hh>
#include <G4RotationMatrix.hh>
#include <G4Colour.hh>
#include <G4SDManager.hh>
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"
#include <G4MaterialPropertiesTable.hh>
// #include <G4Polyhedra.hh>
#include <G4SubtractionSolid.hh>
#include <G4NistManager.hh>
#include <G4Ellipsoid.hh>
#include <G4UnionSolid.hh>
#include <G4SubtractionSolid.hh>
#include <G4Trd.hh>
#include <G4Torus.hh>
#include <G4ThreeVector.hh>
#include <G4EllipticalTube.hh>
#include <G4TransportationManager.hh>
#include <G4Navigator.hh>
#include <G4Orb.hh>
#include <G4Region.hh>
#include <G4RegionStore.hh>
#include <G4UserLimits.hh>
#include "G4StepLimiter.hh"
#include "G4SystemOfUnits.hh"

using std::vector;
using std::string;
using std::stringstream;



//================================================================================
//================================================================================
//================================================================================

BetaChamberDetectorConstruction::BetaChamberDetectorConstruction() : G4VUserDetectorConstruction(),
                        //fEvtaction(eventAction),BetaChamberEventAction* eventAction
                        fScoringVolume(0),SiliconChipSD(0),
                        AbsorberSD(0),
                        fsampleShift(0.),
                        SondenNummer(10),SetupNummer(1),PositionsNummer(1),
                        CalibSetup(1),CalibMaterial(1),CalibSourceSpotDiameter(1),
                        SiliconSampleThickness(467),BismutSampleThickness(1.0),AllowedTransitionMaterial(1),
                        UseSiliconBacking(false),ATSampleShape(1),
                        fPhysicsAbsorber(0),fPhysicsSens(0)
 {
     G4cout <<"Generator of BetaChamberDetectorConstruction"<<"\n";
     fMessenger = new BetaChamberDetectorConstructionMessenger(this);
     SetDetectorDimensions(SiliconSampleThickness,BismutSampleThickness);
     fNameSampleVol= "BismutSampleLOG";
     SetShifts(SetupNummer,PositionsNummer,SondenNummer);
     SetSampleVolumeName(SetupNummer);
     SetSensVol(SiliconChipPHYS);
}

//================================================================================
//================================================================================
//================================================================================

BetaChamberDetectorConstruction::~BetaChamberDetectorConstruction(){
 delete fMessenger;
}

//================================================================================
//====================---- GetDetectorDimensions ---==============================
//================================================================================

G4double BetaChamberDetectorConstruction::GetDimensions(const char* entry) {
	return fDetDimensions[entry];
}

G4double BetaChamberDetectorConstruction::GetDimensions(string entry) {
	return fDetDimensions[entry];
}

//================================================================================
//====================---- SetDetectorDimensions ---==============================
//================================================================================

void BetaChamberDetectorConstruction::SetDetectorDimensions(G4double SiDicke,G4double BismutDicke) {

    G4cout << "BetaChamberDetectorConstruction::SetDetectorDimensions("<< SiDicke<<"," << BismutDicke<<") \n";

      //================================== Labor =================================
    	fDetDimensions["LabHeight"]				= 2.5*m;
    	fDetDimensions["LabLength"]				= 3.*m;
    	fDetDimensions["LabWidth"]				= 2.5*m;

      //============================== neue Probenhalter =============================

            fDetDimensions["GrundKoerperL1"]=26.70*mm;
            fDetDimensions["GrundKoerperL2"]=27.70*mm;
            fDetDimensions["GrundKoerperL3"]=35.0*mm;
            fDetDimensions["GrundKoerperL4"]=40.0*mm;
            fDetDimensions["GrundKoerperD1"]=24.0*mm;
    	    fDetDimensions["GrundKoerperD2"]=20.0*mm;
            fDetDimensions["GrundKoerperD3"]=29.0*mm;
            fDetDimensions["GrundKoerperD4"]=32.0*mm;

    	    fDetDimensions["HalterSondeL1"]=1.0*mm;
            fDetDimensions["HalterSondeL2"]=4.0*mm;
    	    fDetDimensions["HalterSondeD1"]=15.0*mm;
            fDetDimensions["HalterSondeD2"]=20.0*mm;
            fDetDimensions["HalterSondeD3"]=24.0*mm;
    		fDetDimensions["HalterSondeD4"]=GetDimensions("HalterSondeD1")+2*GetDimensions("HalterSondeL1");


    		fDetDimensions["MylarDicke"]=5e-3*mm;

    		fDetDimensions["BismutSchichtDickeStandard"]=2.0e-3*mm;
    		fDetDimensions["BismutSchichtDickeSonde4"]=1.6e-3*mm;
    		fDetDimensions["BismutSchichtDickeSonde5"]=2.1e-3*mm;
    		fDetDimensions["BismutSchichtDickeSonde6"]=1.8e-3*mm;
    		fDetDimensions["BismutSchichtDickeSonde7"]=3e-3*mm;
            fDetDimensions["BismutSchichtDickeSystematics"]=BismutDicke*1.0e-3*mm;

    		fDetDimensions["DistanzRingL1"]=8.0*mm;
    	    fDetDimensions["DistanzRingD1"]=GetDimensions("HalterSondeD2");
    		fDetDimensions["DistanzRingD2"]=GetDimensions("HalterSondeD3");

    		fDetDimensions["GegenSondeL1"]=1.0*mm;
    	    fDetDimensions["GegenSondeD1"]=GetDimensions("HalterSondeD1");
    		fDetDimensions["GegenSondeD2"]=GetDimensions("HalterSondeD3");

    		fDetDimensions["FixierRingL1"]=5.0*mm;
    	    fDetDimensions["FixierRingD1"]=18.0*mm;
    		fDetDimensions["FixierRingD2"]=GetDimensions("HalterSondeD3");

    		fDetDimensions["SiWaferDicke"]=SiDicke*1.0e-3*mm;//Average from 3 measurements

            fDetDimensions["KupferKammerRadius"]	= GetDimensions("GrundKoerperD4")*10;
            fDetDimensions["KupferKammerHohe"]		= GetDimensions("GrundKoerperL4")+10*cm;
            fDetDimensions["KupferDeckelDicke"]		= 2*cm;
            fDetDimensions["VakuumRadius"]			= GetDimensions("KupferKammerRadius")-10*mm;

            //-------- Silizium Probenhalter -------------
            fDetDimensions["GegenSondeL1SiSetup"]=1.0*mm;
    	    fDetDimensions["GegenSondeD1SiSetup"]=13.0*mm;
    		fDetDimensions["GegenSondeD2SiSetup"]=23.6*mm;

            fDetDimensions["HalterSondeTrapezD1"]=4.5*mm;
            fDetDimensions["HalterSondeTrapezD2"]=6.0*mm;
            fDetDimensions["HalterSondeTrapezLZ"]=0.501*mm;
            fDetDimensions["HalterProbenEinfassungD1"]=0.1*mm;


    	 //=============================== PIPS-Detektor ===============================


    		fDetDimensions["PIPSTopRingInnenRadius"]= GetDimensions("PIPSTopZylinderAußenRadius");
    		fDetDimensions["PIPSTopRingAußenRadius"]= 0.5*8.0*mm;
    		fDetDimensions["PIPSTopRingHoehe"]= 3.98*mm;

    		fDetDimensions["PIPSHousingTopInnenRadius"]= GetDimensions("PIPSTopZylinderAußenRadius");
    		fDetDimensions["PIPSHousingTopAußenRadius"]= 0.5*27.1*mm;
    		fDetDimensions["PIPSHousingTopHoehe"]= 3.98*mm;

    		fDetDimensions["PIPSHousingShellAußenRadius"]= 0.5*28.69*mm;
    		fDetDimensions["PIPSHousingShellHoehe"]= 11.79*mm;

    		fDetDimensions["PIPSHousingBottomInnenRadius"]=0.5*20.0*mm;
    		fDetDimensions["PIPSHousingBottomAußenRadius"]= GetDimensions("PIPSHousingShellAußenRadius");
    		fDetDimensions["PIPSHousingBottomHoehe"]= 0.51*mm;

    		fDetDimensions["PIPSMessingStiftHoehe"]= 13.75*mm;
    		fDetDimensions["PIPSMessingPlatteAußenRadius"]=0.5*23.94*mm;
    		fDetDimensions["PIPSMessingPlatteHoehe"]= 0.99*mm;

    		fDetDimensions["PIPSPEBackingHoehe"]= 3.99*mm;


    		fDetDimensions["PIPSSiliziumChipAußenRadius"]=9.97*mm;
    		fDetDimensions["PIPSSiliziumChipHoeheStandard"]= 1.0*mm;

    		fDetDimensions["PIPSSiliziumChipHoeheDetektor1"]= 0.86*mm;
    		fDetDimensions["PIPSSiliziumChipHoeheDetektor3"]= 0.86*mm;
    		fDetDimensions["PIPSSiliziumChipHoeheDetektor4"]= 0.83*mm;
    		fDetDimensions["PIPSSiliziumChipHoeheDetektor5"]= 0.76*mm;


    		//----- Siliconchip in to parts-------
    		 fDetDimensions["PIPSSiliziumChipTubeAußenRadius"]=9.97*mm;
    	       // 	fDetDimensions["PIPSSiliziumChipTubeHoehe"]=0.75*mm;  // Version 2
    	          // 	fDetDimensions["PIPSSiliziumChipConeHoehe"]=0.25*mm;  // Version 2





    		fDetDimensions["PIPSElastomerRingAußenRadius"]=0.5*21.55*mm;
    		fDetDimensions["PIPSElastomerRingBottomInnenRadius"]=GetDimensions("PIPSHousingBottomInnenRadius");
    		fDetDimensions["PIPSElastomerRingBottomHoehe"]=0.49*mm;
    		fDetDimensions["PIPSElastomerRingTopHoehe"]=0.51*mm;

    	        fDetDimensions["PIPSTopZylinderInnenRadius"]= 0.5*0.98*mm;
    		fDetDimensions["PIPSTopZylinderAußenRadius"]= 0.5*6.0*mm;
    		fDetDimensions["PIPSTopZylinderHoehe"]= 3.0*mm+GetDimensions("PIPSTopRingHoehe")+GetDimensions("PIPSHousingTopHoehe");

    		fDetDimensions["PIPSSilberKontaktAußenRadius"]=GetDimensions("PIPSHousingBottomInnenRadius");
    	// 	fDetDimensions["PIPSSilberKontaktDicke"]=14e-6*mm;
    		fDetDimensions["PIPSSilberKontaktDicke"]=50e-6*mm;

    	//=============================== Kalibrierprobe ===============================

    		fDetDimensions["KalProbeBottomPartHoehe"]			= 4.*mm;
    		fDetDimensions["KalProbeBottomPartAußenDurchmesser"]		= 24.*mm;
    		fDetDimensions["KalProbeBottomPartAußenDurchmesserkurz"]	= 20.*mm;
    		fDetDimensions["KalProbeBottomPartInnenDurchmesser"]		= 10.*mm;
    		fDetDimensions["KalProbeBottomPartLochTiefe"]			= 3.*mm;

    		fDetDimensions["KalProbeMylarDicke"]				= 5.e-3*mm;
    		fDetDimensions["KalProbeTopRingHoehe"]			= 1.*mm;
    		fDetDimensions["KalibrierprobeRadiusKugel"] = 5.65*mm;
    		fDetDimensions["KalibrierprobeDurchmesserORB"] = 1e-3*mm;

    	//=============================== Scan Probe ===============================

    		fDetDimensions["ScanSondeInnenRadius"] = 2.0*mm;
    		fDetDimensions["ScanSondeAußenRadius"] = 31.6*mm;
    		fDetDimensions["ScanSondePMMAHoehe"] = 3.0*mm;
    		fDetDimensions["ScanSondeBleiHoehe"] = 4.0*mm;
    		fDetDimensions["ScanSondeAluHoehe"] = 4.0*mm;
    		fDetDimensions["ScanSondeQuelleRadius"] = 0.1*mm;
    		fDetDimensions["ScanSondeQuellePosition"] = 1.5*mm;

    	//=============================== thickness measurement ====================

    		fDetDimensions["ThicknessSiSchichtdicke"] = 467e-6*mm;
    	        fDetDimensions["ThicknessBiSchichtdicke"] = 2*mm;
    	        fDetDimensions["ThicknessAußenRadius"] = 50*mm;
    	        fDetDimensions["ThicknessInnenRadius"]=1*mm;
    		fDetDimensions["ThicknessGeDetektorDicke"]=10*mm;
    	        fDetDimensions["ThicknessGeDetektorAbstand"]=50*mm;
    	        fDetDimensions["ThicknessKupferRingHoehe"]=30*mm;
    	        fDetDimensions["ThicknessPMMAHoehe"]=5*mm;
    	        fDetDimensions["ThicknessCMRadius"]=0.1*mm;
    	        fDetDimensions["ThicknessHuelleHohe"]=300*mm;






    	//================================== Deckel =================================


    		fDetDimensions["AußenrohrDurchmesser"]			= 15*cm;
    		fDetDimensions["AußenrohrWandstärke"]			= 10.*mm;
    		fDetDimensions["AußenrohrHohe"]				= 120.*mm;

    		fDetDimensions["DeckelKupferplatteDickeklein"]		= 10.*mm;
    		fDetDimensions["DeckelKupferplatteDickegross"]		= 13.*mm;

    		fDetDimensions["LoecherFuerDeteckterDurchmesser"]	= 2*cm;
    		fDetDimensions["LochRingDicke"]				= 3*mm;
    		fDetDimensions["LochRingInnendurchmesser"]		= 13*mm;

    		fDetDimensions["DichtRingDicke"]			= 1.0*mm;

    		fDetDimensions["HoeheVakuum"]= GetDimensions("AußenrohrHohe")+2*GetDimensions("DichtRingDicke")
    						-(GetDimensions("DeckelKupferplatteDickegross")-GetDimensions("DeckelKupferplatteDickeklein"))
    						-(GetDimensions("BodenKupferplatteDickegross")-GetDimensions("BodenKupferplatteDickeklein"));

    	 //================================== Boden =================================

    		fDetDimensions["BodenKupferplatteDickeklein"]		= 10.*mm;
    		fDetDimensions["BodenKupferplatteDickegross"]		= 15.*mm;
    		fDetDimensions["Flanschdurchmesser"]			= 20.*mm;
    		fDetDimensions["FlanschWandstaerke"]			= 3.*mm;

    	 //================================== Winkel =================================
    		// -- großer Winkel --
    		fDetDimensions["FSCuWinkelBreite"]			= 40.*mm;
    		fDetDimensions["FSCuWinkelBodenTiefe"]			= 35.*mm;
    		fDetDimensions["FSCuWinkelBodenHoehe"]			= 1.*mm;

    		fDetDimensions["FSCuWinkelSenkrechtTiefe"]		= 1.*mm;
    		fDetDimensions["FSCuWinkelSenkrechtHoehe"]		= 75.*mm;

    		fDetDimensions["FSDelrinSenkrechtTiefe"]		= 3.*mm;
    		fDetDimensions["FSDelrinSenkrechtBreite"]		= 60.*mm;
    		fDetDimensions["FSDelrinSenkrechtHoehe"]		= 92.*mm;

    		// -- kleiner Winkel --

    		fDetDimensions["FSKCuWinkelBreite"]			= 20.*mm;
    		fDetDimensions["FSKDelrinSenkrechtBreite"]		= 40.*mm;

    	 //=============================== Probenhalter ===============================

    		fDetDimensions["PHDicke"]			= 2.*mm;

    		fDetDimensions["PHT1KanteY"]			= 80.*mm;
    		fDetDimensions["PHT1KanteX"]			= 100.*mm;

    		fDetDimensions["PHT2KanteYOben"]			= 66.*mm;
    		fDetDimensions["PHT2KanteYUnten"]			= 100.*mm;
    		fDetDimensions["PHT2KanteZ"]			= 15.*mm;

    		fDetDimensions["PHT3LangeHA"]			= 100.*mm;
    		fDetDimensions["PHT3KurzeHA"]			= 44.*mm;


        // ============================= shifts =====================================


}

//================================================================================
//========================---- Set Shifts ---=====================================
//================================================================================

void BetaChamberDetectorConstruction::SetShifts(G4int Setup, G4int Position,G4int sonde){

    G4cout << "------------------------------------------------------------- \n";
    G4cout << "Setup = " <<Setup <<"\n";
    G4cout << "Position = " <<Position <<"\n";
    G4double Shift=0.*mm;

    G4double H1 = 0.5*GetDimensions("GrundKoerperL1");
    G4double DickeDistanceRing = GetDimensions("DistanzRingL1");

    //-------------------------------------------------------------------------------
    //============= 25 um setup without positioning option so far ===================
    //-------------------------------------------------------------------------------

    if(Setup==1){
        Shift      	= H1
                     -1.0*(GetDimensions("HalterSondeL2")-GetDimensions("HalterSondeL1"))
                     -GetDimensions("HalterSondeL1")
                     -0.5*(25e-3*mm);
    }
    //-------------------------------------------------------------------------------
    //================ calibration setup with positioning option ====================
    //-------------------------------------------------------------------------------
    else if(Setup==2){
        G4double TopRingPlatzierung=H1-0.5*GetDimensions("KalProbeTopRingHoehe");
        G4double MylarPlatzierung = TopRingPlatzierung-0.5*GetDimensions("KalProbeTopRingHoehe")-0.5*GetDimensions("KalProbeMylarDicke");
        G4double ProbePlatzierung = MylarPlatzierung-0.5*GetDimensions("KalProbeMylarDicke")-GetDimensions("KalProbeBottomPartLochTiefe")+ 0.6*GetDimensions("KalibrierprobeDurchmesserORB");
        G4cout << "ProbePlatzierung = "<< ProbePlatzierung/mm <<" mm \n";
        if(Position==1)Shift = ProbePlatzierung;
        else if(Position==2)Shift = ProbePlatzierung - DickeDistanceRing;
        else if(Position==3)Shift = ProbePlatzierung - 2*DickeDistanceRing;
    }
    //-------------------------------------------------------------------------------
    //================ 2um setup with positioning option ============================
    //-------------------------------------------------------------------------------

    else if(Setup==3){

        G4double sampleDicke;
        if(sonde ==10){
            sampleDicke = GetDimensions("BismutSchichtDickeStandard");
        } else{ // there is maybe a quick, easier way to do this, but it works!
           std::string search;
           std::string teil1 = "BismutSchichtDickeSonde";
           stringstream ss;
           ss << sonde;
           std::string teil2 =ss.str();
           search +=teil1;
           search +=teil2;
           sampleDicke = GetDimensions(search);
        }

        G4double HalterSondenPlatzierungPart2 = H1-1.0*(GetDimensions("HalterSondeL2")-GetDimensions("HalterSondeL1"))-0.5*GetDimensions("HalterSondeL1");
        G4double BiSamplePlatzierung= HalterSondenPlatzierungPart2-0.5*GetDimensions("HalterSondeL1")-0.5*(sampleDicke);
        if(Position==1)Shift = BiSamplePlatzierung;
        else if(Position==2)Shift = BiSamplePlatzierung - DickeDistanceRing;
        else if(Position==3)Shift = BiSamplePlatzierung - 2*DickeDistanceRing;
    }


     SetSampleShift(Shift);

     G4cout << "------------------------------------------------------------- \n";
}

//================================================================================
//===================---- Set sample volume ---===================================
//================================================================================

void BetaChamberDetectorConstruction::SetSampleVolumeName(G4int Setup){

    if(Setup==1)fNameSampleVol="BiSampleLOG";
    else if(Setup==2)fNameSampleVol="probeLOG";
    else if(Setup==3)fNameSampleVol="BiSampleLOG";
    else if(Setup==4)fNameSampleVol="SiWaferLOG";
    else if(Setup==5)fNameSampleVol="ATSamplePHYS";

}

//================================================================================
//======================---- DefineMaterials ---==================================
//================================================================================

void BetaChamberDetectorConstruction::DefineMaterials(){

    //   G4NistManager* pNistManager = G4NistManager::Instance();

  //------------elements------

  //================================== elements ====================ProbeSetup%i===============

	G4Element *H  = new G4Element("Hydrogen",  "H",  1.,  1.0079*g/mole);
	G4Element *C  = new G4Element("Carbon",    "C",  6.,  12.011*g/mole);
	G4Element *N  = new G4Element("Nitrogen",  "N",  7.,  14.007*g/mole);
	G4Element *O  = new G4Element("Oxygen",    "O",  8.,  15.999*g/mole);
	G4Element *Al = new G4Element("Aluminium", "Al", 13., 26.982*g/mole);
	G4Element *Si = new G4Element("Silicon",   "Si", 14., 28.086*g/mole);
    G4Element *P  = new G4Element("Phosphor",  "P",  15., 30.974*g/mole);
    G4Element *Cr = new G4Element("Chromium",  "Cr", 24., 51.996*g/mole);
	G4Element *Mn = new G4Element("Manganese", "Mn", 25., 54.938*g/mole);
	G4Element *Fe = new G4Element("Iron",      "Fe", 26., 55.85*g/mole);
	G4Element *Ni = new G4Element("Nickel",    "Ni", 28., 58.693*g/mole);
	G4Element *Cu = new G4Element("Copper",    "Cu", 29., 63.546*g/mole);
	G4Element *Zn = new G4Element("Zinc", 	   "Zn", 30., 65.38*g/mole);
	G4Element *Ge = new G4Element("germanium", "Ge", 32., 72.630*g/mole);
	G4Element *Ag = new G4Element("Silver",    "Ag", 47., 107.8682*g/mole);
	G4Element *Cs = new G4Element("Caesium",   "Cs", 55., 132.905*g/mole);
	G4Element *Ba = new G4Element("Barium",    "Ba", 56., 137.327*g/mole);
	G4Element *Pb = new G4Element("Blei",	   "Pb", 82., 207.21*g/mole);
	G4Element *Bi = new G4Element("Bismut",    "Bi", 83., 208.980*g/mole);
    G4Element *Am = new G4Element("Americium", "Am", 95., 243.061*g/mole);
    G4Element *Cm = new G4Element("Curium",    "Cm", 96., 247.0703*g/mole);

  // ---------colors-----------------
  	G4Colour lgray (0.7,  0.7, 0.7);
  	G4Colour lgrayt (0.7,  0.7, 0.7, 0.2);
  	G4Colour pmtblue(0.8,1.0,1.0);
  	G4Colour xyellow (1.0,1.0,0.8);

  // ------------------------------------
  	const G4int iNbEntries = 3;
    //   G4double z, a, density,temperature,pressure;
    //   G4int ncomponents, natoms;
    //   G4State state;
    //
	//----------------------------------- vacuum ------------------------------------
	G4Material *Vacuum = new G4Material("Vakuum", 1.e-20*g/cm3, 2, kStateGas);
	Vacuum->AddElement(N, 0.755);
	Vacuum->AddElement(O, 0.245);


	//------------------------------------ Air -----------------------------------
    // 	G4Material* Air = pNistManager->FindOrBuildMaterial("G4_AIR");


      //----------------------------- aluminium------------------------------
	G4Material *Alu = new G4Material("Aluminium", 2.7*g/cm3, 1, kStateSolid);
	Alu->AddElement(Al, 1);
    //----------------------------- aluminium------------------------------
    G4Material *saphir = new G4Material("Saphir", 3.95*g/cm3, 2, kStateSolid);
    saphir->AddElement(Al, 2);
    saphir->AddElement(O, 3);
      //------------------------------------ Caesium -----------------------------------
	G4Material *Caesium = new G4Material("Caesium", 1.9*g/cm3, 1);
	Caesium->AddElement(Cs, 1);
      //------------------------------------ copper -----------------------------------
	G4Material *Copper = new G4Material("Copper", 8.92*g/cm3, 1);
	Copper->AddElement(Cu, 1);

	G4double pdCopperPhotonMomentum[iNbEntries] = {1.91*eV, 6.98*eV, 7.05*eV};//{6.91*eV, 6.98*eV, 7.05*eV};
	G4double pdCopperReflectivity[iNbEntries]   = {0.15,    0.2,     0.15};
	G4MaterialPropertiesTable *pCopperPropertiesTable = new G4MaterialPropertiesTable();

	pCopperPropertiesTable->AddProperty("REFLECTIVITY", pdCopperPhotonMomentum, pdCopperReflectivity, iNbEntries);
	Copper->SetMaterialPropertiesTable(pCopperPropertiesTable);

      //------------------------------------ silizium -----------------------------------
	G4Material *Silicon = new G4Material("Silicon", 2.336*g/cm3, 1);
	Silicon->AddElement(Si, 1);

    //------------------------------------ silizium 0-----------------------------------
    G4Material *Silicon0 = new G4Material("Silicon0", 0*g/cm3, 1);
    Silicon0->AddElement(Si, 1);


	//------------------------------------ Bismut -----------------------------------
	G4Material *Bismut = new G4Material("Bismut", 9.78*g/cm3, 1);
	Bismut->AddElement(Bi, 1);
        //---------------------------------- Messing --------------------------------
        G4Material *Messing = new G4Material("messing", 8.353*g/cm3, 2);
	Messing->AddElement(Cu, 0.66028);
	Messing->AddElement(Zn, 0.33972);

	//------------------------------- stainless steel -------------------------------
	G4Material *SS304LSteel = new G4Material("SS304LSteel", 8.00*g/cm3, 5, kStateSolid);
	SS304LSteel->AddElement(Fe, 0.65);
	SS304LSteel->AddElement(Cr, 0.20);
	SS304LSteel->AddElement(Ni, 0.12);
	SS304LSteel->AddElement(Mn, 0.02);
	SS304LSteel->AddElement(Si, 0.01);
		//------------------------------------ Mylar -----------------------------------
        //material for RFD
	G4Material* Mylar = new G4Material("Mylar", /*density= */1.397*g/cm3, 3);
	  Mylar->AddElement(C, 10);
	  Mylar->AddElement(H,  8);
	  Mylar->AddElement(O,  4);
      // 	  myMaterials.push_back(Mylar);

    //------------------------------------Delrin -----------------------------------

	G4Material* Delrin = new G4Material("Delrin", 1.415*g/cm3, 3);
	  Delrin->AddElement(C, 1);
	  Delrin->AddElement(O, 1);
	  Delrin->AddElement(H, 2);
      // 	myMaterials.push_back(Delrin);

	//---------------------------------- Elastomer --------------------------------
        G4Material *Elastomer = new G4Material("Elastomer", 1.3*g/cm3, 4);
	Elastomer->AddElement(C, 8);
	Elastomer->AddElement(H, 24);
	Elastomer->AddElement(O, 2);
	Elastomer->AddElement(Si, 3);
      //------------------------------------ PMMA -----------------------------------
	G4Material* PMMA = new G4Material("PMMA", /*density= */1.19*g/cm3, 3);
	  PMMA->AddElement(C, 5);
	  PMMA->AddElement(H,  8);
	  PMMA->AddElement(O,  2);

      //------------------------------------ polyethilene -----------------------------------
	G4Material *  poly = new G4Material("poly", 0.95*g/cm3, 2);
	poly->AddElement(C, 1);
	poly->AddElement(H, 2);
         //------------------------------------ Silver -----------------------------------
        G4Material *  Germanium = new G4Material("Germanium", 5.323*g/cm3, 1);
        Germanium->AddElement(Ge, 1);


	 //------------------------------------ Silver -----------------------------------
	G4Material *  Silver = new G4Material("Silver", 10.49*g/cm3, 1);
	Silver->AddElement(Ag, 1);

	//------------------------------------ Barium -----------------------------------
	G4Material *  Barium = new G4Material("Barium", 3.62*g/cm3, 1);
	Barium->AddElement(Ba, 1);

	//------------------------------------ Blei -----------------------------------
	G4Material *  Lead = new G4Material("Blei", 11.342 *g/cm3, 1);
	Lead->AddElement(Pb, 1);
	//------------------------------------ Americium -----------------------------------
	G4Material *  Amer = new G4Material("Amer", 13.67 *g/cm3, 1);
	Amer->AddElement(Am, 1);
	//------------------------------------ Americium -----------------------------------
        G4Material *  Curi = new G4Material("curium", 13.51*g/cm3, 1);
        Curi->AddElement(Cm, 1);


}

//================================================================================
//==========================--- Construct ---=====================================
//================================================================================

G4VPhysicalVolume* BetaChamberDetectorConstruction::Construct(){
    G4cout << "BetaChamberDetectorConstruction::Construct() \n";
      SetDetectorDimensions(SiliconSampleThickness,BismutSampleThickness);
      SetShifts(SetupNummer,PositionsNummer,SondenNummer);
      DefineMaterials();
      G4VPhysicalVolume* physWorld = constructLab();
    //   constructPIPS();

      if(SetupNummer==1){
          G4cout << "Setup "<<SetupNummer <<" was chosen!\n";
          constructSampleHolder25umSetup();
      }
      else if(SetupNummer==2){
          G4cout << "Setup "<<SetupNummer <<" was chosen!\n";
          constructSampleHolderCalibration();
      }
      else if(SetupNummer==3){
          G4cout << "Setup "<<SetupNummer <<" was chosen!\n";
          constructSampleHolder2umSetup(SondenNummer);
      }
      else if(SetupNummer==4){
          G4cout << "Setup "<<SetupNummer <<" was chosen!\n";
          constructSampleHoldersSiliconOnly();
      }
      else if(SetupNummer==5){
          G4cout << "Setup "<<SetupNummer <<" was chosen!\n";
          constructSampleHolderSiliziumProbenSetup();
      }
      else if(SetupNummer==6){
          G4cout << "Setup "<<SetupNummer <<" was chosen!\n";
          constructSampleHolderSaphirSetup();
      }
      else if(SetupNummer==7){
          G4cout << "Setup "<<SetupNummer <<" was chosen!\n";
          constructSampleHolderBismutProbenSystematics();
      }
      else if(SetupNummer==8){
          G4cout << "Setup "<<SetupNummer <<" was chosen!\n";
          constructSampleHolderBismutProbenFormTests();
      }
      else if(SetupNummer==9){
          G4cout << "Setup "<<SetupNummer <<" was chosen!\n";
          constructAbsorptionTests();
      }
      else G4cout << "Setup == "<<SetupNummer <<" -> does not correspond to any Setup!\n";
      constructSensitiveDetector();
  return physWorld;
}

//================================================================================
//========================---- Construct Lab ---==================================
//================================================================================


G4VPhysicalVolume* BetaChamberDetectorConstruction::constructLab(){


    G4double  hallwidth=GetDimensions("LabWidth");
    G4double  halllength=GetDimensions("LabLength");
    G4double  hallheight =GetDimensions("LabHeight");

    G4NistManager* pNistManager = G4NistManager::Instance();
    G4Material* Air = pNistManager->FindOrBuildMaterial("G4_AIR");
    G4Material *Vacuum = G4Material::GetMaterial("Vakuum");
    G4Material *Copper = G4Material::GetMaterial("Copper");

    // ----- World
    G4Box *            hall        = new G4Box("Lab",hallwidth,halllength,hallheight);
    m_phallLog  = new G4LogicalVolume(hall, Air, "Lablog",0,0,0);
    m_phallPhys = new G4PVPlacement(0,G4ThreeVector(), m_phallLog,"Lab_phys",0, false, 0);

    // ---- copper and vacuum

    G4Tubs* CopperZylinder = new  G4Tubs("CopperZylinder",0,0.5*(GetDimensions("KupferKammerRadius")),0.5*(GetDimensions("KupferKammerHohe")),0*deg,360*deg);
    G4Tubs* KupferDeckel = new G4Tubs("KupferDeckel",0,0.5*(GetDimensions("KupferKammerRadius")),0.5*GetDimensions("KupferDeckelDicke"),0*deg,360*deg);
    G4Tubs * VakuumVolumen = new G4Tubs("Vakuum",0,0.5*(GetDimensions("VakuumRadius")),0.5*(GetDimensions("KupferKammerHohe")),0*deg,360*deg);


    CopperZylinderLOG= new G4LogicalVolume(CopperZylinder, Copper, "CopperZylinderLOG",0, 0, 0);
    KupferDeckelLOG = new G4LogicalVolume(KupferDeckel, Copper, "CopperDeckelObenLOG",0, 0, 0);
    VacuumLOG= new G4LogicalVolume(VakuumVolumen, Vacuum, "VakuumLOG",0, 0, 0);

    CopperZylinderPHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,0),CopperZylinderLOG, "CopperZylinderPHYS",m_phallLog, false, 0);
    VacuumPHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,0),VacuumLOG, "VakuumPHYS",CopperZylinderLOG, false, 0);

    G4double KupferdeckelPlatzierung=0.5*(GetDimensions("KupferKammerHohe")+0.5*GetDimensions("KupferDeckelDicke"));//Halbe Länge des Kupferzylinders + 0.5*Dicke Deckel
    KupferDeckelObenPHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,KupferdeckelPlatzierung),KupferDeckelLOG, "KupferDeckelObenPHYS",m_phallLog, false, 0);
    KupferDeckelUntenPHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,-1.0*KupferdeckelPlatzierung),KupferDeckelLOG, "KupferDeckelUntePHYS",m_phallLog, false, 0);

    // ---- vis attributes
    m_phallLog->SetVisAttributes(G4VisAttributes::Invisible);



    //
    G4Colour lCopper (0.9,  0.0, 0.1);
    G4VisAttributes *hCopperVisAtt = new G4VisAttributes(lCopper);
    hCopperVisAtt->SetVisibility(false);
    CopperZylinderLOG->SetVisAttributes(hCopperVisAtt);
    KupferDeckelLOG->SetVisAttributes(hCopperVisAtt);
    //
    //
    G4Colour White (1.0,  1.0, 1.0);
    G4VisAttributes *hVacuumVisAtt = new G4VisAttributes(lCopper);
    hVacuumVisAtt->SetVisibility(false);
    VacuumLOG->SetVisAttributes(hVacuumVisAtt);


return m_phallPhys;
}


//================================================================================
//========================---- Construct PIPS ---=================================
//================================================================================

void BetaChamberDetectorConstruction::constructPIPS(){

    // -------------------------------------------------------------------------------------------------------
    // =============================== materials =============================================================
    // -------------------------------------------------------------------------------------------------------

        G4Material * Elastomer = G4Material::GetMaterial("Elastomer");
        G4Material * Poly = G4Material::GetMaterial("poly");
        G4Material * Steel = G4Material::GetMaterial("SS304LSteel");
        G4Material * Silicon = G4Material::GetMaterial("Silicon");
        G4Material * brass = G4Material::GetMaterial("messing");
        G4Material * Silver = G4Material::GetMaterial("Silver");

    // -------------------------------------------------------------------------------------------------------
    // ============================= geometrical volumes =====================================================
    // -------------------------------------------------------------------------------------------------------

         G4double Siliziumdicke = GetDimensions("PIPSSiliziumChipHoeheStandard");
         G4Tubs * cylinderTop= new G4Tubs("PIPScylinderTop",GetDimensions("PIPSTopZylinderInnenRadius"),GetDimensions("PIPSTopZylinderAußenRadius"),0.5*GetDimensions("PIPSTopZylinderHoehe"),0*deg,360*deg);
         G4Tubs * ringTop= new G4Tubs("PIPSringTop",GetDimensions("PIPSTopRingInnenRadius"),GetDimensions("PIPSTopRingAußenRadius"),0.5*GetDimensions("PIPSTopRingHoehe"),0*deg,360*deg);
         G4Tubs * housingTop= new G4Tubs("PIPShousingTop",GetDimensions("PIPSHousingTopInnenRadius"),GetDimensions("PIPSHousingTopAußenRadius"),0.5*GetDimensions("PIPSHousingTopHoehe"),0*deg,360*deg);
         G4Tubs * housingShell= new G4Tubs("PIPShousingShell",GetDimensions("PIPSHousingTopAußenRadius"),GetDimensions("PIPSHousingShellAußenRadius"),0.5*GetDimensions("PIPSHousingShellHoehe"),0*deg,360*deg);
         G4Tubs * housingBottom= new G4Tubs("PIPShousingBottom",GetDimensions("PIPSHousingBottomInnenRadius"),GetDimensions("PIPSHousingBottomAußenRadius"),0.5*GetDimensions("PIPSHousingBottomHoehe"),0*deg,360*deg);
         G4Tubs * messingStift= new G4Tubs("PIPSMessingStift",0,GetDimensions("PIPSTopZylinderInnenRadius"),0.5*GetDimensions("PIPSMessingStiftHoehe"),0*deg,360*deg);
         G4Tubs * messingPlatte= new G4Tubs("PIPSMessingPlatte",GetDimensions("PIPSTopZylinderInnenRadius"),GetDimensions("PIPSMessingPlatteAußenRadius"),0.5*GetDimensions("PIPSMessingPlatteHoehe"),0*deg,360*deg);
         G4Tubs * PEbacking= new G4Tubs("PIPSPEbacking",GetDimensions("PIPSTopZylinderInnenRadius"),GetDimensions("PIPSMessingPlatteAußenRadius"),0.5*GetDimensions("PIPSPEBackingHoehe"),0*deg,360*deg);
         G4Tubs * SiliconChip= new G4Tubs("PIPSSiliChip",0,GetDimensions("PIPSSiliziumChipAußenRadius"),0.5*Siliziumdicke,0*deg,360*deg);
         G4Tubs * ElastomerRingBottom= new G4Tubs("PIPSElastoRingBt",GetDimensions("PIPSElastomerRingBottomInnenRadius"),GetDimensions("PIPSElastomerRingAußenRadius"),0.5*GetDimensions("PIPSElastomerRingBottomHoehe"),0*deg,360*deg);
         G4Tubs * ElastomerRingTop= new G4Tubs("PIPSElastoRingTop",0,GetDimensions("PIPSElastomerRingAußenRadius"),0.5*GetDimensions("PIPSElastomerRingTopHoehe"),0*deg,360*deg);
         G4Tubs * Silberkontakt = new G4Tubs("PIPSSilberKontakt",0,GetDimensions("PIPSSilberKontaktAußenRadius"),0.5*GetDimensions("PIPSSilberKontaktDicke"),0*deg,360*deg);

     // -------------------------------------------------------------------------------------------------------
     // ================================== logical volumes ====================================================
     // -------------------------------------------------------------------------------------------------------

         housingTopLOG = new G4LogicalVolume(housingTop, Steel, "SShousingTopLog",0, 0, 0);
         housingShellLOG = new G4LogicalVolume(housingShell, Steel, "SShousingShellLog",0, 0, 0);
         housingBottomLOG = new G4LogicalVolume(housingBottom, Steel, "SShousingBottomLog",0, 0, 0);
         ElastomerRingBottomLOG= new G4LogicalVolume(ElastomerRingBottom, Elastomer, "ElastomerRingBottomLOG",0, 0, 0);
         SiliconChipLOG= new G4LogicalVolume(SiliconChip, Silicon, "SiliconChipLOG",0, 0, 0);
         ElastomerRingTopLOG= new G4LogicalVolume(ElastomerRingTop, Elastomer, "ElastomerRingTopLOG",0, 0, 0);
         MessingPlateLOG= new G4LogicalVolume(messingPlatte, brass, "MessingPlateLOG",0, 0, 0);
         MessingPinLOG= new G4LogicalVolume(messingStift, brass, "MessingPinLOG",0, 0, 0);
         PEbackingLOG= new G4LogicalVolume(PEbacking, Poly, "PEbackingLOG",0, 0, 0);
         cylinderTopLOG = new G4LogicalVolume(cylinderTop, Steel, "SScylinderTopLOG",0, 0, 0);
         ringTopLOG = new G4LogicalVolume(ringTop, Steel, "SSringTopLOG",0, 0, 0);
         SilberkontaktLOG = new G4LogicalVolume(Silberkontakt, Silver, "SilberkontaktLOG",0, 0, 0);

    // -------------------------------------------------------------------------------------------------------
    // =========================== Positioning of physical volumes ===========================================
    // ------------------------------------------------------------------------------------------------------
         G4double H1 =0.5*GetDimensions("GrundKoerperL4")+0.5*(GetDimensions("GrundKoerperL2")-GetDimensions("GrundKoerperL1"));
         G4double L4L2 = 0.5* (GetDimensions("GrundKoerperL4")-GetDimensions("GrundKoerperL2"));
         G4double PlatzierungBottom =H1-L4L2+0.5*GetDimensions("PIPSHousingBottomHoehe");
         G4double PlatzierungShell=PlatzierungBottom+0.5*GetDimensions("PIPSHousingBottomHoehe")+0.5*GetDimensions("PIPSHousingShellHoehe");
         G4double PlatzierungTop=H1+ 0.5*(GetDimensions("PIPSHousingShellHoehe")-GetDimensions("PIPSHousingTopHoehe"));
         G4double ElastomeringUntenPlatzierung=H1-L4L2+GetDimensions("PIPSHousingBottomHoehe") +0.5*GetDimensions("PIPSElastomerRingBottomHoehe");
         G4double SiliconChipPlatzierung=ElastomeringUntenPlatzierung+0.5*GetDimensions("PIPSElastomerRingBottomHoehe")+0.5*Siliziumdicke;
         G4double SilberkontaktPlatzierung=SiliconChipPlatzierung-0.5*Siliziumdicke-0.5*GetDimensions("PIPSSilberKontaktDicke");
         G4double ElastomeringObenPlatzierung=SiliconChipPlatzierung+0.5*Siliziumdicke+0.5*GetDimensions("PIPSElastomerRingTopHoehe");
         G4double MessingPlatzierung1=ElastomeringObenPlatzierung+0.5*GetDimensions("PIPSElastomerRingTopHoehe")+0.5*GetDimensions("PIPSMessingPlatteHoehe");
         G4double MessingPlatzierung2=ElastomeringObenPlatzierung+0.5*GetDimensions("PIPSElastomerRingTopHoehe")+0.5*GetDimensions("PIPSMessingStiftHoehe");
         G4double PEbackingPlatzierung=ElastomeringObenPlatzierung+0.5*GetDimensions("PIPSElastomerRingTopHoehe")+1.0*GetDimensions("PIPSMessingPlatteHoehe")+0.5*GetDimensions("PIPSPEBackingHoehe");
         G4double cylinderTopPlatzierung=H1+0.5*GetDimensions("PIPSHousingShellHoehe")-1.0*GetDimensions("PIPSHousingTopHoehe")+0.5*GetDimensions("PIPSTopZylinderHoehe");
         G4double ringTopPlatzierung=H1+0.5*GetDimensions("PIPSHousingShellHoehe")+0.5*GetDimensions("PIPSTopRingHoehe");

    // -------------------------------------------------------------------------------------------------------
    // ======================================= physical volumes ==============================================
    // -------------------------------------------------------------------------------------------------------

          housingShellPHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,PlatzierungShell),housingShellLOG, "HousingShellPHYS",VacuumLOG, false, 0);
          housingTopPHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,PlatzierungTop),housingTopLOG, "HousingTopPHYS",VacuumLOG, false, 0);      housingBottomPHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,PlatzierungBottom),housingBottomLOG, "HousingBottomPHYS",VacuumLOG, false, 0);
          ElastomerRingBottomPHYS= new G4PVPlacement(0, G4ThreeVector(0, 0,ElastomeringUntenPlatzierung),ElastomerRingBottomLOG, "ElastomerRingBottomPHYS",VacuumLOG, false, 0);
          SiliconChipPHYS= new G4PVPlacement(0, G4ThreeVector(0, 0,SiliconChipPlatzierung),SiliconChipLOG, "SILIZIUMSENSITIVEPHYS",VacuumLOG, false, 0);
          ElastomerRingTopPHYS= new G4PVPlacement(0, G4ThreeVector(0, 0,ElastomeringObenPlatzierung),ElastomerRingTopLOG, "ElastomerRingTopPHYS",VacuumLOG, false, 0);
          MessingPlatePHYS= new G4PVPlacement(0, G4ThreeVector(0, 0,MessingPlatzierung1),MessingPlateLOG, "MessingPlatePHYS",VacuumLOG, false, 0);
          MessingPinPHYS= new G4PVPlacement(0, G4ThreeVector(0, 0,MessingPlatzierung2),MessingPinLOG, "MessingPinPHYS",VacuumLOG, false, 0);
          PEbackingPHYS= new G4PVPlacement(0, G4ThreeVector(0, 0,PEbackingPlatzierung),PEbackingLOG, "PEbackingPHYS",VacuumLOG, false, 0);
          cylinderTopPHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,cylinderTopPlatzierung),cylinderTopLOG, "cylinderTopPHYS",VacuumLOG, false, 0);
          ringTopPHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,ringTopPlatzierung),ringTopLOG, "ringTopPHYS",VacuumLOG, false, 0);
          SilberkontaktPHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,SilberkontaktPlatzierung),SilberkontaktLOG, "SilberkontaktPHYS",VacuumLOG, false, 0);

    //============================== Silicon sensitivity ======== ========== ==========
          fScoringVolume =SiliconChipLOG;


    // -------------------------------------------------------------------------------------------------------
    // =================================== Visual Attributes =================================================
    // -------------------------------------------------------------------------------------------------------

        G4Colour hSiliconColor(0.5, 0.7, 0.7, 0.01);
        G4VisAttributes *pSiliconVisAtt = new G4VisAttributes(hSiliconColor);
        pSiliconVisAtt->SetVisibility(true);
        SiliconChipLOG->SetVisAttributes(pSiliconVisAtt);

        G4Colour hSteelColor(0.3, 0.7, 0.7, 0.01);
        G4VisAttributes *pSteelVisAtt = new G4VisAttributes(hSteelColor);
        pSteelVisAtt->SetVisibility(false);
        housingBottomLOG->SetVisAttributes(pSteelVisAtt);
        housingShellLOG->SetVisAttributes(pSteelVisAtt);
        housingTopLOG->SetVisAttributes(pSteelVisAtt);


        G4Colour lElast (0.8,  0.5, 0.1);
        G4VisAttributes *hElastVisAtt = new G4VisAttributes(lElast);
        hElastVisAtt->SetVisibility(false);
        ElastomerRingBottomLOG->SetVisAttributes(hElastVisAtt);
        ElastomerRingTopLOG->SetVisAttributes(hElastVisAtt);

        G4Colour lBrass (0.4,  0.2, 0.1);
        G4VisAttributes *hBrassVisAtt = new G4VisAttributes(lBrass);
        hBrassVisAtt->SetVisibility(false);
        MessingPlateLOG->SetVisAttributes(hBrassVisAtt);
        MessingPinLOG->SetVisAttributes(hBrassVisAtt);

        // Stepsize inside SILICON
        //if(BoolStepLimit){
        //  G4cout <<"Manuel step limit set"<<"\n";
        //G4double maxStep = 0.1*Siliziumdicke;
        //fStepLimit = new G4UserLimits(maxStep);
        //SiliconChipLOG->SetUserLimits(fStepLimit);
}

//================================================================================
//==================---- Construct sensitive detector ---=========================
//================================================================================

void BetaChamberDetectorConstruction::constructSensitiveDetector(){


     G4SDManager* sdManager =G4SDManager::GetSDMpointer();
     G4String SDname,SDname2;
     if(!SiliconChipSD){
         G4cout<<" constucting SiliconChipSV "<<"\n";
         SiliconChipSD= new BetaChamberSiliconSD(SDname="/SiliconChipSV");  //,eventAction
         sdManager->AddNewDetector(SiliconChipSD);
     }
    //  SiliconChipLOG->SetSensitiveDetector("SiliconChip",SiliconChipSD);
    SetSensitiveDetector("SiliconChipLOG",SiliconChipSD);

     if(!AbsorberSD){
         G4cout<<" constucting AbsorberSV "<<"\n";
         AbsorberSD= new BetaChamberAbsorberSD(SDname2="/AbsorberSV");
         sdManager->AddNewDetector(AbsorberSD);
     }
      ATSampleLOG->SetSensitiveDetector(AbsorberSD);
    // SetSensitiveDetector("ATSampleLOG",AbsorberSD);



    sdManager->ListTree();
}

//================================================================================
//==================---- Construct 25um sample holder ---=========================
//================================================================================

void BetaChamberDetectorConstruction::constructSampleHolder25umSetup(){

    // ---- Materialien -----

    G4Material *Bismut = G4Material::GetMaterial("Bismut");
    G4Material *mPMMA = G4Material::GetMaterial("PMMA");
    G4Material *mylar = G4Material::GetMaterial("Mylar");

    //============ geometrical volumes  ================================================


        G4Tubs* GEOVolGrundKoerperZylinder1 = new G4Tubs("GVGKZ1",0.5*GetDimensions("GrundKoerperD1"),0.5*GetDimensions("GrundKoerperD4"),0.5*GetDimensions("GrundKoerperL1"),0*deg,360*deg);
        G4Tubs* GEOVolGrundKoerperZylinder2 = new G4Tubs("GVGKZ2",0.5*GetDimensions("GrundKoerperD2"),0.5*GetDimensions("GrundKoerperD4"),0.5*(GetDimensions("GrundKoerperL2")-GetDimensions("GrundKoerperL1")),0*deg,360*deg);
        G4Tubs* GEOVolGrundKoerperZylinder3 = new G4Tubs("GVGKZ3",0.5*GetDimensions("GrundKoerperD3"),0.5*GetDimensions("GrundKoerperD4"),0.5*(GetDimensions("GrundKoerperL4")-GetDimensions("GrundKoerperL2")),0*deg,360*deg);

     //--------------------------

        G4Tubs* GEOVolHalterSondeZylinder1 = new G4Tubs("GVHSZ1",0.5*GetDimensions("HalterSondeD2"),0.5*GetDimensions("HalterSondeD3"),0.5*(GetDimensions("HalterSondeL2")-GetDimensions("HalterSondeL1")),0*deg,360*deg);
        G4Tubs* GEOVolHalterSondeZylinder2 = new G4Tubs("GVHSZ2",0.5*GetDimensions("HalterSondeD1"),0.5*GetDimensions("HalterSondeD3"),0.5*GetDimensions("HalterSondeL1"),0*deg,360*deg);

    //--------------------------
        G4Tubs* DistanzRing = new G4Tubs("GVDSTR",0.5*GetDimensions("DistanzRingD1"),0.5*GetDimensions("DistanzRingD2"),0.5*GetDimensions("DistanzRingL1"),0*deg,360*deg);
    //--------------------------
        G4Tubs* GegenSonde = new G4Tubs("GVGeSo",0.5*GetDimensions("GegenSondeD1"),0.5*GetDimensions("GegenSondeD2"),0.5*GetDimensions("GegenSondeL1"),0*deg,360*deg);
    //--------------------------
        G4Tubs* FixierRing = new G4Tubs("GVFixR",0.5*GetDimensions("FixierRingD1"),0.5*GetDimensions("FixierRingD2"),0.5*GetDimensions("FixierRingL1"),0*deg,360*deg);
    //--------------------------
        G4Tubs* BismutSample = new G4Tubs("BiSample",0,0.5*(16*mm),0.5*(0.025*mm),0*deg,360*deg);
        //--------------------------
        G4Tubs * mylarBacking = new G4Tubs("Mylarfolie",0,0.5*GetDimensions("HalterSondeD2"),0.5*GetDimensions("MylarDicke"),0*deg,360*deg);

    //============ logical volumes ================================================

    GrundKoerper1LOG = new G4LogicalVolume(GEOVolGrundKoerperZylinder1, mPMMA, "GrundKoerper1Log",0, 0, 0);
    GrundKoerper2LOG = new G4LogicalVolume(GEOVolGrundKoerperZylinder2, mPMMA, "GrundKoerper2Log",0, 0, 0);
    GrundKoerper3LOG = new G4LogicalVolume(GEOVolGrundKoerperZylinder3, mPMMA, "GrundKoerper3Log",0, 0, 0);
    HalterSondePart1LOG = new G4LogicalVolume(GEOVolHalterSondeZylinder1, mPMMA, "HalterSondePart1LOG",0, 0, 0);
    HalterSondePart2LOG = new G4LogicalVolume(GEOVolHalterSondeZylinder2, mPMMA, "HalterSondePart2LOG",0, 0, 0);
    DistanzRingLOG = new G4LogicalVolume(DistanzRing, mPMMA, "DistanzLOG",0, 0, 0);
    GegenSondeLOG = new G4LogicalVolume(GegenSonde, mPMMA, "GegenSondeLOG",0, 0, 0);
    FixierRingLOG = new G4LogicalVolume(FixierRing, mPMMA, "FixierRingLOG",0, 0, 0);
    BismutSampleLOG = new G4LogicalVolume(BismutSample, Bismut, "BiSampleLOG",0, 0, 0);
    mylarBackingLOG =new G4LogicalVolume(mylarBacking, mylar, "mylarBackingLOG",0, 0, 0);

    // ============ physical volumes ================================================

    G4double H1 =0.5*GetDimensions("GrundKoerperL1");
    G4double ShiftGKpart2 = H1+0.5*(GetDimensions("GrundKoerperL2")-GetDimensions("GrundKoerperL1"));
    G4double ShiftGKpart3 = ShiftGKpart2+0.5*(GetDimensions("GrundKoerperL2")-GetDimensions("GrundKoerperL1"))+0.5*(GetDimensions("GrundKoerperL4")-GetDimensions("GrundKoerperL2"));
    G4double HalterSondenPlatzierungPart1 = H1-0.5*(GetDimensions("HalterSondeL2")-GetDimensions("HalterSondeL1"));
    G4double HalterSondenPlatzierungPart2 = H1-1.0*(GetDimensions("HalterSondeL2")-GetDimensions("HalterSondeL1"))-0.5*GetDimensions("HalterSondeL1");
    G4double BiSamplePlatzierung= HalterSondenPlatzierungPart2-0.5*GetDimensions("HalterSondeL1")-0.5*(25e-3*mm);
    G4double mylarBackingPlatzierung= BiSamplePlatzierung-0.5*(25e-3*mm)-0.5*GetDimensions("MylarDicke");
    G4double GegenSondePlatzierung=mylarBackingPlatzierung-0.5*GetDimensions("MylarDicke")-0.5*GetDimensions("GegenSondeL1");
    G4double DistanzRingPlatzierung1=GegenSondePlatzierung-0.5*GetDimensions("GegenSondeL1")-0.5*GetDimensions("DistanzRingL1");
    G4double DistanzRingPlatzierung2=DistanzRingPlatzierung1-GetDimensions("DistanzRingL1");
    G4double FixierRingPlatzierung=DistanzRingPlatzierung2-0.5*GetDimensions("DistanzRingL1")-0.5*GetDimensions("FixierRingL1");
     G4cout << "sample shift: "<< BiSamplePlatzierung<< " mm \n";

    // ----------------------------------------

    GrundKoerper1PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,0),GrundKoerper1LOG, "GrundKoerper1PHYS",VacuumLOG, false, 0);
    GrundKoerper2PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,ShiftGKpart2),GrundKoerper2LOG, "GrundKoerper2PHYS",VacuumLOG, false, 0);
    GrundKoerper3PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,ShiftGKpart3),GrundKoerper3LOG, "GrundKoerper3PHYS",VacuumLOG, false, 0);
    HalterSondePart1PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,HalterSondenPlatzierungPart1),HalterSondePart1LOG, "HalterSondePart1PHYS",VacuumLOG, false, 0);
    HalterSondePart2PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,HalterSondenPlatzierungPart2),HalterSondePart2LOG, "HalterSondePart2PHYS",VacuumLOG, false, 0);
    BismutSamplePHYS =  new G4PVPlacement(0, G4ThreeVector(0, 0,BiSamplePlatzierung),BismutSampleLOG, "BismutSamplePHYS",VacuumLOG, false, 0);
    mylarBackingPHYS =  new G4PVPlacement(0, G4ThreeVector(0, 0,mylarBackingPlatzierung),mylarBackingLOG, "mylarBackingPHYS",VacuumLOG, false, 0);
    GegenSondePHYS =  new G4PVPlacement(0, G4ThreeVector(0, 0,GegenSondePlatzierung),GegenSondeLOG, "GegenSondePHYS",VacuumLOG, false, 0);
    DistanzRing1PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,DistanzRingPlatzierung1),DistanzRingLOG, "DistanzRing1PHYS",VacuumLOG, false, 0);
    DistanzRing2PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,DistanzRingPlatzierung2),DistanzRingLOG, "DistanzRing2PHYS",VacuumLOG, false, 0);
    FixierRingPHYS =  new G4PVPlacement(0, G4ThreeVector(0, 0,FixierRingPlatzierung),FixierRingLOG, "FixierRingPHYS",VacuumLOG, false, 0);

    // ====== Vis - Attributes ======


    G4Colour lgray (0.7,  0.7, 0.7);
    G4VisAttributes *hAluVisAtt = new G4VisAttributes(lgray);
    hAluVisAtt->SetVisibility(true);
    // DistanzRingLOG->SetVisAttributes(hAluVisAtt);
    HalterSondePart1LOG->SetVisAttributes(hAluVisAtt);
    HalterSondePart2LOG->SetVisAttributes(hAluVisAtt);
    GegenSondeLOG->SetVisAttributes(hAluVisAtt);

     G4VisAttributes *hBisVisAtt = new G4VisAttributes(lgray);
     hBisVisAtt->SetVisibility(true);
     BismutSampleLOG->SetVisAttributes(hBisVisAtt);

     G4Colour lplastic (0.8,  0.8, 0.0);
     G4VisAttributes *hPlasticVisAtt = new G4VisAttributes(lplastic);
     hPlasticVisAtt->SetVisibility(true);
     GrundKoerper1LOG->SetVisAttributes(hPlasticVisAtt);
     GrundKoerper2LOG->SetVisAttributes(hPlasticVisAtt);
     GrundKoerper3LOG->SetVisAttributes(hPlasticVisAtt);
}

//================================================================================
//===========---- Construct calibration sample holder ---=========================
//================================================================================

void BetaChamberDetectorConstruction::constructSampleHolderCalibration(){

    // -------------------------------------------------------------------------------------------------------
    // =============================== Materialien ===========================================================
    // -------------------------------------------------------------------------------------------------------

        G4Material *mPMMA = G4Material::GetMaterial("PMMA");
        G4Material *mMylar = G4Material::GetMaterial("Mylar");
        G4Material *Bismut = G4Material::GetMaterial("Bismut");
        G4Material *Caesium = G4Material::GetMaterial("Caesium");
        G4Material *Barium = G4Material::GetMaterial("Barium");

    // -------------------------------------------------------------------------------------------------------
    // ============================= geometrical volumes =====================================================
    // -------------------------------------------------------------------------------------------------------

        G4Tubs* GEOVolGrundKoerperZylinder1 = new G4Tubs("GVGKZ1",0.5*GetDimensions("GrundKoerperD1"),0.5*GetDimensions("GrundKoerperD4"),0.5*GetDimensions("GrundKoerperL1"),0*deg,360*deg);
        G4Tubs* GEOVolGrundKoerperZylinder2 = new G4Tubs("GVGKZ2",0.5*GetDimensions("GrundKoerperD2"),0.5*GetDimensions("GrundKoerperD4"),0.5*(GetDimensions("GrundKoerperL2")-GetDimensions("GrundKoerperL1")),0*deg,360*deg);
        G4Tubs* GEOVolGrundKoerperZylinder3 = new G4Tubs("GVGKZ3",0.5*GetDimensions("GrundKoerperD3"),0.5*GetDimensions("GrundKoerperD4"),0.5*(GetDimensions("GrundKoerperL4")-GetDimensions("GrundKoerperL2")),0*deg,360*deg);

        G4Tubs * mylar = new G4Tubs("Mylarfolie",0,0.5*GetDimensions("KalProbeBottomPartAußenDurchmesserkurz"),0.5*GetDimensions("KalProbeMylarDicke"),0*deg,360*deg);
        G4Tubs * TopRing = new G4Tubs("FixierRing",0.5*GetDimensions("KalProbeBottomPartInnenDurchmesser"),0.5*GetDimensions("KalProbeBottomPartAußenDurchmesser"),0.5*GetDimensions("KalProbeTopRingHoehe"),0*deg,360*deg);
        // G4Orb  * probe= new G4Orb("Probe",0.5*GetDimensions("KalibrierprobeDurchmesserORB"));
        G4double a=5.08e-3*mm;
        G4Sphere* probe = new G4Sphere("Probe",0,a,0,360*deg,0*deg,90*deg);

        G4Tubs* DistanzRing = new G4Tubs("GVDSTR",0.5*GetDimensions("DistanzRingD1"),0.5*GetDimensions("DistanzRingD2"),0.5*GetDimensions("DistanzRingL1"),0*deg,360*deg);
        G4Tubs* GegenSonde = new G4Tubs("GVGeSo",0.5*GetDimensions("GegenSondeD1"),0.5*GetDimensions("GegenSondeD2"),0.5*GetDimensions("GegenSondeL1"),0*deg,360*deg);
        G4Tubs* FixierRing = new G4Tubs("GVFixR",0.5*GetDimensions("FixierRingD1"),0.5*GetDimensions("FixierRingD2"),0.5*GetDimensions("FixierRingL1"),0*deg,360*deg);
     // --- creating the base of the calibration source as substraction solid
        G4Orb  * hole= new G4Orb("hole",GetDimensions("KalibrierprobeRadiusKugel"));
        G4Tubs * BaseDisc = new G4Tubs("BasisOhneLoch",0,0.5*GetDimensions("GrundKoerperD1"),0.5*GetDimensions("KalProbeBottomPartHoehe"),0*deg,360*deg);
        G4double shiftSubstraction= GetDimensions("KalibrierprobeRadiusKugel")+0.5*GetDimensions("KalProbeBottomPartHoehe") - GetDimensions("KalProbeBottomPartLochTiefe");
        G4SubtractionSolid* Base =  new G4SubtractionSolid("Basis", BaseDisc,hole,0,G4ThreeVector(0, 0,shiftSubstraction));

    // -------------------------------------------------------------------------------------------------------
    // ================================== logical volumes ====================================================
    // -------------------------------------------------------------------------------------------------------

        GrundKoerper1LOG = new G4LogicalVolume(GEOVolGrundKoerperZylinder1, mPMMA, "GrundKoerper1Log",0, 0, 0);
        GrundKoerper2LOG = new G4LogicalVolume(GEOVolGrundKoerperZylinder2, mPMMA, "GrundKoerper2Log",0, 0, 0);
        GrundKoerper3LOG = new G4LogicalVolume(GEOVolGrundKoerperZylinder3, mPMMA, "GrundKoerper3Log",0, 0, 0);
        BaseLOG = new G4LogicalVolume(Base, mPMMA, "BaseLOG",0, 0, 0);
        mylarLOG = new G4LogicalVolume(mylar, mMylar, "mylarLOG",0, 0, 0);
        TopRingLOG = new G4LogicalVolume(TopRing, mPMMA, "TopRingLOG",0, 0, 0);
        DistanzRingLOG = new G4LogicalVolume(DistanzRing, mPMMA, "DistanzLOG",0, 0, 0);
        GegenSondeLOG = new G4LogicalVolume(GegenSonde, mPMMA, "GegenSondeLOG",0, 0, 0);
        FixierRingLOG = new G4LogicalVolume(FixierRing, mPMMA, "FixierRingLOG",0, 0, 0);

        if(CalibMaterial==1) probeLOG = new G4LogicalVolume(probe, Bismut, "probeLOG",0, 0, 0);
        else if(CalibMaterial==2) probeLOG = new G4LogicalVolume(probe, Caesium, "probeLOG",0, 0, 0);
        else if(CalibMaterial==3) probeLOG = new G4LogicalVolume(probe, Barium, "probeLOG",0, 0, 0);

        // NeueProbeLOG = new G4LogicalVolume(NeueProbenForm, Barium, "NewprobeLOG",0, 0, 0);
    // -------------------------------------------------------------------------------------------------------
    // =========================== Positioning of physical volumes ===========================================
    // -------------------------------------------------------------------------------------------------------

        G4double H1 =0.5*GetDimensions("GrundKoerperL1");
        G4double ShiftGKpart2 = H1+0.5*(GetDimensions("GrundKoerperL2")-GetDimensions("GrundKoerperL1"));
        G4double ShiftGKpart3 = ShiftGKpart2+0.5*(GetDimensions("GrundKoerperL2")-GetDimensions("GrundKoerperL1"))+0.5*(GetDimensions("GrundKoerperL4")-GetDimensions("GrundKoerperL2"));

        //--------------------------------------- sample components ------------------------------------------------------
        G4double TopRingPlatzierung,MylarPlatzierung,BasePlatzierung,ProbePlatzierung;
                 TopRingPlatzierung=MylarPlatzierung=BasePlatzierung=ProbePlatzierung=0*mm;
        G4double DistanzRingPlatzierung1,DistanzRingPlatzierung2,FixierRingPlatzierung;
                 DistanzRingPlatzierung1=DistanzRingPlatzierung2=FixierRingPlatzierung=0*mm;
        G4double NeueProbeTesplatzierung;
        //--- Position 1: Sample | DistanzRing | DistanzRing  | FixierRingPlatzierung

        if(PositionsNummer==1){
            // ------
             TopRingPlatzierung = H1-0.5*GetDimensions("KalProbeTopRingHoehe");
             MylarPlatzierung = TopRingPlatzierung-0.5*GetDimensions("KalProbeTopRingHoehe")-0.5*GetDimensions("KalProbeMylarDicke");
             BasePlatzierung = MylarPlatzierung-0.5*GetDimensions("KalProbeMylarDicke")-0.5*GetDimensions("KalProbeBottomPartHoehe");
             ProbePlatzierung = MylarPlatzierung-0.5*GetDimensions("KalProbeMylarDicke")-GetDimensions("KalProbeBottomPartLochTiefe")+ 0.6*GetDimensions("KalibrierprobeDurchmesserORB");
            //  G4cout << "TopRingPlatzierung = "<<  TopRingPlatzierung/mm <<" mm \n";
            //  G4cout << "MylarPlatzierung = "<<  MylarPlatzierung/mm <<" mm \n";
            //  G4cout << "BasePlatzierung = "<<  BasePlatzierung/mm <<" mm \n";
            //  G4cout << "ProbePlatzierung = "<<  ProbePlatzierung/mm <<" mm \n";
            //  G4cout <<"Unterkante Loch = "<<(ProbePlatzierung- 0.6*GetDimensions("KalibrierprobeDurchmesserORB"))/mm <<" mm \n";
            // ------
             DistanzRingPlatzierung1=BasePlatzierung-0.5*GetDimensions("KalProbeBottomPartHoehe")-0.5*GetDimensions("DistanzRingL1");
             DistanzRingPlatzierung2=DistanzRingPlatzierung1-GetDimensions("DistanzRingL1");
             FixierRingPlatzierung=DistanzRingPlatzierung2-0.5*GetDimensions("DistanzRingL1")-0.5*GetDimensions("FixierRingL1");
        }
        //--- Position 2: DistanzRing | Sample | DistanzRing  | FixierRingPlatzierung
         else if(PositionsNummer==2){
             DistanzRingPlatzierung1=H1-0.5*GetDimensions("DistanzRingL1");
             // ----
             TopRingPlatzierung=DistanzRingPlatzierung1-0.5*GetDimensions("DistanzRingL1")-0.5*GetDimensions("KalProbeTopRingHoehe");
             MylarPlatzierung = TopRingPlatzierung-0.5*GetDimensions("KalProbeTopRingHoehe")-0.5*GetDimensions("KalProbeMylarDicke");
             BasePlatzierung = MylarPlatzierung-0.5*GetDimensions("KalProbeMylarDicke")-0.5*GetDimensions("KalProbeBottomPartHoehe");
             ProbePlatzierung = MylarPlatzierung-0.5*GetDimensions("KalProbeMylarDicke")-GetDimensions("KalProbeBottomPartLochTiefe")+ 0.6*GetDimensions("KalibrierprobeDurchmesserORB");
             // ----
             DistanzRingPlatzierung2=BasePlatzierung-0.5*GetDimensions("KalProbeBottomPartHoehe")-0.5*GetDimensions("DistanzRingL1");
             FixierRingPlatzierung=DistanzRingPlatzierung2-0.5*GetDimensions("DistanzRingL1")-0.5*GetDimensions("FixierRingL1");
         }
         //--- Position3: DistanzRing  | DistanzRing | Sample | FixierRingPlatzierung
         else if(PositionsNummer==3){
             DistanzRingPlatzierung1=H1-0.5*GetDimensions("DistanzRingL1");
             DistanzRingPlatzierung2=DistanzRingPlatzierung1-1.0*GetDimensions("DistanzRingL1");
             // ----
             TopRingPlatzierung=DistanzRingPlatzierung2-0.5*GetDimensions("DistanzRingL1")-0.5*GetDimensions("KalProbeTopRingHoehe");
             MylarPlatzierung = TopRingPlatzierung-0.5*GetDimensions("KalProbeTopRingHoehe")-0.5*GetDimensions("KalProbeMylarDicke");
             BasePlatzierung = MylarPlatzierung-0.5*GetDimensions("KalProbeMylarDicke")-0.5*GetDimensions("KalProbeBottomPartHoehe");
             ProbePlatzierung = MylarPlatzierung-0.5*GetDimensions("KalProbeMylarDicke")-GetDimensions("KalProbeBottomPartLochTiefe")+ 0.6*GetDimensions("KalibrierprobeDurchmesserORB");
             // ----
             FixierRingPlatzierung=BasePlatzierung-0.5*GetDimensions("KalProbeBottomPartHoehe")-0.5*GetDimensions("FixierRingL1");
         }

            // NeueProbeTesplatzierung=FixierRingPlatzierung-0.5*GetDimensions("FixierRingL1")-15*mm;

    // -------------------------------------------------------------------------------------------------------
    // ======================================= physical volumes ==============================================
    // -------------------------------------------------------------------------------------------------------

        GrundKoerper1PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,0),GrundKoerper1LOG, "GrundKoerper1PHYS",VacuumLOG, false, 0);
        GrundKoerper2PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,ShiftGKpart2),GrundKoerper2LOG, "GrundKoerper2PHYS",VacuumLOG, false, 0);
        GrundKoerper3PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,ShiftGKpart3),GrundKoerper3LOG, "GrundKoerper3PHYS",VacuumLOG, false, 0);

        // switch for using only parts of the calibration source
        if(CalibSetup==1){
            TopRingPHYS =new G4PVPlacement(0, G4ThreeVector(0, 0,TopRingPlatzierung),TopRingLOG, "CalibRingPHYS",VacuumLOG, false, 0);
            mylarPHYS =new G4PVPlacement(0, G4ThreeVector(0, 0,MylarPlatzierung),mylarLOG, "CalibMylarPHYS",VacuumLOG, false, 0);
            BasePHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,BasePlatzierung),BaseLOG, "CalibBasePHYS",VacuumLOG, false, 0);
            probePHYS =new G4PVPlacement(0, G4ThreeVector(0, 0,ProbePlatzierung),probeLOG, "PROBEPHYS",VacuumLOG, false, 0);
        }
        else if(CalibSetup==2){
            mylarPHYS =new G4PVPlacement(0, G4ThreeVector(0, 0,MylarPlatzierung),mylarLOG, "CalibMylarPHYS",VacuumLOG, false, 0);
            BasePHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,BasePlatzierung),BaseLOG, "CalibBasePHYS",VacuumLOG, false, 0);
            probePHYS =new G4PVPlacement(0, G4ThreeVector(0, 0,ProbePlatzierung),probeLOG, "PROBEPHYS",VacuumLOG, false, 0);
        }
        else if(CalibSetup==3){
            BasePHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,BasePlatzierung),BaseLOG, "CalibBasePHYS",VacuumLOG, false, 0);
            probePHYS =new G4PVPlacement(0, G4ThreeVector(0, 0,ProbePlatzierung),probeLOG, "PROBEPHYS",VacuumLOG, false, 0);
        }
        else if(CalibSetup==4){
         probePHYS =new G4PVPlacement(0, G4ThreeVector(0, 0,ProbePlatzierung),probeLOG, "PROBEPHYS",VacuumLOG, false, 0);
        }
        else if(CalibSetup==5){
            TopRingPHYS =new G4PVPlacement(0, G4ThreeVector(0, 0,TopRingPlatzierung),TopRingLOG, "CalibRingPHYS",VacuumLOG, false, 0);
            mylarPHYS =new G4PVPlacement(0, G4ThreeVector(0, 0,MylarPlatzierung),mylarLOG, "CalibMylarPHYS",VacuumLOG, false, 0);
            BasePHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,BasePlatzierung),BaseLOG, "CalibBasePHYS",VacuumLOG, false, 0);
        }

        DistanzRing1PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,DistanzRingPlatzierung1),DistanzRingLOG, "DistanzRing1PHYS",VacuumLOG, false, 0);
        DistanzRing2PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,DistanzRingPlatzierung2),DistanzRingLOG, "DistanzRing2PHYS",VacuumLOG, false, 0);
        FixierRingPHYS =  new G4PVPlacement(0, G4ThreeVector(0, 0,FixierRingPlatzierung),FixierRingLOG, "FixierRingPHYS",VacuumLOG, false, 0);
        // NeueProbePHYS =  new G4PVPlacement(0, G4ThreeVector(0, 0,NeueProbeTesplatzierung),NeueProbeLOG, "NeueProbePHYS",VacuumLOG, false, 0);
    // -------------------------------------------------------------------------------------------------------
    // =================================== Visual Attributes =================================================
    // -------------------------------------------------------------------------------------------------------


        G4Colour lgray (0.7,  0.7, 0.7);
        G4VisAttributes *hAluVisAtt = new G4VisAttributes(lgray);
        hAluVisAtt->SetVisibility(true);
        DistanzRingLOG->SetVisAttributes(hAluVisAtt);
        // TopRingLOG->SetVisAttributes(hAluVisAtt);
        // BaseLOG->SetVisAttributes(hAluVisAtt);

        G4Colour lGruen (0.5,  0.0, 0.3, 0.61);
        G4VisAttributes *hBisVisAtt = new G4VisAttributes(lGruen);
        hBisVisAtt->SetVisibility(true);
        probeLOG->SetVisAttributes(hBisVisAtt);
        TopRingLOG->SetVisAttributes(hBisVisAtt);
        BaseLOG->SetVisAttributes(hBisVisAtt);
        // NeueProbeLOG->SetVisAttributes(hBisVisAtt);

        G4Colour lplastic (0.8,  0.8, 0.0);
        G4VisAttributes *hPlasticVisAtt = new G4VisAttributes(lplastic);
        hPlasticVisAtt->SetVisibility(false);
        GrundKoerper1LOG->SetVisAttributes(hPlasticVisAtt);
        GrundKoerper2LOG->SetVisAttributes(hPlasticVisAtt);
        GrundKoerper3LOG->SetVisAttributes(hPlasticVisAtt);

}

//================================================================================
//==================---- Construct 2um sample holder ---==========================
//================================================================================

void  BetaChamberDetectorConstruction::constructSampleHolder2umSetup(G4int sonde){

    // -------------------------------------------------------------------------------------------------------
    // =============================== Materialien ===========================================================
    // -------------------------------------------------------------------------------------------------------

          G4Material *Bismut = G4Material::GetMaterial("Bismut");
          G4Material *mPMMA = G4Material::GetMaterial("PMMA");
          G4Material *silicon = G4Material::GetMaterial("Silicon");

    // -------------------------------------------------------------------------------------------------------
    // ============================= geometrical volumes =====================================================
    // -------------------------------------------------------------------------------------------------------

          G4Tubs* GEOVolGrundKoerperZylinder1 = new G4Tubs("GVGKZ1",0.5*GetDimensions("GrundKoerperD1"),0.5*GetDimensions("GrundKoerperD4"),0.5*GetDimensions("GrundKoerperL1"),0*deg,360*deg);
          G4Tubs* GEOVolGrundKoerperZylinder2 = new G4Tubs("GVGKZ2",0.5*GetDimensions("GrundKoerperD2"),0.5*GetDimensions("GrundKoerperD4"),0.5*(GetDimensions("GrundKoerperL2")-GetDimensions("GrundKoerperL1")),0*deg,360*deg);
          G4Tubs* GEOVolGrundKoerperZylinder3 = new G4Tubs("GVGKZ3",0.5*GetDimensions("GrundKoerperD3"),0.5*GetDimensions("GrundKoerperD4"),0.5*(GetDimensions("GrundKoerperL4")-GetDimensions("GrundKoerperL2")),0*deg,360*deg);
          G4Tubs* GEOVolHalterSondeZylinder1 = new G4Tubs("GVHSZ1",0.5*GetDimensions("HalterSondeD2"),0.5*GetDimensions("HalterSondeD3"),0.5*(GetDimensions("HalterSondeL2")-GetDimensions("HalterSondeL1")),0*deg,360*deg);
          G4Tubs* GEOVolHalterSondeZylinder2 = new G4Tubs("GVHSZ2",0.5*GetDimensions("HalterSondeD1"),0.5*GetDimensions("HalterSondeD3"),0.5*GetDimensions("HalterSondeL1"),0*deg,360*deg);
          G4Tubs* DistanzRing = new G4Tubs("GVDSTR",0.5*GetDimensions("DistanzRingD1"),0.5*GetDimensions("DistanzRingD2"),0.5*GetDimensions("DistanzRingL1"),0*deg,360*deg);
          G4Tubs* GegenSonde = new G4Tubs("GVGeSo",0.5*GetDimensions("GegenSondeD1"),0.5*GetDimensions("GegenSondeD2"),0.5*GetDimensions("GegenSondeL1"),0*deg,360*deg);
          G4Tubs* FixierRing = new G4Tubs("GVFixR",0.5*GetDimensions("FixierRingD1"),0.5*GetDimensions("FixierRingD2"),0.5*GetDimensions("FixierRingL1"),0*deg,360*deg);
          G4Tubs * SiWafer = new G4Tubs("SiliconWafer",0,0.5*GetDimensions("HalterSondeD2"),0.5*GetDimensions("SiWaferDicke"),0*deg,360*deg);

          // -------- setting bismuth sample thickness -------------------------
            G4double sampleDicke;
            G4cout<<" SondenNummer ="<<SondenNummer <<"\n";

            if(sonde ==10){
                sampleDicke = GetDimensions("BismutSchichtDickeStandard");
                G4cout<<"The standard sample was selected, with a bi-thickness of "<< sampleDicke<<" mm."<<"\n";
            } else{ // there is maybe a quick, easier way to do this, but it works!
               std::string search;
               std::string teil1 = "BismutSchichtDickeSonde";
               stringstream ss;
               ss << sonde;
               std::string teil2 =ss.str();
               search +=teil1;
               search +=teil2;
               G4cout<<"looking for "<< search<< " in map"<<"\n";
               sampleDicke = GetDimensions(search);
               G4cout<<"Sonde "<< sonde<< " selected, with a bi-thickness of "<< sampleDicke<<" um."<<"\n";
            }
        G4Tubs* BismutSample = new G4Tubs("BiSample",0,0.5*(16*mm),0.5*(sampleDicke),0*deg,360*deg);

        G4cout<<"Construction 2um Sample Holders: all Solid Volumes set"<<"\n";

    // -------------------------------------------------------------------------------------------------------
    // ================================== logical volumes ====================================================
    // -------------------------------------------------------------------------------------------------------

        GrundKoerper1LOG = new G4LogicalVolume(GEOVolGrundKoerperZylinder1, mPMMA, "GrundKoerper1Log",0, 0, 0);
        GrundKoerper2LOG = new G4LogicalVolume(GEOVolGrundKoerperZylinder2, mPMMA, "GrundKoerper2Log",0, 0, 0);
        GrundKoerper3LOG = new G4LogicalVolume(GEOVolGrundKoerperZylinder3, mPMMA, "GrundKoerper3Log",0, 0, 0);
        HalterSondePart1LOG = new G4LogicalVolume(GEOVolHalterSondeZylinder1, mPMMA, "HalterSondePart1LOG",0, 0, 0);
        HalterSondePart2LOG = new G4LogicalVolume(GEOVolHalterSondeZylinder2, mPMMA, "HalterSondePart2LOG",0, 0, 0);
        DistanzRingLOG = new G4LogicalVolume(DistanzRing, mPMMA, "DistanzLOG",0, 0, 0);
        GegenSondeLOG = new G4LogicalVolume(GegenSonde, mPMMA, "GegenSondeLOG",0, 0, 0);
        FixierRingLOG = new G4LogicalVolume(FixierRing, mPMMA, "FixierRingLOG",0, 0, 0);
        BismutSampleLOG = new G4LogicalVolume(BismutSample, Bismut, "BiSampleLOG",0, 0, 0);
        SiWaferLOG=new G4LogicalVolume(SiWafer, silicon, "SiWaferLOG",0, 0, 0);

        G4cout <<"Construction 2um Sample Holders: all Logical Volumes Set"<<"\n";

    // -------------------------------------------------------------------------------------------------------
    // =========================== Positioning of physical volumes ===========================================
    // -------------------------------------------------------------------------------------------------------


        G4double H1 =0.5*GetDimensions("GrundKoerperL1");
        G4double ShiftGKpart2 = H1+0.5*(GetDimensions("GrundKoerperL2")-GetDimensions("GrundKoerperL1"));
        G4double ShiftGKpart3 = ShiftGKpart2+0.5*(GetDimensions("GrundKoerperL2")-GetDimensions("GrundKoerperL1"))+0.5*(GetDimensions("GrundKoerperL4")-GetDimensions("GrundKoerperL2"));

        G4RotationMatrix* yRot = new G4RotationMatrix;  // Rotates X and Z axes only
        		          yRot->rotateY(M_PI*rad);

    //--------------------------------------- sample components ------------------------------------------------------
        G4double HalterSondenPlatzierungPart1,HalterSondenPlatzierungPart2,BiSamplePlatzierung,SiWaferPlatzierung;
        G4double GegenSondePlatzierung,DistanzRingPlatzierung1,DistanzRingPlatzierung2,FixierRingPlatzierung;

        HalterSondenPlatzierungPart1=HalterSondenPlatzierungPart2=BiSamplePlatzierung=SiWaferPlatzierung=GegenSondePlatzierung=DistanzRingPlatzierung1=DistanzRingPlatzierung2=FixierRingPlatzierung=0.*cm;

    // ----------Position 1: [ Sample + holder ] -> Distanzring1 -> Distanzring2 -------------------------------------
        if(PositionsNummer==1){
          HalterSondenPlatzierungPart1 = H1-0.5*(GetDimensions("HalterSondeL2")-GetDimensions("HalterSondeL1"));
          HalterSondenPlatzierungPart2 = H1-1.0*(GetDimensions("HalterSondeL2")-GetDimensions("HalterSondeL1"))-0.5*GetDimensions("HalterSondeL1");
          BiSamplePlatzierung= HalterSondenPlatzierungPart2-0.5*GetDimensions("HalterSondeL1")-0.5*(sampleDicke);
          SiWaferPlatzierung= BiSamplePlatzierung-0.5*(sampleDicke)-0.5*GetDimensions("SiWaferDicke");
          GegenSondePlatzierung=SiWaferPlatzierung-0.5*GetDimensions("SiWaferDicke")-0.5*GetDimensions("GegenSondeL1");
          DistanzRingPlatzierung1=GegenSondePlatzierung-0.5*GetDimensions("GegenSondeL1")-0.5*GetDimensions("DistanzRingL1");
          DistanzRingPlatzierung2=DistanzRingPlatzierung1-GetDimensions("DistanzRingL1");
          FixierRingPlatzierung=DistanzRingPlatzierung2-0.5*GetDimensions("DistanzRingL1")-0.5*GetDimensions("FixierRingL1");

          G4cout<<"Platzierung der Bismut-Folie: "<< BiSamplePlatzierung <<" mm\n";
        }

    // ----------Position 2 : Distanzring1  -> [ Sample + holder ] -> Distanzring2 -------------------------------------
        else if(PositionsNummer==2){
          DistanzRingPlatzierung1=H1-0.5*GetDimensions("DistanzRingL1");
          HalterSondenPlatzierungPart1 = DistanzRingPlatzierung1-0.5*GetDimensions("DistanzRingL1")-0.5*(GetDimensions("HalterSondeL2")-GetDimensions("HalterSondeL1"));
          HalterSondenPlatzierungPart2 = DistanzRingPlatzierung1-1.0*(GetDimensions("HalterSondeL2")-GetDimensions("HalterSondeL1"))-0.5*GetDimensions("HalterSondeL1");
          BiSamplePlatzierung= HalterSondenPlatzierungPart2-0.5*GetDimensions("HalterSondeL1")-0.5*(sampleDicke);
          SiWaferPlatzierung= BiSamplePlatzierung-0.5*(sampleDicke)-0.5*GetDimensions("SiWaferDicke");
          GegenSondePlatzierung=SiWaferPlatzierung-0.5*GetDimensions("SiWaferDicke")-0.5*GetDimensions("GegenSondeL1");
          DistanzRingPlatzierung2=GegenSondePlatzierung-0.5*GetDimensions("GegenSondeL1")-0.5*GetDimensions("DistanzRingL1");
          FixierRingPlatzierung=DistanzRingPlatzierung2-0.5*GetDimensions("DistanzRingL1")-0.5*GetDimensions("FixierRingL1");

          G4cout<<"Platzierung der Bismut-Folie: "<< BiSamplePlatzierung <<" mm\n";
        }

    // ----------Position 2 :  Distanzring1  -> Distanzring2 -> [ Sample + holder ] ----------------------
        else if(PositionsNummer==3){
          DistanzRingPlatzierung1=H1-0.5*GetDimensions("DistanzRingL1");
          DistanzRingPlatzierung2=DistanzRingPlatzierung1-1.0*GetDimensions("DistanzRingL1");
          HalterSondenPlatzierungPart1 = DistanzRingPlatzierung2-0.5*GetDimensions("DistanzRingL1")-0.5*(GetDimensions("HalterSondeL2")-GetDimensions("HalterSondeL1"));
          HalterSondenPlatzierungPart2 = DistanzRingPlatzierung2-1.0*(GetDimensions("HalterSondeL2")-GetDimensions("HalterSondeL1"))-0.5*GetDimensions("HalterSondeL1");
          BiSamplePlatzierung= HalterSondenPlatzierungPart2-0.5*GetDimensions("HalterSondeL1")-0.5*(sampleDicke);
          SiWaferPlatzierung= BiSamplePlatzierung-0.5*(sampleDicke)-0.5*GetDimensions("SiWaferDicke");
          GegenSondePlatzierung=SiWaferPlatzierung-0.5*GetDimensions("SiWaferDicke")-0.5*GetDimensions("GegenSondeL1");
          FixierRingPlatzierung=GegenSondePlatzierung-0.5*GetDimensions("GegenSondeL1")-0.5*GetDimensions("FixierRingL1");

          G4cout<<"Platzierung der Bismut-Folie: "<< BiSamplePlatzierung <<" mm\n";
        }

    // -------------------------------------------------------------------------------------------------------
    // ======================================= physical volumes ==============================================
    // -------------------------------------------------------------------------------------------------------

        GrundKoerper1PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,0),GrundKoerper1LOG, "GrundKoerper1PHYS",VacuumLOG, false, 0);
        GrundKoerper2PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,ShiftGKpart2),GrundKoerper2LOG, "GrundKoerper2PHYS",VacuumLOG, false, 0);
        GrundKoerper3PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,ShiftGKpart3),GrundKoerper3LOG, "GrundKoerper3PHYS",VacuumLOG, false, 0);
        HalterSondePart1PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,HalterSondenPlatzierungPart1),HalterSondePart1LOG, "HalterSondePart1PHYS",VacuumLOG, false, 0);
        HalterSondePart2PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,HalterSondenPlatzierungPart2),HalterSondePart2LOG, "HalterSondePart2PHYS",VacuumLOG, false, 0);
        BismutSamplePHYS =  new G4PVPlacement(yRot, G4ThreeVector(0, 0,BiSamplePlatzierung),BismutSampleLOG, "BismutSamplePHYS",VacuumLOG, false, 0);
        SiWaferPHYS =  new G4PVPlacement(0, G4ThreeVector(0, 0,SiWaferPlatzierung),SiWaferLOG, "SiWaferPHYS",VacuumLOG, false, 0);
        GegenSondePHYS =  new G4PVPlacement(0, G4ThreeVector(0, 0,GegenSondePlatzierung),GegenSondeLOG, "GegenSondePHYS",VacuumLOG, false, 0);
        DistanzRing1PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,DistanzRingPlatzierung1),DistanzRingLOG, "DistanzRing1PHYS",VacuumLOG, false, 0);
        DistanzRing2PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,DistanzRingPlatzierung2),DistanzRingLOG, "DistanzRing2PHYS",VacuumLOG, false, 0);
        FixierRingPHYS =  new G4PVPlacement(0, G4ThreeVector(0, 0,FixierRingPlatzierung),FixierRingLOG, "FixierRingPHYS",VacuumLOG, false, 0);

        G4cout <<"Construction New Sample Holders: all Physical Volumes Set"<<"\n";

    // -------------------------------------------------------------------------------------------------------
    // =================================== Visual Attributes =================================================
    // -------------------------------------------------------------------------------------------------------

         G4Colour lgray (0.7,  0.7, 0.7);
         G4VisAttributes *hAluVisAtt = new G4VisAttributes(lgray);
         hAluVisAtt->SetVisibility(false);
         DistanzRingLOG->SetVisAttributes(hAluVisAtt);
         HalterSondePart1LOG->SetVisAttributes(hAluVisAtt);
         HalterSondePart2LOG->SetVisAttributes(hAluVisAtt);
         FixierRingLOG->SetVisAttributes(hAluVisAtt);
         GegenSondeLOG->SetVisAttributes(hAluVisAtt);
        //
         G4VisAttributes *hBisVisAtt = new G4VisAttributes(lgray);
         hBisVisAtt->SetVisibility(true);
         BismutSampleLOG->SetVisAttributes(hBisVisAtt);
        //
         G4Colour lplastic (0.8,  0.8, 0.0);
         G4VisAttributes *hPlasticVisAtt = new G4VisAttributes(lplastic);
         hPlasticVisAtt->SetVisibility(false);
         GrundKoerper1LOG->SetVisAttributes(hPlasticVisAtt);
         GrundKoerper2LOG->SetVisAttributes(hPlasticVisAtt);
         GrundKoerper3LOG->SetVisAttributes(hPlasticVisAtt);

}

//================================================================================
//=============---- Construct sample holder with solicon only ---=================
//================================================================================

void BetaChamberDetectorConstruction::constructSampleHoldersSiliconOnly(){


    // ---- Materialien -----
    G4Material *mPMMA = G4Material::GetMaterial("PMMA");
    G4Material *mylar = G4Material::GetMaterial("Mylar");
    G4Material *silicon = G4Material::GetMaterial("Silicon");

    // ---------- geometrical volumes -----------------------------------------------------------------------
    G4Tubs* GEOVolGrundKoerperZylinder1 = new G4Tubs("GVGKZ1",0.5*GetDimensions("GrundKoerperD1"),0.5*GetDimensions("GrundKoerperD4"),0.5*GetDimensions("GrundKoerperL1"),0*deg,360*deg);
    G4Tubs* GEOVolGrundKoerperZylinder2 = new G4Tubs("GVGKZ2",0.5*GetDimensions("GrundKoerperD2"),0.5*GetDimensions("GrundKoerperD4"),0.5*(GetDimensions("GrundKoerperL2")-GetDimensions("GrundKoerperL1")),0*deg,360*deg);
    G4Tubs* GEOVolGrundKoerperZylinder3 = new G4Tubs("GVGKZ3",0.5*GetDimensions("GrundKoerperD3"),0.5*GetDimensions("GrundKoerperD4"),0.5*(GetDimensions("GrundKoerperL4")-GetDimensions("GrundKoerperL2")),0*deg,360*deg);
    G4Tubs* GEOVolHalterSondeZylinder1 = new G4Tubs("GVHSZ1",0.5*GetDimensions("HalterSondeD2"),0.5*GetDimensions("HalterSondeD3"),0.5*(GetDimensions("HalterSondeL2")-GetDimensions("HalterSondeL1")),0*deg,360*deg);
    G4Tubs* GEOVolHalterSondeZylinder2 = new G4Tubs("GVHSZ2",0.5*GetDimensions("HalterSondeD1"),0.5*GetDimensions("HalterSondeD3"),0.5*GetDimensions("HalterSondeL1"),0*deg,360*deg);
    G4Tubs* DistanzRing = new G4Tubs("GVDSTR",0.5*GetDimensions("DistanzRingD1"),0.5*GetDimensions("DistanzRingD2"),0.5*GetDimensions("DistanzRingL1"),0*deg,360*deg);
    G4Tubs* GegenSonde = new G4Tubs("GVGeSo",0.5*GetDimensions("GegenSondeD1"),0.5*GetDimensions("GegenSondeD2"),0.5*GetDimensions("GegenSondeL1"),0*deg,360*deg);
    G4Tubs* FixierRing = new G4Tubs("GVFixR",0.5*GetDimensions("FixierRingD1"),0.5*GetDimensions("FixierRingD2"),0.5*GetDimensions("FixierRingL1"),0*deg,360*deg);
    G4double sampleDicke;
    G4Tubs * SiWafer = new G4Tubs("SiliconWafer",0,0.5*GetDimensions("HalterSondeD2"),0.5*GetDimensions("SiWaferDicke"),0*deg,360*deg);

    G4cout<<"====================================="<<"\n";
    G4cout<<"SiliconSampleThickness was set to "<< SiliconSampleThickness<<"\n";
    G4cout<<"This corresponds to a Si-Wafer thickness of "<<GetDimensions("SiWaferDicke") <<" um"<<"\n";
    G4cout<<"====================================="<<"\n";
    // --------- logical volumes----------------------------------------------------------------------------------------------

    GrundKoerper1LOG = new G4LogicalVolume(GEOVolGrundKoerperZylinder1, mPMMA, "GrundKoerper1Log",0, 0, 0);
    GrundKoerper2LOG = new G4LogicalVolume(GEOVolGrundKoerperZylinder2, mPMMA, "GrundKoerper2Log",0, 0, 0);
    GrundKoerper3LOG = new G4LogicalVolume(GEOVolGrundKoerperZylinder3, mPMMA, "GrundKoerper3Log",0, 0, 0);
    HalterSondePart1LOG = new G4LogicalVolume(GEOVolHalterSondeZylinder1, mPMMA, "HalterSondePart1LOG",0, 0, 0);
    HalterSondePart2LOG = new G4LogicalVolume(GEOVolHalterSondeZylinder2, mPMMA, "HalterSondePart2LOG",0, 0, 0);
    DistanzRingLOG = new G4LogicalVolume(DistanzRing, mPMMA, "DistanzLOG",0, 0, 0);
    GegenSondeLOG = new G4LogicalVolume(GegenSonde, mPMMA, "GegenSondeLOG",0, 0, 0);
    FixierRingLOG = new G4LogicalVolume(FixierRing, mPMMA, "FixierRingLOG",0, 0, 0);

    SiWaferLOG=new G4LogicalVolume(SiWafer, silicon, "SiWaferLOG",0, 0, 0);

    G4cout<<"Construction New Sample Holders: all Logical Volumes Set"<<"\n";



    // =======================================================================================================================
    // ================================= positioning of physical volumes =====================================================
    // =======================================================================================================================

    // -------------------------------------------- basics -----------------------------------------------------------

    G4double H1 =0.5*GetDimensions("GrundKoerperL1");
    G4double ShiftGKpart2 = H1+0.5*(GetDimensions("GrundKoerperL2")-GetDimensions("GrundKoerperL1"));
    G4double ShiftGKpart3 = ShiftGKpart2+0.5*(GetDimensions("GrundKoerperL2")-GetDimensions("GrundKoerperL1"))+0.5*(GetDimensions("GrundKoerperL4")-GetDimensions("GrundKoerperL2"));


    G4RotationMatrix* yRot = new G4RotationMatrix;  // Rotates X and Z axes only
    		  yRot->rotateY(M_PI*rad);

    //--------------------------------------- sample components ------------------------------------------------------
      G4double HalterSondenPlatzierungPart1,HalterSondenPlatzierungPart2,BiSamplePlatzierung,SiWaferPlatzierung,
    	   GegenSondePlatzierung,DistanzRingPlatzierung1,DistanzRingPlatzierung2,FixierRingPlatzierung;

      HalterSondenPlatzierungPart1=HalterSondenPlatzierungPart2=BiSamplePlatzierung=SiWaferPlatzierung=GegenSondePlatzierung=DistanzRingPlatzierung1=DistanzRingPlatzierung2=FixierRingPlatzierung=0.*cm;

    // ----------Position 1: [ Sample + holder ] -> Distanzring1 -> Distanzring2 -------------------------------------
    if(PositionsNummer==1){
      HalterSondenPlatzierungPart1 = H1-0.5*(GetDimensions("HalterSondeL2")-GetDimensions("HalterSondeL1"));
      HalterSondenPlatzierungPart2 = H1-1.0*(GetDimensions("HalterSondeL2")-GetDimensions("HalterSondeL1"))-0.5*GetDimensions("HalterSondeL1");
      SiWaferPlatzierung= HalterSondenPlatzierungPart2-0.5*GetDimensions("HalterSondeL1")-0.5*GetDimensions("SiWaferDicke");
      GegenSondePlatzierung=SiWaferPlatzierung-0.5*GetDimensions("SiWaferDicke")-0.5*GetDimensions("GegenSondeL1");
      DistanzRingPlatzierung1=GegenSondePlatzierung-0.5*GetDimensions("GegenSondeL1")-0.5*GetDimensions("DistanzRingL1");
      DistanzRingPlatzierung2=DistanzRingPlatzierung1-GetDimensions("DistanzRingL1");
      FixierRingPlatzierung=DistanzRingPlatzierung2-0.5*GetDimensions("DistanzRingL1")-0.5*GetDimensions("FixierRingL1");
    }

    // ----------Position 2 : Distanzring1  -> [ Sample + holder ] -> Distanzring2 -------------------------------------
    else if(PositionsNummer==2){
      DistanzRingPlatzierung1=H1-0.5*GetDimensions("DistanzRingL1");
      HalterSondenPlatzierungPart1 = DistanzRingPlatzierung1-0.5*GetDimensions("DistanzRingL1")-0.5*(GetDimensions("HalterSondeL2")-GetDimensions("HalterSondeL1"));
      HalterSondenPlatzierungPart2 = DistanzRingPlatzierung1-1.0*(GetDimensions("HalterSondeL2")-GetDimensions("HalterSondeL1"))-0.5*GetDimensions("HalterSondeL1");
      SiWaferPlatzierung= HalterSondenPlatzierungPart2-0.5*GetDimensions("HalterSondeL1")-0.5*GetDimensions("SiWaferDicke");
      GegenSondePlatzierung=SiWaferPlatzierung-0.5*GetDimensions("SiWaferDicke")-0.5*GetDimensions("GegenSondeL1");
      DistanzRingPlatzierung2=GegenSondePlatzierung-0.5*GetDimensions("GegenSondeL1")-0.5*GetDimensions("DistanzRingL1");
      FixierRingPlatzierung=DistanzRingPlatzierung2-0.5*GetDimensions("DistanzRingL1")-0.5*GetDimensions("FixierRingL1");
    }

    // ----------Position 2 :  Distanzring1  -> Distanzring2 -> [ Sample + holder ] -------------------------------------
    else if(PositionsNummer==3){
      DistanzRingPlatzierung1=H1-0.5*GetDimensions("DistanzRingL1");
      DistanzRingPlatzierung2=DistanzRingPlatzierung1-1.0*GetDimensions("DistanzRingL1");
      HalterSondenPlatzierungPart1 = DistanzRingPlatzierung2-0.5*GetDimensions("DistanzRingL1")-0.5*(GetDimensions("HalterSondeL2")-GetDimensions("HalterSondeL1"));
      HalterSondenPlatzierungPart2 = DistanzRingPlatzierung2-1.0*(GetDimensions("HalterSondeL2")-GetDimensions("HalterSondeL1"))-0.5*GetDimensions("HalterSondeL1");
      SiWaferPlatzierung= HalterSondenPlatzierungPart2-0.5*GetDimensions("HalterSondeL1")-0.5*GetDimensions("SiWaferDicke");
      GegenSondePlatzierung=SiWaferPlatzierung-0.5*GetDimensions("SiWaferDicke")-0.5*GetDimensions("GegenSondeL1");
      FixierRingPlatzierung=GegenSondePlatzierung-0.5*GetDimensions("GegenSondeL1")-0.5*GetDimensions("FixierRingL1");
    }

    GrundKoerper1PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,0),GrundKoerper1LOG, "GrundKoerper1PHYS",VacuumLOG, false, 0);
    GrundKoerper2PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,ShiftGKpart2),GrundKoerper2LOG, "GrundKoerper2PHYS",VacuumLOG, false, 0);
    GrundKoerper3PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,ShiftGKpart3),GrundKoerper3LOG, "GrundKoerper3PHYS",VacuumLOG, false, 0);
    HalterSondePart1PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,HalterSondenPlatzierungPart1),HalterSondePart1LOG, "HalterSondePart1PHYS",VacuumLOG, false, 0);
    HalterSondePart2PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,HalterSondenPlatzierungPart2),HalterSondePart2LOG, "HalterSondePart2PHYS",VacuumLOG, false, 0);
    SiWaferPHYS =  new G4PVPlacement(0, G4ThreeVector(0, 0,SiWaferPlatzierung),SiWaferLOG, "SiWaferPHYS",VacuumLOG, false, 0);
    GegenSondePHYS =  new G4PVPlacement(0, G4ThreeVector(0, 0,GegenSondePlatzierung),GegenSondeLOG, "GegenSondePHYS",VacuumLOG, false, 0);
    DistanzRing1PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,DistanzRingPlatzierung1),DistanzRingLOG, "DistanzRing1PHYS",VacuumLOG, false, 0);
    DistanzRing2PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,DistanzRingPlatzierung2),DistanzRingLOG, "DistanzRing2PHYS",VacuumLOG, false, 0);
    FixierRingPHYS =  new G4PVPlacement(0, G4ThreeVector(0, 0,FixierRingPlatzierung),FixierRingLOG, "FixierRingPHYS",VacuumLOG, false, 0);
      G4cout<<"Construction New Sample Holders: all Physical Volumes Set"<<"\n";

    // ====== Vis - Attributes ======


     G4Colour lgray (0.7,  0.7, 0.7);
     G4VisAttributes *hAluVisAtt = new G4VisAttributes(lgray);
     hAluVisAtt->SetVisibility(false);
     DistanzRingLOG->SetVisAttributes(hAluVisAtt);
     HalterSondePart1LOG->SetVisAttributes(hAluVisAtt);
     HalterSondePart2LOG->SetVisAttributes(hAluVisAtt);
     FixierRingLOG->SetVisAttributes(hAluVisAtt);
     GegenSondeLOG->SetVisAttributes(hAluVisAtt);
    //
    //  G4VisAttributes *hBisVisAtt = new G4VisAttributes(lgray);
    //  hBisVisAtt->SetVisibility(true);
    //  BismutSampleLOG->SetVisAttributes(hBisVisAtt);
    //
     G4Colour lplastic (0.8,  0.8, 0.0);
     G4VisAttributes *hPlasticVisAtt = new G4VisAttributes(lplastic);
     hPlasticVisAtt->SetVisibility(false);
     GrundKoerper1LOG->SetVisAttributes(hPlasticVisAtt);
     GrundKoerper2LOG->SetVisAttributes(hPlasticVisAtt);
     GrundKoerper3LOG->SetVisAttributes(hPlasticVisAtt);


    //
     G4Colour lCopper (0.9,  0.0, 0.1);
     G4VisAttributes *hCopperVisAtt = new G4VisAttributes(lCopper);
     hCopperVisAtt->SetVisibility(false);
     CopperZylinderLOG->SetVisAttributes(hCopperVisAtt);
     KupferDeckelLOG->SetVisAttributes(hCopperVisAtt);

}

//================================================================================
//==========---- Construct sample holder for allowed transitions ---==============
//================================================================================

void BetaChamberDetectorConstruction::constructSampleHolderSiliziumProbenSetup(){


    // ---- Materialien -----
    G4Material *mPMMA = G4Material::GetMaterial("PMMA");
    G4Material *mylar = G4Material::GetMaterial("Mylar");
    G4Material *silicon = G4Material::GetMaterial("Silicon");
    // G4Material *silicon0 = G4Material::GetMaterial("Silicon0");

    // ---------- geometrical volumes -----------------------------------------------------------------------
    G4Tubs* GEOVolGrundKoerperZylinder1 = new G4Tubs("GVGKZ1",0.5*GetDimensions("GrundKoerperD1"),0.5*GetDimensions("GrundKoerperD4"),0.5*GetDimensions("GrundKoerperL1"),0*deg,360*deg);
    G4Tubs* GEOVolGrundKoerperZylinder2 = new G4Tubs("GVGKZ2",0.5*GetDimensions("GrundKoerperD2"),0.5*GetDimensions("GrundKoerperD4"),0.5*(GetDimensions("GrundKoerperL2")-GetDimensions("GrundKoerperL1")),0*deg,360*deg);
    G4Tubs* GEOVolGrundKoerperZylinder3 = new G4Tubs("GVGKZ3",0.5*GetDimensions("GrundKoerperD3"),0.5*GetDimensions("GrundKoerperD4"),0.5*(GetDimensions("GrundKoerperL4")-GetDimensions("GrundKoerperL2")),0*deg,360*deg);
    G4Tubs* DistanzRing = new G4Tubs("GVDSTR",0.5*GetDimensions("DistanzRingD1"),0.5*GetDimensions("DistanzRingD2"),0.5*GetDimensions("DistanzRingL1"),0*deg,360*deg);
    G4Tubs* FixierRing = new G4Tubs("GVFixR",0.5*GetDimensions("FixierRingD1"),0.5*GetDimensions("FixierRingD2"),0.5*GetDimensions("FixierRingL1"),0*deg,360*deg);

    // ---------- Anpassung an quadratische Probenform -----------------------------------------------------------
    G4Tubs* GegenSonde = new G4Tubs("GVGeSo",0.5*GetDimensions("GegenSondeD1SiSetup"),0.5*GetDimensions("GegenSondeD2SiSetup"),0.5*GetDimensions("GegenSondeL1SiSetup"),0*deg,360*deg);
    G4Tubs* GEOVolHalterSondeZylinder1 = new G4Tubs("GVHSZ1",0.5*GetDimensions("HalterSondeD2"),0.5*GetDimensions("HalterSondeD3"),0.5*(GetDimensions("HalterSondeL2")-GetDimensions("HalterSondeL1")),0*deg,360*deg);

       // --- substractionSolid
       G4Tubs* HalterSondeScheibe = new G4Tubs("GVHSScheibe",0,0.5*GetDimensions("HalterSondeD3"),0.5*GetDimensions("HalterSondeL1"),0*deg,360*deg);
       G4Trd*  HalterSondeTrapez= new G4Trd("GVHSTrapez",GetDimensions("HalterSondeTrapezD1"),GetDimensions("HalterSondeTrapezD2"),GetDimensions("HalterSondeTrapezD1"),GetDimensions("HalterSondeTrapezD2"),GetDimensions("HalterSondeTrapezLZ"));
       G4ThreeVector zTransTrapez(0, 0, 0);

    G4SubtractionSolid* GEOVolHalterSondeZylinder2 = new G4SubtractionSolid("GVHS", HalterSondeScheibe, HalterSondeTrapez,0,zTransTrapez);

        // --- substractionSolid
        G4Tubs* HalterSondeScheibeProbenEinfassung = new G4Tubs("GVHSScheibeProbenEinfassung",0,0.5*GetDimensions("HalterSondeD3"),0.5*GetDimensions("HalterProbenEinfassungD1"),0*deg,360*deg);
        G4Box*  HalterSondeBoxProbenEinfassung = new G4Box("GVHSBoxProbenEinfassung",GetDimensions("HalterSondeTrapezD2"),GetDimensions("HalterSondeTrapezD2"),0.6*GetDimensions("HalterProbenEinfassungD1"));
    G4SubtractionSolid* GEOVolHalterSondeProbenEinfassung = new G4SubtractionSolid("GVHSProbenEinfassung", HalterSondeScheibeProbenEinfassung, HalterSondeBoxProbenEinfassung);

    //------------------ Sample -----------------------------------------------------------------------------

    G4cout<<" ===================================== "<<"\n";
    G4cout<<" SiliconSampleThickness was set to "<< SiliconSampleThickness<<"\n";
    G4cout<<" This corresponds to a Si-Wafer thickness of "<<GetDimensions("SiWaferDicke") <<" um"<<"\n";
    G4cout<<" ===================================== "<<"\n";


    G4double sampleDicke=GetDimensions("SiWaferDicke");
    G4Box *  QuadraticATSample = new G4Box("QATSample",GetDimensions("HalterSondeTrapezD2")-1*mm,GetDimensions("HalterSondeTrapezD2")-1*mm,0.5*sampleDicke);


    G4cout <<" sample dimensions in x and y "<<"\n";
    G4cout <<" X-axis: "<<  QuadraticATSample->GetXHalfLength()*2 <<" mm"<<"\n";
    G4cout <<" Y-axis: "<<  QuadraticATSample->GetYHalfLength()*2 <<" mm"<<"\n";
    G4cout<<" ===================================== "<<"\n";

    // --------- logical volumes----------------------------------------------------------------------------------------------

    GrundKoerper1LOG = new G4LogicalVolume(GEOVolGrundKoerperZylinder1, mPMMA, "GrundKoerper1Log",0, 0, 0);
    GrundKoerper2LOG = new G4LogicalVolume(GEOVolGrundKoerperZylinder2, mPMMA, "GrundKoerper2Log",0, 0, 0);
    GrundKoerper3LOG = new G4LogicalVolume(GEOVolGrundKoerperZylinder3, mPMMA, "GrundKoerper3Log",0, 0, 0);
    HalterSondePart1LOG = new G4LogicalVolume(GEOVolHalterSondeZylinder1, mPMMA, "HalterSondePart1LOG",0, 0, 0);
    HalterSondePart2LOG = new G4LogicalVolume(GEOVolHalterSondeZylinder2, mPMMA, "HalterSondePart2LOG",0, 0, 0);
    HalterSondePart3LOG = new G4LogicalVolume(GEOVolHalterSondeProbenEinfassung, mPMMA, "HalterSondePart3LOG",0, 0, 0);
    DistanzRingLOG = new G4LogicalVolume(DistanzRing, mPMMA, "DistanzLOG",0, 0, 0);
    GegenSondeLOG = new G4LogicalVolume(GegenSonde, mPMMA, "GegenSondeLOG",0, 0, 0);
    FixierRingLOG = new G4LogicalVolume(FixierRing, mPMMA, "FixierRingLOG",0, 0, 0);
    ATSampleLOG=new G4LogicalVolume(QuadraticATSample, silicon, "ATSampleLOG",0, 0, 0);


    G4cout<<"Construction New Sample Holders: all Logical Volumes Set"<<"\n";

    // =======================================================================================================================
    // ================================= positioning of physical volumes =====================================================
    // =======================================================================================================================

    // -------------------------------------------- basics -----------------------------------------------------------

    G4double H1 =0.5*GetDimensions("GrundKoerperL1");
    G4double ShiftGKpart2 = H1+0.5*(GetDimensions("GrundKoerperL2")-GetDimensions("GrundKoerperL1"));
    G4double ShiftGKpart3 = ShiftGKpart2+0.5*(GetDimensions("GrundKoerperL2")-GetDimensions("GrundKoerperL1"))+0.5*(GetDimensions("GrundKoerperL4")-GetDimensions("GrundKoerperL2"));
    G4double HaltersondePart1Dicke =GetDimensions("HalterSondeL2")-GetDimensions("HalterSondeL1");

    G4RotationMatrix* yRot = new G4RotationMatrix;  // Rotates X and Z axes only
    		  yRot->rotateY(M_PI*rad);

    //--------------------------------------- sample components ------------------------------------------------------
      G4double HalterSondenPlatzierungPart1,HalterSondenPlatzierungPart2,HalterSondenPlatzierungPart3,ATSamplePlatzierung,
    	       GegenSondePlatzierung,DistanzRingPlatzierung1,DistanzRingPlatzierung2,FixierRingPlatzierung,SiWaferPlatzierung;

      HalterSondenPlatzierungPart1=HalterSondenPlatzierungPart2=HalterSondenPlatzierungPart3=ATSamplePlatzierung=GegenSondePlatzierung=DistanzRingPlatzierung1=DistanzRingPlatzierung2=FixierRingPlatzierung=SiWaferPlatzierung=0.*cm;

    // ----------Position 1: [ Sample + holder ] -> Distanzring1 -> Distanzring2 -------------------------------------
    if(PositionsNummer==1){
      HalterSondenPlatzierungPart1 = H1-0.5*(HaltersondePart1Dicke);
      HalterSondenPlatzierungPart2 = H1-1.0*(HaltersondePart1Dicke)-0.5*GetDimensions("HalterSondeL1");
      HalterSondenPlatzierungPart3 = HalterSondenPlatzierungPart2-0.5*GetDimensions("HalterSondeL1")-0.5*GetDimensions("HalterProbenEinfassungD1");
      ATSamplePlatzierung= HalterSondenPlatzierungPart2-0.5*GetDimensions("HalterSondeL1")-0.5*sampleDicke;
      GegenSondePlatzierung=HalterSondenPlatzierungPart3-0.5*GetDimensions("HalterProbenEinfassungD1")-0.5*GetDimensions("GegenSondeL1SiSetup");
      DistanzRingPlatzierung1=GegenSondePlatzierung-0.5*GetDimensions("GegenSondeL1SiSetup")-0.5*GetDimensions("DistanzRingL1");
      DistanzRingPlatzierung2=DistanzRingPlatzierung1-GetDimensions("DistanzRingL1");
      FixierRingPlatzierung=DistanzRingPlatzierung2-0.5*GetDimensions("DistanzRingL1")-0.5*GetDimensions("FixierRingL1");
    }

    // ----------Position 2 : Distanzring1  -> [ Sample + holder ] -> Distanzring2 -------------------------------------
    else if(PositionsNummer==2){
      DistanzRingPlatzierung1=H1-0.5*GetDimensions("DistanzRingL1");
      HalterSondenPlatzierungPart1 = DistanzRingPlatzierung1-0.5*GetDimensions("DistanzRingL1")-0.5*(HaltersondePart1Dicke);
      HalterSondenPlatzierungPart2 = HalterSondenPlatzierungPart1-0.5*(HaltersondePart1Dicke)-0.5*GetDimensions("HalterSondeL1");
      HalterSondenPlatzierungPart3 = HalterSondenPlatzierungPart2-0.5*GetDimensions("HalterSondeL1")-0.5*GetDimensions("HalterProbenEinfassungD1");
      ATSamplePlatzierung= HalterSondenPlatzierungPart2-0.5*GetDimensions("HalterSondeL1")-0.5*sampleDicke;
      GegenSondePlatzierung=HalterSondenPlatzierungPart3-0.5*GetDimensions("HalterProbenEinfassungD1")-0.5*GetDimensions("GegenSondeL1SiSetup");
      DistanzRingPlatzierung2=GegenSondePlatzierung-0.5*GetDimensions("GegenSondeL1SiSetup")-0.5*GetDimensions("DistanzRingL1");
      FixierRingPlatzierung=DistanzRingPlatzierung2-0.5*GetDimensions("DistanzRingL1")-0.5*GetDimensions("FixierRingL1");
    }

    // ----------Position 2 :  Distanzring1  -> Distanzring2 -> [ Sample + holder ] -------------------------------------
    else if(PositionsNummer==3){
      DistanzRingPlatzierung1=H1-0.5*GetDimensions("DistanzRingL1");
      DistanzRingPlatzierung2=DistanzRingPlatzierung1-1.0*GetDimensions("DistanzRingL1");
      HalterSondenPlatzierungPart1 = DistanzRingPlatzierung2-0.5*GetDimensions("DistanzRingL1")-0.5*(HaltersondePart1Dicke);
      HalterSondenPlatzierungPart2 = HalterSondenPlatzierungPart1-0.5*(HaltersondePart1Dicke)-0.5*GetDimensions("HalterSondeL1");
      HalterSondenPlatzierungPart3 = HalterSondenPlatzierungPart2-0.5*GetDimensions("HalterSondeL1")-0.5*GetDimensions("HalterProbenEinfassungD1");
      ATSamplePlatzierung= HalterSondenPlatzierungPart2-0.5*GetDimensions("HalterSondeL1")-0.5*sampleDicke;
      GegenSondePlatzierung=HalterSondenPlatzierungPart3-0.5*GetDimensions("HalterProbenEinfassungD1")-0.5*GetDimensions("GegenSondeL1SiSetup");
      FixierRingPlatzierung=DistanzRingPlatzierung2-0.5*GetDimensions("DistanzRingL1")-0.5*GetDimensions("FixierRingL1");
    }
    //-------------------------------------
     G4cout<<"HalterSondenPlatzierungPart1 = "<< HalterSondenPlatzierungPart1<<" mm\n";
     G4cout<<"HalterSondenPlatzierungPart2 = "<< HalterSondenPlatzierungPart2<<" mm\n";
     G4cout<<"HalterSondenPlatzierungPart3 = "<< HalterSondenPlatzierungPart3<<" mm\n";
     G4cout<<"ATSamplePlatzierung = "<< ATSamplePlatzierung<<" mm\n";
     G4cout<<"GegenSondePlatzierung = "<< GegenSondePlatzierung<<" mm\n";
     G4cout<<"DistanzRingPlatzierung1 = "<< DistanzRingPlatzierung1<<" mm\n";
     G4cout<<"DistanzRingPlatzierung2 = "<< DistanzRingPlatzierung2<<" mm\n";
     G4cout<<"FixierRingPlatzierung = "<< FixierRingPlatzierung<<" mm\n";
    //-------------------------------------
    GrundKoerper1PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,0),GrundKoerper1LOG, "GrundKoerper1PHYS",VacuumLOG, false, 0);
    GrundKoerper2PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,ShiftGKpart2),GrundKoerper2LOG, "GrundKoerper2PHYS",VacuumLOG, false, 0);
    GrundKoerper3PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,ShiftGKpart3),GrundKoerper3LOG, "GrundKoerper3PHYS",VacuumLOG, false, 0);
    HalterSondePart1PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,HalterSondenPlatzierungPart1),HalterSondePart1LOG, "HalterSondePart1PHYS",VacuumLOG, false, 0);
    HalterSondePart2PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,HalterSondenPlatzierungPart2),HalterSondePart2LOG, "HalterSondePart2PHYS",VacuumLOG, false, 0);
    HalterSondePart3PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,HalterSondenPlatzierungPart3),HalterSondePart3LOG, "HalterSondePart3PHYS",VacuumLOG, false, 0);
    ATSamplePHYS =  new G4PVPlacement(0, G4ThreeVector(0, 0,ATSamplePlatzierung),ATSampleLOG, "ATSamplePHYS",VacuumLOG, false, 0);
    GegenSondePHYS =  new G4PVPlacement(0, G4ThreeVector(0, 0,GegenSondePlatzierung),GegenSondeLOG, "GegenSondePHYS",VacuumLOG, false, 0);
    DistanzRing1PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,DistanzRingPlatzierung1),DistanzRingLOG, "DistanzRing1PHYS",VacuumLOG, false, 0);
    DistanzRing2PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,DistanzRingPlatzierung2),DistanzRingLOG, "DistanzRing2PHYS",VacuumLOG, false, 0);
    FixierRingPHYS =  new G4PVPlacement(0, G4ThreeVector(0, 0,FixierRingPlatzierung),FixierRingLOG, "FixierRingPHYS",VacuumLOG, false, 0);
      G4cout<<"Construction New Sample Holders: all Physical Volumes Set"<<"\n";

    // ====== Sensitive Volume ======

    fScoringVolume =GrundKoerper3LOG;

    // ====== Vis - Attributes ======

     G4Colour lgray (0.7,  0.7, 0.7);
     G4VisAttributes *hAluVisAtt = new G4VisAttributes(lgray);
     hAluVisAtt->SetVisibility(true);
     DistanzRingLOG->SetVisAttributes(hAluVisAtt);
     HalterSondePart1LOG->SetVisAttributes(hAluVisAtt);
     HalterSondePart2LOG->SetVisAttributes(hAluVisAtt);
      HalterSondePart3LOG->SetVisAttributes(hAluVisAtt);
     FixierRingLOG->SetVisAttributes(hAluVisAtt);
     GegenSondeLOG->SetVisAttributes(hAluVisAtt);
    //
    //  G4VisAttributes *hBisVisAtt = new G4VisAttributes(lgray);
    //  hBisVisAtt->SetVisibility(true);
    //  BismutSampleLOG->SetVisAttributes(hBisVisAtt);
    //
     G4Colour lplastic (0.8,  0.8, 0.0);
     G4VisAttributes *hPlasticVisAtt = new G4VisAttributes(lplastic);
     hPlasticVisAtt->SetVisibility(false);
     GrundKoerper1LOG->SetVisAttributes(hPlasticVisAtt);
     GrundKoerper2LOG->SetVisAttributes(hPlasticVisAtt);
     GrundKoerper3LOG->SetVisAttributes(hPlasticVisAtt);


    //
     G4Colour lCopper (0.9,  0.0, 0.1);
     G4VisAttributes *hCopperVisAtt = new G4VisAttributes(lCopper);
     hCopperVisAtt->SetVisibility(false);
     CopperZylinderLOG->SetVisAttributes(hCopperVisAtt);
     KupferDeckelLOG->SetVisAttributes(hCopperVisAtt);

     G4Colour lPink (1.0,  0.0, 1.0);
     G4VisAttributes *hSampleVisAtt = new G4VisAttributes(lPink);
     hSampleVisAtt->SetVisibility(true);
     ATSampleLOG->SetVisAttributes(hSampleVisAtt);
}

//================================================================================
//================---- Construct sample holder with saphir ---====================
//================================================================================

void BetaChamberDetectorConstruction::constructSampleHolderSaphirSetup(){


    // ---- Materialien -----
    G4Material *mPMMA = G4Material::GetMaterial("PMMA");
    G4Material *mylar = G4Material::GetMaterial("Mylar");
    G4Material *silicon = G4Material::GetMaterial("Silicon");
    G4Material *saphier  = G4Material::GetMaterial("Saphier");

    // ---------- geometrical volumes -----------------------------------------------------------------------
    G4Tubs* GEOVolGrundKoerperZylinder1 = new G4Tubs("GVGKZ1",0.5*GetDimensions("GrundKoerperD1"),0.5*GetDimensions("GrundKoerperD4"),0.5*GetDimensions("GrundKoerperL1"),0*deg,360*deg);
    G4Tubs* GEOVolGrundKoerperZylinder2 = new G4Tubs("GVGKZ2",0.5*GetDimensions("GrundKoerperD2"),0.5*GetDimensions("GrundKoerperD4"),0.5*(GetDimensions("GrundKoerperL2")-GetDimensions("GrundKoerperL1")),0*deg,360*deg);
    G4Tubs* GEOVolGrundKoerperZylinder3 = new G4Tubs("GVGKZ3",0.5*GetDimensions("GrundKoerperD3"),0.5*GetDimensions("GrundKoerperD4"),0.5*(GetDimensions("GrundKoerperL4")-GetDimensions("GrundKoerperL2")),0*deg,360*deg);
    G4Tubs* GEOVolHalterSondeZylinder1 = new G4Tubs("GVHSZ1",0.5*GetDimensions("HalterSondeD2"),0.5*GetDimensions("HalterSondeD3"),0.5*(GetDimensions("HalterSondeL2")-GetDimensions("HalterSondeL1")),0*deg,360*deg);
    G4Tubs* GEOVolHalterSondeZylinder2 = new G4Tubs("GVHSZ2",0.5*GetDimensions("HalterSondeD1"),0.5*GetDimensions("HalterSondeD3"),0.5*GetDimensions("HalterSondeL1"),0*deg,360*deg);
    G4Tubs* DistanzRing = new G4Tubs("GVDSTR",0.5*GetDimensions("DistanzRingD1"),0.5*GetDimensions("DistanzRingD2"),0.5*GetDimensions("DistanzRingL1"),0*deg,360*deg);
    G4Tubs* GegenSonde = new G4Tubs("GVGeSo",0.5*GetDimensions("GegenSondeD1"),0.5*GetDimensions("GegenSondeD2"),0.5*GetDimensions("GegenSondeL1"),0*deg,360*deg);
    G4Tubs* FixierRing = new G4Tubs("GVFixR",0.5*GetDimensions("FixierRingD1"),0.5*GetDimensions("FixierRingD2"),0.5*GetDimensions("FixierRingL1"),0*deg,360*deg);
    G4double sampleDicke=30e-3*mm;
    G4double backingDicke=0.5*mm;
    G4Tubs * ATSample = new G4Tubs("ATSample",0,0.5*GetDimensions("HalterSondeD2"),0.5*sampleDicke,0*deg,360*deg);
    G4Tubs * SaphirBacking = new G4Tubs("SaphirBacking",0,0.5*GetDimensions("HalterSondeD2"),0.5*backingDicke,0*deg,360*deg);

    // --------- logical volumes----------------------------------------------------------------------------------------------

    GrundKoerper1LOG = new G4LogicalVolume(GEOVolGrundKoerperZylinder1, mPMMA, "GrundKoerper1Log",0, 0, 0);
    GrundKoerper2LOG = new G4LogicalVolume(GEOVolGrundKoerperZylinder2, mPMMA, "GrundKoerper2Log",0, 0, 0);
    GrundKoerper3LOG = new G4LogicalVolume(GEOVolGrundKoerperZylinder3, mPMMA, "GrundKoerper3Log",0, 0, 0);
    HalterSondePart1LOG = new G4LogicalVolume(GEOVolHalterSondeZylinder1, mPMMA, "HalterSondePart1LOG",0, 0, 0);
    HalterSondePart2LOG = new G4LogicalVolume(GEOVolHalterSondeZylinder2, mPMMA, "HalterSondePart2LOG",0, 0, 0);
    DistanzRingLOG = new G4LogicalVolume(DistanzRing, mPMMA, "DistanzLOG",0, 0, 0);
    GegenSondeLOG = new G4LogicalVolume(GegenSonde, mPMMA, "GegenSondeLOG",0, 0, 0);
    FixierRingLOG = new G4LogicalVolume(FixierRing, mPMMA, "FixierRingLOG",0, 0, 0);
    ATSampleLOG=new G4LogicalVolume(ATSample, silicon, "ATSampleLOG",0, 0, 0);
    SaphirBackingLOG = new G4LogicalVolume(SaphirBacking, saphier, "SaphirBackingLOG",0, 0, 0);


    G4cout<<"Construction New Sample Holders: all Logical Volumes Set"<<"\n";



    // =======================================================================================================================
    // ================================= positioning of physical volumes =====================================================
    // =======================================================================================================================

    // -------------------------------------------- basics -----------------------------------------------------------

    G4double H1 =0.5*GetDimensions("GrundKoerperL1");
    G4double ShiftGKpart2 = H1+0.5*(GetDimensions("GrundKoerperL2")-GetDimensions("GrundKoerperL1"));
    G4double ShiftGKpart3 = ShiftGKpart2+0.5*(GetDimensions("GrundKoerperL2")-GetDimensions("GrundKoerperL1"))+0.5*(GetDimensions("GrundKoerperL4")-GetDimensions("GrundKoerperL2"));


    G4RotationMatrix* yRot = new G4RotationMatrix;  // Rotates X and Z axes only
    		  yRot->rotateY(M_PI*rad);

    //--------------------------------------- sample components ------------------------------------------------------
      G4double HalterSondenPlatzierungPart1,HalterSondenPlatzierungPart2,ATSamplePlatzierung,SaphirPlatzierung,
    	       GegenSondePlatzierung,DistanzRingPlatzierung1,DistanzRingPlatzierung2,FixierRingPlatzierung;

      HalterSondenPlatzierungPart1=HalterSondenPlatzierungPart2=SaphirPlatzierung=ATSamplePlatzierung=GegenSondePlatzierung=DistanzRingPlatzierung1=DistanzRingPlatzierung2=FixierRingPlatzierung=0.*cm;

    // ----------Position 1: [ Sample + holder ] -> Distanzring1 -> Distanzring2 -------------------------------------
    if(PositionsNummer==1){
      HalterSondenPlatzierungPart1 = H1-0.5*(GetDimensions("HalterSondeL2")-GetDimensions("HalterSondeL1"));
      HalterSondenPlatzierungPart2 = H1-1.0*(GetDimensions("HalterSondeL2")-GetDimensions("HalterSondeL1"))-0.5*GetDimensions("HalterSondeL1");
      ATSamplePlatzierung= HalterSondenPlatzierungPart2-0.5*GetDimensions("HalterSondeL1")-0.5*sampleDicke;
      SaphirPlatzierung=ATSamplePlatzierung-0.5*sampleDicke-0.5*backingDicke;
      GegenSondePlatzierung=SaphirPlatzierung-0.5*backingDicke-0.5*GetDimensions("GegenSondeL1");
      DistanzRingPlatzierung1=GegenSondePlatzierung-0.5*GetDimensions("GegenSondeL1")-0.5*GetDimensions("DistanzRingL1");
      DistanzRingPlatzierung2=DistanzRingPlatzierung1-GetDimensions("DistanzRingL1");
      FixierRingPlatzierung=DistanzRingPlatzierung2-0.5*GetDimensions("DistanzRingL1")-0.5*GetDimensions("FixierRingL1");
    }

    // ----------Position 2 : Distanzring1  -> [ Sample + holder ] -> Distanzring2 -------------------------------------
    else if(PositionsNummer==2){
      DistanzRingPlatzierung1=H1-0.5*GetDimensions("DistanzRingL1");
      HalterSondenPlatzierungPart1 = DistanzRingPlatzierung1-0.5*GetDimensions("DistanzRingL1")-0.5*(GetDimensions("HalterSondeL2")-GetDimensions("HalterSondeL1"));
      HalterSondenPlatzierungPart2 = DistanzRingPlatzierung1-1.0*(GetDimensions("HalterSondeL2")-GetDimensions("HalterSondeL1"))-0.5*GetDimensions("HalterSondeL1");
      ATSamplePlatzierung= HalterSondenPlatzierungPart2-0.5*GetDimensions("HalterSondeL1")-0.5*sampleDicke;
      SaphirPlatzierung=ATSamplePlatzierung-0.5*sampleDicke-0.5*backingDicke;
      GegenSondePlatzierung=SaphirPlatzierung-0.5*backingDicke-0.5*GetDimensions("GegenSondeL1");
      DistanzRingPlatzierung2=GegenSondePlatzierung-0.5*GetDimensions("GegenSondeL1")-0.5*GetDimensions("DistanzRingL1");
      FixierRingPlatzierung=DistanzRingPlatzierung2-0.5*GetDimensions("DistanzRingL1")-0.5*GetDimensions("FixierRingL1");
    }

    // ----------Position 2 :  Distanzring1  -> Distanzring2 -> [ Sample + holder ] -------------------------------------
    else if(PositionsNummer==3){
      DistanzRingPlatzierung1=H1-0.5*GetDimensions("DistanzRingL1");
      DistanzRingPlatzierung2=DistanzRingPlatzierung1-1.0*GetDimensions("DistanzRingL1");
      HalterSondenPlatzierungPart1 = DistanzRingPlatzierung2-0.5*GetDimensions("DistanzRingL1")-0.5*(GetDimensions("HalterSondeL2")-GetDimensions("HalterSondeL1"));
      HalterSondenPlatzierungPart2 = DistanzRingPlatzierung2-1.0*(GetDimensions("HalterSondeL2")-GetDimensions("HalterSondeL1"))-0.5*GetDimensions("HalterSondeL1");
      ATSamplePlatzierung= HalterSondenPlatzierungPart2-0.5*GetDimensions("HalterSondeL1")-0.5*sampleDicke;
      SaphirPlatzierung=ATSamplePlatzierung-0.5*sampleDicke-0.5*backingDicke;
      GegenSondePlatzierung=SaphirPlatzierung-0.5*backingDicke-0.5*GetDimensions("GegenSondeL1");
      FixierRingPlatzierung=GegenSondePlatzierung-0.5*GetDimensions("GegenSondeL1")-0.5*GetDimensions("FixierRingL1");
    }

    GrundKoerper1PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,0),GrundKoerper1LOG, "GrundKoerper1PHYS",VacuumLOG, false, 0);
    GrundKoerper2PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,ShiftGKpart2),GrundKoerper2LOG, "GrundKoerper2PHYS",VacuumLOG, false, 0);
    GrundKoerper3PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,ShiftGKpart3),GrundKoerper3LOG, "GrundKoerper3PHYS",VacuumLOG, false, 0);
    HalterSondePart1PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,HalterSondenPlatzierungPart1),HalterSondePart1LOG, "HalterSondePart1PHYS",VacuumLOG, false, 0);
    HalterSondePart2PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,HalterSondenPlatzierungPart2),HalterSondePart2LOG, "HalterSondePart2PHYS",VacuumLOG, false, 0);
    ATSamplePHYS =  new G4PVPlacement(0, G4ThreeVector(0, 0,ATSamplePlatzierung),ATSampleLOG, "ATSamplePHYS",VacuumLOG, false, 0);
    SaphirBackingPHYS =  new G4PVPlacement(0, G4ThreeVector(0, 0,SaphirPlatzierung),SaphirBackingLOG, "SaphirBackingPHYS",VacuumLOG, false, 0);
    GegenSondePHYS =  new G4PVPlacement(0, G4ThreeVector(0, 0,GegenSondePlatzierung),GegenSondeLOG, "GegenSondePHYS",VacuumLOG, false, 0);
    DistanzRing1PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,DistanzRingPlatzierung1),DistanzRingLOG, "DistanzRing1PHYS",VacuumLOG, false, 0);
    DistanzRing2PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,DistanzRingPlatzierung2),DistanzRingLOG, "DistanzRing2PHYS",VacuumLOG, false, 0);
    FixierRingPHYS =  new G4PVPlacement(0, G4ThreeVector(0, 0,FixierRingPlatzierung),FixierRingLOG, "FixierRingPHYS",VacuumLOG, false, 0);
      G4cout<<"Construction New Sample Holders: all Physical Volumes Set"<<"\n";

    // ====== Vis - Attributes ======


     G4Colour lgray (0.7,  0.7, 0.7);
     G4VisAttributes *hAluVisAtt = new G4VisAttributes(lgray);
     hAluVisAtt->SetVisibility(false);
     DistanzRingLOG->SetVisAttributes(hAluVisAtt);
     HalterSondePart1LOG->SetVisAttributes(hAluVisAtt);
     HalterSondePart2LOG->SetVisAttributes(hAluVisAtt);
     FixierRingLOG->SetVisAttributes(hAluVisAtt);
     GegenSondeLOG->SetVisAttributes(hAluVisAtt);
    //
    //  G4VisAttributes *hBisVisAtt = new G4VisAttributes(lgray);
    //  hBisVisAtt->SetVisibility(true);
    //  BismutSampleLOG->SetVisAttributes(hBisVisAtt);
    //
     G4Colour lplastic (0.8,  0.8, 0.0);
     G4VisAttributes *hPlasticVisAtt = new G4VisAttributes(lplastic);
     hPlasticVisAtt->SetVisibility(false);
     GrundKoerper1LOG->SetVisAttributes(hPlasticVisAtt);
     GrundKoerper2LOG->SetVisAttributes(hPlasticVisAtt);
     GrundKoerper3LOG->SetVisAttributes(hPlasticVisAtt);


    //
     G4Colour lCopper (0.9,  0.0, 0.1);
     G4VisAttributes *hCopperVisAtt = new G4VisAttributes(lCopper);
     hCopperVisAtt->SetVisibility(false);
     CopperZylinderLOG->SetVisAttributes(hCopperVisAtt);
     KupferDeckelLOG->SetVisAttributes(hCopperVisAtt);

}

//================================================================================
//======---- Construct sample holder for Bismut sample systematics ---============
//================================================================================

void  BetaChamberDetectorConstruction::constructSampleHolderBismutProbenSystematics(){

    // -------------------------------------------------------------------------------------------------------
    // =============================== Materialien ===========================================================
    // -------------------------------------------------------------------------------------------------------

          G4Material *Bismut = G4Material::GetMaterial("Bismut");
          G4Material *mPMMA = G4Material::GetMaterial("PMMA");
          G4Material *silicon = G4Material::GetMaterial("Silicon");

    // -------------------------------------------------------------------------------------------------------
    // ============================= geometrical volumes =====================================================
    // -------------------------------------------------------------------------------------------------------

          G4Tubs* GEOVolGrundKoerperZylinder1 = new G4Tubs("GVGKZ1",0.5*GetDimensions("GrundKoerperD1"),0.5*GetDimensions("GrundKoerperD4"),0.5*GetDimensions("GrundKoerperL1"),0*deg,360*deg);
          G4Tubs* GEOVolGrundKoerperZylinder2 = new G4Tubs("GVGKZ2",0.5*GetDimensions("GrundKoerperD2"),0.5*GetDimensions("GrundKoerperD4"),0.5*(GetDimensions("GrundKoerperL2")-GetDimensions("GrundKoerperL1")),0*deg,360*deg);
          G4Tubs* GEOVolGrundKoerperZylinder3 = new G4Tubs("GVGKZ3",0.5*GetDimensions("GrundKoerperD3"),0.5*GetDimensions("GrundKoerperD4"),0.5*(GetDimensions("GrundKoerperL4")-GetDimensions("GrundKoerperL2")),0*deg,360*deg);
          G4Tubs* GEOVolHalterSondeZylinder1 = new G4Tubs("GVHSZ1",0.5*GetDimensions("HalterSondeD2"),0.5*GetDimensions("HalterSondeD3"),0.5*(GetDimensions("HalterSondeL2")-GetDimensions("HalterSondeL1")),0*deg,360*deg);
          G4Tubs* GEOVolHalterSondeZylinder2 = new G4Tubs("GVHSZ2",0.5*GetDimensions("HalterSondeD1"),0.5*GetDimensions("HalterSondeD3"),0.5*GetDimensions("HalterSondeL1"),0*deg,360*deg);
          G4Tubs* DistanzRing = new G4Tubs("GVDSTR",0.5*GetDimensions("DistanzRingD1"),0.5*GetDimensions("DistanzRingD2"),0.5*GetDimensions("DistanzRingL1"),0*deg,360*deg);
          G4Tubs* GegenSonde = new G4Tubs("GVGeSo",0.5*GetDimensions("GegenSondeD1"),0.5*GetDimensions("GegenSondeD2"),0.5*GetDimensions("GegenSondeL1"),0*deg,360*deg);
          G4Tubs* FixierRing = new G4Tubs("GVFixR",0.5*GetDimensions("FixierRingD1"),0.5*GetDimensions("FixierRingD2"),0.5*GetDimensions("FixierRingL1"),0*deg,360*deg);
          G4Tubs * SiWafer = new G4Tubs("SiliconWafer",0,0.5*GetDimensions("HalterSondeD2"),0.5*GetDimensions("SiWaferDicke"),0*deg,360*deg);

          // -------- setting bismuth sample thickness -------------------------
          G4double sampleDicke = GetDimensions("BismutSchichtDickeSystematics");
          G4Tubs* BismutSample = new G4Tubs("BiSample",0,0.5*(16*mm),0.5*(sampleDicke),0*deg,360*deg);

          G4cout<<"Construction 2um Sample Holders: all Solid Volumes set"<<"\n";

    // -------------------------------------------------------------------------------------------------------
    // ================================== logical volumes ====================================================
    // -------------------------------------------------------------------------------------------------------

        GrundKoerper1LOG = new G4LogicalVolume(GEOVolGrundKoerperZylinder1, mPMMA, "GrundKoerper1Log",0, 0, 0);
        GrundKoerper2LOG = new G4LogicalVolume(GEOVolGrundKoerperZylinder2, mPMMA, "GrundKoerper2Log",0, 0, 0);
        GrundKoerper3LOG = new G4LogicalVolume(GEOVolGrundKoerperZylinder3, mPMMA, "GrundKoerper3Log",0, 0, 0);
        HalterSondePart1LOG = new G4LogicalVolume(GEOVolHalterSondeZylinder1, mPMMA, "HalterSondePart1LOG",0, 0, 0);
        HalterSondePart2LOG = new G4LogicalVolume(GEOVolHalterSondeZylinder2, mPMMA, "HalterSondePart2LOG",0, 0, 0);
        DistanzRingLOG = new G4LogicalVolume(DistanzRing, mPMMA, "DistanzLOG",0, 0, 0);
        GegenSondeLOG = new G4LogicalVolume(GegenSonde, mPMMA, "GegenSondeLOG",0, 0, 0);
        FixierRingLOG = new G4LogicalVolume(FixierRing, mPMMA, "FixierRingLOG",0, 0, 0);
        BismutSampleLOG = new G4LogicalVolume(BismutSample, Bismut, "BiSampleLOG",0, 0, 0);
        SiWaferLOG=new G4LogicalVolume(SiWafer, silicon, "SiWaferLOG",0, 0, 0);

        G4cout <<"Construction 2um Sample Holders: all Logical Volumes Set"<<"\n";

    // -------------------------------------------------------------------------------------------------------
    // =========================== Positioning of physical volumes ===========================================
    // -------------------------------------------------------------------------------------------------------


        G4double H1 =0.5*GetDimensions("GrundKoerperL1");
        G4double ShiftGKpart2 = H1+0.5*(GetDimensions("GrundKoerperL2")-GetDimensions("GrundKoerperL1"));
        G4double ShiftGKpart3 = ShiftGKpart2+0.5*(GetDimensions("GrundKoerperL2")-GetDimensions("GrundKoerperL1"))+0.5*(GetDimensions("GrundKoerperL4")-GetDimensions("GrundKoerperL2"));

        G4RotationMatrix* yRot = new G4RotationMatrix;  // Rotates X and Z axes only
        		          yRot->rotateY(M_PI*rad);

    //--------------------------------------- sample components ------------------------------------------------------
        G4double HalterSondenPlatzierungPart1,HalterSondenPlatzierungPart2,BiSamplePlatzierung,SiWaferPlatzierung;
        G4double GegenSondePlatzierung,DistanzRingPlatzierung1,DistanzRingPlatzierung2,FixierRingPlatzierung;

        HalterSondenPlatzierungPart1=HalterSondenPlatzierungPart2=BiSamplePlatzierung=SiWaferPlatzierung=GegenSondePlatzierung=DistanzRingPlatzierung1=DistanzRingPlatzierung2=FixierRingPlatzierung=0.*cm;

    // ----------Position 1: [ Sample + holder ] -> Distanzring1 -> Distanzring2 -------------------------------------
        if(PositionsNummer==1){
          HalterSondenPlatzierungPart1 = H1-0.5*(GetDimensions("HalterSondeL2")-GetDimensions("HalterSondeL1"));
          HalterSondenPlatzierungPart2 = H1-1.0*(GetDimensions("HalterSondeL2")-GetDimensions("HalterSondeL1"))-0.5*GetDimensions("HalterSondeL1");
          BiSamplePlatzierung= HalterSondenPlatzierungPart2-0.5*GetDimensions("HalterSondeL1")-0.5*(sampleDicke);
          SiWaferPlatzierung= BiSamplePlatzierung-0.5*(sampleDicke)-0.5*GetDimensions("SiWaferDicke");
          GegenSondePlatzierung=SiWaferPlatzierung-0.5*GetDimensions("SiWaferDicke")-0.5*GetDimensions("GegenSondeL1");
          DistanzRingPlatzierung1=GegenSondePlatzierung-0.5*GetDimensions("GegenSondeL1")-0.5*GetDimensions("DistanzRingL1");
          DistanzRingPlatzierung2=DistanzRingPlatzierung1-GetDimensions("DistanzRingL1");
          FixierRingPlatzierung=DistanzRingPlatzierung2-0.5*GetDimensions("DistanzRingL1")-0.5*GetDimensions("FixierRingL1");

          G4cout<<"Platzierung der Bismut-Folie: "<< BiSamplePlatzierung <<" mm\n";
        }

    // ----------Position 2 : Distanzring1  -> [ Sample + holder ] -> Distanzring2 -------------------------------------
        else if(PositionsNummer==2){
          DistanzRingPlatzierung1=H1-0.5*GetDimensions("DistanzRingL1");
          HalterSondenPlatzierungPart1 = DistanzRingPlatzierung1-0.5*GetDimensions("DistanzRingL1")-0.5*(GetDimensions("HalterSondeL2")-GetDimensions("HalterSondeL1"));
          HalterSondenPlatzierungPart2 = DistanzRingPlatzierung1-1.0*(GetDimensions("HalterSondeL2")-GetDimensions("HalterSondeL1"))-0.5*GetDimensions("HalterSondeL1");
          BiSamplePlatzierung= HalterSondenPlatzierungPart2-0.5*GetDimensions("HalterSondeL1")-0.5*(sampleDicke);
          SiWaferPlatzierung= BiSamplePlatzierung-0.5*(sampleDicke)-0.5*GetDimensions("SiWaferDicke");
          GegenSondePlatzierung=SiWaferPlatzierung-0.5*GetDimensions("SiWaferDicke")-0.5*GetDimensions("GegenSondeL1");
          DistanzRingPlatzierung2=GegenSondePlatzierung-0.5*GetDimensions("GegenSondeL1")-0.5*GetDimensions("DistanzRingL1");
          FixierRingPlatzierung=DistanzRingPlatzierung2-0.5*GetDimensions("DistanzRingL1")-0.5*GetDimensions("FixierRingL1");

          G4cout<<"Platzierung der Bismut-Folie: "<< BiSamplePlatzierung <<" mm\n";
        }

    // ----------Position 2 :  Distanzring1  -> Distanzring2 -> [ Sample + holder ] ----------------------
        else if(PositionsNummer==3){
          DistanzRingPlatzierung1=H1-0.5*GetDimensions("DistanzRingL1");
          DistanzRingPlatzierung2=DistanzRingPlatzierung1-1.0*GetDimensions("DistanzRingL1");
          HalterSondenPlatzierungPart1 = DistanzRingPlatzierung2-0.5*GetDimensions("DistanzRingL1")-0.5*(GetDimensions("HalterSondeL2")-GetDimensions("HalterSondeL1"));
          HalterSondenPlatzierungPart2 = DistanzRingPlatzierung2-1.0*(GetDimensions("HalterSondeL2")-GetDimensions("HalterSondeL1"))-0.5*GetDimensions("HalterSondeL1");
          BiSamplePlatzierung= HalterSondenPlatzierungPart2-0.5*GetDimensions("HalterSondeL1")-0.5*(sampleDicke);
          SiWaferPlatzierung= BiSamplePlatzierung-0.5*(sampleDicke)-0.5*GetDimensions("SiWaferDicke");
          GegenSondePlatzierung=SiWaferPlatzierung-0.5*GetDimensions("SiWaferDicke")-0.5*GetDimensions("GegenSondeL1");
          FixierRingPlatzierung=GegenSondePlatzierung-0.5*GetDimensions("GegenSondeL1")-0.5*GetDimensions("FixierRingL1");

          G4cout<<"Platzierung der Bismut-Folie: "<< BiSamplePlatzierung <<" mm\n";
        }

    // -------------------------------------------------------------------------------------------------------
    // ======================================= physical volumes ==============================================
    // -------------------------------------------------------------------------------------------------------

        GrundKoerper1PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,0),GrundKoerper1LOG, "GrundKoerper1PHYS",VacuumLOG, false, 0);
        GrundKoerper2PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,ShiftGKpart2),GrundKoerper2LOG, "GrundKoerper2PHYS",VacuumLOG, false, 0);
        GrundKoerper3PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,ShiftGKpart3),GrundKoerper3LOG, "GrundKoerper3PHYS",VacuumLOG, false, 0);
        HalterSondePart1PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,HalterSondenPlatzierungPart1),HalterSondePart1LOG, "HalterSondePart1PHYS",VacuumLOG, false, 0);
        HalterSondePart2PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,HalterSondenPlatzierungPart2),HalterSondePart2LOG, "HalterSondePart2PHYS",VacuumLOG, false, 0);
        BismutSamplePHYS =  new G4PVPlacement(yRot, G4ThreeVector(0, 0,BiSamplePlatzierung),BismutSampleLOG, "BismutSamplePHYS",VacuumLOG, false, 0);
        SiWaferPHYS =  new G4PVPlacement(0, G4ThreeVector(0, 0,SiWaferPlatzierung),SiWaferLOG, "SiWaferPHYS",VacuumLOG, false, 0);
        GegenSondePHYS =  new G4PVPlacement(0, G4ThreeVector(0, 0,GegenSondePlatzierung),GegenSondeLOG, "GegenSondePHYS",VacuumLOG, false, 0);
        DistanzRing1PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,DistanzRingPlatzierung1),DistanzRingLOG, "DistanzRing1PHYS",VacuumLOG, false, 0);
        DistanzRing2PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,DistanzRingPlatzierung2),DistanzRingLOG, "DistanzRing2PHYS",VacuumLOG, false, 0);
        FixierRingPHYS =  new G4PVPlacement(0, G4ThreeVector(0, 0,FixierRingPlatzierung),FixierRingLOG, "FixierRingPHYS",VacuumLOG, false, 0);

        G4cout <<"Construction New Sample Holders: all Physical Volumes Set"<<"\n";

    // -------------------------------------------------------------------------------------------------------
    // =================================== Visual Attributes =================================================
    // -------------------------------------------------------------------------------------------------------

         G4Colour lgray (0.7,  0.7, 0.7);
         G4VisAttributes *hAluVisAtt = new G4VisAttributes(lgray);
         hAluVisAtt->SetVisibility(false);
         DistanzRingLOG->SetVisAttributes(hAluVisAtt);
         HalterSondePart1LOG->SetVisAttributes(hAluVisAtt);
         HalterSondePart2LOG->SetVisAttributes(hAluVisAtt);
         FixierRingLOG->SetVisAttributes(hAluVisAtt);
         GegenSondeLOG->SetVisAttributes(hAluVisAtt);
        //
         G4VisAttributes *hBisVisAtt = new G4VisAttributes(lgray);
         hBisVisAtt->SetVisibility(true);
         BismutSampleLOG->SetVisAttributes(hBisVisAtt);
        //
         G4Colour lplastic (0.8,  0.8, 0.0);
         G4VisAttributes *hPlasticVisAtt = new G4VisAttributes(lplastic);
         hPlasticVisAtt->SetVisibility(false);
         GrundKoerper1LOG->SetVisAttributes(hPlasticVisAtt);
         GrundKoerper2LOG->SetVisAttributes(hPlasticVisAtt);
         GrundKoerper3LOG->SetVisAttributes(hPlasticVisAtt);

}

//================================================================================
//=======---- Construct sample holder for Bismut sample form tests ---============
//================================================================================


void  BetaChamberDetectorConstruction::constructSampleHolderBismutProbenFormTests(){

    // -------------------------------------------------------------------------------------------------------
    // =============================== Materialien ===========================================================
    // -------------------------------------------------------------------------------------------------------

          G4Material *Bismut = G4Material::GetMaterial("Bismut");
          G4Material *mPMMA = G4Material::GetMaterial("PMMA");
          G4Material *silicon = G4Material::GetMaterial("Silicon");

    // -------------------------------------------------------------------------------------------------------
    // ============================= geometrical volumes =====================================================
    // -------------------------------------------------------------------------------------------------------

          G4Tubs* GEOVolGrundKoerperZylinder1 = new G4Tubs("GVGKZ1",0.5*GetDimensions("GrundKoerperD1"),0.5*GetDimensions("GrundKoerperD4"),0.5*GetDimensions("GrundKoerperL1"),0*deg,360*deg);
          G4Tubs* GEOVolGrundKoerperZylinder2 = new G4Tubs("GVGKZ2",0.5*GetDimensions("GrundKoerperD2"),0.5*GetDimensions("GrundKoerperD4"),0.5*(GetDimensions("GrundKoerperL2")-GetDimensions("GrundKoerperL1")),0*deg,360*deg);
          G4Tubs* GEOVolGrundKoerperZylinder3 = new G4Tubs("GVGKZ3",0.5*GetDimensions("GrundKoerperD3"),0.5*GetDimensions("GrundKoerperD4"),0.5*(GetDimensions("GrundKoerperL4")-GetDimensions("GrundKoerperL2")),0*deg,360*deg);
          G4Tubs* GEOVolHalterSondeZylinder1 = new G4Tubs("GVHSZ1",0.5*GetDimensions("HalterSondeD2"),0.5*GetDimensions("HalterSondeD3"),0.5*(GetDimensions("HalterSondeL2")-GetDimensions("HalterSondeL1")),0*deg,360*deg);
          G4Tubs* GEOVolHalterSondeZylinder2 = new G4Tubs("GVHSZ2",0.5*GetDimensions("HalterSondeD1"),0.5*GetDimensions("HalterSondeD3"),0.5*GetDimensions("HalterSondeL1"),0*deg,360*deg);
          G4Tubs* DistanzRing = new G4Tubs("GVDSTR",0.5*GetDimensions("DistanzRingD1"),0.5*GetDimensions("DistanzRingD2"),0.5*GetDimensions("DistanzRingL1"),0*deg,360*deg);
          G4Tubs* GegenSonde = new G4Tubs("GVGeSo",0.5*GetDimensions("GegenSondeD1"),0.5*GetDimensions("GegenSondeD2"),0.5*GetDimensions("GegenSondeL1"),0*deg,360*deg);
          G4Tubs* FixierRing = new G4Tubs("GVFixR",0.5*GetDimensions("FixierRingD1"),0.5*GetDimensions("FixierRingD2"),0.5*GetDimensions("FixierRingL1"),0*deg,360*deg);
          G4Tubs * SiWafer = new G4Tubs("SiliconWafer",0,0.5*GetDimensions("HalterSondeD2"),0.5*GetDimensions("SiWaferDicke"),0*deg,360*deg);

          // -------- setting bismuth sample thickness -------------------------
          G4double InnerRadius =4*mm;
          G4double OuterRadius =8*mm;
          G4double sampleThickness = GetDimensions("BismutSchichtDickeSystematics");
          G4double OuterPartThickness = 0.7*sampleThickness;

          G4Tubs* BismutSamplePart1 = new G4Tubs("BismutSamplePart1",0,InnerRadius,0.5*(sampleThickness),0*deg,360*deg);
          G4Tubs* BismutSamplePart2 = new G4Tubs("BismutSamplePart2",InnerRadius,OuterRadius,0.5*(OuterPartThickness),0*deg,360*deg);


        //   G4Tubs* BismutSample = new G4Tubs("BiSample",0,0.5*(16*mm),0.5*(sampleThickness),0*deg,360*deg);

          G4cout<<"Construction 2um Sample Holders: all Solid Volumes set"<<"\n";

    // -------------------------------------------------------------------------------------------------------
    // ================================== logical volumes ====================================================
    // -------------------------------------------------------------------------------------------------------

        GrundKoerper1LOG = new G4LogicalVolume(GEOVolGrundKoerperZylinder1, mPMMA, "GrundKoerper1Log",0, 0, 0);
        GrundKoerper2LOG = new G4LogicalVolume(GEOVolGrundKoerperZylinder2, mPMMA, "GrundKoerper2Log",0, 0, 0);
        GrundKoerper3LOG = new G4LogicalVolume(GEOVolGrundKoerperZylinder3, mPMMA, "GrundKoerper3Log",0, 0, 0);
        HalterSondePart1LOG = new G4LogicalVolume(GEOVolHalterSondeZylinder1, mPMMA, "HalterSondePart1LOG",0, 0, 0);
        HalterSondePart2LOG = new G4LogicalVolume(GEOVolHalterSondeZylinder2, mPMMA, "HalterSondePart2LOG",0, 0, 0);
        DistanzRingLOG = new G4LogicalVolume(DistanzRing, mPMMA, "DistanzLOG",0, 0, 0);
        GegenSondeLOG = new G4LogicalVolume(GegenSonde, mPMMA, "GegenSondeLOG",0, 0, 0);
        FixierRingLOG = new G4LogicalVolume(FixierRing, mPMMA, "FixierRingLOG",0, 0, 0);
        BismutSamplePart1LOG = new G4LogicalVolume(BismutSamplePart1, Bismut, "BiSamplePart1LOG",0, 0, 0);
        BismutSamplePart2LOG = new G4LogicalVolume(BismutSamplePart2, Bismut, "BiSamplePart2LOG",0, 0, 0);
        SiWaferLOG=new G4LogicalVolume(SiWafer, silicon, "SiWaferLOG",0, 0, 0);

        G4cout <<"Construction 2um Sample Holders: all Logical Volumes Set"<<"\n";

    // -------------------------------------------------------------------------------------------------------
    // =========================== Positioning of physical volumes ===========================================
    // -------------------------------------------------------------------------------------------------------


        G4double H1 =0.5*GetDimensions("GrundKoerperL1");
        G4double ShiftGKpart2 = H1+0.5*(GetDimensions("GrundKoerperL2")-GetDimensions("GrundKoerperL1"));
        G4double ShiftGKpart3 = ShiftGKpart2+0.5*(GetDimensions("GrundKoerperL2")-GetDimensions("GrundKoerperL1"))+0.5*(GetDimensions("GrundKoerperL4")-GetDimensions("GrundKoerperL2"));

        G4RotationMatrix* yRot = new G4RotationMatrix;  // Rotates X and Z axes only
        		          yRot->rotateY(M_PI*rad);

    //--------------------------------------- sample components ------------------------------------------------------
        G4double HalterSondenPlatzierungPart1,HalterSondenPlatzierungPart2,BiSamplePlatzierungPart1,BiSamplePlatzierungPart2,SiWaferPlatzierung;
        G4double GegenSondePlatzierung,DistanzRingPlatzierung1,DistanzRingPlatzierung2,FixierRingPlatzierung;
        HalterSondenPlatzierungPart1=HalterSondenPlatzierungPart2=BiSamplePlatzierungPart1=BiSamplePlatzierungPart2=SiWaferPlatzierung=GegenSondePlatzierung=DistanzRingPlatzierung1=DistanzRingPlatzierung2=FixierRingPlatzierung=0.*cm;

    // ----------Position 1: [ Sample + holder ] -> Distanzring1 -> Distanzring2 -------------------------------------
        if(PositionsNummer==1){
          HalterSondenPlatzierungPart1 = H1-0.5*(GetDimensions("HalterSondeL2")-GetDimensions("HalterSondeL1"));
          HalterSondenPlatzierungPart2 = H1-1.0*(GetDimensions("HalterSondeL2")-GetDimensions("HalterSondeL1"))-0.5*GetDimensions("HalterSondeL1");
          BiSamplePlatzierungPart1= HalterSondenPlatzierungPart2-0.5*GetDimensions("HalterSondeL1")-0.5*(sampleThickness);
          BiSamplePlatzierungPart2=BiSamplePlatzierungPart1-0.5*(sampleThickness-OuterPartThickness);
          SiWaferPlatzierung= BiSamplePlatzierungPart1-0.5*(sampleThickness)-0.5*GetDimensions("SiWaferDicke");
          GegenSondePlatzierung=SiWaferPlatzierung-0.5*GetDimensions("SiWaferDicke")-0.5*GetDimensions("GegenSondeL1");
          DistanzRingPlatzierung1=GegenSondePlatzierung-0.5*GetDimensions("GegenSondeL1")-0.5*GetDimensions("DistanzRingL1");
          DistanzRingPlatzierung2=DistanzRingPlatzierung1-GetDimensions("DistanzRingL1");
          FixierRingPlatzierung=DistanzRingPlatzierung2-0.5*GetDimensions("DistanzRingL1")-0.5*GetDimensions("FixierRingL1");

          G4cout<<"Platzierung der Bismut-Folie: "<< BiSamplePlatzierungPart1 <<" mm\n";
        }

    // ----------Position 2 : Distanzring1  -> [ Sample + holder ] -> Distanzring2 -------------------------------------
        else if(PositionsNummer==2){
          DistanzRingPlatzierung1=H1-0.5*GetDimensions("DistanzRingL1");
          HalterSondenPlatzierungPart1 = DistanzRingPlatzierung1-0.5*GetDimensions("DistanzRingL1")-0.5*(GetDimensions("HalterSondeL2")-GetDimensions("HalterSondeL1"));
          HalterSondenPlatzierungPart2 = DistanzRingPlatzierung1-1.0*(GetDimensions("HalterSondeL2")-GetDimensions("HalterSondeL1"))-0.5*GetDimensions("HalterSondeL1");
          BiSamplePlatzierungPart1= HalterSondenPlatzierungPart2-0.5*GetDimensions("HalterSondeL1")-0.5*(sampleThickness);
          BiSamplePlatzierungPart2=BiSamplePlatzierungPart1-0.5*(sampleThickness-OuterPartThickness);
          SiWaferPlatzierung= BiSamplePlatzierungPart1-0.5*(sampleThickness)-0.5*GetDimensions("SiWaferDicke");
          GegenSondePlatzierung=SiWaferPlatzierung-0.5*GetDimensions("SiWaferDicke")-0.5*GetDimensions("GegenSondeL1");
          DistanzRingPlatzierung2=GegenSondePlatzierung-0.5*GetDimensions("GegenSondeL1")-0.5*GetDimensions("DistanzRingL1");
          FixierRingPlatzierung=DistanzRingPlatzierung2-0.5*GetDimensions("DistanzRingL1")-0.5*GetDimensions("FixierRingL1");

          G4cout<<"Platzierung der Bismut-Folie: "<< BiSamplePlatzierungPart1 <<" mm\n";
        }

    // ----------Position 2 :  Distanzring1  -> Distanzring2 -> [ Sample + holder ] ----------------------
        else if(PositionsNummer==3){
          DistanzRingPlatzierung1=H1-0.5*GetDimensions("DistanzRingL1");
          DistanzRingPlatzierung2=DistanzRingPlatzierung1-1.0*GetDimensions("DistanzRingL1");
          HalterSondenPlatzierungPart1 = DistanzRingPlatzierung2-0.5*GetDimensions("DistanzRingL1")-0.5*(GetDimensions("HalterSondeL2")-GetDimensions("HalterSondeL1"));
          HalterSondenPlatzierungPart2 = DistanzRingPlatzierung2-1.0*(GetDimensions("HalterSondeL2")-GetDimensions("HalterSondeL1"))-0.5*GetDimensions("HalterSondeL1");
          BiSamplePlatzierungPart1= HalterSondenPlatzierungPart2-0.5*GetDimensions("HalterSondeL1")-0.5*(sampleThickness);
          BiSamplePlatzierungPart2=BiSamplePlatzierungPart1-0.5*(sampleThickness-OuterPartThickness);
          SiWaferPlatzierung= BiSamplePlatzierungPart1-0.5*(sampleThickness)-0.5*GetDimensions("SiWaferDicke");
          GegenSondePlatzierung=SiWaferPlatzierung-0.5*GetDimensions("SiWaferDicke")-0.5*GetDimensions("GegenSondeL1");
          FixierRingPlatzierung=GegenSondePlatzierung-0.5*GetDimensions("GegenSondeL1")-0.5*GetDimensions("FixierRingL1");

          G4cout<<"Platzierung der Bismut-Folie: "<< BiSamplePlatzierungPart1 <<" mm\n";
        }

    // -------------------------------------------------------------------------------------------------------
    // ======================================= physical volumes ==============================================
    // -------------------------------------------------------------------------------------------------------

        GrundKoerper1PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,0),GrundKoerper1LOG, "GrundKoerper1PHYS",VacuumLOG, false, 0);
        GrundKoerper2PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,ShiftGKpart2),GrundKoerper2LOG, "GrundKoerper2PHYS",VacuumLOG, false, 0);
        GrundKoerper3PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,ShiftGKpart3),GrundKoerper3LOG, "GrundKoerper3PHYS",VacuumLOG, false, 0);
        HalterSondePart1PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,HalterSondenPlatzierungPart1),HalterSondePart1LOG, "HalterSondePart1PHYS",VacuumLOG, false, 0);
        HalterSondePart2PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,HalterSondenPlatzierungPart2),HalterSondePart2LOG, "HalterSondePart2PHYS",VacuumLOG, false, 0);
        BismutSamplePart1PHYS =  new G4PVPlacement(yRot, G4ThreeVector(0, 0,BiSamplePlatzierungPart1),BismutSamplePart1LOG, "BismutSamplePart1PHYS",VacuumLOG, false, 0);
        BismutSamplePart2PHYS =  new G4PVPlacement(yRot, G4ThreeVector(0, 0,BiSamplePlatzierungPart2),BismutSamplePart2LOG, "BismutSamplePart2PHYS",VacuumLOG, false, 0);
        SiWaferPHYS =  new G4PVPlacement(0, G4ThreeVector(0, 0,SiWaferPlatzierung),SiWaferLOG, "SiWaferPHYS",VacuumLOG, false, 0);
        GegenSondePHYS =  new G4PVPlacement(0, G4ThreeVector(0, 0,GegenSondePlatzierung),GegenSondeLOG, "GegenSondePHYS",VacuumLOG, false, 0);
        DistanzRing1PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,DistanzRingPlatzierung1),DistanzRingLOG, "DistanzRing1PHYS",VacuumLOG, false, 0);
        DistanzRing2PHYS = new G4PVPlacement(0, G4ThreeVector(0, 0,DistanzRingPlatzierung2),DistanzRingLOG, "DistanzRing2PHYS",VacuumLOG, false, 0);
        FixierRingPHYS =  new G4PVPlacement(0, G4ThreeVector(0, 0,FixierRingPlatzierung),FixierRingLOG, "FixierRingPHYS",VacuumLOG, false, 0);

        G4cout <<"Construction New Sample Holders: all Physical Volumes Set"<<"\n";

    // -------------------------------------------------------------------------------------------------------
    // =================================== Visual Attributes =================================================
    // -------------------------------------------------------------------------------------------------------

         G4Colour lgray (0.7,  0.7, 0.7);
         G4VisAttributes *hAluVisAtt = new G4VisAttributes(lgray);
         hAluVisAtt->SetVisibility(false);
         DistanzRingLOG->SetVisAttributes(hAluVisAtt);
         HalterSondePart1LOG->SetVisAttributes(hAluVisAtt);
         HalterSondePart2LOG->SetVisAttributes(hAluVisAtt);
         FixierRingLOG->SetVisAttributes(hAluVisAtt);
         GegenSondeLOG->SetVisAttributes(hAluVisAtt);
        //
         G4VisAttributes *hBisVisAtt = new G4VisAttributes(lgray);
         hBisVisAtt->SetVisibility(true);
         BismutSampleLOG->SetVisAttributes(hBisVisAtt);
        //
         G4Colour lplastic (0.8,  0.8, 0.0);
         G4VisAttributes *hPlasticVisAtt = new G4VisAttributes(lplastic);
         hPlasticVisAtt->SetVisibility(false);
         GrundKoerper1LOG->SetVisAttributes(hPlasticVisAtt);
         GrundKoerper2LOG->SetVisAttributes(hPlasticVisAtt);
         GrundKoerper3LOG->SetVisAttributes(hPlasticVisAtt);

}

//================================================================================
//====================---- Construct Absorption test ---==========================
//================================================================================


void BetaChamberDetectorConstruction::constructAbsorptionTests(){



    // -------------------------------------------------------------------------------------------------------
    // =============================== Materialien ===========================================================
    // -------------------------------------------------------------------------------------------------------


       G4Material *mPMMA = G4Material::GetMaterial("PMMA");
       G4Material *silicon = G4Material::GetMaterial("Silicon");


    // -------------------------------------------------------------------------------------------------------
    // =============================== Geometry ==============================================================
    // -------------------------------------------------------------------------------------------------------
             G4double Siliziumdicke = GetDimensions("PIPSSiliziumChipHoeheStandard");
             G4double sampleDicke=GetDimensions("SiWaferDicke");


        G4Tubs * SiliconChip= new G4Tubs("PIPSSiliChip",0,GetDimensions("PIPSSiliziumChipAußenRadius"),0.5*Siliziumdicke,0*deg,360*deg);
        G4Box *  QuadraticATSample = new G4Box("QATSample",GetDimensions("HalterSondeTrapezD2")-1*mm,GetDimensions("HalterSondeTrapezD2")-1*mm,0.5*sampleDicke);


    // -------------------------------------------------------------------------------------------------------
    // =============================== Logical  ==============================================================
    // -------------------------------------------------------------------------------------------------------
         SiliconChipLOG= new G4LogicalVolume(SiliconChip, silicon, "SiliconChipLOG",0, 0, 0);
         ATSampleLOG=new G4LogicalVolume(QuadraticATSample, silicon, "ATSampleLOG",0, 0, 0);



        G4cout<<"Construction New Sample Holders: all Logical Volumes Set"<<"\n";

    // -------------------------------------------------------------------------------------------------------
    // =============================== Physical  =============================================================
    // -------------------------------------------------------------------------------------------------------
        G4double ATSamplePlatzierung= -0.5*(Siliziumdicke+sampleDicke)-PositionsNummer*1.*mm;
        G4cout<< ATSamplePlatzierung <<" mm"<<"\n";

        SiliconChipPHYS= new G4PVPlacement(0, G4ThreeVector(0, 0,0),SiliconChipLOG, "SILIZIUMSENSITIVEPHYS",VacuumLOG, false, 0);
        ATSamplePHYS =  new G4PVPlacement(0, G4ThreeVector(0, 0,ATSamplePlatzierung),ATSampleLOG, "ATSamplePHYS",VacuumLOG, false, 0);

        G4cout<<"Construction New Sample Holders: all Physical Volumes Set"<<"\n";
        BetaChamberDetectorConstruction::SetSensVol(SiliconChipPHYS);

}
