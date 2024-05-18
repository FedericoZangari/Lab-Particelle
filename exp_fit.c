#include "TH1F.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TF1.h"
#include "TStyle.h"
#include "TLine.h"
#include "TLegend.h"
#include "TPaveText.h"

#include "RooWorkspace.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooLandau.h"
#include "RooExponential.h"
#include "RooFFTConvPdf.h"
#include "RooPlot.h"
#include "TVirtualFitter.h"

#include "Math/Minimizer.h"
#include "Math/Factory.h"
#include "Math/Functor.h"
#include "TMinuit.h"
#include "Minuit2/Minuit2Minimizer.h"
#include <TVirtualFitter.h>


#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sys/stat.h> 
#include <cstdio> 
#include <cstring> 

using namespace RooFit;

// Funzione analitica convoluzione exp+gauss
double ExpoConv1Gauss(double *x, double *par){
    return ( par[1]*exp((pow(par[3],2)+2*(-x[0]+par[0])*par[2])/(2*par[2]*par[2]))*(1-erf( (par[3]*par[3]+(-x[0]+par[0])*par[2])/(pow(2,0.5)*par[3]*par[2]) )))/(2*par[2]);
// par0 = mu
// par1 = norm
// par2 = tau
// par3 = sigma
}

double ExpoSum(double *x, double *par){	      //par[0]=Norm_tot, par[7]=Q1(peso1) --> peso1+peso2=1
   double parexp1[4]={par[1], 1.0, par[2], par[3]};  //mu1,norm,tau1,sigma1
   double parexp2[4]={par[4], 1.0, par[5], par[6]};  //mu2,norm,tau2,sigma2
   return par[0]*((par[7]*ExpoConv1Gauss(x, &parexp1[0]))+((1-par[7])*ExpoConv1Gauss(x, &parexp2[0])));
}





TH1F* ReadFillAlll( const char* Filename) {
    
	ifstream f(Filename);
  TH1F *h1 = new TH1F("h1", "Distribuzione esponenziale", 10000, 1, 10000);
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

void posi2(){
    // Creazione di un istogramma con dati di esempio
    TH1F *histogram = ReadFillAlll("spettro_tempi2.dat");
    histogram->Rebin(4); 


    double Fitmin = 32;
    double FitMax= 55; 
    TF1 *myFunc = new TF1("myFunc", ExpoSum, Fitmin, FitMax, 8);
    myFunc->SetParameters(2000, 47, 0.5, 0.2, 47, 1, 0.82);  
  // TF1 *myFunc = new TF1("myFunc", "[0]*exp(-[1]*x)+[2]*exp(-[3]*x)");
     // Imposta il numero massimo di iterazioni
    TVirtualFitter::SetMaxIterations(1000000);  


   float initialValueA = 9000;
    float initialValueLambda1 = 0.1;
    myFunc->SetParameter(0, initialValueA);
    myFunc->SetParameter(1, initialValueLambda1);
   float initialValueB = 9000;
    float initialValueLambda2 = 0.1;
    myFunc->SetParameter(2, initialValueB);
    myFunc->SetParameter(3, initialValueLambda2);

    myFunc->SetParameter(4, 0);
    myFunc->SetParameter(5, 0);

    // Imposta l'intervallo del fit
    double fitRangeMin = 0;
    double fitRangeMax = 1400;
    myFunc->SetRange(fitRangeMin, fitRangeMax);

    

    // Esecuzione del fit solo nell'intervallo specificato
    TFitResultPtr fitResult = histogram->Fit(myFunc, "R", "E1");


    TCanvas *canvas = new TCanvas("canvas", "Fit Result", 1000, 600);
    histogram->GetXaxis()->SetTitle("channel");
    histogram->GetYaxis()->SetTitle("entries"); 
          TLegend *legend = new TLegend(0.7, 0.7, 0.9, 0.9);  // (x1, y1, x2, y2) coordinates for the legend

   
    histogram->Draw();
    legend->AddEntry(histogram, "Data", "PE"); 
      legend->AddEntry(myFunc, "Fit", "l"); 
      
    legend->Draw("same"); 
   myFunc->Draw("same"); 

}
