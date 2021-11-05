/// \file
/// \ingroup tutorial_graphs
/// \notebook
/// Draw a simple graph.
///
/// \macro_image
/// \macro_code
///
/// \author Rene Brun
#include <TArrow.h>

void graph2() {
   TCanvas *c1 = new TCanvas("c1","A Simple Graph Example",200,10,700,500);

   c1->SetGrid();

   const Int_t n = 22;
   Double_t x[n], y[n];
   for (Int_t i=0;i<n-2;i++) {
     x[i] = i*0.1;
     y[i] = 10*sin(x[i]+0.2);
     printf(" i %i %f %f \n",i,x[i],y[i]);
   }
	x[20] = 2.5;
	y[20] = 6;
	x[21] = 3;
	y[21] = 4;
   TGraph *gr = new TGraph(n,x,y);
   gr->SetLineColor(2);
   gr->SetLineWidth(4);
   gr->SetMarkerColor(4);
   gr->SetMarkerStyle(21);
   gr->SetTitle("Example: A simple graph");
   gr->GetXaxis()->SetTitle("X title");
   gr->GetXaxis()->CenterTitle(true);
   gr->GetYaxis()->SetTitle("Y title");
   gr->GetYaxis()->CenterTitle(true);
   gr->Draw("ACP");

	//TArrow *ar1 = new TArrow(1.0,5.0,x[5],y[5], 0.2, "|>");
    //ar1->Draw();
	TArrow *ar1 = new TArrow(1.0,5.0,x[5],y[5]);
	ar1->Draw();
	TArrow *ar2 = new TArrow(1.0, 5.0, x[21], y[21]);
	ar2->SetFillColor(2);
	ar2->Draw();

   // TCanvas::Update() draws the frame, after which one can change it
   c1->Update();
   c1->GetFrame()->SetBorderSize(12);
   c1->Modified();
}
