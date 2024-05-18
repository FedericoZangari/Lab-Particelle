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
          h1->SetBinContent(i-7320,var);		
									
					if(f.eof()){
						cout << "End of file reached "<< endl;
						break;
					}
          i++;		
			}
		}	f.close();

	return h1;
}





void exp_fit2() {
    TCanvas *c1 =new TCanvas("c1","c1", 900, 550);

    TH1F *histogram = ReadFillAll("spettro_tempi.dat");
    histogram->Rebin(4);
    


    RooRealVar x("x", "x", 0, 1900);
    RooDataHist data("data", "data", x, Import(*histogram));
    SumW2Error(true);

    RooRealVar lambda1("lambda1", "lambda1", -0.000573, -1., 0.);
    RooExponential exp1("exp1", "exp1", x, lambda1);

    RooRealVar lambda2("lambda2", "lambda2", -0.0005158, -1., 0.);
    RooExponential exp2("exp2", "exp2", x, lambda2);
    
    
    RooRealVar frac("frac", "fraction of component 1 in signal", 0.77, 0., 1.);
    RooAddPdf sum("model","2 exp", RooArgList(exp1,exp2), frac);

    RooRealVar gauss_mean("gauss_mean", "gauss_mean", 0, -10, 500);
    RooRealVar gauss_sigma("gauss_sigma", "gauss_sigma", 170, 0, 5000);
    RooGaussian gauss("gauss", "gauss", x, gauss_mean, gauss_sigma);

    RooFFTConvPdf model("model", "model", x, sum, gauss);
    //model.fitTo(h);
    x.setBins(2000,"cache");
    //model.chi2FitTo(data);
    RooFitResult *fitResult = model.fitTo(data, RooFit::Save(true));
    //gauss1.fitTo(h);
    RooPlot *frame = x.frame();
    data.plotOn(frame, Name("data"));
    //exp1.plotOn(frame, LineColor(kRed), Name("exp1"));
    //exp2.plotOn(frame, LineColor(kBlue), Name("exp2"));
    model.plotOn(frame, LineColor(kGreen), Name("model"));
    //RooHist* hresid = frame->residHist() ;
    //RooChi2MCSModule chi();
    //frame->addPlotable(hresid,"P") ;
    //frame2->addPlotable(hresid,"P") ;
    //gauss1.plotOn(frame);
    frame->GetXaxis()->SetTitle("channel");
    frame->GetYaxis()->SetTitle("counts");
    frame->Draw();

    double chiSquareValue = frame->chiSquare("model", "data", 5);
    cout << "CHI SQUARE: " << chiSquareValue << endl;
 
}


