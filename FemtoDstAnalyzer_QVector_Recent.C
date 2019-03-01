/**
 * \brief Example of how to read a file (list of files) using StFemtoEvent classes
 *
 * RunFemtoDstAnalyzer.C is an example of reading FemtoDst format.
 * One can use either FemtoDst file or a list of femtoDst files (inFile.lis or
 * inFile.list) as an input, and preform physics analysis
 *
 * \author Grigory Nigmatkulov
 * \date May 29, 2018
 */

// This is needed for calling standalone classes
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
void FemtoDstAnalyzer_QVector_Recent (const Char_t *inFile = "st_physics_12150008_raw_4030001.femtoDst.root", const Char_t *outFileName = "outFile_2.root") {

  std::cout << "Hi! Lets do some physics, Master!" << std::endl;

  TFile *f2 = new TFile(outFileName,"RECREATE");

  TH1D *H_Qx2_rec = new TH1D ("Qx2_rec","Q_{x}- vector(2); Q_{x},[GeV/c]", 500, -40, 40 );

  TH1D *H_Qy2_rec = new TH1D ("Qy2_rec","Q_{y}- vector(2); Q_{y},[GeV/c]", 500, -40, 40 );

  TH1D *H_Qx3_rec = new TH1D ("Qx3_rec","Q_{x}- vector(3); Q_{x},[GeV/c]", 500, -40, 40 );

  TH1D *H_Qy3_rec = new TH1D ("Qy3_rec","Q_{y}- vector(3); Q_{y},[GeV/c]", 500, -40, 40 );

  TH1D *H_Phy2 = new TH1D ("Phy2","Event plane(2); #Psi", 250, -TMath :: Pi() / 2.0 - 1, TMath :: Pi() / 2.0 + 1 );

  TH1D *H_Phy2_rec = new TH1D ("Phy2_rec","Event plane with recentering(2); #Psi", 250, -TMath :: Pi() / 2.0 - 1, TMath :: Pi() / 2.0 + 1 );

  TH1D *H_Phy3 = new TH1D ("Phy3","Event plane(3); #Psi", 250, -TMath :: Pi() / 3.0 - 1, TMath :: Pi() / 3.0 + 1 );

  TH1D *H_Phy3_rec = new TH1D ("Phy3_rec","Event plane with recentering(3); #Psi", 250, -TMath :: Pi() / 3.0 - 1, TMath :: Pi() / 3.0 + 1 );

  TFile *f = new TFile("outFile.root","READ");

  TProfile2D *p_Qx2 = (TProfile2D*)f -> Get("RunId_cent_prof_Qx2");

  TProfile2D *p_Qy2 = (TProfile2D*)f -> Get("RunId_cent_prof_Qy2");

  TProfile2D *p_Qx3 = (TProfile2D*)f -> Get("RunId_cent_prof_Qx3");

  TProfile2D *p_Qy3 = (TProfile2D*)f -> Get("RunId_cent_prof_Qy3");  
  //TH1F

  //vectors


  #if ROOT_VERSION_CODE < ROOT_VERSION(6,0,0)
  gSystem->Load("../libStFemtoDst.so");
  #endif

  StFemtoDstReader* femtoReader = new StFemtoDstReader(inFile);
  femtoReader->Init();

  // This is a way if you want to spead up IO
  std::cout << "Explicit read status for some branches" << std::endl;
  femtoReader->SetStatus("*",0);
  femtoReader->SetStatus("Event",1);
  femtoReader->SetStatus("Track",1);
  std::cout << "Status has been set" << std::endl;

  std::cout << "Now I know what to read, Master!" << std::endl;

  if( !femtoReader->chain() ) {
    std::cout << "No chain has been found." << std::endl;
  }
  Long64_t eventsInTree = femtoReader->tree()->GetEntries();
  std::cout << "eventsInTree: "  << eventsInTree << std::endl;
  Long64_t events2read = femtoReader->chain()->GetEntries();
  
  std::cout << "Number of events to read: " << events2read << std::endl;

  Double_t Qx2 = 0, Qy2 = 0, Phy2 = 0, Qx2_rec = 0, Qy2_rec = 0, Phy2Rec = 0;

  Double_t Qx3 = 0, Qy3 = 0, Phy3 = 0, Qx3_rec = 0, Qy3_rec = 0, Phy3Rec = 0;

  // Loop over events
  for(Long64_t iEvent=0; iEvent<events2read; iEvent++) {

    // std::cout << "Working on event #[" << (iEvent+1)
   //          << "/" << events2read << "]" << std::endl;

    Bool_t readEvent = femtoReader->readFemtoEvent(iEvent);
    if( !readEvent ) {
      std::cout << "Something went wrong, Master! Nothing to analyze..." << std::endl;
      break;
    }

    // Retrieve femtoDst
    StFemtoDst *dst = femtoReader->femtoDst();

    // Retrieve event information
    StFemtoEvent *event = dst->event();
    if( !event ) {
      std::cout << "Something went wrong, Master! Event is hiding from me..." << std::endl;
      break;
    }

    // Return primary vertex position
    TVector3 pVtx = event->primaryVertex();

    // Reject vertices that are far from the central membrane along the beam


    // Reject vertices that are far from the central membrane along the beam
    if( TMath::Abs( pVtx.Z() ) > 70. ) continue; // |VertexZ| < 70

    if (TMath::Sqrt( (pow(pVtx.X(),2) + pow(pVtx.Y(),2)) ) > 2. ) continue; // Sqrt( VertexX^2 + VertexY^2 ) < 2
    //Filling event Histograms
    
    // Track analysis
   
    Int_t nTracks = dst->numberOfTracks();

    Qx2 = 0, Qy2 = 0, Phy2 = 0, Qx2_rec = 0, Qy2_rec = 0, Phy2Rec = 0;
    Qx3 = 0, Qy3 = 0, Phy3 = 0, Qx3_rec = 0, Qy3_rec = 0, Phy3Rec = 0;

    // Track loop
    for(Int_t iTrk=0; iTrk<nTracks; iTrk++) {
      
      // Retrieve i-th femto track
      StFemtoTrack *femtoTrack = dst->track(iTrk);

      if (!femtoTrack) continue;

      // Must be a primary track
      if ( !femtoTrack->isPrimary() ) continue;

      if ( ( femtoTrack -> dEdx() ) == 0 ) continue;
      
      // Simple single-track cut
      if( TMath::Abs( femtoTrack -> eta() ) > 1 ) { continue;} // |eta|<1
      if( femtoTrack -> nHits() < 15 ) { continue;} // Nhits > 15
      if( femtoTrack -> p() > 10 ) { continue;} // p < 10
      if( femtoTrack -> pt() < 0.2 ) { continue;} // pt > 0.2
      if( femtoTrack -> nHitsPoss() < 0 ) { continue;} // Nhitsposs > 0
      //if( (femtoTrack -> nHits())/(femtoTrack -> nHitsPoss()) < 0.51 ) { continue;} // Nhits / Nhitsposs > 0.51
      if( femtoTrack->gMom().Mag() < 0.1 || femtoTrack->gDCA(pVtx).Mag() > 3. ) { continue;} // |DCA| < 3


          // Qx and Qy

          Qx2 += femtoTrack -> pt() * cos( 2 * femtoTrack -> phi() );

          Qy2 += femtoTrack -> pt() * sin( 2 * femtoTrack -> phi() );

          Qx3 += femtoTrack -> pt() * cos( 3 * femtoTrack -> phi() );
          
          Qy3 += femtoTrack -> pt() * sin( 3 * femtoTrack -> phi() );
          

          //Track histograms
          // Check if track has TOF signal
             if ( femtoTrack->isTofTrack() ) {     
        // с бэтой\масса 
        // cut for track for Minv
      } //if( isTofTrack() )
    } //for(Int_t iTrk=0; iTrk<nTracks; iTrk++)

    //Event plane and Q
    Qx2_rec = Qx2 - p_Qx2 -> GetBinContent(p_Qx2 -> FindBin(event -> runId(),event -> cent9()));

    Qy2_rec = Qy2 - p_Qy2 -> GetBinContent(p_Qy2 -> FindBin(event -> runId(),event -> cent9()));

    Qx3_rec = Qx3 - p_Qx3 -> GetBinContent(p_Qx3 -> FindBin(event -> runId(),event -> cent9()));

    Qy3_rec = Qy3 - p_Qy3 -> GetBinContent(p_Qy3 -> FindBin(event -> runId(),event -> cent9()));

    Phy2Rec = 1.0/2.0 * ( TMath :: ATan2 ( Qy2_rec , Qx2_rec ) );

    Phy3Rec = 1.0/3.0 * ( TMath :: ATan2 ( Qy3_rec , Qx3_rec ) );

    Phy2 = 1.0 /2.0 * ( TMath :: ATan2( Qy2 , Qx2 ) );

    Phy3 = 1.0 /3.0 * ( TMath :: ATan2( Qy3 , Qx3 ) );
   
    //Filling profile
    
    //Filling histograms

    H_Qx2_rec -> Fill(Qx2_rec);

    H_Qy2_rec -> Fill(Qy2_rec);

    H_Qx3_rec -> Fill(Qx3_rec);

    H_Qy3_rec -> Fill(Qy3_rec);

    H_Phy2 -> Fill(Phy2);

    H_Phy2_rec -> Fill(Phy2Rec);

    H_Phy3 -> Fill(Phy3);

    H_Phy3_rec -> Fill(Phy3Rec);

  } //for(Long64_t iEvent=0; iEvent<events2read; iEvent++)
  femtoReader->Finish();
  f->Close();
  f2->Write();
  f2->Close();

  std::cout << "I'm done with analysis. We'll have a Nobel Prize, Master!"
            << std::endl;
}
