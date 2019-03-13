#define _VANILLA_ROOT_

// C++ headers
#include <iostream>
#include <vector>
#include "TLorentzVector.h"

// ROOT headers
#include "TROOT.h"
#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TSystem.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TMath.h"

// FemtoDst headers
#include "../StFemtoDstReader.h"
#include "../StFemtoDst.h"
#include "../StFemtoEvent.h"
#include "../StFemtoTrack.h"

// Load libraries (for ROOT_VERSTION_CODE >= 393215)
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,0,0)
R__LOAD_LIBRARY(../libStFemtoDst)
#endif

// inFile - is a name of name.FemtoDst.root file or a name
//          of a name.lis(t) files, that contains a list of
//          name1.FemtoDst.root, name2.FemtoDst.root, ... files

//_________________
void Article_data () {

  std::cout << "Hi! Lets do some physics, Master!" << std::endl;

  gROOT->ForceStyle();
  gROOT->SetStyle("Pub");

   TCanvas *c1 = new TCanvas("c1","A Simple Graph with error bars",200,10,700,500);
   c1->SetFillColor(42);
   c1->SetGrid();
   c1->GetFrame()->SetFillColor(21);
   c1->GetFrame()->SetBorderSize(12);
   const Int_t n = 14;
   Double_t x[n]  = {0.3, 0.5, 0.7, 0.9, 1.1, 1.3,1.5,1.7,1.9,2.1,2.3,2.5,2.7,3.0};
   Double_t y[n]  = {0.0208,0.0377,0.0526,0.0664,0.0799,0.0927,0.1046,0.1141,0.1244,0.1334,0.1379,0.1420,0.1477,0.1575};
   Double_t ex[n] = {0};
   Double_t ey[n] = {.0010,.0019,.0026,.0033,.0040,.0046,.0052,.0057,.0062,.0067,.0069,.0072,.0074,.0079};
   TGraphErrors *gr = new TGraphErrors(n,x,y,ex,ey);
   gr->SetTitle("TGraphErrors Example");
   gr->SetMarkerColor(4);
   gr->SetMarkerStyle(21);
   gr->Draw("ALP");

   Double_t bin[15] = {0.2,0.4,0.6,0.8,1.0,1.2,1.4,1.6,1.8,2.0,2.2,2.4,2.6,2.8,3.1};
   TFile *f = new TFile("outFile_reso_4.root","READ");
   TProfile2D *p_v2 = (TProfile2D*)f -> Get("v2_pt_prof");
   TH1D * hist = (TH1D*) p_v2->ProfileX("prof",7,7)->Rebin(14,"hist",bin);
   hist->SetMarkerStyle(24);
   hist->Draw("same");
  std::cout << "I'm done with analysis. We'll have a Nobel Prize, Master!"
            << std::endl;

  c1->SaveAs("/home/red/Nauchka/kulyamin/StFemtoEvent/macros/article.png");
}
