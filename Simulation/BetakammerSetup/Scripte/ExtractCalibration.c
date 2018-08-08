#include "TStopwatch.h"
#include "TFile.h"

void ExtractCalibration(){


  //======================================================================================================================================
  //======================================================= Setup ========================================================================
  //======================================================================================================================================

  bool submit= true;
  int Position = 3;
  int sourceConfig=1;
  const int chooseisotop=1; // 1 = Bi207,2 = Cs137, 3 = Ba133
  const int NLines[3] = {20,7,51};
  const int nlines=NLines[chooseisotop-1];


  char isotop[100];
  if(chooseisotop==1) sprintf(isotop,"Bi207");
  else if(chooseisotop==2)sprintf(isotop,"Cs137");
  else if(chooseisotop==3)sprintf(isotop,"Ba133");
  else sprintf(isotop,"noisotop");

  TStopwatch Watch;
  char place[500];
  sprintf(place,  "/raid1/users/thurn/BetaChamber/Simulation");

   TFile * fCombined = new TFile(Form("%s/Simulationen/Histogramme/Calibration_%s_Position%i_SourceConfig=%i_Combined.root",place,isotop,Position,sourceConfig),"RECREATE");
  //======================================================================================================================================
  //============================================= Macro itself ===========================================================================
  //======================================================================================================================================
    Watch.Start();
    for (int i = 1; i <= nlines; i++)    {
      cout <<"Startlloop "<< i<<endl;


      //---------------------
      char rootName[500];
      char saveName[500];
      sprintf(rootName, "%s/Simulationen/Raw_Files/%s_calibration_Position%i_SourceConfig=%i_Energie_%i.root",place,isotop,Position,sourceConfig,i);
      sprintf(saveName, "calibration/%s/%s_calibration_Position%i_SourceConfig=%i_Energie_%i.root",isotop,isotop,Position,sourceConfig,i);



      string executeString = Form(" ./ExtractHistogramm  %s %s",rootName,saveName);

      cout << executeString << endl;
      if (submit)
        {
        system(executeString.c_str());
        }
      //-----------------

       char searchFile[500];
       sprintf(searchFile, "%s/Simulationen/Histogramme/calibration/%s/%s_calibration_Position%i_SourceConfig=%i_Energie_%i.root",place,isotop,isotop,Position,sourceConfig,i);
       TFile * fEnergie = new TFile(searchFile,"READ");
       TH1F* hist = (TH1F*)fEnergie->Get("EnergieDeposited_WithResOfDetektor2");
       hist->SetName(Form("%s_Energie_%i_WithResOfDetektor2",isotop,i));
       TH1F* hist2 = (TH1F*)fEnergie->Get("EnergieDeposited");
       hist2->SetName(Form("%s_Energie_%i",isotop,i));

      fCombined->cd();
      hist->SetDirectory(0);
      hist->Write();
      hist2->SetDirectory(0);
      hist2->Write();
    }



   Watch.Stop();
   cout<<  Watch.RealTime()<<endl;
}
