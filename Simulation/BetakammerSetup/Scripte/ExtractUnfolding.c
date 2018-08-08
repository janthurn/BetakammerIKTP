#include "TStopwatch.h"
#include "TFile.h"

void ExtractUnfolding(){


  //======================================================================================================================================
  //======================================================= Options ========================================================================
  //======================================================================================================================================

  int Position		=1;
  int chooseisotop	=1;
  bool SetBacking	=true;
  double Siliziumdicke	=70.; // um
  int chooseshape	=2;
  bool submit= true;
  
  //double Qwert[2]={1491.5,1710.6}; //Si31:1491.5, P32: 1710.6 keV
  double Upper_End[2]={1520,1740};
  int bins = Upper_End[chooseisotop-1]/20;

   //======================================================================================================================================
  //===================================================== Filename =======================================================================
  //======================================================================================================================================
  
    char place[100];
    sprintf(place,  "/raid1/users/thurn/BetaChamber/Simulation");
   //------
    char isotop[10];
    if(chooseisotop==1) sprintf(isotop, "Si31");
    else if(chooseisotop==2)sprintf(isotop,"P32");
    else sprintf(isotop,"noisotop");
   //------
    char shape[100];
    if(chooseshape==1) sprintf(shape,"Tube");
    else if(chooseshape==2) sprintf(shape,"Cube");
    else sprintf(shape,"Tube");
    
   //------
    char SaveFileName[500];
    char rootNamePart1[200];
    char rootNamePart2[200];
  
    sprintf(rootNamePart1,"%s/Simulationen/Raw_Files/",place);
    //------
    if(SetBacking){
       sprintf(rootNamePart2,"%s_unfolding_Position%i_thickness%.1fum_withSiBacking",isotop,Position,Siliziumdicke,shape);
       sprintf(SaveFileName,"%s/Simulationen/Histogramme/%s_Position%i_thickness%.1fum_%s_withSiBacking_UnfoldingCombined.root",place,isotop,Position,Siliziumdicke,shape);
    }
    else {
	  sprintf(rootNamePart2,"%s_unfolding_Position%i_thickness%.1fum_%s",isotop,Position,Siliziumdicke,shape);
          sprintf(SaveFileName,"%s/Simulationen/Histogramme/%s_Position%i_thickness%.1fum_%s_UnfoldingCombined.root",place,isotop,Position,Siliziumdicke,shape);
    
    }
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
