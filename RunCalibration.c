#include "TStopwatch.h"
#include "TH1.h"
#include "TMath.h"

double energie1[100],energie2[100],intensity[100];
double FehlerE[100],FehlerI[100],Additional[100];

//======================================================================================================================================
//======================================================================================================================================
//======================================================================================================================================

double calculateActivity(const int chooseisotop,int ndays){

  // see page 14 of Bachelor Thesis by Anne-Sophie Reimer from 2016 or page 30 of Bachelor Thesis by Mark Felix from 2017 for Cs137
  double activity[3]={3.03,313.0,60.23}; // Bq
  double activityError[3]={0.02,13,2.87};
  double HL[3]={31.55,30.08,10.551}; // years

  double activityAtDayOfMeasurement= 0.;
  double expo = (ndays/365.25)/HL[chooseisotop-1];
  double factor = TMath::Power(0.5,expo);
  activityAtDayOfMeasurement=activity[chooseisotop-1]*factor;

  return activityAtDayOfMeasurement;
}

//======================================================================================================================================
//======================================================================================================================================
//======================================================================================================================================

int CalculateDaysPassed(const int isotop,int dayOfMeasurement,int MonthOfMeasurement,int YearOfMeasurement){
  int daysPassed=0;

  // ------------------------
  const int DaysPerMonth[12]={31,28,31,30,31,30,31,31,30,31,30,31};
  const int DayOfActivityMeasurement[3]={6,1,19};
  const int MonthOfActivityMeasurement[3]={11,04,11};
  const int YearOfActivityMeasurement[3]={2015,2017,2015};

  // relative methode zu 1.1.2015 = relativeDay
    int RelDaysActivityMeasurement=0;
    const int ypassed = YearOfActivityMeasurement[isotop-1]-2015;
    RelDaysActivityMeasurement+=365*ypassed;
    const int mpassed = MonthOfActivityMeasurement[isotop-1]-1;
    for(int m=0;m<mpassed;m++) {
      RelDaysActivityMeasurement+=DaysPerMonth[m];
     }
     RelDaysActivityMeasurement+=DayOfActivityMeasurement[isotop-1];
  //---
    int RelDaysSinceMeasurement=0;
    // cout << RelDaysSinceMeasurement<<" days"<<endl;
    const int  ypassed = YearOfMeasurement-2015;
    RelDaysSinceMeasurement+=365*ypassed;
    // cout << RelDaysSinceMeasurement<<" days"<<endl;
    // cout << ypassed<<" yr"<<endl;
    const int  mpassed=MonthOfMeasurement-1;
    for(int m=0;m<mpassed;m++) {
      RelDaysSinceMeasurement+=DaysPerMonth[m];
    }
      // cout << RelDaysSinceMeasurement<<" days"<<endl;
      RelDaysSinceMeasurement +=dayOfMeasurement;
      // cout << RelDaysSinceMeasurement<<" days"<<endl;
      if(YearOfMeasurement>2016) RelDaysSinceMeasurement++; //schaltjahr 2016
      else if(YearOfMeasurement>2016 && YearOfMeasurement>2020) RelDaysSinceMeasurement++; //schaltjahr 2020
      // cout << RelDaysSinceMeasurement<<" days"<<endl;
  //---
    daysPassed=RelDaysSinceMeasurement-RelDaysActivityMeasurement;
  return daysPassed;
}

//======================================================================================================================================
//======================================================================================================================================
//======================================================================================================================================


const int ReadEnergies(const int chooseisotop){

    char isotop[100];
    if(chooseisotop==1) sprintf(isotop,"Calibration/Bi207.dat");
    else if(chooseisotop==2)sprintf(isotop,"Calibration/Cs137.dat");
    else if(chooseisotop==3)sprintf(isotop,"Calibration/Ba133.dat");
    else sprintf(isotop,"noisotop");

  ///==============================
    const int NLines[3] = {20,7,51};
    cout << chooseisotop<<endl;
    const int nlines=NLines[chooseisotop-1];
    int f1[nlines],f2[nlines],ad[nlines];
    // char *data_raa_95CL = "./input_data/contours/RAAContours/new_raa_95CL.csv";

    ifstream datafile(isotop);
    for (int i=0; i<nlines; i++) {
        datafile >> energie1[i] >> FehlerE[i]>> intensity[i] >> FehlerI[i]>> Additional[i] >> energie2[i];
        cout << energie1[i] << " , "<< FehlerE[i]<< " , "<< intensity[i]<< " , " << FehlerI[i]<< " , " << Additional[i]<< " , " << energie2[i]<< endl;

    }

   return nlines;
}

  //======================================================================================================================================
  //======================================================================================================================================
  //======================================================================================================================================

void RunCalibration(){

  //======================================================================================================================================
  //======================================================= Options ======================================================================
  //======================================================================================================================================
  int Position =1;
  const int chooseisotop=2; // 1 = Bi207,2 = Cs137, 3 = Ba133
  int sourceConfig=1;
  bool submit= false;
  const int NumberLines = ReadEnergies(chooseisotop);
  int timepassedindays =CalculateDaysPassed(chooseisotop,27,4,2017);
  double activity =calculateActivity(chooseisotop,timepassedindays);
  cout << timepassedindays<< " days passed"<<endl;
  cout << activity<<" Bq remaining"<<endl;
  double runtime=420000.0;
  if(chooseisotop==2)runtime/=10;     // activity of source is to high to simulate the
  int TotalNumberOfEvents = activity[chooseisotop-1]*runtime;
  cout <<"Total: "<<TotalNumberOfEvents<<endl;

  //======================================================================================================================================
  //===================================================== Filename =======================================================================
  //======================================================================================================================================

    char place[500];
    sprintf(place,  "/raid1/users/thurn/BetaChamber/Simulation");
   //------
    char isotop[100];
    if(chooseisotop==1) sprintf(isotop,"Bi207");
    else if(chooseisotop==2)sprintf(isotop,"Cs137");
    else if(chooseisotop==3)sprintf(isotop,"Ba133");
    else sprintf(isotop,"noisotop");
   //------

   //------
    char macroNamePart1[500];
    char rootNamePart1[500];
    //sprintf(macroNamePart1,"%s/Betakammer/macros/unfolding/%s_unfolding_Position%i_thickness%.1fum",place,isotop,Position,Siliziumdicke);


   sprintf(macroNamePart1,"%s/Betakammer/macros/calibration/%s_calibration_Position%i_SourceConfig=%i",place,isotop,Position,sourceConfig);
	 sprintf(rootNamePart1,"%s/Simulationen/Raw_Files/%s_calibration_Position%i_SourceConfig=%i",place,isotop,Position,sourceConfig);



  //======================================================================================================================================
  //=============================================== Loop start and stop ==================================================================
  //======================================================================================================================================


  TStopwatch Watch;
  Watch.Start();
  //======================================================================================================================================
  //============================================= Macro itself ===========================================================================
  //======================================================================================================================================

  for(int line=0;line<NumberLines;line++){

      char macroName[500];
      char rootName[500];
      double energy= energie1[line];
      int Nevents = int(intensity[line]/100*TotalNumberOfEvents);
      cout << energy <<" keV "<< Nevents<<endl;
      sprintf(macroName, "%s_Energie_%i",macroNamePart1,line+1);
      sprintf(rootName, "%s_Energie_%i.root",rootNamePart1,line+1);



      string executeString = Form("nice -n 8 ./BetaChamberSetup %s",macroName);
      std::ofstream file(macroName);    // put in macro commands for each file
      {
	  //       file << "/MG/manager/heprandomseed " << time(0) + i "<< std::endl;
           file << "/BetaChamber/geometry/SetupNumber 2 "<< std::endl;
           file << "/BetaChamber/geometry/SondenPositionNummer  "<< Position<< std::endl;
           file << "/BetaChamber/geometry/KalibrationConfig " << sourceConfig<< std::endl;
           file << "/BetaChamber/geometry/KalibrationMaterial  " << chooseisotop<< std::endl;
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
           file << "/gun/energy " << energy << " keV "<< std::endl;
           file << " "<< std::endl;
           file << "/run/printProgress 50000 "<< std::endl;
            file << " /run/beamOn  "<< Nevents << std::endl;

      }
      file.close();

      // submission string
      cout << rootName << endl;
      cout << executeString << endl;
      if (submit)
        {
        system(executeString.c_str());
        }

    }

    Watch.Stop();
   cout<<  Watch.RealTime()<<endl;
}
