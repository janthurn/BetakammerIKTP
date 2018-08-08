#include "TStopwatch.h"
#include "TFile.h"

void ExtractUnfolding_Bismut(){


  //======================================================================================================================================
  //======================================================= Options ========================================================================
  //======================================================================================================================================
  int BismutSonde =10;
  int Position		=1;
  bool submit= true;

  double Qwert =1161.5;
  double Upper_End =1180;
  int bins = Upper_End/20;

   //======================================================================================================================================
  //===================================================== Filename =======================================================================
  //======================================================================================================================================

    char place[100];
    sprintf(place,  "/raid1/users/thurn/BetaChamber/Simulation");
   //------
    char isotop[10];
    sprintf(isotop, "Bi210");


   //------
    char SaveFileName[500];
    char rootNamePart1[200];
    char rootNamePart2[200];

    sprintf(rootNamePart1,"%s/Simulationen/Raw_Files/",place);
    sprintf(rootNamePart2,"%s_unfolding_Position%i_Sonde%i",isotop,Position,BismutSonde);
    sprintf(SaveFileName,"%s/Simulationen/Histogramme/%s_Position%i_Sonde%i_UnfoldingCombined.root",place,isotop,Position,BismutSonde);


   //------


  TFile * fCombined = new TFile(SaveFileName,"RECREATE");
  TStopwatch Watch;
  //======================================================================================================================================
  //================================== Extraction from simulation files ==================================================================
  //======================================================================================================================================
    Watch.Start();
   for (int i = 1; i <= bins; i++)    {
     cout <<"Startlloop"<<endl;
     double energy=20*i;
     char rootName[500];
     char saveName[500];
//      sprintf(rootName, "%s/Simulationen/Raw_Files/%s_unfolding_Position%i_Energie_%.1f.root",place,isotop,Position,energy);

      sprintf(saveName, "unfolding/%s_Energie_%.1f.root",rootNamePart2,energy);
      sprintf(rootName,"%s%s_Energie_%.1f.root",rootNamePart1,rootNamePart2,energy);


      string executeString = Form(" ./ExtractHistogramm  %s %s",rootName,saveName);

      // submission string

      cout << executeString << endl;
      if (submit)
        {
         system(executeString.c_str());
        }

       TString searchFile=  Form("%s/Simulationen/Histogramme/unfolding/%s_Energie_%.1f.root",place,rootNamePart2,energy);

      TFile * fEnergie = new TFile(searchFile,"READ");
      TH1F* hist = (TH1F*)fEnergie->Get("EnergieDeposited_WithResOfDetektor2");
      hist->SetName(Form("%.1f_keV_WithResOfDetektor2",energy));
      fCombined->cd();
      hist->SetDirectory(0);
      hist->Write();

    }
    Watch.Stop();
   cout<<  Watch.RealTime()<<endl;



}
