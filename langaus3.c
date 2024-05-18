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

void langaus3(){
    // Creazione di un istogramma con dati di esempio
    TH1F *histogram = ReadFillAll("dati_landau4.dat");
    histogram->Rebin(4);

    // Creazione delle variabili RooFit
    RooRealVar x("x", "x", 2300, 5500);
    RooDataHist data("data", "data", x, Import(*histogram));

    // Creazione dei parametri per la funzione di Landau
    RooRealVar mean("mean", "mean", 2665, 0, 5000);
    RooRealVar sigma("sigma", "sigma", 68, 0, 5000);
    RooLandau landau("landau", "landau", x, mean, sigma);

    // Creazione dei parametri per la funzione Gaussiana
    RooRealVar gauss_mean("gauss_mean", "gauss_mean", 0, -0.1, 0.1);
    RooRealVar gauss_sigma("gauss_sigma", "gauss_sigma", 170, 0, 5000);
    RooGaussian gauss("gauss", "gauss", x, gauss_mean, gauss_sigma);

    // Set #bins to be used for FFT sampling                                                                                                                                                                                                                                              
    x.setBins(10000,"cache"); 
    // Creazione della PDF convoluta
    RooFFTConvPdf model("model", "model", x, landau, gauss);

    // Esecuzione del fit
    RooFitResult *fitResult = model.fitTo(data, RooFit::Save(true));

    // Creazione di un plot con i risultati del fit
    RooPlot *frame = x.frame();
    data.plotOn(frame);
    model.plotOn(frame);

    // Creazione di un canvas e visualizzazione del risultato
    TCanvas *canvas = new TCanvas("canvas", "Fit Result", 800, 600);
    frame->GetXaxis()->SetRangeUser(0,180);
    frame->Draw();
    canvas->SaveAs("dati_landau4.png");
} 
