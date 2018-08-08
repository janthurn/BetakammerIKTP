#include "TStopwatch.h"
#include "TH1.h"
#include "TFile.h"
#include "TRandom3.h"
#include "time.h"


void RunSystematicsBismuthThickness(){



  //======================================================================================================================================
  //======================================================= Options ======================================================================
  //======================================================================================================================================
  TRandom3* r = new TRandom3(time(NULL));
  int Position =1;        // In which position is the sample inside the sample holder (1,2,3) with 1 being the closest position
  bool submit= true;
  int WhereIsTheData=1;   // choose between raid1 and raid2
  cout<< ""<<endl;
  cout<< "====================================================================="<<endl;
  cout<< "Where should the data be stored ? (Raid1 or Raid2 = > Type 1 or 2)"<<endl;
  cin>>WhereIsTheData;
  cout<< "====================================================================="<<endl;

  //======================================================================================================================================
  //=============================================== Loop start and stop ==================================================================
  //======================================================================================================================================

  int start,stop;
  start=1;
  stop=100;
  cout<< ""<<endl;
  cout<< "================================================="<<endl;
  cout<< "Where should the loop start ? (1-19999)"<<endl;
  cin>>start;
  cout<< "Where should the loop end ? (2-20000)"<<endl;
  cin>>stop;
  cout<< "================================================="<<endl;
  cout<< ""<<endl;



  //======================================================================================================================================
  //======================================================= Histogramm ===================================================================
  //======================================================================================================================================

  TH1F* hDickenEinzel=new TH1F("hDickenEinzel",";Simulation",20000,0,20000);
  TH1F* hDicken=new TH1F("hDicken",";thickness (#mum)",400,0,4);
  TString SFname = Form("/raid1/users/thurn/BetaChamber/Simulation/Simulationen/Histogramme/systematics/Bi210/SaveThickness%ito%i.root",start,stop);
  TFile * fSaveDicken;

  if(!gSystem->AccessPathName(SFname)){
    cout << "file allready exist! " <<endl;
  }
  else{
      cout << "file does not exist->Will be createt! " <<endl;
      fSaveDicken=new TFile(SFname,"NEW");
      hDickenEinzel->SetDirectory(0);
      hDickenEinzel->Write();
      hDicken->SetDirectory(0);
      hDicken->Write();
      fSaveDicken->Close();
  }
  //======================================================================================================================================
  //===================================================== Filename =======================================================================
  //======================================================================================================================================

  char placeRaid1[500];
  char placeRaid2[500];
  sprintf(placeRaid1,  "/raid1/users/thurn/BetaChamber/Simulation");
  sprintf(placeRaid2,  "/raid2/users/thurn/Systematics-Zwischenspeicher");
    char isotop[100];
    sprintf(isotop,"Bi210");
   //------
    char macroNamePart1[500];
    char rootNamePart1[500];
    //sprintf(macroNamePart1,"%s/Betakammer/macros/unfolding/%s_unfolding_Position%i_thickness%.1fum",place,isotop,Position,Siliziumdicke);
    if(WhereIsTheData==1){
      sprintf(macroNamePart1,"%s/Betakammer/macros/systematics/%s_systematics_Position%i",placeRaid1,isotop,Position);
      sprintf(rootNamePart1,"%s/Simulationen/Raw_Files/%s_systematics_Position%i",placeRaid1,isotop,Position);
    }
    else if(WhereIsTheData==2){
      sprintf(macroNamePart1,"%s/Betakammer/macros/systematics/%s_systematics_Position%i",placeRaid1,isotop,Position);
      sprintf(rootNamePart1,"%s/%s/%s_systematics_Position%i",placeRaid2,isotop,isotop,Position);
    }


  TStopwatch Watch;
  //======================================================================================================================================
   //============================================= Macro itself ===========================================================================
   //======================================================================================================================================
    Watch.Start();
   for (int i = start; i <= stop; i++)    {
      cout <<"Startlloop "<< i<<endl;
      char macroName[500];
      char rootName[500];
      double dicke = r->Gaus(2,0.5);
      TFile* fsave = new TFile(SFname,"UPDATE");

      TH1F* hthickness =(TH1F*)fsave->Get("hDicken");
      hthickness->Fill(dicke);
      hthickness->Write("",TObject::kOverwrite);
      TH1F* hSims =(TH1F*)fsave->Get("hDickenEinzel");
      hSims->SetBinContent(i,dicke);
      hSims->Write("",TObject::kOverwrite);
      fsave->Close();

      sprintf(macroName, "%s_Thickness_%i.mac",macroNamePart1,i);
      sprintf(rootName, "%s_Thickness_%i.root",rootNamePart1,i);

      string executeString = Form("nice -n 8 ./BetaChamberSetup %s",macroName);
      std::ofstream file(macroName);    // put in macro commands for each file
      {
	  //       file << "/MG/manager/heprandomseed " << time(0) + i "<< std::endl;
           file << "/BetaChamber/geometry/SetupNumber 7 "<< std::endl;
           file << "/BetaChamber/geometry/SondenPositionNummer  "<< Position<< std::endl;
           file << "/BetaChamber/geometry/BismutSchichtDicke "<< dicke <<std::endl;
           file << "/run/initialize "<< std::endl;
           file << "/BetaChamber/RunAction/SetFileName  " << rootName << std::endl;
           file << ""<< std::endl;
           file << "/BetaChamber/generator/select decay0 "<< std::endl;
           file << "/BetaChamber/generator/volume  BismutSamplePHYS "<< std::endl;
           file << "/BetaChamber/generator/confine volume "<< std::endl;
           file << "/BetaChamber/generator/decay0/filename /raid1/users/thurn/DECAY0.1/Bi210_1e7.dat"<< std::endl;
           file << " "<< std::endl;
           file << "/control/verbose 2 "<< std::endl;
           file << "/run/verbose 2 "<< std::endl;
           file << " "<< std::endl;
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
