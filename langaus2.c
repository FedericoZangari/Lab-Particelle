using namespace RooFit;

void langaus2(){ 
    TCanvas* c = new TCanvas();
    int data[100] = {0,0,0,0,0,0,2,6,11,18,18,55,90,141,255,323,454,563,681,
                    737,821,796,832,720,637,558,519,460,357,291,279,241,212,
                    153,164,139,106,95,91,76,80,80,59,58,51,30,49,23,35,28,23,
                    22,27,27,24,20,16,17,14,20,12,12,13,10,17,7,6,12,6,12,4,
                    9,9,10,3,4,5,2,4,1,5,5,1,7,1,6,3,3,3,4,5,4,4,2,2,7,2,4};
    TH1F *hist = new TH1F("hist","langaus fit",400,0,400);
    for (int i=0; i<100; i++) hist->Fill(i,data[i]);   
    
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
    RooRealVar sl("Landau - sigma","sigma landau",0.04,0.,0.2);
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
    xframe->GetXaxis()->SetRangeUser(0,70);
    xframe->GetYaxis()->SetTitleOffset(1.4);
    xframe->Draw();
    // Calculate chi2                                                                                                                                                                                                                                                                     
    double chi2 = xframe->chiSquare(3);
    cout << chi2 << endl;
    c->SaveAs("langaus2_test.png");
}