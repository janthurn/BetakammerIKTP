#include "TStopwatch.h"
#include "TFile.h"

void ExtractSystematicsSiliconThickness(){
  //======================================================================================================================================
  //======================================================= Options ========================================================================
  //======================================================================================================================================

  int Position		=1;
  int chooseisotop	=1;
  bool SetBacking	=true;
  int chooseshape	=2;
  bool submit= true;
  int WhereIsTheData=1;
  cout<< ""<<endl;
  cout<< "====================================================================="<<endl;
  cout<< "Where is the data stored ? (Raid1 or Raid2 = > Type 1 or 2)"<<endl;
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
  cout<< "Where did the loop start ? (1-19999)"<<endl;
  cin>>start;
  cout<< "Where did the loop end ? (2-20000)"<<endl;
  cin>>stop;
  cout<< "================================================="<<endl;
  cout<< ""<<endl;


  //======================================================================================================================================
  //===================================================== Filename =======================================================================
  //======================================================================================================================================

  char placeRaid1[500];
  char placeRaid2[500];
  sprintf(placeRaid1,  "/raid1/users/thurn/BetaChamber/Simulation");
  sprintf(placeRaid2,  "/raid2/users/thurn/Systematics-Zwischenspeicher");
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

    if(WhereIsTheData==1)sprintf(rootNamePart1,"%s/Simulationen/Raw_Files/",placeRaid1);
    else if(WhereIsTheData==2)sprintf(rootNamePart1,"%s/%s/",placeRaid2,isotop);

    //------
    if(SetBacking){
       sprintf(rootNamePart2,"%s_systematics_Position%i_%s_withSiBacking",isotop,Position,shape);
       sprintf(SaveFileName,"%s/Simulationen/Histogramme/systematics/Si31/%s_Position%i_%s_withSiBacking_SystematicsThickness%ito%iCombined.root",placeRaid1,isotop,Position,shape,start,stop);
    }
    else {
       sprintf(rootNamePart2,"%s_systematics_Position%i_%s",isotop,Position,shape);
       sprintf(SaveFileName,"%s/Simulationen/Histogramme/systematic/Si31/%s_Position%i_%s_SystematicsThickness%ito%iCombined.root",placeRaid1,isotop,Position,shape,start,stop);
    }
   //------
  TFile * fCombined = new TFile(SaveFileName,"RECREATE");
  TStopwatch Watch;
  //======================================================================================================================================
  //================================== Extraction from simulation files ==================================================================
  //======================================================================================================================================
    Watch.Start();
    for (int i = start; i <= stop; i++)    {
     cout <<"Startlloop"<<endl;
     double energy=20*i;
     char rootName[500];
     char saveName[500];
  //      sprintf(rootName, "%s/Simulationen/Raw_Files/%s_unfolding_Position%i_Energie_%.1f.root",place,isotop,Position,energy);

      sprintf(saveName, "systematics/Si31/%s_Thickness_Sim%i.root",rootNamePart2,i);
      sprintf(rootName,"%s%s_Thickness_%i.root",rootNamePart1,rootNamePart2,i);


      string executeString = Form(" ./ExtractHistogramm  %s %s",rootName,saveName);

      // submission string

      cout << executeString << endl;
      if (submit)
        {
         system(executeString.c_str());
        }

       TString searchFile=  Form("%s/Simulationen/Histogramme/systematics/Si31/%s_Thickness_Sim%i.root",place,rootNamePart2,i);

      TFile * fEnergie = new TFile(searchFile,"READ");
      TH1F* hist = (TH1F*)fEnergie->Get("EnergieDeposited_WithResOfDetektor2");
      hist->SetName(Form("Simulation_%i",i));
      fCombined->cd();
      hist->SetDirectory(0);
      hist->Write();

    }
    Watch.Stop();
   cout<<  Watch.RealTime()<<endl;



}
