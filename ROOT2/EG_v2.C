/* 
   In this ROOT function we generate a distribution according to 1+2*v_2*cos(2*phi),
   between 0 and pi

   To run do:
   root 
   .L rootgenerate_v2.C+ 
   rootfuncgenerate(100000, 10, 0.2)
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

void rootfuncgenerate(Int_t nEvents, Int_t nTracks, Double_t v2); // ROOT method (a bit dangerous since we don't know exactly what happens!)


//________________________________________________________________________
void rootfuncgenerate(Int_t nEvents, Int_t nTracks, Double_t v2) 
{
  cout << "Generating " << nEvents << " events" << endl << endl;

  // create histogram that we will fill with random values
  TH1D* hPhi = new TH1D("hPhi", "ROOT func generated v2_#phi distribution; #phi; Counts", 
			100, 0, TMath::Pi());

  // Define the function we want to generate (for filling our histogram)
  TF1* phiFunc = new TF1("phiFunc", "(1+2*[0]*cos(2*x))", 0, TMath::Pi()); 
	phiFunc->SetParameter(0,v2);

  ofstream file("phi_dist.dat");

  //For each Event, there will be n Tracks (as specified by user) with n phi angles #P.S here I mean n = nTracks

  // make a loop for the number of events
  for(Int_t n = 0; n < nEvents; n++) {
	//print Event no. in standard output
	//then write it also in the open file
    cout << "Event " << n << endl; 
	file << "Event " << n << endl; 
	
	//print no. of Tracks (to loop through for each event) in standard output
	//then write it also in file
	cout << "nTracks " << nTracks << endl;
	file << "nTracks " << nTracks << endl;
	
	Double_t phi[nTracks]; // temp array to store phi angles for n Tracks for each event
    // generate nTracks phi angles
    for (Int_t nt = 0; nt < nTracks; nt++) {
  		// Fill the array
  		phi[nt] = phiFunc->GetRandom(0, TMath::Pi()); //get random angle following the phi distribution
		//print the angle in standard output and also write to file	
	    cout << nt << " : " << phi[nt] << endl; 
	    file << nt << " : " << phi[nt] << endl;     
		}
	//this loop fills histogram with phi values
	for (Int_t i = 0; i < nTracks; i++) {
		hPhi->Fill(phi[i]);
	}

    // fill our Phi dist histogram
    //hPhi->Fill(phiFunc->GetRandom(0, TMath::Pi()); //this was for 1 event get 1 angle 
  }  
  // close file
  file.close();
  
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
  
  // create 1d function that we will use to fit our generated data to ensure
  // that the generation works
  TF1* fitFunc = new TF1("fitFunc", "([0]+2*[1]*cos(2*x))", 0, 2*TMath::Pi());
  fitFunc->SetParameters(50, 50);// guessing parameters for fit
  fitFunc->SetLineColor(kRed);
  hPhi->Fit(fitFunc);
  
  // Save the canvas as a picture
  c1->SaveAs("v2_rootfunc.jpg");


}
