#include "TFile.h"
#include "TF1.h"
#include "TH1.h"
#include "TH2.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TTree.h"
#include "TBranch.h"
#include "TLine.h"
#include "TRandom3.h"
#include <iostream>
#include <algorithm>
#include <sstream>
#include <vector>
#include <fstream>
#include <TSystem.h>
#include "TGraphErrors.h"
#include <TLegend.h>
#include <string>
#include <map>
#include <cstddef>
#include "TROOT.h"
#include "/home/jan/workspace/Betakammer/BetaKammerKlasse/BetaKammer.hh"

using namespace std;


BetaKammer * beta = new BetaKammer();

double DipTiefe(TH1* hin,TString canvasName, bool before, bool draw){

    double binwith= hin->GetBinWidth(2);
    int nbins= hin->GetNbinsX();
    double average_BeforeDipp=0.;
    double average_Dipp=0.;
    double averageAfterDipp=0.;

    int nbefore,ndip,nafter;

    for(int bin =1;bin<=nbins;bin++){
        double energy = hin->GetBinCenter(bin);
        if(energy>=85 && energy<=120){
            nbefore++;
            average_BeforeDipp+=hin->GetBinContent(bin) ;
        }
        else if(energy>=270 && energy<=330){
            ndip++;
            average_Dipp+=hin->GetBinContent(bin) ;
        }
        else if(energy>=350 && energy<=430){
            nafter++;
            averageAfterDipp+=hin->GetBinContent(bin) ;
        }
    }

    average_BeforeDipp/=nbefore;
    average_Dipp/=ndip;
    averageAfterDipp/=nafter;
    cout <<" ----------------------"<<endl;
    cout << " Values and Ratios "<<endl;
    cout << average_BeforeDipp <<" , " << average_Dipp <<" , "<< averageAfterDipp<<endl;

    TLine * lineBefore= new TLine(0,average_BeforeDipp,1600,average_BeforeDipp);
    TLine * lineDip = new TLine(0,average_Dipp,1600,average_Dipp);
    TLine * lineAfter = new TLine(0,averageAfterDipp,1600,averageAfterDipp);
    TLine * lineTiefe = new TLine(0,(average_BeforeDipp+averageAfterDipp)/2,1600,(average_BeforeDipp+averageAfterDipp)/2);
    lineTiefe->SetLineColor(kMagenta);
    lineDip->SetLineColor(kGreen+2);
    if(draw){
        TCanvas *cDipTiefe= new TCanvas(canvasName,"How deep ist the Dip",800,800);
          hin->Draw("E");
          lineBefore->Draw("Same");
          lineDip->Draw("Same");
          lineAfter->Draw("Same");
            lineTiefe->Draw("Same");
    }
      cout << "before: " << average_BeforeDipp/average_Dipp<< " after: " << averageAfterDipp/average_Dipp << endl;

      double depth = (average_BeforeDipp+averageAfterDipp)/(2*average_Dipp);


      double ratio = 1.0;
      if(before)  ratio = average_BeforeDipp/average_Dipp;
      else ratio = averageAfterDipp/average_Dipp;


 return depth;
}

//==========================================================================================================

TH1D*  ExtractSiliconEnergy(TString filename){

    TH1D* energydepo= new TH1D(Form("EnergieDeposited_%s",filename),";energy(keV)",400,0,2000);

    Double_t Eelectron;

    TFile * myfile= new TFile(filename,"READ");
    TTree* tree= (TTree*)myfile->Get("siliconOutput");
    tree->SetBranchStatus("SiliconEnergy",1);
    tree->SetBranchAddress("SiliconEnergy",&Eelectron);
    int entries= tree->GetEntries();
        for (int i=0; i<entries; i++)    {
            tree->GetEntry(i);
            energydepo->Fill(Eelectron);
    }

 return energydepo;
}

//==========================================================================================================

TH1D*  ExtractSiliconEnergyXCutYCut(TString filename,double Limit1, bool outside){

    TH1D* hOut;
    TH1D* energy_border= new TH1D(Form("EnergieDeposited_BorderEvents_%s",filename),";energy(keV) ",200,0,2000);
    TH1D* energy_inside= new TH1D(Form("EnergieDeposited_InnerEvents_%s",filename),";energy(keV) ",200,0,2000);
    TFile * myfile= new TFile(filename,"READ");
    Double_t Eelectron,mxp,myp;
    TTree* tree= (TTree*)myfile->Get("siliconOutput");
    tree->SetBranchStatus("SiliconEnergy",1);
    tree->SetBranchAddress("SiliconEnergy",&Eelectron);
    TTree* tree2= (TTree*)myfile->Get("MC_Input");
    tree2->SetBranchAddress("mc_xpos",&mxp);
    tree2->SetBranchAddress("mc_ypos",&myp);
    int entries= tree->GetEntries();
    int inside =0;
    int border =0;
    cout <<" test "<< Limit1<<endl;
    for (int i=0; i<entries; i++)    {
            tree->GetEntry(i);
            tree2->GetEntry(i);
            // if(i %100000 ==0)    cout << "(x , y) = (" << mxp<< ", " << myp << ")"<<"\n";
            if((mxp<=Limit1 && mxp >=-1*Limit1) && (myp<=Limit1 && myp>=-1*Limit1)) {
                inside++;
                energy_inside->Fill(Eelectron);

            }
        else if((mxp>Limit1 ||  mxp < (-1*Limit1)) ||  (myp>Limit1 ||  myp <-1*Limit1)){
                 border++;
                energy_border->Fill(Eelectron);
            }
            // if(i %100000 ==0)std::cout << "entry="<<i <<" inside ="<< inside<<" border ="<< border<< "\n";
    }
    if(outside) hOut =energy_border;
    else hOut =energy_inside;

 return hOut;
}

//==========================================================================================================
    /*
    Double_t MCEkin;
    Double_t mxp,myp;
    Double_t Eelectron;
    // TH1D* mc_xposition= new TH1D("MC_xpos","MC X position;x position (mm) ;entries / 0.1 mm",120,-6,6);
    // TH1D* mc_yposition= new TH1D("MC_ypos","MC Y position;y position (mm) ;entries / 0.1 mm",120,-6,6);
    // TH1D* energydepo= new TH1D("EnergieDeposited",";energy(keV)",200,0,2000);
    //
    //
    // TH1D* MCenergy_border= new TH1D("MCenergy_border"," ",200,0,2000);
    // TH1D* MCenergy_Inner= new TH1D("MCenergy_Inner"," ",200,0,2000);
    // // --- Trees

    TTree* tree= (TTree*)myfile2->Get("MC_Input");
    tree->SetBranchAddress("MC_Ekin",&MCEkin);
    tree->SetBranchAddress("mc_xpos",&mxp);
    tree->SetBranchAddress("mc_ypos",&myp);
    tree->SetBranchAddress("mc_ypos",&myp);
    int entries= tree->GetEntries();
    double Limit1 =2.0;


    for (int i=0; i<entries; i++)    {
    // for (int i=0; i<1000; i++)    {
    tree->GetEntry(i);
    energy->Fill(MCEkin);
    mc_xposition->Fill(mxp);
    mc_yposition->Fill(myp);
    tree2->GetEntry(i);
    energydepo->Fill(Eelectron);

    // cout << "(x , y) = (" << mxp<< ", " << myp << ")";


    if((mxp<=Limit1 && mxp >=-1*Limit1) && (myp<=Limit1 && myp>=-1*Limit1)) {
    inside++;
    // std::cout << " -> inside border" << "\n";
    energy_inside->Fill(Eelectron);
    MCenergy_Inner->Fill(MCEkin);

    }
    // if((mxp<=Limit1 || mxp >=-Limit1) && (myp<=Limit1 || myp>=-Limit1)) {
    else if((mxp>Limit1 ||  mxp < (-1*Limit1)) ||  (myp>Limit1 ||  myp <-1*Limit1)){
    // std::cout << " -> outside border" << "\n";
    outside++;
    energy_border->Fill(Eelectron);
    MCenergy_border->Fill(MCEkin);
    }


    }
    cout << " Selection: "<<inside << " , "<<outside<<endl;
    cout << " Expectation: "<< 2e6*pow(Limit1/5,2)<< " , "<<2e6*(1-pow(Limit1/5,2))<<endl;
    energy_inside->SetLineColor(kRed);
    MCenergy_Inner->SetLineColor(kRed);
    energy_border->SetLineColor(kMagenta);
    MCenergy_border->SetLineColor(kMagenta);
    energydepo->GetYaxis()->SetRangeUser(0,11000);
    energydepo->SetLineWidth(2);
    energy_inside->SetLineWidth(2);
    energy_border->SetLineWidth(2);
    TCanvas *MCTrue= new TCanvas (Form("MCTrue_%.1fmm",Limit1),"Monte Carlo true variables",800,800);
    MCTrue->Divide(3,1);
    MCTrue->cd(1);
    energy->Draw("");
    MCenergy_Inner->Draw("Same");
    MCenergy_border->Draw("Same");
    MCTrue->cd(2);
    mc_xposition->Draw("");
    lineLowL->Draw("Same");
    lineUPL->Draw("Same");
    MCTrue->cd(3);
    mc_yposition->Draw("");
    lineLowL->Draw("Same");
    lineUPL->Draw("Same");
    */

//==========================================================================================================

void TestDifferentExtraction4(){

  gStyle->SetOptStat(0);
     // histograms





    // TFile * myfile= new TFile("Si31SampleTest3.root","READ");
    // TFile * myfile2= new TFile("Si31_Dip_OhneHalterSonde.root","READ");
    // TFile * myfile3= new TFile("Si31_Dip_NurSample.root","READ");


    TH1D* hNormal = ExtractSiliconEnergy("Si31SampleTest3.root");
    TH1D* hOhneHS = ExtractSiliconEnergy("Si31_Dip_OhneHalterSonde.root");
    TH1D* hOnlysample = ExtractSiliconEnergy("Si31_Dip_NurSample.root");
    hNormal->Scale(0.5);

    // filling

    hNormal->SetLineColor(kRed);
    hOhneHS->SetLineColor(kBlue);
    hOnlysample->SetLineColor(kMagenta);
    hNormal->GetYaxis()->SetRangeUser(0,3000);
    // TLine * lineLowL= new TLine(-1*Limit1,0,-1*Limit1,20500);
    // TLine * lineUPL = new TLine(Limit1,0,Limit1,20500);
    // lineLowL->SetLineColor(kGreen+2);
    // lineUPL->SetLineColor(kGreen+2);
    // lineLowL->SetLineWidth(2);
    // lineUPL->SetLineWidth(2);

    TLegend * leg1 = new TLegend(0.35,0.67,0.9,0.9);
    leg1->SetFillColor(0);
    // leg1->SetBorderSize(0);
    leg1->AddEntry(hNormal,"complete sample holder","l");
    leg1->AddEntry(hOhneHS,"complete sample holder without top part","l");
    leg1->AddEntry(hOnlysample,"Only Sample","l");



     TCanvas *PIPSenergySpectrum= new TCanvas("PIPSEnergySpectrum","Energie deposited in PIPS",800,800);
       hNormal->Draw("E");
       hOhneHS->Draw("Same,E");
       hOnlysample->Draw("Same,E");
       leg1->Draw();

     double normal = DipTiefe(hNormal,"complete",false,true);
      double ohne HS  =DipTiefe(hOhneHS,"complete ohne HS",false,false);
      double onlySample= DipTiefe(hOnlysample,"only",false,false);


      ///=========================================

      TGraph * gDipRatioBefore = new TGraph();
      TGraph * gDipRatioAfter = new TGraph();
      TGraph * gDipTiefe = new TGraph();
      int p=-1;
      for(int i=0;i<=25;i++){
          double l = 5-0.1*i;
          p++;
          cout << ""<<endl;
          cout << "Limit = "<<l<<" mm"<<endl;
          cout << ""<<endl;

          TH1D* hWithLimit =ExtractSiliconEnergyXCutYCut("Si31SampleTest3.root",l,false);
        //   double ratiobefore=DipTiefe(hWithLimit,"name",true,false);
        //   double ratioafter=DipTiefe(hWithLimit,"name",false,false);
          double Tiefe=DipTiefe(hWithLimit,"name",false,false);
        //   gDipRatioBefore->SetPoint(p,l,ratiobefore);
        //   gDipRatioAfter->SetPoint(p,l,ratioafter);
          gDipTiefe->SetPoint(p,l,Tiefe);
      }
      gDipRatioBefore->GetYaxis()->SetRangeUser(1,1.4);
    //   beta->SetGraphStyling(gDipRatioBefore,"Limit on X and Y position (mm)","Ratio_{1}",21,kRed,0.7);
    //   beta->SetGraphStyling(gDipRatioAfter,"Limit on X and Y position (mm)","Ratio_{3}",22,kBlue,0.7);
       beta->SetGraphStyling(gDipTiefe,"Limit on X and Y position (mm)","Depth",22,kBlue,0.7);

      TCanvas *Diptiefe= new TCanvas("Diptiefe","Ratio of dip to peak before and after ",800,800);
        gDipTiefe->Draw("AP");


}
