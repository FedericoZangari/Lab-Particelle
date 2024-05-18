#include "TH1F.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1F.h"
#include "TStyle.h"
#include "TLine.h"
#include "TLegend.h"
#include "TPaveText.h"

#include "RooWorkspace.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooLandau.h"
#include "RooFFTConvPdf.h"
#include "RooPlot.h"



#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sys/stat.h> 
#include <cstdio> 
#include <cstring> 

using namespace RooFit;

TH1F* ReadFillAll( const char* Filename) {
    
	ifstream f(Filename);
  TH1F *h1 = new TH1F("h1", "Spettro", 16384, 1, 16384);
  int i = 0;
	if(!f){
  		cerr <<"Error: cannot open file " <<endl;
		exit(0);
	}
  else{
      double var;
			for (;;){
					
          f >> var;
          h1->SetBinContent(i,var);		
									
					if(f.eof()){
						cout << "End of file reached "<< endl;
						break;
					}
          i++;		
			}
		}	f.close();

	return h1;
}

void langaus_giulia(){
    // Creazione di un istogramma con dati di esempio
    TH1F *histogram = ReadFillAll("dati_landau5.dat");
    histogram->Rebin(14); 


    // Creazione delle variabili RooFit
    RooRealVar x("Channel number", "Channel number", 0, 5500);
    RooDataHist data("data", "data", x, Import(*histogram));

    // Creazione dei parametri per la funzione di Landau
    RooRealVar mean("mean", "mean", 2600, 2500, 3000);
    RooRealVar sigma("sigma", "sigma", 100, 30, 200);
    RooLandau landau("landau", "landau", x, mean, sigma);

    // Creazione dei parametri per la funzione Gaussiana
   RooRealVar gauss_mean("gauss_mean", "gauss_mean", 0,-10,10);
    RooRealVar gauss_sigma("gauss_sigma", "gauss_sigma", 90, 10, 1000); //ultimo: 100
 RooGaussian gauss("gauss", "gauss", x, gauss_mean, gauss_sigma);

    // Set #bins to be used for FFT sampling                                                                                                                                                                                                                                              
      x.setBins(5500,"cache"); 
    // Creazione della PDF convoluta
    RooFFTConvPdf model("model", "model", x, landau, gauss);
   

    // Esecuzione del fit
    RooFitResult *fitResult = model.fitTo(data, RooFit::Save(true), RooFit::Range(2200, 5000), RooFit::Minimizer("Minuit2"));     

    // Creazione di un plot con i risultati del fit
    RooPlot *frame = x.frame();
   data.plotOn(frame, RooFit::MarkerSize(1), RooFit::MarkerStyle(20), RooFit::DataError(RooAbsData::SumW2), RooFit::Name("data"));
model.plotOn(frame, RooFit::LineColor(kRed), RooFit::Name("model"));



    // Creazione di un canvas e visualizzazione del risultato
    TCanvas *canvas = new TCanvas("canvas", "Fit Result", 1000, 600);
        TLegend *legend = new TLegend(0.7, 0.7, 0.9, 0.9);  // (x1, y1, x2, y2) coordinates for the legend

  //Int_t npar = model.getParameters(data)->selectByAttrib("Constant",kFALSE)-> getSize(); // select floating parameters and count their number
    
    frame->SetTitle("Spettro di energia depositata"); 
   // frame->GetYaxis()->SetTitleOffset(+0.5); 
    frame->GetYaxis()->SetTitle("#events"); 

    frame->Draw();
    legend->AddEntry(frame->findObject("data"), "Data", "PE"); 
      legend->AddEntry(frame->findObject("model"), "Fit", "l"); 
      
    legend->Draw("same"); 

double chiSquareValue = frame->chiSquare("model", "data", 4);
if (!std::isnan(chiSquareValue)) {
    cout << "CHI SQUARE: " << chiSquareValue << endl;
} else {
    cout << "CHI SQUARE is NaN. There may be an issue with the fit or model parameters." << endl;
}
cout << frame->chiSquare() << endl;
    frame->GetXaxis()->SetRange(0,5500);

     TPaveText *fitParams = new TPaveText(0.15, 0.5, 0.4, 0.85, "NDC");
    fitParams->AddText(Form("Landau mean: %.2f", mean.getVal()));
    fitParams->AddText(Form("Landau sigma: %.2f", sigma.getVal()));
   fitParams->AddText(Form("Gaussian mean: %.2f", gauss_mean.getVal()));
    fitParams->AddText(Form("Gaussian sigma: %.2f", gauss_sigma.getVal()));
   // fitParams->AddText(Form("Chi^2: %.2f", chi2));  // Aggiungi il chi-quadro

    fitParams->Draw("same");

    canvas->SaveAs("dati_landau5.png");
} 