/* 
   In this ROOT function we 

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

void analyze_v2_2particle();

//____________________________________________________________________________
void analyze_v2_2particle(){

	//histogram which will be filled with data from phi_dist.dat file
	TH1D* hPhi = new TH1D("hPhi", "v2_#phi distribution stored in phi_dist.dat; #phi; Counts", 
			100, 0, TMath::Pi());


	Int_t nEvents = 0; // event counter
	string helpString; // help variable
	string helpString2;// help variable 2
	// open input file
	ifstream file("phi_dist.dat");
	Int_t eventNo = -1;
	
	Double_t SumTwoParticleCF=0; //here <2> is refered as TwoParticleCF, for use with the standard method
//	Double_t TwoParticleCFEventAvg=0;
	Double_t SumTwoParticleCF_Q2=0; // for <2> using Q2 sum over events
	
	while(file >> helpString >> eventNo) {
		//cout << "Reading event : " << eventNo << endl; //uncomment to print current event number if wanted
		nEvents++;
		Int_t nTracks = -1;
		file >> helpString >> nTracks;
		//cout << "Event contains " << nTracks << " tracks" << endl; 
		//uncomment previous line to print no. of tracks for current event no. if wanted

		Double_t phi[nTracks];
		Int_t trackNo;

		for(Int_t nt = 0; nt < nTracks; nt++) {
		// Read back the phi values
			file >> helpString >> helpString2 >>phi[nt] ;
			//fill hPhi with the read phi values
			hPhi->Fill(phi[nt]);
		}
		// Here we want to analyze the phi values
		// 2-particle correlations (Sec. 2.2)
		Double_t sum_cos2_diff = 0;
		
		// Q vector (Sec. 2.3)
		Double_t sum_cos2 = 0;
		Double_t sum_sin2 = 0;
		
		for (Int_t i = 0; i < nTracks; i++) {
			// Compute Q vector , only needs 1 loop
			sum_cos2 += TMath::Cos(2*phi[i]);
			sum_sin2 += TMath::Sin(2*phi[i]);
			
			//Using the double sum method Eqn 4 Sec 2.2
			for (Int_t j = 0; j < nTracks; j++) {
				if (j!=i){
				sum_cos2_diff += TMath::Cos(2*(phi[i]-phi[j]));
				}
			}
		}
		// calculate <2> (here TwoParticleCF) using standard method for this event, Eqn4 Sec 2.2
		Double_t TwoParticleCF = sum_cos2_diff/(nTracks*(nTracks-1));
		// calculate Q2 for current event
		TComplex Q2_Event(sum_cos2, sum_sin2);
		//use this Q2 to get <2> (here TwoParticleCF_Q2)
		Double_t AbsQ2 = TComplex::Abs(Q2_Event);
		Double_t TwoParticleCF_Q2 = ((AbsQ2*AbsQ2)-nTracks)/(nTracks*(nTracks-1));
		// Update <2> event average using standard method for 2-particle and find v2
		SumTwoParticleCF += TwoParticleCF;
		Double_t TwoParticleCF_EventAvg = SumTwoParticleCF/nEvents;
		Double_t v2_2ParticleCorr = TMath::Sqrt(TwoParticleCF_EventAvg); 
		//Update <2> event average for Q-vector and find v2
		SumTwoParticleCF_Q2 += TwoParticleCF_Q2;
		Double_t TwoParticleCF_EventAvg_Q2 = SumTwoParticleCF_Q2/nEvents;
		Double_t v2_Q2 = TMath::Sqrt(TwoParticleCF_EventAvg_Q2);
		
		//compare v2 from standard method or using Q2 for event by event
		cout << "v2 (Average of " << nEvents << " events) = " << v2_2ParticleCorr <<endl;
		cout << "Q: v2 (Average of " << nEvents << " events) = " << v2_Q2 <<endl;
		cout << endl;
		
		
	}
	//Plot the Histogram for the phi datas read
	  // Set ROOT drawing styles
  gStyle->SetStatX(0.55); //positions the statistics box
  gStyle->SetStatW(0.1);
  gStyle->SetLabelOffset(1.5);
  gStyle->SetLabelFont(72);

  gStyle->SetOptStat(1111);
  gStyle->SetOptFit(1111);

  // create canvas for hPhi
  TCanvas* c1 = new TCanvas("c1", "Phi canvas", 900, 600);
  hPhi->SetMinimum(0);
  hPhi->Draw();
	
}

