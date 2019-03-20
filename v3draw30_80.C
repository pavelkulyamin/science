
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



void v3draw30_80(int icent=0)
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
   Double_t v2pt19gev_st[n1]  = {0.0208,0.0377,0.0526,0.0664,0.0799,0.0927,0.1046,0.1141,0.1244,0.1334,0.1379,0.1420,0.1477,0.1575};
   Double_t pt19err_st[n1] = {0};
   Double_t v2pt19err_st[n1] = {.0010,.0019,.0026,.0033,.0040,.0046,.0052,.0057,.0062,.0067,.0069,.0072,.0074,.0079};

   Double_t bin[15] = {0.2,0.4,0.6,0.8,1.0,1.2,1.4,1.6,1.8,2.0,2.2,2.4,2.6,2.8,3.2};
   TFile *f = new TFile("outFile_reso_4.root","READ");
   TProfile2D *p_v2 = (TProfile2D*)f -> Get("v3_pt_prof");
   TH1D * hist1 = (TH1D*) p_v2->ProfileX("prof",1,6)->Rebin(14,"hist1",bin);
   Double_t pt19gev[n1]  = {0.3, 0.5, 0.7, 0.9, 1.1, 1.3,1.5,1.7,1.9,2.1,2.3,2.5,2.7,3.0};
   Double_t v2pt19gev[n1];
   Double_t pt19err[n1] = {0};
   Double_t v2pt19err[n1];
   for (int i=0;i<14;i++){
    v2pt19gev[i] = hist1 -> GetBinContent(hist1->FindBin(i*0.2+0.3));
    v2pt19err[i] = hist1 -> GetBinError(hist1->FindBin(i*0.2+0.3));
   }
  
 
 TCanvas *c2;

 c2 = new TCanvas("c2","Flow analysis results",150,10,800,650);
  
  c2->GetFrame()->SetBorderSize(115);
  c2->cd();
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
  float ymax1=0.15;

 TH2F *hr2 = new TH2F("hr2",title1, 2,xmin1,xmax1,2,ymin1,ymax1);

  hr2->SetYTitle("V_{3}          ");
  hr2->SetXTitle("P_{T} (GeV/c) ");


  hr2->Draw();


  TGraphErrors *gr1;

 gr1 = new TGraphErrors(14,pt19gev,v2pt19gev,pt19err,v2pt19err);
 gr1->SetTitle("v3 19.6 GeV ");
 gr1->SetMarkerColor(kRed);
 gr1->SetMarkerStyle(22);
 gr1->SetMarkerSize(1.2);
 gr1->Draw("sameP");



 TLatex* harmMark = new TLatex(0.06, 0.13, "Au+Au #sqrt{s_{NN}}= 19.6 GeV, 30-80%");
        harmMark->SetTextFont(47);
        harmMark->SetTextSize(21);
        harmMark->Draw("same");

 TLatex* harmMark2 = new TLatex(0.06, 0.11, "charged hadrons");
        harmMark2->SetTextFont(47);
        harmMark2->SetTextSize(21);
        harmMark2->Draw("same");
 
 

 TLegend *legC2 = new TLegend(0.74,0.6,0.89,0.7);

 legC2->AddEntry(gr1,"v3 19.6 GeV Article","p");

 legC2->SetFillColor(kWhite);
 legC2->SetBorderSize(0); 
 legC2->Draw();

 c2 -> SaveAs("./v3_0_30.jpg");
}
