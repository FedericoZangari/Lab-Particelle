using namespace RooFit;

void langaus3(){
    // Creazione di un istogramma con dati di esempio
    TH1D *histogram = new TH1D("histogram", "Landau convoluta con Gaussiana", 100, -50, 200);
    for (int i = 0; i < 100000; ++i) {
        double value = 35.0 + gRandom->Landau(0, 10) + gRandom->Gaus(0, 5);
        histogram->Fill(value);
    }

    // Creazione delle variabili RooFit
    RooRealVar x("x", "x", -50, 200);
    RooDataHist data("data", "data", x, Import(*histogram));

    // Creazione dei parametri per la funzione di Landau
    RooRealVar mean("mean", "mean", 1, 0, 500);
    RooRealVar sigma("sigma", "sigma", 1, 0, 200);
    RooLandau landau("landau", "landau", x, mean, sigma);

    // Creazione dei parametri per la funzione Gaussiana
    RooRealVar gauss_mean("gauss_mean", "gauss_mean", 10, -100, 100);
    RooRealVar gauss_sigma("gauss_sigma", "gauss_sigma", 22, 0, 100);
    RooGaussian gauss("gauss", "gauss", x, gauss_mean, gauss_sigma);

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
    frame->Draw();
    canvas->SaveAs("langaus3_test.png");
} 