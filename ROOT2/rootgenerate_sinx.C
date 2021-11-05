/* 
   In this ROOT function we generate a distribution according to sin(x)
   between 0 and pi

   To run do:
   root 
   .L rootgenerate_sinx.C+ 
   rootfuncgenerate(10000)
*/

// include C++ STL headers 
#include <iostream>

using namespace std;

// ROOT library obejcts
#include <TF1.h> // 1d function class
#include <TH1.h> // 1d histogram classes
#include <TStyle.h>  // style object
#include <TMath.h>   // math functions
#include <TCanvas.h> // canvas object

void rootfuncgenerate(Int_t nEvents); // ROOT method (a bit dangerous since we don't know exactly what happens!)


//________________________________________________________________________
void rootfuncgenerate(Int_t nEvents) 
{
  cout << "Generating " << nEvents << " events" << endl << endl;

  // create histogram that we will fill with random values
  TH1D* hSin = new TH1D("hSin", "ROOT func generated sin(x) distribution; x; Counts", 
			100, 0, TMath::Pi());

  // Define the function we want to generate
  TF1* sinFunc = new TF1("sinFunc", "sin(x)", 0, TMath::Pi());
  
  // make a loop for the number of events
  for(Int_t n = 0; n < nEvents; n++) {
    
    if((n+1)%1000==0)
      cout << "event " << n+1 << endl;
    
    // fill our sin dist histogram
    hSin->Fill(sinFunc->GetRandom()); 
  }
  
  // Set ROOT drawing styles
  gStyle->SetOptStat(1111);
  gStyle->SetOptFit(1111);

  // create canvas for hSin
  TCanvas* c1 = new TCanvas("c1", "sin canvas", 900, 600);
  hSin->SetMinimum(0);
  hSin->Draw();
  
  // create 1d function that we will use to fit our generated data to ensure
  // that the generation works
  TF1* fitFunc = new TF1("fitFunc", "[0]*sin(x)", 0, TMath::Pi());
  fitFunc->SetParameter(0, 10);
  fitFunc->SetLineColor(kRed);
  hSin->Fit(fitFunc);
  
  // Save the canvas as a picture
  c1->SaveAs("sinx_rootfunc.jpg");
}
