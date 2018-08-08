#include "TStopwatch.h"


void RunKalibrationsEnergien(){


  //======================================================================================================================================
  //======================================================= Setup ========================================================================
  //======================================================================================================================================

  bool submit= true;
  
  //======================================================================================================================================
   //============================================= Macro itself ===========================================================================
   //======================================================================================================================================
    Watch.Start();
    for (int i = 1; i <= linien; i++)    {
      cout <<"Startlloop"<<endl;

      char fileName[500];
      char rootName[500];
      char isotop[100];
      if(i<=4) sprintf(isotop,"Cs137");
      else if(i>4 &&i<=9)sprintf(isotop,"Ba133");
      else if(i>9) sprintf(isotop,"Bi207");
      else sprintf(isotop,"noisotop");



      sprintf(fileName, "macros/calibration/Kalibrations_Position%i_%s_Energie_%.1f.mac",Position,isotop, Energien[i-1]);
      sprintf(rootName, "../Simulationen/Raw_Files/Kalibratons_Position%i_%s_Energie_%.1f.root",Position,isotop, Energien[i-1]);


      string executeString = Form("nice -n 8 ./BetaChamberSetup %s",fileName);
      std::ofstream file(fileName);    // put in macro commands for each file
      {
	  //       file << "/MG/manager/heprandomseed " << time(0) + i "<< std::endl;
           file << "/BetaChamber/geometry/SetupNumber 2 "<< std::endl;
           file << "/BetaChamber/geometry/SondenPositionNummer  "<< Position<< std::endl;
           file << "/BetaChamber/geometry/KalibrationConfig 1 "<< std::endl;
           file << "/BetaChamber/geometry/KalibrationMaterial  2  "<< std::endl; //hier 1, 2, 3
           file << "/BetaChamber/geometry/KalibrationQuellpunktDurchmesser  1 "<< std::endl;
           file << "/run/initialize "<< std::endl;
           file << "/BetaChamber/RunAction/SetFileName  " << rootName << std::endl;
           file << ""<< std::endl;
           file << "/BetaChamber/generator/select gun "<< std::endl;
           file << "/BetaChamber/generator/volume  PROBEPHYS "<< std::endl;
           file << "/BetaChamber/generator/confine volume "<< std::endl;
           file << " "<< std::endl;
           file << "/control/verbose 2 "<< std::endl;
           file << "/run/verbose 2 "<< std::endl;
           file << " "<< std::endl;
           file << "/gun/particle e- "<< std::endl;
           file << "/gun/energy " << Energien[i-1] << " keV "<< std::endl;
           file << " "<< std::endl;
           file << "/run/printProgress 10000 "<< std::endl;
            file << " /run/beamOn 1000000 "<< std::endl;

      }
      file.close();

      // submission string

      cout << executeString << endl;
      if (submit)
        {
          system(executeString.c_str());
        }

    }

    Watch.Stop();
   cout<<  Watch.RealTime()<<endl;
}
