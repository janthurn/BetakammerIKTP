#include "TFile.h"
#include "TF1.h"
#include "TH1.h"
#include "TH2.h"
#include "TChain.h"
#include "TTree.h"
#include "TBranch.h"
#include "TRandom3.h"
#include <iostream>
#include <algorithm>
#include <sstream>
#include <vector>
#include <fstream>
#include <TSystem.h>
#include "TGraphErrors.h"
#include <string>
#include <map>
#include <cstddef>

using namespace std;
// using std::stringstream;

// Programm to extract necessary histogramms from rootfile to minimize needed storage space
// use this after make according to: ./ExtractHistogramm RAW-Simulation/.../name.root savename





std::vector<TString> names;
std::vector<TH1F*> hEnergies;
std::vector<TH1F*> resHist;

bool createBinnedHistogramms=true;

// ============================================================-

TH1F * ApplyResolution(TH1F* h_in,TF1* sigmafunc){

    TH1F* h_out = (TH1F*) h_in->Clone(TString::Format("%s_WithGaussianER", h_in->GetName())); // Clone, to get the same binnning
          h_out->Reset();

     TRandom3 r;

     Int_t nbins = h_in->GetNbinsX();
     Int_t i;

     Double_t E, Sigma,gesamt,change;
     Double_t inhalt,j,Rundung;
     cout << h_in->Integral() <<endl;

      for(i = 2; i < nbins + 1; i++) {
        E = h_in->GetBinCenter(i);
    	inhalt = (Double_t) h_in->GetBinContent(i);
    // 	if(i>=430 && i<=640){/*cout <<"Inhalt vor Rundung: "<<inhalt<<endl;*/}
    	gesamt+=inhalt;
    	Rundung=inhalt-(Int_t)inhalt;
    	if(Rundung>=0.5){inhalt=(Int_t)inhalt+1;}
    	if(Rundung<0.5){inhalt=(Int_t)inhalt;}
        double resolution = sigmafunc->Eval(E);
    	Sigma = resolution*E/2.3548;
	//cout << E << " keV ->"<< Sigma<<endl;
    	for(j = 0; j < inhalt; j++) {
    	  h_out->Fill(r.Gaus(E, 0.5*Sigma));
    	}
      }
      return h_out;

}

// ============================================================-

TF1 * GetResolution(){

  TFile* hfResfileDet2 = new TFile("/raid1/users/thurn/BetaChamber/Simulation/BetakammerSetup/Aufloesung/AufloesungenDetector2.root","READ");
  TF1* fResfuncDet2 = (TF1*)hfResfileDet2->Get("Aufloesung_detector2_Fit"); // Sigma(E)
  if(fResfuncDet2)cout<<"Resolution for detector 2 was found!"<<endl;

 return fResfuncDet2;
}

// ============================================================-

void CreateHistWithDetectorBinning(TString binning){

        TString plotnameDet1="Detektor6_binning";
        names.push_back(plotnameDet1);
        TString plotnameDet3="Detektor4_binning";
        names.push_back(plotnameDet3);
        TString plotnameDet4="Detektor10_binning";
        names.push_back(plotnameDet4);
        TString plotnameDet5="Detektor2_binning";
        names.push_back(plotnameDet5);
        TString plotnameDet6="Detektor6_feines_binning";
        names.push_back(plotnameDet6);
        TString plotnameDet7="Detektor4_feines_binning";
        names.push_back(plotnameDet7);
        TString plotnameDet8="Detektor10_feines_binning";
        names.push_back(plotnameDet8);
        TString plotnameDet9="Detektor2_feines_binning";
        names.push_back(plotnameDet9);
	TString plotnameDet10="MC_Detektor6_feines_binning";
        names.push_back(plotnameDet10);
	TString plotnameDet11="MC_Detektor4_feines_binning";
        names.push_back(plotnameDet11);
	TString plotnameDet12="MC_Detektor10_feines_binning";
        names.push_back(plotnameDet12);
	TString plotnameDet13="MC_Detektor2_feines_binning";
        names.push_back(plotnameDet13);



    // ==================================================
    // ========= Calibrationsparameter  =================
    // ==================================================
	 std::vector<float>  ACalib(4),BCalib(4);

	   cout << "Calibration choosen ="<< binning <<endl;

	if(binning=="Alt" || binning=="alt"){
	 float ACalib_Alt[4]={0.,7.937270,9.393390,9.283360};
	 float BCalib_Alt[4]={0.873950,0.793990,0.759516,0.712669};
	 std::copy(ACalib_Alt,ACalib_Alt+4,ACalib.begin());
	 std::copy(BCalib_Alt,BCalib_Alt+4,BCalib.begin());
	}
	else if(binning=="Triga" || binning=="triga"){
	 float ACalib_TRIGA[4]={10.5197,10.8308,11.5395,9.36966};
	 float BCalib_TRIGA[4]={0.828279,0.829159,0.830369,0.830578};
	 std::copy(ACalib_TRIGA,ACalib_TRIGA+4,ACalib.begin());
	 std::copy(BCalib_TRIGA,BCalib_TRIGA+4,BCalib.begin());
	}
	else if(binning=="before50muSi" || binning=="50muSi"){
	 float ACalib_50mu[4]={8.67774,7.31703,9.62066,7.91932};
	 float BCalib_50mu[4]={0.828847,0.831704,0.830294,0.831063};
	 std::copy(ACalib_50mu,ACalib_50mu+4,ACalib.begin());
	 std::copy(BCalib_50mu,BCalib_50mu+4,BCalib.begin());
	}
	else if(binning=="special" || binning=="spezial"){
	 float ACalib_50mu[4]={11.7479,11.7479,11.7479,11.7479};
	 float BCalib_50mu[4]={0.829234,0.829234,0.829234,0.829234};
	 std::copy(ACalib_50mu,ACalib_50mu+4,ACalib.begin());
	 std::copy(BCalib_50mu,BCalib_50mu+4,BCalib.begin());
	}
	else if(binning=="ADC4" || binning=="adc4"){
	 float ACalib_50mu[4]={-12.6406,-10.9395,11.7479,11.7479};
	 float BCalib_50mu[4]={0.827824,0.829613,0.829234,0.829234};
	 std::copy(ACalib_50mu,ACalib_50mu+4,ACalib.begin());
	 std::copy(BCalib_50mu,BCalib_50mu+4,BCalib.begin());
	}
	else if(binning=="test" || binning=="Test"){
	 float ACalib_50mu[4]={-0.69061,-0.69061,11.7479,11.7479};
	 float BCalib_50mu[4]={0.829234,0.829234,0.829234,0.829234};
	 std::copy(ACalib_50mu,ACalib_50mu+4,ACalib.begin());
	 std::copy(BCalib_50mu,BCalib_50mu+4,BCalib.begin());
	}


    // ==================================================
    // ========= Binning wie Kalibration =================
    // ==================================================

        for(int det=0;det<4;det++){
          double a = ACalib[det];
          double b = BCalib[det];
          double minEnergy=5*b+a;
          double binBreite = 5*b;
          double max = 330*binBreite+minEnergy;
          double BinBreiteDET=2*binBreite;


          double xminDet=minEnergy;
          double xmaxDet=max;
          int nDet = (xmaxDet-xminDet)/BinBreiteDET;

          cout << "Binning: TH1F* energy = new TH1F("<< names[det]<<", ,"<<nDet<<","<<xminDet<<","<<xmaxDet<<");" << endl;
          TH1F* energy= new TH1F(names[det],"",nDet,xminDet,xmaxDet);
          hEnergies.push_back(energy);
        }

    // ==================================================
    // ========= Binning viel feiner ====================
    // ==================================================

        for(int det=0;det<4;det++){
          double a = ACalib[det];
          double b = BCalib[det];
          double minEnergy=a;
          double binBreite = b;
          double max = 2048*binBreite+minEnergy;
          double BinBreiteDET=binBreite;


          double xminDet=minEnergy;
          double xmaxDet=max;
          int nDet = (xmaxDet-xminDet)/BinBreiteDET;


          cout << "Binning: TH1F* energy = new TH1F("<< names[det+4]<<", ,"<<nDet<<","<<xminDet<<","<<xmaxDet<<");" << endl;
          TH1F* energy= new TH1F(names[det+4],"",nDet,xminDet,xmaxDet);
          hEnergies.push_back(energy);
       }
    //=================== MC ============================
    // ==================================================
    // ========= Binning viel feiner ====================
    // ==================================================

        for(int det=0;det<4;det++){
          double a = ACalib[det];
          double b = BCalib[det];
          double minEnergy=a;
          double binBreite = b;
          double max = 2048*binBreite+minEnergy;
          double BinBreiteDET=binBreite;


          double xminDet=minEnergy;
          double xmaxDet=max;
          int nDet = (xmaxDet-xminDet)/BinBreiteDET;


          cout << "Binning: TH1F* energy = new TH1F("<< names[det+8]<<", ,"<<nDet<<","<<xminDet<<","<<xmaxDet<<");" << endl;
          TH1F* energy= new TH1F(names[det+8],"",nDet,xminDet,xmaxDet);
          hEnergies.push_back(energy);
       }



}

// ============================================================-

int main(int argc,char ** argv){

    const char * SavePath = "/raid1/users/thurn/BetaChamber/Simulation/Simulationen/Histogramme";
    char * CalibName  = argv[1];
    char * FileName  = argv[2];
    char * savename = argv[3];

    cout <<"argc ="<<argc<<endl;

    TString CalibChoosen=Form("%s",CalibName);
    TString file;
    if(argc<4){
      cout <<"No Savename given->script will use the original one"<<endl;
      std::string str_FileName(FileName);
      std::size_t found = str_FileName.find_last_of("/\\");
      std::cout << "path: "<< str_FileName.substr(0,found)<<endl;
      std::cout << "file: "<< str_FileName.substr(found+1)<<endl;
      string file_str = str_FileName.substr(found+1);
      const char * file_char = file_str.c_str();
      file=Form("%s",file_char);
     }
    else file =Form("%s",savename);

    Float_t Eelectron;
    Float_t MCEkin;
    // ====================================================================
    // =============== setup for extraction ===============================
    // ====================================================================
        TChain* extractionChain =  new TChain("siliconOutput","monoenergetic electrons for Unfolding");
                extractionChain->SetBranchStatus("*",0);
                extractionChain->SetBranchStatus("SiliconEnergy",1);
                extractionChain->SetBranchAddress("SiliconEnergy",&Eelectron);
                extractionChain->SetBranchStatus("MC_Ekin",1);
                extractionChain->SetBranchAddress("MC_Ekin",&MCEkin);

        TString plotname="Mc_Ekin";
        TString plotname2="EnergieDeposited";

        TH1F* energy= new TH1F(plotname,"",2000,0,2000);
        TH1F* energy2= new TH1F(plotname2,"",2000,0,2000);
        hEnergies.push_back(energy);
        hEnergies.push_back(energy2);

        cout << ""<<endl;
        cout << "MC: TH1F* energy = new TH1F("<< plotname<<", ,"<<2000<<","<<0<<","<<2000<<");" << endl;
        cout << "Normal: TH1F* energy2 = new TH1F("<< plotname2<<", ,"<<2000<<","<<0<<","<<2000<<");" << endl;
        if(createBinnedHistogramms==true)CreateHistWithDetectorBinning(CalibChoosen);


    // ====================================================================
    // =============== Extract Histogramms from root-File =================
    // ====================================================================


        std::cout << "Adding "<< FileName << " to chain."<<endl;
        extractionChain->AddFile(FileName);
        cout << extractionChain->GetFile()<<endl;
        cout << "Extraction: extracting histogramm from chain"<<endl;
        extractionChain->Draw(Form("MC_Ekin>>%s",plotname.Data()));
        extractionChain->Draw(Form("SiliconEnergy>>%s",plotname2.Data()));
      	if(createBinnedHistogramms==true){
	   for(int i=0;i<names.size();i++){
             TString histname=names[i];
	     if(i>=8)extractionChain->Draw(Form("MC_Ekin>>%s",histname.Data()));
             else extractionChain->Draw(Form("SiliconEnergy>>%s",histname.Data()));
          }
	}

        cout << "finished data extraction "<<endl;

    // ====================================================================
    // ================== Adding Energy Resolution to it ==================
    // ====================================================================
        int Detektor[1]={2};
	    int nhists = hEnergies.size();
	    cout << nhists<< " histogramms are extracted"<<endl;
        for(int hist=1;hist<nhists;hist++){
         //if(det>=4) cout <<" smear simulated histogramm with energy resolution of detector "<< Detektor[det-4]<<endl;
         cout <<"Hist "<< hist <<" Script will smear simulated histogramm with energy resolution of detector "<<2<<" !"<<endl;
         TF1* resFunc = GetResolution();
         TH1F* SmearedEnergy = ApplyResolution(hEnergies[hist],resFunc);
         cout <<"" <<endl;

         if(hist>=5){
	  cout <<Form("%s_WithResOfDetektor%i",hEnergies[hist]->GetName(),2) <<endl;
          SmearedEnergy->SetName(Form("%s_WithResOfDetektor%i",hEnergies[hist]->GetName(),2));
	 }
         else {
	    cout <<Form("%s_WithResOfDetektor%i",hEnergies[hist]->GetName(),2) <<endl;
            SmearedEnergy->SetName(Form("%s_WithResOfDetektor%i",hEnergies[hist]->GetName(),2));
  	 }
         cout <<""<< SmearedEnergy<<endl;
         resHist.push_back(SmearedEnergy);
        }
	cout <<"   hier  " <<endl;
    // ====================================================================
    // ================== saving all histogramms ==========================
    // ====================================================================
        TString SFname =Form("%s/",SavePath);
        cout<<"SFname = "<< SFname<<endl;

        if(argc<4){  SFname+=file;
         }
        else SFname+=Form("%s",savename);
        cout<<"SFname = "<< SFname<<endl;


        TFile * fElectronHistogramms;
        cout << "Saving histogramms into "<< SFname <<endl;


        if(!gSystem->AccessPathName(SFname)){
          cout << "file allready exist->Will be recreated! " <<endl;
          fElectronHistogramms=new TFile(SFname,"RECREATE");
        }
        else fElectronHistogramms=new TFile(SFname,"RECREATE");



        fElectronHistogramms->cd();
	cout <<hEnergies.size()	<<endl;
	cout <<resHist.size()	<<endl;

        for(int h=0;h<hEnergies.size();h++){
	cout <<hEnergies[h]	<<endl;
         hEnergies[h]->Write();
         hEnergies[h]->SetDirectory(0);
        }


        for(int i =0; i<resHist.size(); i++){

         resHist[i]->Write();
         resHist[i]-> SetDirectory(0);
        }



        // extractionChain->Delete();
        // fElectronHistogramms->Close();



    }
