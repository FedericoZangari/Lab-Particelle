void calibrazione_alvolo(){

    ifstream f1("riv1.dat");
    ifstream f2("riv2.dat");

    TGraphErrors* g1 = new TGraphErrors();
    TGraphErrors* g2 = new TGraphErrors();
    double x, sigma_x;
    double y1 = 1.1732;     //primo picco del 60Co (MeV)
    double y2 = 1.3325;     //secondo picco del 60Co (MeV) 
    f1 >> x >> sigma_x;
    g1->SetPoint(0, x, y1);
    g1->SetPointError(0, sigma_x, 0);

    f1 >> x >> sigma_x;
    g1->SetPoint(1, x, y2);
    g1->SetPointError(1, sigma_x, 0);


    g1->Fit("pol1");
    TF1* t1 = g1->GetFunction("pol1");


    f2 >> x >> sigma_x;
    g2->SetPoint(0, x, y1);
    g2->SetPointError(0, sigma_x, 0);

    f2 >> x >> sigma_x;
    g2->SetPoint(1, x, y2);
    g2->SetPointError(1, sigma_x, 0);

    g2->Fit("pol1");
    TF1* t2 = g2->GetFunction("pol1");

    cout << "inserisci il bin del primo rivelatore:  ";
    cin >> x;
    cout << "corrisponde all'energia (MeV):  " << t1->Eval(x) << endl;

    cout << "inserisci il bin del secondo rivelatore:  ";
    cin >> x;
    cout << "corrisponde all'energia (MeV):  " << t2->Eval(x) << endl;
}