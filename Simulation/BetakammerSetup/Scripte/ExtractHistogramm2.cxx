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


        // TString plotnameDet6="Detektor6_feines_binning";
        //     names.push_back(plotnameDet6);
        // TString plotnameDet7="Detektor4_feines_binning";
        //     names.push_back(plotnameDet7);
        // TString plotnameDet8="Detektor10_feines_binning";
        //     names.push_back(plotnameDet8);
        // TString plotnameDet9="Detektor2_feines_binning";
        //     names.push_back(plotnameDet9);
	    // TString plotnameDet10="MC_Detektor6_feines_binning";
        //     names.push_back(plotnameDet10);
	    // TString plotnameDet11="MC_Detektor4_feines_binning";
        //     names.push_back(plotnameDet11);
	    // TString plotnameDet12="MC_Detektor10_feines_binning";
        //     names.push_back(plotnameDet12);
	    // TString plotnameDet13="MC_Detektor2_feines_binning";
        //     names.push_back(plotnameDet13);


            // TString plotnameDet6="Detektor6_feines_binning";
            // TString plotnameDet7="Detektor4_feines_binning";

                names.push_back("Detektor6_feines_binning");
                names.push_back("Detektor4_feines_binning");
                names.push_back("Detektor10_feines_binning");
                names.push_back("Detektor2_feines_binning");
                names.push_back("MC_Detektor6_feines_binning");
                names.push_back("MC_Detektor4_feines_binning");
                names.push_back("MC_Detektor10_feines_binning");
                names.push_back("MC_Detektor2_feines_binning");

            // // TString plotnameDet8="Detektor10_feines_binning";
            // // TString plotnameDet9="Detektor2_feines_binning";
            // TString plotnameDet10="MC_Detektor6_feines_binning";
            // TString plotnameDet11="MC_Detektor4_feines_binning";
            // TString plotnameDet12="MC_Detektor10_feines_binning";
            // TString plotnameDet13="MC_Detektor2_feines_binning";


    // ==================================================
    // ========= Calibrationsparameter  =================
    // ==================================================
	 std::vector<float>  ACalib(4),BCalib(4);
       std::cout <<""<<endl;
	   cout << "Calibration choosen = "<< binning <<endl;
       std::cout <<""<<endl;

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
          double minEnergy=a;
          double binBreite = b;
          double max = 2048*binBreite+minEnergy;
          double BinBreiteDET=binBreite;


          double xminDet=minEnergy;
          double xmaxDet=max;
          int nDet = (xmaxDet-xminDet)/BinBreiteDET;


          cout << "Binning: TH1F* energy = new TH1F("<< names[det]<<", ,"<<nDet<<","<<xminDet<<","<<xmaxDet<<");" << endl;
          TH1F* energy= new TH1F(names[det],"",nDet,xminDet,xmaxDet);
          hEnergies.push_back(energy);
       }
        std::cout <<""<<endl;
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


          cout << "Binning: TH1F* energy = new TH1F("<< names[det+4]<<", ,"<<nDet<<","<<xminDet<<","<<xmaxDet<<");" << endl;
          TH1F* energy= new TH1F(names[det+4],"",nDet,xminDet,xmaxDet);
          hEnergies.push_back(energy);
       }



}


    // =================================================================================================================
    // =================================================================================================================
    // =================================================================================================================

int main(int argc,char ** argv){


        std::cout <<""<<endl;
        std::cout <<"----------------------------------------------------------------------------"<<endl;
        std::cout << "             Working with Input from execution of the script               "<<endl;
        std::cout <<""<<endl;

        //----------------------

            const char * SavePath = "/raid1/users/thurn/BetaChamber/Simulation/Simulationen/Histogramme";
            char * CalibName  = argv[1];
            char * FileName  = argv[2];
            char * savename = argv[3];

            cout <<"Number of arguments given is argc ="<<argc<<endl;

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



    // =================================================================================================================
    // ==================================== setup for extraction =======================================================
    // =================================================================================================================
        std::cout <<""<<endl;
        std::cout <<"----------------------------------------------------------------------------"<<endl;
        std::cout << "        Creating TChains and histogramms to fill in the next step          "<<endl;
        std::cout <<""<<endl;

        //----------------------

            Float_t MCEkin,Eelectron,EAbsorber;
            Double_t PathLengthPIPS;
             TChain* extractionChain1 =  new TChain("siliconOutput","Deposited Energy inside PIPS");
                     extractionChain1->SetBranchAddress("SiliconEnergy",&Eelectron);
                     extractionChain1->SetBranchAddress("SiliconStepLength",&PathLengthPIPS);
             TChain* extractionChain2 =  new TChain("MC_Input","MonteCarlo input");
                     extractionChain2->SetBranchStatus("*",0);
                     extractionChain2->SetBranchStatus("MC_Ekin",1);
                     extractionChain2->SetBranchAddress("MC_Ekin",&MCEkin);
             TChain* extractionChain3 =  new TChain("AbsorberOutput","Deposited Energy inside Absorber");
                     extractionChain3->SetBranchAddress("AbsorberEnergy",&EAbsorber);

                int Nhistogramspre =4;
                TString plotname="Mc_Ekin";
                TString plotname2="EnergieDepositedinPIPS";
                TString plotname3="EnergieDepositedinAbsorber";
                TString plotname4="tracklengthInPIPS";

                TH1F* energy= new TH1F(plotname,"",2000,0,2000);
                TH1F* energy2= new TH1F(plotname2,"",2000,0,2000);
                TH1F* energy3= new TH1F(plotname3,"",500,0,500);
                TH1F* energy4= new TH1F(plotname4,"",100,0,1);

                hEnergies.push_back(energy);
                hEnergies.push_back(energy2);
                hEnergies.push_back(energy3);
                hEnergies.push_back(energy4);
                cout << ""<<endl;
                cout << "MC: TH1F* energy = new TH1F("<< plotname<<", ,"<<2000<<","<<0<<","<<2000<<");" << endl;
                cout << "PIPS: TH1F* energy2 = new TH1F("<< plotname2<<", ,"<<2000<<","<<0<<","<<2000<<");" << endl;
                cout << "Absorber: TH1F* energy3 = new TH1F("<< plotname3<<", ,"<<500<<","<<0<<","<<500<<");" << endl;

                if(createBinnedHistogramms==true)CreateHistWithDetectorBinning(CalibChoosen);
        //----------------------


        std::cout <<""<<endl;
        std::cout <<"                      finished creating Histogramms                         "<<endl;
        std::cout <<""<<endl;
        std::cout <<"----------------------------------------------------------------------------"<<endl;

    // =================================================================================================================
    // ===================================== Extract Histogramms from root-File ========================================
    // =================================================================================================================
        std::cout <<""<<endl;
        std::cout <<"----------------------------------------------------------------------------"<<endl;
        std::cout << "                      Start extraction process                             "<<endl;
        std::cout <<""<<endl;

        //----------------------
             std::cout << "Adding "<< FileName << " to chains."<<endl;
                extractionChain1->AddFile(FileName);
                extractionChain2->AddFile(FileName);
                extractionChain3->AddFile(FileName);
             std::cout << "done!->Test ( ";
                cout << extractionChain1->GetFile()<<" , ";
                cout << extractionChain2->GetFile()<<" , ";
                cout << extractionChain3->GetFile();
             std::cout <<" )"<<endl;
             std::cout <<""<<endl;
             std::cout << "Extracting Data into standard 1 keV Histogramms"<<endl;
                extractionChain1->Draw(Form("SiliconStepLength>>%s",plotname4.Data()));
                extractionChain1->Draw(Form("SiliconEnergy>>%s",plotname2.Data()));
                extractionChain2->Draw(Form("MC_Ekin>>%s",plotname.Data()));
                extractionChain3->Draw(Form("AbsorberEnergy>>%s",plotname3.Data()));
          	if(createBinnedHistogramms==true){
        	    for(int i=0;i<names.size();i++){
                     TString histname=names[i];
        	         if(i>=4)extractionChain2->Draw(Form("MC_Ekin>>%s",histname.Data()));
                     else extractionChain1->Draw(Form("SiliconEnergy>>%s",histname.Data()));
                  }
        	}
       //------------------------

        std::cout <<""<<endl;
        std::cout <<"                      finished data extraction                              "<<endl;
        std::cout <<"----------------------------------------------------------------------------"<<endl;
        std::cout <<""<<endl;
    // =================================================================================================================
    // ======================================= Adding Energy Resolution to it ==========================================
    // =================================================================================================================
        std::cout <<"----------------------------------------------------------------------------"<<endl;
        std::cout <<"           Adding Energy Resolution to Deposited Energy in PIPS             "<<endl;
        std::cout <<""<<endl;
        // ----
        std::cout <<"hEnergies[0,1,2,3] = MC_ekin, Edep inside Pips, Edep inside Absorber,Path length in PIPS"<<endl;
        std::cout <<"hEnergies[4,5,6,7] = Edep inside Pips with Binning according to calibration"<<endl;
        std::cout <<"hEnergies[8,9,10,11] = MC_ekin with Binning according to calibration"<<endl;

        TF1* resFunc = GetResolution();
        std::cout <<""<<endl;
        std::cout <<"Smearing Edep histogramm with standard binning: hEnergies[1]"<<endl;
        //------------------------
            TH1F* SmearedEnergyStandard = ApplyResolution(hEnergies[1],resFunc);
            SmearedEnergyStandard->SetName(Form("%s_WithResOfDetektor%i",hEnergies[1]->GetName(),2));
            resHist.push_back(SmearedEnergyStandard);
        //------------------------

        std::cout <<""<<endl;
        std::cout <<"Smearing Edep histogramm with detector specific binning: hEnergies[3-6]"<<endl;
        std::cout <<""<<endl;
        //------------------------
            for(int hist=Nhistogramspre;hist<Nhistogramspre+4;hist++){
                std::cout <<"hEnergies["<< hist<<"]"<<endl;
                TH1F* SmearedEnergy = ApplyResolution(hEnergies[hist],resFunc);
                SmearedEnergy->SetName(Form("%s_WithResOfDetektor%i",hEnergies[hist]->GetName(),2));
                resHist.push_back(SmearedEnergy);
            }
        //------------------------
        std::cout <<""<<endl;
        std::cout <<"                      finished energy smearing                              "<<endl;
        std::cout <<"----------------------------------------------------------------------------"<<endl;

    // =================================================================================================================
    // ========================================= saving all histogramm =================================================
    // =================================================================================================================
        std::cout <<"----------------------------------------------------------------------------"<<endl;
        std::cout <<"                   Saving all "<<hEnergies.size()<<" + "<< resHist.size()<<"  histogramms         "<<endl;
        std::cout <<""<<endl;
        //------------------------

            TString SFname =Form("%s/",SavePath);
            if(argc<4){  SFname+=file;
             }
            else SFname+=Form("%s",savename);
            cout << "Saving histogramms into "<< SFname <<endl;

            TFile * fElectronHistogramms;

            if(!gSystem->AccessPathName(SFname)){
              cout << "file allready exist->Will be recreated! " <<endl;
              fElectronHistogramms=new TFile(SFname,"RECREATE");
            }
            else fElectronHistogramms=new TFile(SFname,"RECREATE");
            fElectronHistogramms->cd();
    	//------------------------
        std::cout <<""<<endl;
        std::cout <<"unsmeared histogramms"<<endl;
        std::cout <<""<<endl;
        //------------------------
            for(int h=0;h<hEnergies.size();h++){
    	     cout <<"histogramm "<<h+1<<": "<<hEnergies[h]	<<endl;
             hEnergies[h]->Write();
             hEnergies[h]->SetDirectory(0);
            }
        std::cout <<""<<endl;
        std::cout <<"smeared histogramms"<<endl;
        std::cout <<""<<endl;
        //------------------------
            for(int i =0; i<resHist.size(); i++){
             cout <<"histogramm "<<i+1<<": "<<resHist[i]	<<endl;
             resHist[i]->Write();
             resHist[i]->SetDirectory(0);
            }
      //------------------------
      std::cout <<""<<endl;
      std::cout <<"                      finished saving histogramms                           "<<endl;
      std::cout <<"----------------------------------------------------------------------------"<<endl;

    }
