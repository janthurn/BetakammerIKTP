#include "TFile.h"
#include <TPad.h>
#include "TH1.h"
#include "TF1.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLine.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <TSystem.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include "/home/jan/workspace/Betakammer/BetaKammerKlasse/BetaKammer.hh"
#include "TPaveStats.h"
#include <string>         // std::string

using namespace std;



void Verschmierung(){


TH1F* hOriginal = new TH1F("originalLinie",";energy (keV);intensity /(0.5keV)",500,200,450);
hOriginal->Fill(320.0259,1000);

hOriginal->Draw();


}
