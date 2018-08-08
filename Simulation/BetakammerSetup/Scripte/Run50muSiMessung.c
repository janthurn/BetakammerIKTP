#include "TStopwatch.h"


void Run50muSiMessung(){


  //======================================================================================================================================
  //======================================================= Setup ========================================================================
  //======================================================================================================================================

  bool submit= true;
  double Dicken[6]={42.3,32.1,34.7,38.5,41.5,38.5};
  int Position[6]={2,2,1,1,1,2};

  TStopwatch Watch;
  //======================================================================================================================================
   //============================================= Macro itself ===========================================================================
   //======================================================================================================================================
    Watch.Start();
for (int i = 0; i < 6; i++)    {
    cout <<"Startlloop"<<endl;

      char fileName[500];
      char rootName[500];




      sprintf(fileName, "macros/Si31_50mu_Sonde%i_Position%i.mac",i+1,Position[i]);
      sprintf(rootName, "../Simulationen/Raw_Files/Si31_50mu_Sonde%i_Position%i.root",i+1,Position[i]);


      string executeString = Form("nice -n 8 ./BetaChamberSetup %s",fileName);
      std::ofstream file(fileName);    // put in macro commands for each file
      {
	  //       file << "/MG/manager/heprandomseed " << time(0) + i "<< std::endl;
           file << "/BetaChamber/geometry/SetupNumber 5 "<< std::endl;
           file << "/BetaChamber/geometry/SondenPositionNummer  "<< Position[i]<< std::endl;
           file << "/BetaChamber/geometry/SiliziumDicke "<< Dicken[i] <<std::endl;
           file << "/run/initialize "<< std::endl;
           file << "/BetaChamber/RunAction/SetFileName  " << rootName << std::endl;
           file << ""<< std::endl;
           file << "/BetaChamber/generator/select decay0 "<< std::endl;
           file << "/BetaChamber/generator/volume  ATSamplePHYS "<< std::endl;
           file << "/BetaChamber/generator/confine volume "<< std::endl;
           file << "/BetaChamber/generator/decay0/filename /raid1/users/thurn/DECAY0.1/Si31.dat"<< std::endl;
           file << " "<< std::endl;
           file << "/control/verbose 2 "<< std::endl;
           file << "/run/verbose 2 "<< std::endl;
           file << " "<< std::endl;
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
