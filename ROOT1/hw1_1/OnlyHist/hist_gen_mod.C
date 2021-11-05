#include<TH1F.h> 
#include<TF1.h>
#include<TStyle.h>
#include<iostream>

void hist_gen_mod(const Int_t nRandom, const Double_t sigma) //nRandom- number of random numbers generated (The higher the better), sigma- width of the Gaussian
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
	hist->Fit("pol2", "+", "", -3, 3);

	//Storing the Chisquare, the fit parameters and their error 
	Double_t chi2 = pol2->GetChisquare();

	Double_t a = pol2->GetParameter(0);
	Double_t b = pol2->GetParameter(1);
	Double_t c = pol2->GetParameter(2);

	Double_t aError = pol2->GetParError(0);
	Double_t bError = pol2->GetParError(1);
	Double_t cError = pol2->GetParError(2);

//	std::cout << a << b << c << chi2 << endl;

	//print the fit probability, parameter names, values, and errors with this, 
	//display the fit parameters with the TStyle::SetOptFit() method. 
	//This parameter has four digits: mode = pcev (default = 0111) 
	//p-print probability, c-Chi-square/no.of degrees of freedom, e-errors, v-name/values of parameters
	//p-the probabilty of having certain value of chi2 for given number of degrees of freedom.
	// Where degrees of freedom is the differnce between number of points you fit and number of independent parameters in your fit.
	gStyle->SetOptFit(1111);

  hist->Draw();
}

//Q13. Around what nRandom does the fit become "bad" for sigma=3.0?
// Answer: When nRandom is above 1500, the fit looks more or less good by eye. The fit looks very appropriate nRandom above 3000. Below nRandom of 1000, the fits seems to look bad as the bin counts seems to be more or less the same for most bins and the Gaussian form is not distinct. 
