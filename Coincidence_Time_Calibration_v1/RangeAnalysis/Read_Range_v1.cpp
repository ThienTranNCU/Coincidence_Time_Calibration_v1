//Analysis is ased on the Singles

#include "Riostream.h"
#include <string.h>
#include <cmath>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <math.h>

#include "TCanvas.h"
#include "TStyle.h"
#include "TH1.h"
#include "TGaxis.h"
#include "TRandom.h"
#include "TH1F.h"
#include "TF1.h"
#include "TH2F.h"
#include "TRint.h"
#include "TObject.h"
#include "TPad.h"
#include "TMultiGraph.h"
#include "TGraphErrors.h"
#include "TStyle.h"
#include "TAxis.h"
#include "TGraph.h"
#include "TLine.h"
#include "TVirtualFitter.h"
#include "TSpectrum.h"


#include "TFile.h"
#include "TROOT.h"
#include "TTree.h"
#include "TRandom.h"
#include "TClassTable.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TBrowser.h"

#include <stdlib.h>
#include <string.h>
#include <cmath>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <math.h>
#include <complex>
#include <TApplication.h>
#include <TMath.h>
//using namespace std;

void  Read_Range_v1()
{
	

	ifstream in;
	in.open(Form("Range4/Range_Fitting.txt"));

	Double_t parf[200][4]={0},parb[200][4]={0};
	Double_t RangeX[200],time[200],Error[200],ter[200];
	Double_t offx=0.4*3;
	Double_t st,en,er1,er2;
	
	 	////////////////////
 	 TGraphErrors *gr1 = new  TGraphErrors();
 gr1->SetMarkerStyle(kFullCircle);
 gr1->SetMarkerSize(1.0);
 gr1->SetMarkerColor(4);
 gr1->SetLineColor(4);
 gr1->SetLineWidth(1);
 gr1->GetXaxis()->SetTitle("time (sec)");
gr1->GetYaxis()->SetTitle("Range (mm)");

 
  TGraph *gr2 = new TGraph();
 gr2->SetMarkerStyle(kFullCircle);
 gr2->SetMarkerSize(1.0);
 gr2->SetMarkerColor(1);
 gr2->SetLineColor(1);
 gr2->SetLineWidth(1);
  gr2->GetXaxis()->SetTitle("time (sec)");
gr2->GetYaxis()->SetTitle("Range Error (mm)");

 /////////////////////
	
Double_t test;
for(Int_t i=0;i<200;i++){	 
	 
	  in>>parf[i][0]>>parf[i][1]>>parf[i][2]>>parf[i][3]>>parb[i][0]>>parb[i][1]>>parb[i][2]>>parb[i][3];
	
		st= parb[i][0]*3.2;
		en= parf[i][0]*3.2+offx;
		RangeX[i]=	en-10;
		er1= (1.0/parb[i][2])*3.2/3.255;
		er2= (1.0/parf[i][2])*3.2/3.255;
		Error[i]=sqrt(er1*er1+er2*er2);
		time[i]=i*10+10;
		ter[i]=0.1;
		
		gr1->SetPoint(i,time[i],RangeX[i]);
		gr1->SetPointError(i,ter[i],Error[i]);
		gr2->SetPoint(i,time[i],Error[i]);
}
	
 	in.close();
 	

 	
 	/////////////////////////////

 TCanvas *c1 = new TCanvas("c1","c1",1000,500);
 c1->SetGrid(1);
 gr1->GetXaxis()->SetRangeUser(0,900);
 gr1->Draw("APL");
 
  TCanvas *c2 = new TCanvas("c2","c2",1000,500);
 c2->SetGrid(1);
 gr2->Draw("APL");
 gr2->GetXaxis()->SetRangeUser(0,900);

}
