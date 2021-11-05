#include<TH1F.h> 
#include<TF1.h>
void hist_gen()
{
  TH1D* hist = new TH1D("hist", "Histogram", 40, -3, 3);
  
  TF1* fGaus = new TF1("fGaus", "gaus", -3, 3);
  fGaus->SetParameters(1, 0, 1); // amplitude, mean, sigma

  for(Int_t i = 0; i < 10000; i++) {

    hist->Fill(fGaus->GetRandom());
  }

  hist->Draw();
}
