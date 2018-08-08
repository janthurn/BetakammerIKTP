#include "TStopwatch.h"
#include "TFile.h"

void ExtractKalibrationsEnergien(){


  //======================================================================================================================================
  //======================================================= Setup ========================================================================
  //======================================================================================================================================

  bool submit= true;
  const unsigned linien=14;
  double Energien[linien]={624.216,655.668,660.364,661.404, //Cs-137
                           240.4143,266.8662,320.0283,347.8639,350.2986, //Ba-133
                          481.6935,553.8372,975.651 ,1047.795,1059.805  };

  int Position =3;
  TStopwatch Watch;
  char place[500];
  sprintf(place,  "/raid1/users/thurn/BetaChamber/Simulation");
  
   TFile * fCombined = new TFile(Form("%s/Simulationen/Histogramme/Calibration_Position%i_Combined.root",place,Position),"RECREATE");
  //======================================================================================================================================
  //============================================= Macro itself ===========================================================================
  //======================================================================================================================================
    Watch.Start();
    for (int i = 1; i <= linien; i++)    {
      cout <<"Startlloop "<< i<<endl;
      char isotop[100];
      if(i<=4) sprintf(isotop,"Cs137");
      else if(i>4 &&i<=9)sprintf(isotop,"Ba133");
      else if(i>9) sprintf(isotop,"Bi207");
      else sprintf(isotop,"noisotop");


      //---------------------
      char rootName[500];
      char saveName[500];
      sprintf(rootName, "%s/Simulationen/Raw_Files/Kalibratons_Position%i_%s_Energie_%.1f.root",place,Position,isotop, Energien[i-1]);
      sprintf(saveName, "calibration/%s/%s_calibration_Position%i_Energie_%.1f.root",isotop,isotop,Position,Energien[i-1]);

      string executeString = Form(" ./ExtractHistogramm  %s %s",rootName,saveName);

      cout << executeString << endl;
      if (submit)
        {
        system(executeString.c_str());
        }
      //-----------------

       char searchFile[500];
       sprintf(searchFile, "%s/Simulationen/Histogramme/calibration/%s/%s_calibration_Position%i_Energie_%.1f.root",place,isotop,isotop,Position,Energien[i-1]);
       TFile * fEnergie = new TFile(searchFile,"READ");
       TH1F* hist = (TH1F*)fEnergie->Get("EnergieDeposited_WithResOfDetektor2");

      hist->SetName(Form("%s_%.1f_keV_WithResOfDetektor2",isotop,Energien[i-1]));
      fCombined->cd();
      hist->SetDirectory(0);
      hist->Write();

    }



   Watch.Stop();
   cout<<  Watch.RealTime()<<endl;
}
