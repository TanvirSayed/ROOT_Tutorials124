#include<TH1F.h> 
#include<TF1.h>
#include<TStyle.h>
#include<iostream>
#include<TGraph.h>
#include<TCanvas.h>
#include<TROOT.h>

Double_t hist_gen_mod(const Int_t nRandom, const Double_t sigma) //nRandom- number of random numbers generated (The higher the better), sigma- width of the Gaussian
{
  TH1D* hist = new TH1D("hist", "Histogram", 40, -3, 3); 
	hist->GetXaxis()->SetTitle("Bins");
	hist->GetYaxis()->SetTitle("Counts");
	
  
  TF1* fGaus = new TF1("fGaus", "gaus", -3, 3);
  fGaus->SetParameters(1, 0, sigma); // amplitude, mean, sigma

  for(Int_t i = 0; i < nRandom; i++) {

    hist->Fill(fGaus->GetRandom());
  }

	//By default, for each bin, the sum of weights is computed at fill time
	// If Sumw2 has been called, the error per bin is computed as the sqrt(sum of squares of weights)
	// otherwise the error is set equal to the sqrt(bin content) # as in the case here
	//statistical uncertainties for each bin, sqrt(N) , where N- no. of entries  

	Double_t binError[nRandom];
	for (Int_t i=0;i<nRandom;i++) {
		binError[i] = hist->GetBinError(i);  //storing error for bin i
//		std::cout << binError[i] << endl;	 //print error for bin i, comment this line out if printing per bin not wanted
   } 


	//Gaussian approximated well by polynomial of degree 2 (-ax^2+bx+c, -a so that the pol2 has a global max with +ve a) 
	//when sigma is comparable to half range of histogram 
	//range of our histogram = 3-(-3) = 6
	if (sigma>3) {
	hist->SetMinimum(0);}

	//creating pol2 function for x range between x=-3 and x=+3
	TF1* pol2 = new TF1("pol2", "[0]*-x*x + [1]*x + [2]", -3, 3);
	pol2->SetParameters(90, 0, 900); // guessing fit parameters such that it suitable fits the histogram 
	
	//fitting histogram with pol2 function
	//hist->Fit("pol2", "+", "", -3, 3);// use this if hist_gen_mod not run several times as it stores and prints a lot
	hist->Fit("pol2", "NOQ", "", -3, 3); //Using NOQ to reduce amounts of storing and printing since its not required in scan_fit_quality;

	//Storing the Chisquare, no. of degrees of freedom ndf, the fit parameters and their error 
	Double_t chi2 = pol2->GetChisquare();
	Double_t ndf = pol2->GetNDF();

	// uncomment lines 55-61 if interested in running hist_gen_mod function and accessing the parameters and their errors
//	Double_t a = pol2->GetParameter(0);
//	Double_t b = pol2->GetParameter(1);
//	Double_t c = pol2->GetParameter(2);

//	Double_t aError = pol2->GetParError(0);
//	Double_t bError = pol2->GetParError(1);
//	Double_t cError = pol2->GetParError(2);

//	std::cout << a << b << c << chi2 << endl;

	//print the fit probability, parameter names, values, and errors with this, 
	//display the fit parameters with the TStyle::SetOptFit() method. 
	//This parameter has four digits: mode = pcev (default = 0111) 
	//p-print probability, c-Chi-square/no.of degrees of freedom, e-errors, v-name/values of parameters
	//p-the probabilty of having certain value of chi2 for given number of degrees of freedom.
	// Where degrees of freedom is the differnce between number of points you fit and number of independent parameters in your fit.
//	gStyle->SetOptFit(1111);

//  hist->Draw();


	
	//returning reduced Chi^2
	Double_t reducedChi2 = chi2/ndf;
	return reducedChi2;
}

void scan_fit_quality(const Int_t nRandomMin, const Int_t nRandomMax ,const Double_t sigma) //nRandomMin-minimum number of random numbers generated, nRandomMax-minimum number of random numbers generated, sigma- width of the Gaussian
{
	Int_t No_nRandom = (nRandomMax - nRandomMin) / 10; //Number of nRandom to be plotted
  	Double_t nRandomList[No_nRandom];
	Double_t reducedChi2List[No_nRandom];

	//Getting the reducedChi2 for corresponding nRandom and storing them
	Int_t index=0; //to be used as index for the lists
	for (Int_t i=nRandomMin; i<nRandomMax; i+=10) {
		nRandomList[index]=i;
		reducedChi2List[index]=hist_gen_mod(i, sigma);
		delete gROOT->FindObject("hist"); //need to delete previous 'hist' to create new 'hist' 
		index++; 
	}

	//making a new pad for the plot
	TCanvas *c2 = new TCanvas("c2","Plot of reduced chi^2");
	c2->SetCanvasSize(1200, 550);

    c2->SetGrid();
 

	TGraph* plotReducedChi2 = new TGraph(No_nRandom, nRandomList, reducedChi2List); 	
	plotReducedChi2->GetXaxis()->SetTitle("nRandom");
	plotReducedChi2->GetYaxis()->SetTitle("reduced chi^2");
	plotReducedChi2->SetLineColor(6);
    //plotReducedChi2->SetLineWidth(4);
    plotReducedChi2->SetMarkerColor(4);
    plotReducedChi2->SetMarkerStyle(5);
    plotReducedChi2->SetTitle(""); //good plots have no titles (or thats what I heard in the course)
    plotReducedChi2->Draw("ACP");//"ACP" for drawing a curve through data points

}

//Q13. Around what nRandom does the fit become "bad" for sigma=3.0?
// Answer: To answer this question the macro hist_gen_mod.C was used before implementing it with scan_fit_quality.C so the hist_gen_mod.C seen here is slightly modified.
// When nRandom is above 1500, the fit looks more or less good by eye. The fit looks very appropriate (good) for nRandom above 3000. Below nRandom of 1000, the fits seems to look bad as the bin counts seems to be more or less the same for most bins and the Gaussian form is not distinct. 
