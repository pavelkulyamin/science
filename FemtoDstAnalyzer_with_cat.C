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
void FemtoDstAnalyzer_with_cat (const Char_t *inFile = "st_physics_12150008_raw_4030001.femtoDst.root", const Char_t *outFileName = "outFile.root") {

  std::cout << "Hi! Lets do some physics, Master!" << std::endl;
  TFile *f = new TFile(outFileName,"RECREATE");
  //TH1F
  //TH1F *H_MinvPi = new TH1F ("h_MinvPi", "invariant mass pion^{+}pion^{-}; M_{inv} [GeV]; N", 500, 0.25, 1.75);
	//TH1F *H_MinvKa = new TH1F ("h_MinvKa", "invariant mass K^{+}K^{-}; M_{inv} [GeV]; N", 500, 0.98, 1.98);
	//TH1F *H_MinvP_K = new TH1F ("h_Minv_p_K","invariant mass p K^{-}; M_{inv} [GeV]; N",500,1.4,3);
  //TH1F *H_Minv_Pion = new TH1F ("h_MinvPi", "invariant mass pion^{+}_{-}; M_{inv} [GeV]; N", 300, 0.25, 1.75);
  TH1D *H_massSqr = new TH1D ("massSqr","Squared mass; m^{2},[(GeV/c)^2]", 120, 0, 1.2 );
  TH1D *H_invBeta = new TH1D ("invBeta", "1/beta", 420, -0.2, 4);
  TH1D *H_refMult = new TH1D ("refMult", "Multiplicity; N_{ch}; N_{count}",600, 0, 600);
  TH1D *H_cent16 = new TH1D ("cent16", "Centrality; Centrality bin; N_{count}", 16, 0, 30);
  TH1D *H_nHits = new TH1D ("nHits", "N_hits distribution ; N_{hits}", 70, 0, 70);
  TH1D *H_VtxZ = new TH1D("VtxZ","Vertex Z distribution; VtxZ,[cm]; N_{count}" ,500, -100, 100);
  TH1D *H_dEdx = new TH1D("dEdx","Energy loss; dE/dx,[a.u.]",500, 0, 0.00007);
  TH1D *H_Pt = new TH1D("Pt","Transverse momentum distribution; Pt,[GeV/c]", 1000 , 0, 6);
  TH1D *H_Px = new TH1D("Px","Px distribution; Px,[GeV/c]", 1000, 0, 6);
  TH1D *H_Py = new TH1D("Py","Py distribution; Py,[GeV/c]", 1000, 0,6);
  TH1D *H_Phi = new TH1D("Phi","Phi distribution; φ [rad]", 1000, -3.5, 3.5);
  TH1D *H_Eta = new TH1D("Eta","Eta distribution; η", 1000, -2, 2);
  TH1D *H_VpdVz = new TH1D("VpdVz","Vertex distribution: VpdVz,[cm];N_{count}", 500, -100, 100);
  //TH2D
  TH2D *H_nSigmaPion = new TH2D("h_nsigmaPion","N_{sigma} Pion vs Q*P; Q*P [GeV/c]; N_{sigma}", 400,-4,4,400, -10, 10);
  TH2D *H_nSigmaKaon = new TH2D("h_nsigmaKaon","N_{sigma} Kaon vs Q*P; Q*P [GeV/c]; N_{sigma}", 400,-4,4,400, -10, 10);
  TH2D *H_nSigmaProton = new TH2D("h_nsigmaProton","N_{sigma} Proton vs Q*P; Q*P [GeV/c]; N_{sigma}", 400,-4,4,400, -10, 10);
  TH2D *H_PtEta = new TH2D("PtEta","P_{t} vs Eta; P_{t},[GeV/c]; η", 1000, 0,6,   1000,-2,2);
  TH2D *H_PtPhi = new TH2D("PtPhi","P_{t} vs Phi; P_{t},[GeV/c] ; φ[rad]", 1000,0,6,  1000,-3.2,3.2);
  TH2D *H_EtaPhi = new TH2D("EtaPhi","Eta vs Phi; η; φ[rad]", 1000,-3.2,3.2,  1000,-2,2);
  TH2D *H_dEdxQP = new TH2D("dEdxQP","dE/dx vs q*p; Q*P,[GeV/c]; dE/dx,[a.u.]",1000,-1.5,1.5,1000,0.0,14.0);
  TH2D *H_m2QP = new TH2D("m2qp","m^{2} vs q*p; Q*P,[GeV/c]; m^{2},[(GeV/c)^{2}]", 400,-4,4,400,-1,1.5);
  TH2D *H_invBetaQP = new TH2D("invBetaqp","1/beta vs q*p",1000,-6,6,1000,0.5,4);
  //vectors
	std::vector<int> trackPionPos, trackPionNeg, trackKaonPos, trackKaonNeg, trackProtonPos, trackProtonNeg;
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

  // Loop over events
  for(Long64_t iEvent=0; iEvent<events2read; iEvent++) {

  	// std::cout << "Working on event #[" << (iEvent+1)
   //  	       << "/" << events2read << "]" << std::endl;

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
    if( event -> vpdVz() != 0.0){

       H_VpdVz->Fill(event -> vpdVz());
    }

    H_refMult->Fill(event->refMult());
    H_cent16->Fill(event->cent16());
    H_VtxZ->Fill(pVtx.Z());
   

    // Track loop
    for(Int_t iTrk=0; iTrk<nTracks; iTrk++) {
      
      // Retrieve i-th femto track
      StFemtoTrack *femtoTrack = dst->track(iTrk);

      if (!femtoTrack) continue;

      // Must be a primary track
      if ( !femtoTrack->isPrimary() ) continue;

      //if ( ( femtoTrack -> dEdx() ) == 0 ) continue;
      
      // Simple single-track cut

      if( TMath::Abs( femtoTrack -> eta() ) > 1 ) { continue;} // |eta|<1
      if( femtoTrack -> nHits() < 15 ) { continue;} // Nhits > 15
      if( femtoTrack -> p() > 10 ) { continue;} // p < 10
      if( femtoTrack -> pt() < 0.2 ) { continue;} // pt > 0.2
      if( femtoTrack -> nHitsPoss() < 0 ) { continue;} // Nhitsposs > 0
      //if( (femtoTrack -> nHits())/(femtoTrack -> nHitsPoss()) < 0.51 ) { continue;} // Nhits / Nhitsposs > 0.51
      if( femtoTrack->gMom().Mag() < 0.1 || femtoTrack->gDCA(pVtx).Mag() > 3. ) { continue;} // |DCA| < 3


      //Track histograms
    			
      			H_dEdx->Fill(femtoTrack->dEdx());
		      	H_Pt->Fill(femtoTrack->pt());
		      	H_Px->Fill((femtoTrack->pt())*cos(femtoTrack->phi()));
		      	H_Py->Fill((femtoTrack->pt())*sin(femtoTrack->phi()));
		      	H_Phi->Fill(femtoTrack->phi());
		      	H_Eta->Fill(femtoTrack->eta());
		      	H_PtEta->Fill(femtoTrack->pt(),femtoTrack->eta());
		      	H_PtPhi->Fill(femtoTrack->pt(),femtoTrack->phi());
		      	H_EtaPhi->Fill(femtoTrack->phi(),femtoTrack->eta());
		      	H_nHits->Fill(femtoTrack->nHits());


		      	H_nSigmaPion->Fill(femtoTrack->p()*femtoTrack->charge(),femtoTrack->nSigmaPion());
		      	H_nSigmaKaon->Fill(femtoTrack->p()*femtoTrack->charge(),femtoTrack->nSigmaKaon());
		      	H_nSigmaProton->Fill(femtoTrack->p()*femtoTrack->charge(),femtoTrack->nSigmaProton());
		      	H_dEdxQP->Fill(femtoTrack->p()*femtoTrack->charge(),femtoTrack->dEdx()*1e6);

		      	// Check if track has TOF signal
		      	 if ( femtoTrack->isTofTrack() ) {
		      	  H_massSqr->Fill(femtoTrack->massSqr());
		      	  H_invBeta->Fill(femtoTrack->invBeta());
		      	 	H_m2QP->Fill(femtoTrack->p()*femtoTrack->charge(),femtoTrack->massSqr());
		      	 	H_invBetaQP->Fill(femtoTrack->p()*femtoTrack->charge(),femtoTrack->invBeta()); 
			// с бэтой\масса 
		    // cut for track for Minv
/*      if( TMath::Abs( femtoTrack -> massSqr() - 0.01948006 ) < 2*5.82843e-02 && femtoTrack -> charge() > 0 &&
           TMath::Abs( femtoTrack -> p() ) < 1.2 ) {
        trackPionPos.push_back(iTrk);
      }
      if( TMath::Abs( femtoTrack -> massSqr() - 0.01948006 ) < 2*5.82843e-02 && femtoTrack -> charge() < 0 &&
           TMath::Abs( femtoTrack -> p() ) < 1.2 ) {
        trackPionNeg.push_back(iTrk);
      }
      if( TMath::Abs( femtoTrack -> massSqr() - 0.24371698 ) < 2*6.53498e-02  && femtoTrack -> charge() > 0 &&
          TMath::Abs( femtoTrack -> p() ) < 1.2 ) {
        trackKaonPos.push_back(iTrk);
      }
      if( TMath::Abs( femtoTrack -> massSqr() - 0.24371698 ) < 2*6.53498e-02  && femtoTrack -> charge() < 0 &&
           TMath::Abs( femtoTrack -> p() ) < 1.2 ) {
        trackKaonNeg.push_back(iTrk);
      }
      if( TMath::Abs( femtoTrack -> massSqr() - 0.8803545  ) < 2*8.18035e-02 &&
       TMath::Abs( femtoTrack -> p() ) < 2.0) {
        trackProtonPos.push_back(iTrk);
      }
*/
/*			if( (TMath::Abs( femtoTrack -> massSqr() - 0.01948006 ) < 2*7.05234e-02 &&  TMath::Abs( femtoTrack -> nSigmaPion()  < 2) &&
            femtoTrack -> charge() > 0 && femtoTrack -> p() > 0.15 && femtoTrack -> p() < 0.6 ) || 
        ( TMath::Abs( femtoTrack -> massSqr() - 0.01948006 ) < 2*7.05234e-02 && femtoTrack -> charge() > 0 && TMath::Abs( femtoTrack -> p() ) < 1.8 ) ){
				trackPionPos.push_back(iTrk);
      }
			if( (TMath::Abs( femtoTrack -> massSqr() - 0.01948006 ) < 2*7.05234e-02 && TMath::Abs( femtoTrack -> nSigmaPion() ) < 2 && 
            femtoTrack -> charge() < 0 && femtoTrack -> p() > 0.15 && femtoTrack -> p() < 0.6 ) || 
        (TMath::Abs( femtoTrack -> massSqr() - 0.01948006 ) < 2*7.05234e-02 && femtoTrack -> charge() < 0 && TMath::Abs( femtoTrack -> p() ) < 1.8 )) {
				trackPionNeg.push_back(iTrk);
      }
			if( (TMath::Abs( femtoTrack -> massSqr() - 0.24371698 ) < 2*5.97667e-02  && TMath::Abs( femtoTrack -> nSigmaKaon() ) < 2 && 
          femtoTrack -> charge() > 0 && femtoTrack -> p() > 0.15 && femtoTrack -> p() < 0.5) || 
        (TMath::Abs( femtoTrack -> massSqr() - 0.24371698 ) < 2*5.97667e-02  && femtoTrack -> charge() > 0 && TMath::Abs( femtoTrack -> p() ) < 1.5 ) ) {
				trackKaonPos.push_back(iTrk);
      }
			if( (TMath::Abs( femtoTrack -> massSqr() - 0.24371698 ) < 2*5.97667e-02  && TMath::Abs( femtoTrack -> nSigmaKaon() ) < 2 &&
         femtoTrack -> charge() < 0 && femtoTrack -> p() > 0.15 && femtoTrack -> p() < 0.5) || 
        (TMath::Abs( femtoTrack -> massSqr() - 0.24371698 ) < 2*5.97667e-02  && femtoTrack -> charge() < 0 && TMath::Abs( femtoTrack -> p() ) < 1.5 ) ) {
				trackKaonNeg.push_back(iTrk);
      }
			if(  ( ( TMath::Abs( femtoTrack -> massSqr() - 0.8803545  ) < 2*1.07257e-01 && TMath::Abs( femtoTrack -> nSigmaProton() ) < 2 &&
         femtoTrack -> p() > 0.15 && femtoTrack -> p() < 0.9  ) || 
        (TMath::Abs( femtoTrack -> massSqr() - 0.8803545  ) < 2*1.07257e-01 && TMath::Abs( femtoTrack -> p() ) < 2.0)  ) &&
        femtoTrack -> charge() > 0 ) {
				trackProtonPos.push_back(iTrk);
      }
*/
      } //if( isTofTrack() )

    } //for(Int_t iTrk=0; iTrk<nTracks; iTrk++)
   




/*    //pion+-
		for ( Int_t iTrk = 0; iTrk < trackPionPos.size(); iTrk++) {
			StFemtoTrack *femtoTrack = dst -> track( trackPionPos.at(iTrk) );
			TLorentzVector v1( femtoTrack -> pMom().x(), femtoTrack -> pMom().y(), femtoTrack -> pMom().z(),sqrt( pow( ( femtoTrack -> p() ), 2) + pow( 0.139571, 2) ) );
			
			for ( Int_t jTrk = 0; jTrk < trackPionNeg.size(); jTrk++ ) {
				StFemtoTrack *femtoTrack = dst -> track( trackPionNeg.at(jTrk) );
				TLorentzVector v2( femtoTrack -> pMom().x(), femtoTrack -> pMom().y(), femtoTrack -> pMom().z(),sqrt( pow( ( femtoTrack -> p() ), 2) + pow( 0.139571, 2) ) );
				float Minv = (v1 + v2).M();
				H_MinvPi -> Fill( Minv );
				
			}
		}
			
		//Kaon+-
		for ( Int_t iTrk = 0; iTrk < trackKaonPos.size(); iTrk++) {
			StFemtoTrack *femtoTrack = dst -> track( trackKaonPos.at(iTrk) );
			TLorentzVector v1( femtoTrack -> pMom().x(), femtoTrack -> pMom().y(), femtoTrack -> pMom().z(),sqrt( pow( ( femtoTrack -> p() ), 2) + pow( 0.493677, 2) ) );
			for ( Int_t jTrk = 0; jTrk < trackKaonNeg.size(); jTrk++ ) {
				StFemtoTrack *femtoTrack = dst -> track( trackKaonNeg.at(jTrk) );
				TLorentzVector v2( femtoTrack -> pMom().x(), femtoTrack -> pMom().y(), femtoTrack -> pMom().z(),sqrt( pow( ( femtoTrack -> p() ), 2) + pow( 0.493677, 2) ) );
				float Minv = (v1 + v2).M();
				H_MinvKa -> Fill( Minv );
			}
		}

		for ( Int_t iTrk = 0; iTrk < trackProtonPos.size(); iTrk++) {
			StFemtoTrack *femtoTrack = dst -> track( trackProtonPos.at(iTrk) );
			TLorentzVector v1( femtoTrack -> pMom().x(), femtoTrack -> pMom().y(), femtoTrack -> pMom().z(),sqrt( pow( ( femtoTrack -> p() ), 2) + pow( 0.93827208, 2) ) );
			
			for ( Int_t jTrk = 0; jTrk < trackKaonNeg.size(); jTrk++ ) {
				StFemtoTrack *femtoTrack = dst -> track( trackKaonNeg.at(jTrk) );
				TLorentzVector v2( femtoTrack -> pMom().x(), femtoTrack -> pMom().y(), femtoTrack -> pMom().z(),sqrt( pow( ( femtoTrack -> p() ), 2) + pow( 0.493677, 2) ) );
				float Minv = (v1 + v2).M();
				H_MinvP_K -> Fill( Minv );
			}
		}
		
		// clear of vector
		trackPionPos.clear();
		trackPionNeg.clear();
		trackKaonPos.clear();
 		trackKaonNeg.clear(); 
		trackProtonPos.clear();
 		trackProtonNeg.clear();
*/

  } //for(Long64_t iEvent=0; iEvent<events2read; iEvent++)
  femtoReader->Finish();
  f->Write();
  f->Close();
  std::cout << "I'm done with analysis. We'll have a Nobel Prize, Master!"
	          << std::endl;
}
