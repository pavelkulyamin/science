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
void FemtoDstAnalyzer_QVector (const Char_t *inFile = "st_physics_12150008_raw_4030001.femtoDst.root", const Char_t *outFileName = "outFile.root") {

  std::cout << "Hi! Lets do some physics, Master!" << std::endl;
  TFile *f = new TFile(outFileName,"RECREATE");
  //TH1F

  TH1D *H_Qx2 = new TH1D ("Qx2","Q_{x}- vector(2); Q_{x},[GeV/c]", 500, -40, 40 );

  TH1D *H_Qy2 = new TH1D ("Qy2","Q_{y}- vector(2); Q_{y},[GeV/c]", 500, -40, 40 );

  TH1D *H_Q2 = new TH1D ("Q2","Q - vector; Q,[GeV/c]", 500, 0, 40);

  TProfile *H_Q2_cent_prof = new TProfile("Q2_cent_prof","Profile of cent versus Q(2)", 50, 0, 10, -100, 100);

  TProfile *H_Qx2_cent_prof = new TProfile("Qx2_cent_prof","Profile of cent versus Q_{x}(2)", 250, 0, 10, -100, 100);

  TProfile *H_Qy2_cent_prof = new TProfile("Qy2_cent_prof","Profile of cent versus Q_{y}(2)", 250, 0, 10, -100, 100);

  TH1D *H_Phy2 = new TH1D ("Phy2","Event plane(2); {Psi}", 250, -TMath :: Pi() / 2.0 - 1, TMath :: Pi() / 2.0 + 1 );

  TH1D *H_Qx3 = new TH1D ("Qx3","Q_{x}- vector(3); Q_{x},[GeV/c]", 500, -40, 40 );

  TH1D *H_Qy3 = new TH1D ("Qy3","Q_{y}- vector(3); Q_{y},[GeV/c]", 500, -40, 40 );

  TH1D *H_Q3 = new TH1D ("Q3","Q - vector; Q,[GeV/c]", 500, 0, 40);

  TProfile *H_Q3_cent_prof = new TProfile("Q3_cent_prof","Profile of cent versus Q(3)", 50, 0, 10, -100, 100);

  TProfile *H_Qx3_cent_prof = new TProfile("Qx3_cent_prof","Profile of cent versus Q_{x}(3)", 250, 0, 10, -100, 100);

  TProfile *H_Qy3_cent_prof = new TProfile("Qy3_cent_prof","Profile of cent versus Q_{y}(3)", 250, 0, 10, -100, 100);

  TH1D *H_Phy3 = new TH1D ("Phy3","Event plane(3); {Psi}", 250, -TMath :: Pi() / 3.0 - 1, TMath :: Pi() / 3.0 + 1 );

  TProfile2D *H_RunID_cent_prof_Qx2 = new TProfile2D("RunId_cent_prof_Qx2","Profile of RunId vs Centrality vs Qx2",2, 12120001.5, 12120003.5, 9, -0.5, 8.5); 

  TProfile2D *H_RunID_cent_prof_Qy2 = new TProfile2D("RunId_cent_prof_Qy2","Profile of RunId vs Centrality vs Qx2",2, 12120001.5, 12120003.5, 9, -0.5, 8.5); 

  TProfile2D *H_RunID_cent_prof_Qx3 = new TProfile2D("RunId_cent_prof_Qx3","Profile of RunId vs Centrality vs Qx2",2, 12120001.5, 12120003.5, 9, -0.5, 8.5); 

  TProfile2D *H_RunID_cent_prof_Qy3 = new TProfile2D("RunId_cent_prof_Qy3","Profile of RunId vs Centrality vs Qx2",2, 12120001.5, 12120003.5, 9, -0.5, 8.5); 

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

  Double_t Qx2 = 0, Qy2 = 0, Phy2 = 0, Q2 = 0;

  Double_t Qx3 = 0, Qy3 = 0, Phy3 = 0, Q3 = 0;

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
    
    // Track analysis
   
    Int_t nTracks = dst->numberOfTracks();

    Qx2 = 0; Qy2 = 0; Phy2 = 0; Q2 = 0;
    Qx3 = 0; Qy3 = 0; Phy3 = 0; Q3 = 0;

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
      //if( (femtoTrack -> nHits()) / (femtoTrack -> nHitsPoss()) < 0.51 ) { continue;} // Nhits / Nhitsposs > 0.51
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

    Phy2 = 1.0 /2.0 * ( TMath :: ATan2( Qy2 , Qx2 ) );

    Phy3 = 1.0 /3.0 * ( TMath :: ATan2( Qy3 , Qx3 ) );

    Q2 = TMath :: Sqrt( pow(Qx2,2) + pow(Qy2,2) );

    Q3 = TMath :: Sqrt( pow(Qx3,2) + pow(Qy3,2) );

    //Filling profile

    H_Q2_cent_prof -> Fill(event -> cent9() , Q2 );

    H_Qx2_cent_prof -> Fill( event -> cent9() , Qx2 );

    H_Qy2_cent_prof -> Fill( event -> cent9() , Qy2 );    

    H_Q3_cent_prof -> Fill( event -> cent9() , Q3 );

    H_Qx3_cent_prof -> Fill( event -> cent9() , Qy3 );

    H_Qy3_cent_prof -> Fill( event -> cent9() , Qy3 );

    H_RunID_cent_prof_Qx2 -> Fill(event -> runId() , event -> cent9() , Qx2 , 1 );
 
    H_RunID_cent_prof_Qy2 -> Fill(event -> runId() , event -> cent9() , Qy2 , 1 );
 
    H_RunID_cent_prof_Qx3 -> Fill(event -> runId() , event -> cent9() , Qx3 , 1 );

    H_RunID_cent_prof_Qy3 -> Fill(event -> runId() , event -> cent9() , Qy3 , 1 );

    //Filling histograms

    H_Q2 -> Fill(Q2);

    H_Q3 -> Fill(Q3);

    H_Qx2 -> Fill(Qx2);

    H_Qy2 -> Fill(Qy2);

    H_Phy2 -> Fill(Phy2);

    H_Qx3 -> Fill(Qx3);

    H_Qy3 -> Fill(Qy3);

    H_Phy3 -> Fill(Phy3);

  } //for(Long64_t iEvent=0; iEvent<events2read; iEvent++)
  femtoReader->Finish();
  f->Write();
  f->Close();
  std::cout << "I'm done with analysis. We'll have a Nobel Prize, Master!"
            << std::endl;
}
