// C++ headers
#include <iostream>
#include <string>
#include <cstring>


//ROOT headers
#include "TLorentzVector.h"
#include "TROOT.h"
#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TSystem.h"
#include "TH1.h"
#include "TH2.h"
#include "TMath.h"

void makeplotstyle(){
  
  TStyle *mystyle = new TStyle("PlottingInStyle", "Style for Summary Plots");
  mystyle->SetPalette(1);
  mystyle->SetCanvasColor(10);
  mystyle->SetHistFillColor(10);
  mystyle->SetHistFillStyle(0);
  mystyle->SetOptTitle(0);
  mystyle->SetOptStat(0);
  mystyle->SetCanvasBorderMode(0);//removes the yellow frame around the canvas
  mystyle->SetPadLeftMargin(0.16);
  mystyle->SetPadBottomMargin(0.15);
  mystyle->SetPadTickX(1);
  mystyle->SetPadTickY(1);
  mystyle->SetAxisColor(1, "X");
  mystyle->SetAxisColor(1, "Y");
  mystyle->SetLabelColor(1, "X");
  mystyle->SetLabelColor(1, "Y");
  mystyle->SetTickLength(0.03, "X");
  mystyle->SetTickLength(0.03, "Y");
  mystyle->SetTitleXSize(0.05);
  mystyle->SetTitleYSize(0.05);
  mystyle->SetNdivisions(505, "X");
  mystyle->SetNdivisions(508, "Y");
  mystyle->SetTitleXOffset(1.2);
  mystyle->SetTitleYOffset(1.2);
  mystyle->SetLabelOffset(0.02, "X");
  mystyle->SetLabelOffset(0.02, "Y");
  mystyle->SetLabelSize(0.05, "X");
  mystyle->SetLabelSize(0.05, "Y");
  //mystyle->SetGridx();

  TFile f("style.root", "RECREATE");
  f.cd();
  mystyle->Write();
  f.Close();
}

void one_canvas_star_mpd(){
/*
	makeplotstyle();

	TFile *fstyle = new TFile("style.root");
  	TStyle *tsty = (TStyle *)fstyle->Get("PlottingInStyle");
  	tsty->cd();
  	gStyle->SetOptStat(0);

  	gROOT->ForceStyle();
  	gStyle->SetOptStat(0);*/

	std :: cout << "Let's get started!" << std :: endl;

	TCanvas *c1 = new TCanvas("c1","c1",800,1000);

	TFile *f1 = new TFile("/home/pepe-frog/MPD/DataPetrMPD/11GeV/Mult_k_tracks_mpd.root","READ");
	TH1D *mpd = (TH1D*)f1 -> Get("h_Mult");
	mpd -> Draw();
	

	TFile *f2 = new TFile("/home/pepe-frog/MPD/DataGanFemto/11GeV/outAuAu11GeV_all.root","READ");
	TH1D *star = (TH1D*)f2 -> Get("refMult2");
	star -> Scale(1.0/10.0);
	star -> Draw("same");
	
	star -> Write();
	mpd -> Write();

	c1->SaveAs("/home/pepe-frog/MPD/macro/pic.png");

	//f1->Close();
	//f2->Close();


	std::cout << "The analysis is complete! You are amazing!"
	          << std::endl;
}