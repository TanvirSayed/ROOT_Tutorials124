/* 
  To run do:
   root 
   .L Analyze.C+ 
   analyze_v2_2particle()
*/

// include C++ STL headers 
#include <iostream>
#include<fstream> 

using namespace std;

// ROOT library obejcts
#include <TF1.h> // 1d function class
#include <TH1.h> // 1d histogram classes
#include <TStyle.h>  // style object
#include <TMath.h>   // math functions
#include <TCanvas.h> // canvas object
#include <TComplex.h>// complex numbers
#include <TFile.h>  
#include <TTree.h>  
#include <TClonesArray.h>  
#include "MyClasses.C"

void analyze_v2_2particle();

//____________________________________________________________________________
void analyze_v2_2particle(){

	// Open the file and get the tree
	TFile* file = TFile::Open("phi_dist.root");
	TTree* tree = (TTree*)file->Get("tree");
	MyEvent* event = 0;
	TClonesArray* trackArray = 0;
	tree->SetBranchAddress("event", &event);
	tree->SetBranchAddress("track", &trackArray);
	
	Int_t nEvents = tree->GetEntries();
	Double_t SumTwoParticleCF_Q2=0; // for <2> using Q2 sum over events
	
	for(Int_t eventNo = 0; eventNo < nEvents; eventNo++) {

		// Read in event n (fills event and trackArray)
		tree->GetEntry(eventNo);
		const Int_t nTracks = trackArray->GetEntries();
		
		// Q vector (Sec. 2.3)
		Double_t sum_cos2 = 0;
		Double_t sum_sin2 = 0;
			
		for(Int_t i = 0; i < nTracks; i++) {
			MyTrack* track = (MyTrack*)trackArray->At(i);
		// the analysis
			
			Double_t phi_i = track->fPhi;
			// To Compute Q vector , only 1 loop
			sum_cos2 += TMath::Cos(2*phi_i);
			sum_sin2 += TMath::Sin(2*phi_i);
		}
		// calculate Q2 for current event
		TComplex Q2_Event(sum_cos2, sum_sin2);
		//use this Q2 to get <2> (here TwoParticleCF_Q2)
		Double_t AbsQ2 = TComplex::Abs(Q2_Event);
		Double_t TwoParticleCF_Q2 = ((AbsQ2*AbsQ2)-nTracks)/(nTracks*(nTracks-1));
		//Update <2> event average for Q-vector and find v2
		SumTwoParticleCF_Q2 += TwoParticleCF_Q2;
		Double_t TwoParticleCF_EventAvg_Q2 = SumTwoParticleCF_Q2/nEvents;
		Double_t v2_Q2 = TMath::Sqrt(TwoParticleCF_EventAvg_Q2);
		cout << "Q: v2 (Average of " << eventNo+1 << " events) = " << v2_Q2 <<endl;
		//cout << endl;
	}	
}
