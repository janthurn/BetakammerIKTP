#include "TStopwatch.h"
#include "TH1.h"

void RunUnfolding_Bismut(){

  //======================================================================================================================================
  //======================================================= Options ======================================================================
  //======================================================================================================================================

  int Position =1;
  bool submit= true;
  int BismutSonde =10;
  //======================================================================================================================================
  //======================================================= Histogramm ===================================================================
  //======================================================================================================================================

  double Qwert =1161.5;
  double Upper_End =1180.0;
  int bins = Upper_End/20;

  TH1F* hSpectrum=new TH1F("hSpectrum","energy",bins,0,Upper_End);
  cout << "Test: "<< hSpectrum->GetBinWidth(2)<<" keV with "<< bins<<" bins"<<endl;

  //======================================================================================================================================
  //===================================================== Filename =======================================================================
  //======================================================================================================================================

    char place[500];
    sprintf(place,  "/raid1/users/thurn/BetaChamber/Simulation");
   //------
    char isotop[100];
    sprintf(isotop,"Bi210");
   //------
    char macroNamePart1[500];
    char rootNamePart1[500];

   sprintf(macroNamePart1,"%s/Betakammer/macros/unfolding/%s_unfolding_Position%i_Sonde%i",place,isotop,Position,BismutSonde);
	 sprintf(rootNamePart1,"%s/Simulationen/Raw_Files/%s_unfolding_Position%i_Sonde%i",place,isotop,Position,BismutSonde);
    


  //======================================================================================================================================
  //=============================================== Loop start and stop ==================================================================
  //======================================================================================================================================

  int start,stop;
  start=1;
  stop=bins;
  cout<< ""<<endl;
  cout<< "================================================="<<endl;
  cout<< "Where should the loop start ? (complete =1)"<<endl;
  cin>>start;
  cout<< "Where should the loop end ? (complete = 59)"<<endl;
  cin>>stop;
  cout<< "================================================="<<endl;
  cout<< ""<<endl;
  TStopwatch Watch;
  //======================================================================================================================================
   //============================================= Macro itself ===========================================================================
   //======================================================================================================================================
    Watch.Start();
   for (int i = start; i <= stop; i++)    {
      cout <<"Startlloop"<<endl;
      double energy=20*i;
      char macroName[500];
      char rootName[500];

      sprintf(macroName, "%s_Energie_%.1f.root",macroNamePart1,energy);
      sprintf(rootName, "%s_Energie_%.1f.root",rootNamePart1,energy);



      string executeString = Form("nice -n 8 ./BetaChamberSetup %s",macroName);
      std::ofstream file(macroName);    // put in macro commands for each file
      {
	  //       file << "/MG/manager/heprandomseed " << time(0) + i "<< std::endl;
           file << "/BetaChamber/geometry/SetupNumber 3 "<< std::endl;
           file << "/BetaChamber/geometry/SondenPositionNummer  "<< Position<< std::endl;
           file << "/BetaChamber/geometry/SampleNumber " << BismutSonde << std::endl;
           file << "/run/initialize "<< std::endl;
           file << "/BetaChamber/RunAction/SetFileName  " << rootName << std::endl;
           file << ""<< std::endl;
           file << "/BetaChamber/generator/select gun "<< std::endl;
           file << "/BetaChamber/generator/volume  BismutSamplePHYS "<< std::endl;
           file << "/BetaChamber/generator/confine volume "<< std::endl;
           file << " "<< std::endl;
           file << "/control/verbose 2 "<< std::endl;
           file << "/run/verbose 2 "<< std::endl;
           file << " "<< std::endl;
           file << "/gun/particle e- "<< std::endl;
           file << "/gun/energy " << energy << " keV "<< std::endl;
           file << " "<< std::endl;
           file << "/run/printProgress 50000 "<< std::endl;
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
