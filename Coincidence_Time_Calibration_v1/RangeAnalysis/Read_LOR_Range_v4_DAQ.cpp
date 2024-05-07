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
using namespace std;

int main(int argc, char *argv[])
{
	
	char file1[1000],file2[1000],fileBG[1000],fileDAQ[1000];
	sprintf(file1,"%s",argv[1]);
	sprintf(file2,"%s",argv[2]);
	sprintf(fileBG,"%s",argv[3]);
//	cout<<file1<<endl;
	ifstream in[3];
	//in[0].open(Form("Data/091448.906_A1-2MINS_Processed2/Data-LOR.txt"));
//	in[1].open(Form("Data/091650.765_A1-13MINS_Processed2/Data-LOR.txt"));
	
	in[0].open(Form(file1));
	in[1].open(Form(file2));
	in[2].open(Form(fileBG));
	
	/////////////////BG
	TH1D *hBG=new TH1D("hBG"," ",64,0,32);
	Double_t bg;
	for(Int_t i=0;i<64;i++){
		in[2]>>bg;
		hBG->SetBinContent(i+1,bg);
	}
	
	//////////////
	
	Double_t time1=120; 
	Double_t  rf1=9.5,rf2=16,rb1=1,rb2=6;
	Double_t parf[200][4]={0},parb[200][4]={0};

	if(argc>=4){time1=strtod(argv[4],NULL);}
	if(argc>=8){rb1=strtod(argv[5],NULL);rb2=strtod(argv[6],NULL);rf1=strtod(argv[7],NULL);rf2=strtod(argv[8],NULL);}
	
	//////////////////////////////////////////// input DAQ
	Double_t CorDAQ[64]={0};
	Double_t DAQ[64]={1},daq;
	Int_t ci;
	for(Int_t i=0;i<64;i++){
			DAQ[i]=1;
			CorDAQ[64]=0;
	}
	if(argc>=9){               
		sprintf(fileDAQ,"%s",argv[9]);
		ifstream inDAQ;
		inDAQ.open(Form(fileDAQ));
		for(Int_t i=0;i<64;i++){
			inDAQ>>ci>>daq;
			DAQ[ci]=daq;
		}
	}
	
	//ofstream out;
	//out.open(Form("data/Peak_fit_v2.txt"));
	
	     TF1 *fG1 = new TF1("fG1","[0]+([1]-[0])/(1+exp([2]*(x-[3])))",rf1,rf2);
	     fG1->SetParameters(200.0,80.0,0.3,(rf1+rf2)/2.0);
	     
	     TF1 *fG2 = new TF1("fG2","[0]+([1]-[0])/(1+exp([2]*(x-[3])))",rb1,rb2);
	     fG2->SetParameters(200.0,20.0,0.3,3.0);
	
	
	char fname[1000];
	
	
	TH1F *hR[200];
	
	for(Int_t i=0;i<200;i++){
		sprintf(fname,"Range%dsec",i*10+10);
		hR[i]= new TH1F(fname,fname,64,0,32);
	}
	
	
	Int_t dead=0;
	TH1D *h1=new TH1D("h1"," ",64,0,32);
	TH1D *h1c=new TH1D("h1c"," ",64,0,32);
	TH2D *h2=new TH2D("h2"," ",64,0,32,32,0,16);
	Int_t g[6],s[6],ch[6];
	Double_t peak[6];
	Double_t pos[6],res[6],real[6];
	Double_t p0,p1,p2,chi2;
	Double_t pr0,pr1,pr2;
	TGraphErrors *gr1[4][4][64];
	
	Int_t b1,b2,x1,x2,y1,y2,tc=0,time,tm=10;
	Double_t X,Y;
	
	while(1){
	  if (!in[0].good()) break;  
	  in[0]>>b1>>x1>>y1>>b2>>x2>>y2>>time;
		if(time<1000){
		X=(x1*1.0+x2*1.0)/2.0;
		Y=(y1*1.0+y2*1.0)/2.0;
		tc=time/10;
		h1->Fill(X);
		h2->Fill(X,Y);
		if(time>tm){
		//	h1->Add(hBG,tc*(-10));
		///////////////////////////DAQ correction
		for(Int_t i=0;i<64;i++){
			CorDAQ[i]=h1->GetBinContent(i+1);
			CorDAQ[i]=CorDAQ[i]/DAQ[i];
			h1c->SetBinContent(i+1,CorDAQ[i]);
		}
		/////////////////////////////////////			
			
			hR[tc-1]->Add(h1c,1);
			//hR[tc-1]->Add(hBG,tc*(-10));
			tm=tm+10;
		//	tc++;
			cout<<time<<"\t"<<tc<<endl;
		}
	}
}
	
 	while(1){
	  if (!in[1].good()) break;  
	  in[1]>>b1>>x1>>y1>>b2>>x2>>y2>>time;
	  if(time<1000){
		time=time+time1;
		X=(x1*1.0+x2*1.0)/2.0;
		Y=(y1*1.0+y2*1.0)/2.0;
		tc=time/10;
		h1->Fill(X);
		//hR[tc]->Fill(X);
			
		h2->Fill(X,Y);
		if(time>tm){
		//	h1->Add(hBG,tc*(-10));
		///////////////////////////DAQ correction
		for(Int_t i=0;i<64;i++){
			CorDAQ[i]=h1->GetBinContent(i+1);
			CorDAQ[i]=CorDAQ[i]/DAQ[i];
			h1c->SetBinContent(i+1,CorDAQ[i]);
		}
		/////////////////////////////////////
			
			hR[tc-1]->Add(h1c,1);
			//hR[tc-1]->Add(hBG,tc*(-10));
			tm=tm+10;
			//tc++;
		}
	}
}
 //h1->Add(hBG,tc*(-10));
 /////////////////////////Fitting function for range
 
 ofstream out;
out.open(Form("Range4/Range_Fitting.txt"));
 
 
 for(Int_t i=0;i<200;i++){
	  hR[i]->Fit(fG1,"RQ");
	  hR[i]->Fit(fG2,"RQ+");
	  parf[i][0]=fG1->GetParameter(3);
	  parf[i][1]=fG1->GetParError(3);
	  parf[i][2]=fG1->GetParameter(2);
	  parf[i][3]=fG1->GetParError(2);
	  
	  parb[i][0]=fG2->GetParameter(3);
	  parb[i][1]=fG2->GetParError(3);
	  parb[i][2]=fG2->GetParameter(2);
	  parb[i][3]=fG2->GetParError(2);
	  out<< parf[i][0]<<"\t"<< parf[i][1]<<"\t"<< parf[i][2]<<"\t"<< parf[i][3]<<"\t"<< parb[i][0]<<"\t"<< parb[i][1]<<"\t"<< parb[i][2]<<"\t"<< parb[i][3]<<endl;
}
 
 out.close();
 
//############################################################################################
 for(Int_t d=0;d<2;d++){
in[d].close();
}
//out.close();
//TLegend *leg[200];
   ///////////////////////////////////////
        TCanvas *cR[200];
    for(Int_t i=0; i<200; i++)
    {
        			
                sprintf(fname,"RangeC%dsec",i*10+10);
                cR[i] = new TCanvas(fname,fname,1000,500);
				cR[i]->SetGrid(1);
                   
                 hR[i]->GetXaxis()->SetTitle("X index (mm)");
                 hR[i]->GetYaxis()->SetTitle("Counts");
                 hR[i]->SetLineWidth(3);
                   
                 hR[i]->Draw();
/*if(i==11){
							TLegend *leg= new TLegend(0.7,0.7,0.98,0.98);
							leg->AddEntry(hR[i],"Reconstructed 1D PAG image","lep");
							leg->AddEntry(fG1,"Range fitting function","lep");
								leg->Draw();
					}*/
                 
				sprintf(fname,"Range4/RBOX/Range%dsec.png",i*10+10);
                cR[i]->SaveAs(fname);
                sprintf(fname,"Range4/RBOX/Range%dsec.root",i*10+10);
                cR[i]->SaveAs(fname);
         
    }

 TCanvas *c1 = new TCanvas("c1","c1",1000,500);
 c1->SetGrid(1);
  h1->GetXaxis()->SetTitle("X index (mm)");
   h1->GetYaxis()->SetTitle("Counts");
   h1->SetLineWidth(3);
                   
    h1->Draw();
 sprintf(fname,"Range4/RangeTotal.png");
 c1->SaveAs(fname);
  sprintf(fname,"Range4/RangeTotal.root");
  c1->SaveAs(fname);

TCanvas *c2= new TCanvas("c2","c2",1000,500);
c2->SetGrid(1);
h2->GetXaxis()->SetTitle("X index (mm)");
h2->GetYaxis()->SetTitle("Y index(mm)");
h2->Draw("colz");
 sprintf(fname,"Range4/2D_PAG.pdf");
 c2->SaveAs(fname);
  sprintf(fname,"Range4/2D_PAG.root");
  c2->SaveAs(fname);

}
