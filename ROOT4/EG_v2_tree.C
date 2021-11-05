/* 
   In this ROOT function we generate a distribution according to 1+2*v_2*cos(2*phi),
   between 0 and pi and store it in a Root tree

   To run do:
   root 
   .L EG_v2_tree.C+ 
   rootfuncgenerate(100000, 1000, 300, 0.2, 0.05)
*/

// include C++ STL headers 
#include <iostream>
#include<fstream> 

using namespace std;

// ROOT library obejcts
#include <TF1.h> // 1d function class //bracket for non-local files only
#include <TH1.h> // 1d histogram classes
#include <TStyle.h>  // style object
#include <TMath.h>   // math functions
#include <TCanvas.h> // canvas object
#include <TFile.h>
#include <TTree.h> 
#include <TRandom.h> // generate random numbers
#include <TClonesArray.h>

#include "MyClasses.C" //

void rootfuncgenerate(Int_t nEvents, Int_t nTracks, Double_t sigmaTracks, Double_t v2, Double_t sigmaV2); // ROOT method (a bit dangerous since we don't know exactly what happens!)


//________________________________________________________________________
void rootfuncgenerate(Int_t nEvents, Int_t nTracks, Double_t sigmaTracks, Double_t v2, Double_t sigmaV2)
{
	// Create the outfile and data structure before the loop
TFile* file = new TFile("phi_dist.root", "RECREATE");
TTree* tree = new TTree("tree", "Output tree");
MyEvent* event = new MyEvent();
tree->Branch("event", &event);
TClonesArray* trackArray = new TClonesArray("MyTrack", 1000);
tree->Branch("track", "TClonesArray", &trackArray);
//Int_t nT = 0;

//for generating random nTracks and V2 in the loop
TRandom *RandomTracksGen = new TRandom();
Double_t Double_nTracks= nTracks;
TRandom *RandomV2Gen = new TRandom();
		
// In the generate loop: for each event set nTracks and fV2
cout << "Generating " << nEvents << " events" << endl << endl;

  // make a loop for the number of events
  for(Int_t n = 0; n < nEvents; n++) {
	//print Event no. in standard output
    cout << "Event " << n << endl; 

	//Generate random no. of tracks for every event
	Double_t Random_nTracksD = RandomTracksGen->Gaus(Double_nTracks, sigmaTracks);
	Int_t Random_nTracks = TMath::Nint(Random_nTracksD); //Convert Double nTracks to nearest integer

	//Get random v2 value using gaussian distribution with mean v2 and sigma sigmaV2  (generate for every event)
    Double_t Randomv2 = RandomV2Gen->Gaus(v2, sigmaV2);
	
	event->nTracks=Random_nTracks;
	event->fV2=Randomv2;

  // Define the function we want to generate (for filling our histogram)
    TF1* phiFunc = new TF1("phiFunc", "(1+2*[0]*cos(2*x))", 0, TMath::Pi()); 
    phiFunc->SetParameter(0,Randomv2);
    
	//print no. of Tracks (to loop through for each event) in standard output
	cout << "nTracks " << Random_nTracks << endl;
	
	// In the track loop: for each track
	for (Int_t nt = 0; nt < Random_nTracks; nt++) {
		MyTrack* track = new((*trackArray)[nt]) MyTrack();
		track->fPhi=phiFunc->GetRandom(0, TMath::Pi()); //get random angle following the phi distribution
		cout << nt << " : " << track->fPhi << endl; 
	}
	
// In the end of each event loop
	tree->Fill();
	trackArray->Clear(); // reset the array but do not delete memory
 }
// After all the generation is done
file->Write();
file->Close();

}
