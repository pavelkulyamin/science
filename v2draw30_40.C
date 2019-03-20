
void makeplotstyle(){
  
  TStyle *mystyle = new TStyle("PlottingInStyle", "Style for Summary Plots");
  mystyle->SetPalette(1);
  mystyle->SetCanvasColor(10);
  mystyle->SetHistFillColor(10);
  mystyle->SetHistFillStyle(0);
  mystyle->SetOptTitle(0);
  mystyle->SetOptStat(0);
  mystyle->SetCanvasBorderMode(0);//removes the yellow frame around the canvas
  mystyle->SetPadLeftMargin(0.16);
  mystyle->SetPadBottomMargin(0.15);
  mystyle->SetPadTickX(1);
  mystyle->SetPadTickY(1);
  mystyle->SetAxisColor(1, "X");
  mystyle->SetAxisColor(1, "Y");
  mystyle->SetLabelColor(1, "X");
  mystyle->SetLabelColor(1, "Y");
  mystyle->SetTickLength(0.03, "X");
  mystyle->SetTickLength(0.03, "Y");
  mystyle->SetTitleXSize(0.05);
  mystyle->SetTitleYSize(0.05);
  mystyle->SetNdivisions(505, "X");
  mystyle->SetNdivisions(508, "Y");
  mystyle->SetTitleXOffset(1.2);
  mystyle->SetTitleYOffset(1.2);
  mystyle->SetLabelOffset(0.02, "X");
  mystyle->SetLabelOffset(0.02, "Y");
  mystyle->SetLabelSize(0.05, "X");
  mystyle->SetLabelSize(0.05, "Y");
  //mystyle->SetGridx();

  TFile f("style.root", "RECREATE");
  f.cd();
  mystyle->Write();
  f.Close();
}



void v2draw30_40(int icent=0)
{


 makeplotstyle();

 TFile *fstyle = new TFile("style.root");
  TStyle *tsty = (TStyle *)fstyle->Get("PlottingInStyle");
  tsty->cd();

  gROOT->ForceStyle();
  gStyle->SetOptStat(0);





char title1[800];
char title2[800];
char title3[800];

sprintf(title1,"");

// 10 - 20%

 const Int_t n1 = 14;
   Double_t pt19gev_st[n1]  = {0.3, 0.5, 0.7, 0.9, 1.1, 1.3,1.5,1.7,1.9,2.1,2.3,2.5,2.7,3.0};
   Double_t v2pt19gev_st[n1]  = {0.0294,0.0544,0.0772,0.0981,0.1183,0.1357,0.1531,0.1687,0.1789,0.1921,0.2065,0.2036,0.2229,0.2289};
   Double_t pt19err_st[n1] = {0};
   Double_t v2pt19err_st[n1] = {.0015,.0027,.0039,.0049,.0059,.0068,.0077,.0084,.0089,.0096,.0103,.0102,.0111,.0114};

   Double_t bin[15] = {0.2,0.4,0.6,0.8,1.0,1.2,1.4,1.6,1.8,2.0,2.2,2.4,2.6,2.8,3.2};
   TFile *f = new TFile("outFile_reso_4.root","READ");
   TProfile2D *p_v2 = (TProfile2D*)f -> Get("v2_pt_prof");
   TH1D * hist1 = (TH1D*) p_v2->ProfileX("prof",5,5)->Rebin(14,"hist1",bin);
   Double_t pt19gev[n1]  = {0.3, 0.5, 0.7, 0.9, 1.1, 1.3,1.5,1.7,1.9,2.1,2.3,2.5,2.7,3.0};
   Double_t v2pt19gev[n1];
   Double_t pt19err[n1] = {0};
   Double_t v2pt19err[n1];
   for (int i=0;i<14;i++){
    v2pt19gev[i] = hist1 -> GetBinContent(hist1->FindBin(i*0.2+0.3));
    v2pt19err[i] = hist1 -> GetBinError(hist1->FindBin(i*0.2+0.3));
   }
  
 
 TCanvas *c2;

TPad *pad1 = new TPad("pad1", "", 0.0, 0.3, 1.0, 1.0, 0, 0, 0);
pad1->SetFrameBorderMode(0);
pad1->SetBottomMargin(0);
TPad *pad2 = new TPad("pad2", "", 0.0, 0.0, 1.0, 0.3, 0, 0, 0);
pad2->SetFrameBorderMode(0);
pad2->SetTopMargin(0);
 c2 = new TCanvas("c2","Flow analysis results",150,10,800,650);
  
  c2->GetFrame()->SetBorderSize(115);
  c2->cd();
  pad1->Draw();
  pad2->Draw();
  gStyle->SetOptStat(0);

 c2->GetFrame()->SetFillColor(21);
 c2->GetFrame()->SetBorderSize(115);

  gStyle->SetOptStat(0);
  gStyle->SetPalette(0);
  gStyle->SetCanvasColor(0);
  gStyle->SetHistFillColor(10);
  gStyle->SetHistFillStyle(0);
  gStyle->SetOptTitle(1);
  gStyle->SetOptStat(0);
  c2->SetBorderMode(0);

  c2->SetFillColor(0);
  float xmin1=0.0;
  float xmax1=3.1;
  float ymin1=0.00;
  float ymax1=0.25;

 TH2F *hr2 = new TH2F("hr2",title1, 2,xmin1,xmax1,2,ymin1,ymax1);

  hr2->SetYTitle("V_{2}          ");
  //hr2->SetXTitle("P_{T} (GeV/c) ");


pad1->cd();
  hr2->Draw();


  TGraphErrors *gr1_art;
  TGraphErrors *gr1;
  TGraphErrors *gr1_ratio;

 gr1_art = new TGraphErrors(14,pt19gev_st,v2pt19gev_st,pt19err_st,v2pt19err_st);
 gr1_art->SetTitle("v2 19.6 GeV Article ");
 gr1_art->SetMarkerColor(kBlue);
 gr1_art->SetMarkerStyle(21);
 gr1_art->SetMarkerSize(1.2);
 gr1_art->Draw("P");
 gr1 = new TGraphErrors(14,pt19gev,v2pt19gev,pt19err,v2pt19err);
 gr1->SetTitle("v2 19.6 GeV ");
 gr1->SetMarkerColor(kRed);
 gr1->SetMarkerStyle(22);
 gr1->SetMarkerSize(1.2);
 gr1->Draw("sameP");



 TLatex* harmMark = new TLatex(0.2, 0.22, "Au+Au #sqrt{s_{NN}}= 19.6 GeV, 30-40%");
        harmMark->SetTextFont(47);
        harmMark->SetTextSize(21);
        harmMark->Draw("same");

 TLatex* harmMark2 = new TLatex(0.2, 0.19, "charged hadrons");
        harmMark2->SetTextFont(47);
        harmMark2->SetTextSize(21);
        harmMark2->Draw("same");
 
 

 TLegend *legC2 = new TLegend(0.74,0.3,0.89,0.4);

 legC2->AddEntry(gr1,"v2 19.6 GeV Article","p");
 legC2->AddEntry(gr1_art,"v2 19.6 GeV ","p");

 legC2->SetFillColor(kWhite);
 legC2->SetBorderSize(0); 
 legC2->Draw();

  float xmin1_rat=0.0;
  float xmax1_rat=3.1;
  float ymin1_rat=0.9;
  float ymax1_rat=1.2;

 TH2F *hr2_rat = new TH2F("hr2_rat",title1, 2,xmin1_rat,xmax1_rat,2,ymin1_rat,ymax1_rat);

  hr2_rat->SetXTitle("P_{T} (GeV/c) ");
  hr2_rat->SetYTitle("Ratio          ");
  //hr2_rat->GetXaxis()->SetLabelSize(0.05);

 pad2->cd();

 hr2_rat->Draw();
 TF1 *f1 = new TF1("f1","pol6",0.3,3.2);

 gr1_art -> Fit(f1,"0R");

 Double_t x_coord_1[n1], y_coord_1[n1];
 Double_t x_coord_1err[n1] = {0.0};
 Double_t y_coord_1err[n1];

 for (int i=0;i<14;i++){
    x_coord_1[i] = hist1->GetBinCenter(hist1->FindBin(i*0.2+0.3));
    y_coord_1[i] = (Double_t)(hist1->GetBinContent(hist1->FindBin(i*0.2+0.3)) / (Double_t)f1->Eval(x_coord_1[i]));
    }
 for (int i=0;i<14;i++){
    y_coord_1err[i] = TMath::Sqrt( pow( (1.0 / ( (Double_t)f1->Eval(x_coord_1[i]))) , 2 )*pow( v2pt19err[i],2 ) 
                      + pow(( v2pt19gev[i] / pow(v2pt19gev_st[i],2) ),2 ) * pow( v2pt19err_st[i],2 ) );
 }

 gr1_ratio = new TGraphErrors(14, x_coord_1, y_coord_1, x_coord_1err, y_coord_1err);
 gr1_ratio->SetTitle("");
 gr1_ratio->SetMarkerColor(kBlack);
 gr1_ratio->SetMarkerStyle(21);
 gr1_ratio->SetMarkerSize(1.2);
 gr1_ratio->Draw("P");
 c2 -> SaveAs("./v2_30_40.jpg");
}
