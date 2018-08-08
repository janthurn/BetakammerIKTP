#include "TStopwatch.h"
#include "TH1.h"




void TestFilerReading(){

  const int chooseisotop=1;
  char isotop[100];
  if(chooseisotop==1) sprintf(isotop,"Calibration/Bi207.dat");
  else if(chooseisotop==2)sprintf(isotop,"Calibration/Cs137.dat");
  else if(chooseisotop==3)sprintf(isotop,"Calibration/Ba133.dat");
  else sprintf(isotop,"noisotop");

///==============================
  const int NLines[3] = {20,7,51};
  cout << chooseisotop<<endl;
  const int nlines=NLines[chooseisotop-1];
  double energie1[nlines];
  double energie2[nlines];
  double intensity[nlines];
  int f1[nlines],f2[nlines],ad[nlines];
  // char *data_raa_95CL = "./input_data/contours/RAAContours/new_raa_95CL.csv";



  char *data = "Calibration/Cs137.dat";
  ifstream datafile(isotop);
  for (int i=0; i<nlines; i++) {
      datafile >> energie1[i] >> f1[i]>> intensity[i] >> f2[i]>> ad[i] >> energie2[i];
      cout << "energie1[i] = " << energie1[i] << "<--> intensity[i] = " << intensity[i] << endl;

  }



}
