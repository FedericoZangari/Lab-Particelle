using namespace RooFit;

void langaus2(){ 
    TCanvas* c = new TCanvas();
    TH1D *hist = new TH1D("histogram", "Landau convoluta con Gaussiana", 100, -50, 200);
    for (int i = 0; i < 100000; ++i) {
        double value = 35.0 + gRandom->Landau(0, 10) + gRandom->Gaus(0, 5);
        hist->Fill(value);
    }
      
    
    int nbins = hist->GetSize()-2;

    // Get mean and standard deviation for Gauss - Landau initial parameterization                                                                                                                                                                                                         
    double mean   = hist->GetMean();
    double sigma  = hist->GetStdDev();

    // Construct observable                                                                                                                                                                                                                                                               
    double minX = hist->GetXaxis()->GetBinLowEdge(1);
    double maxX = hist->GetXaxis()->GetBinUpEdge(nbins);
    RooRealVar t("t","t", minX, maxX);
 

    // Construct gauss(t,mg,sg)                                                                                                                                                                                                                                                           
    RooRealVar mg("Gauss - mean","mg",0);
    RooRealVar sg("Gauss - sigma","sg",sigma,0.1*sigma,5.*sigma);
    RooGaussian gauss("gauss","gauss",t,mg,sg);

    // Construct landau(t,ml,sl)                                                                                                                                                                                                                                                          
    RooRealVar ml("Landau - mean","mean landau",mean,mean-sigma,mean+sigma);
    RooRealVar sl("Landau - sigma","sigma landau",0.04,0.,100);
    RooLandau landau("lx","lx",t,ml,sl);

    // C o n s t r u c t   c o n v o l u t i o n   p d f                                                                                                                                                                                                                                  
    // ---------------------------------------                                                                                                                                                                                                                                            

    // Set #bins to be used for FFT sampling                                                                                                                                                                                                                                              
    t.setBins(5000,"cache");

    // Construct landau (x) gauss                                                                                                                                                                                                                                                         
    RooFFTConvPdf lxg("lxg","landau (X) gauss",t,landau,gauss);

    // S a m p l e ,   f i t   a n d   p l o t   c o n v o l u t e d   p d f                                                                                                                                                                                                              
    // ----------------------------------------------------------------------                                                                                                                                                                                                             

    RooDataHist tofit("dh", "dh", RooArgList(t), hist) ;

    // Fit lxg to data                                                                                                                                                                                                                                                                    
    lxg.fitTo(tofit);

    // Plot data, landau pdf, landau (X) gauss pdf                                                                                                                                                                                                                                        
    RooPlot* xframe = t.frame();
    tofit.plotOn(xframe);
    lxg.plotOn(xframe);
    xframe->GetYaxis()->SetTitle("au");
    xframe->GetXaxis()->SetTitle("#DeltaE/#Deltax [MeV/mm]");
    xframe->GetXaxis()->SetRangeUser(0,200);
    xframe->GetYaxis()->SetTitleOffset(1.4);
    xframe->Draw();
    // Calculate chi2                                                                                                                                                                                                                                                                     
    double chi2 = xframe->chiSquare(3);
    cout << chi2 << endl;
    c->SaveAs("langaus2_test.png");
}