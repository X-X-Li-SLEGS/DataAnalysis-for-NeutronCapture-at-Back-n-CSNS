//lxx2021
//2021.04.08

#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TChain.h"
#include "TObject.h"
#include <math.h>
#include <TMath.h>
#include "algorithm"
#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <cstring> 
#include <numeric>
#include <fstream>
#include <TSystem.h>
using namespace std;

class C6D6Data
{
public:
  ULong64_t GPS_sec;  
  ULong64_t GPS_nsec;  
  Int_t    T0id;
  Int_t    BCid;
  Double_t  Energy;
  Double_t Tof;
  Float_t    Ph;
  Float_t  PeakValue;
  Double_t PeakPoint;    
} ; 

class LiSiData
{
public:
  ULong64_t GPS_sec;  
  ULong64_t GPS_nsec;  
  Int_t    T0id;
  Int_t    BCid;
  Double_t SiTof;
  Float_t    SiPeakValue;
} ;

int main(int argc, char **argv){
     //------------------Read Data--------------
     C6D6Data C6D6Data_1;
     LiSiData LiSiData_1; 
     TChain * C6D6chain;
     TChain * LiSichain;

     C6D6chain= new TChain("NeuDataTree");
     LiSichain= new TChain("SiDataTree");
     if(C6D6chain==0) return 0;
     if(LiSichain==0) return 0;

     std::stringstream ss1,ss2;
     int start,end;
     ss1<<argv[1];
     ss1>>start;
     ss2<<argv[2];
     ss2>>end;
     char fname[100];
     for(int i = int(start); i <= end; i++){//
           sprintf(fname,"/home/xinxiangli/lxx/CSNS/2021/Data/%s/daq-%s-NORM-%05d.root",argv[3],argv[3],i);
           C6D6chain->Add(fname); 
           LiSichain->Add(fname);
        }
     C6D6chain->SetBranchAddress("NeuData_branch",&C6D6Data_1);
     LiSichain->SetBranchAddress("SiData_branch",&LiSiData_1);
   
    Long64_t C6D6_nentries = C6D6chain->GetEntries();
    cout<<"C6D6_nentries =  "<<C6D6_nentries<<endl;
    Long64_t LiSi_nentries = LiSichain->GetEntries();
    cout<<"LiSi_nentries =  "<<LiSi_nentries<<endl;

     //------------------ Build hist_PH-------------
      const Int_t kNNbins=1150;//1150
      Double_t kNEdges[kNNbins+1];
      Double_t kNmax=TMath::Log10(1.0E+05);
      Double_t kNmin=TMath::Log10(1.0E+00);//eV
      Double_t kNint;
      kNint=(kNmax-kNmin)/kNNbins;
      Float_t kNlog;

      for(int i=0;i<kNNbins+1;i++){
        kNlog=kNint*i+kNmin;
        kNEdges[i]=TMath::Power(10,kNlog);
      } 

     Int_t ph_bin[3] = {4096, 0, 4096};//4096,0,4096
     TH1F* Peak[kNNbins];
     char ph_61name[100];
     for(int i = 0; i<kNNbins; i++)
        {
           sprintf(ph_61name,"Peak_%04d",i);
           Peak[i] = new TH1F(ph_61name,ph_61name,ph_bin[0],ph_bin[1],ph_bin[2]);
        } 
     
     TH1F* Yield =  new TH1F("Yield","Yield",kNNbins,kNEdges);
     for(int i = 0; i<kNNbins; i++)
        {
         for(int ii = 0; ii<C6D6chain->GetEntries(); ii++)
            {
             C6D6chain->GetEntry(ii);
             if(C6D6Data_1.Energy*1e6>=kNEdges[i] && C6D6Data_1.Energy*1e6<kNEdges[i+1])
               {
                Peak[i]->Fill(C6D6Data_1.PeakValue);
               }
             }
         }

     for(int i = 0; i<kNNbins; i++)
       {
        Yield->SetBinContent(i,Peak[i]->Integral(1,ph_bin[0]));
       }

     //------------------ Output-------------

     char fileout[256];
     sprintf(fileout,"Out/Data.root");
     TFile *fout = new TFile(fileout,"recreate");
     Yield->Write();
     fout->Close();

     std::ofstream outfile_Data;
     char TName[100];
     Double_t Chn,Counts;
     for(int i = 0; i<kNNbins; i++){
         sprintf(TName,"%s%d.txt","PH/",i);
         for(int ii=ph_bin[1];ii<ph_bin[2];ii++){
            Chn      = Peak[i]->GetBinCenter(ii);
            Counts   = Peak[i]->GetBinContent(ii);
            outfile_Data.open(TName,ios::app);
            outfile_Data<<Chn<<" "<<Counts<<endl;
            outfile_Data.close();
         }
     }
  
     //------------ Finish -------------
     clock_t stop;
     stop = clock(); 
     cout<<"time = "<<stop/CLOCKS_PER_SEC<<"s"<<endl; 
     gSystem->Exit(1);

}
 

