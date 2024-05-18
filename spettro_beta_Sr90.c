TGraph* ReadFillAll( const char* Filename) {
    
    ifstream f(Filename);
    TGraph *h1 = new TGraph();
    int i = 0;
	if(!f){
  		cerr <<"Error: cannot open file " <<endl;
		exit(0);
	}
    else{
        double x,y, er;
		for (;;){
					
            f >> x >> y >> er;
            h1->SetPoint(i,x, y);		
									
			if(f.eof()){
                cout << "End of file reached "<< endl;
				break;
			}
            i++;		
		}
	}
    
    f.close();

	return h1;
}


void spettro_beta_Sr90(){
    TGraph* sr = ReadFillAll("stronzio.dat");
    TGraph* it = ReadFillAll("ittirio.dat");
    gStyle->SetPadLeftMargin(0.12);
    TCanvas* c =  new TCanvas();
    it->Draw("AL");
    sr->Draw("SAMEL");
    //sr->SetMarkerStyle(43);
	sr->SetLineWidth(3);
    //it->SetMarkerStyle(41);
	it->SetLineWidth(3);
    sr->SetLineColor(kRed);
    it->SetLineColor(kGreen+3);
    it->SetTitle("Spettro del decadimento #beta^{-} dello ^{90}Sr");
    it->GetXaxis()->SetTitle("Energia [kev]");
	it->GetYaxis()->SetTitle("dN/dE ");

    
    it->GetYaxis()->SetRangeUser(0,0.0035);

    TLegend *leg = new TLegend(0.6,0.65,0.8,0.8);
    leg->SetTextSize(0.05);
    leg->SetLineWidth(0);
    leg->AddEntry(sr, "^{90}Sr #beta decay", "l");
    leg->AddEntry(it, "^{90}Y  #beta decay", "l");
    leg->Draw();

    c->SaveAs("spettro_beta_Sr90.png");
}