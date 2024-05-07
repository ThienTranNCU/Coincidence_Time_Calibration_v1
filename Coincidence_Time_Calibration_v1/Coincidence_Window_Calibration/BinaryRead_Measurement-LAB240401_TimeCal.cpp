//230416 Modified by Tran Cong Thien
//This program read the .bin output from the ASPET measurement and give single detection and coinincidence back-to-back reconstructed image.
//This program required the energy calibration file to set a precise energy threshold.
// each of the two board is divided into 16 half-sticID sections
//The coincidence time window for each of the 16 half-sticID section between 2 boards is reconigzed (256 correlation)
//(x,y)=(32,16)
//(8)(9)(10)(11)(12)(13)(14)(15)  
//(0) (1) (2)  (3)   (4)   (5)  (6)   (7)
//A folder of the same input file's name, with the extention of TimeCal, are created to put all the processed output in. 

#include "TCanvas.h"
#include "TStyle.h"
#include "TH1.h"
#include "TGaxis.h"
#include "TRandom.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"
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
#include "TPaveStats.h"

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

///TIME
#include <chrono>

//#include <vector>


using namespace std;


int compare(const void *arg1, const void *arg2)
{
    //    if(arg1>arg2){return 1;}
    //    if(arg1<arg2){return -1;}
    //    return (arg1>arg2);
    const int *lhs = static_cast<const  int*>(arg1);
    const int *rhs = static_cast<const  int*>(arg2);
    if ( lhs[0] < rhs[0] ) return -1;
    if ( lhs[0] > rhs[0] ) return +1;
    if ( lhs[0] == rhs[0] ) return 0;
}

int compare2(const void *arg1, const void *arg2)   //Frame | Time
{
    //    if(arg1>arg2){return 1;}
    //    if(arg1<arg2){return -1;}
    //    return (arg1>arg2);
    const int *lhs = static_cast<const  int*>(arg1);
    const int *rhs = static_cast<const  int*>(arg2);
    if ( lhs[3] > rhs[3] ) return +1;
    if ( lhs[3] < rhs[3] ) return -1;
    if(lhs[3]==rhs[3])
    {
        if ( lhs[0] < rhs[0] ) return -1;
        if ( lhs[0] > rhs[0] ) return +1;
        if ( lhs[0] == rhs[0] ) return 0;
    }
}

int compare3(const void *arg1, const void *arg2)   //Frame | STiC ID
{
    //    if(arg1>arg2){return 1;}
    //    if(arg1<arg2){return -1;}
    //    return (arg1>arg2);
    const int *lhs = static_cast<const  int*>(arg1);
    const int *rhs = static_cast<const  int*>(arg2);
    if ( lhs[3] > rhs[3] ) return +1;
    if ( lhs[3] < rhs[3] ) return -1;
    if(lhs[3]==rhs[3])
    {
        if ( lhs[1] < rhs[1] ) return -1;
        if ( lhs[1] > rhs[1] ) return +1;
        if ( lhs[1] == rhs[1] )
        {
            if ( lhs[6] < rhs[6] ) return -1;
            if ( lhs[6] > rhs[6] ) return +1;
        }
    }
}

//0tcc,1sid,2board,3frame,4ch,5energy, 6eventRecNo, 7time, 8gmslid
int compare4(const void *arg1, const void *arg2)   //Frame | GMSL ID |EventID
{
    //    if(arg1>arg2){return 1;}
    //    if(arg1<arg2){return -1;}
    //    return (arg1>arg2);
    const int *lhs = static_cast<const  int*>(arg1);
    const int *rhs = static_cast<const  int*>(arg2);
    if ( lhs[3] > rhs[3] ) return +1;
    if ( lhs[3] < rhs[3] ) return -1;
    if(lhs[3]==rhs[3])
    {
        if ( lhs[8] < rhs[8] ) return -1;
        if ( lhs[8] > rhs[8] ) return +1;
        if ( lhs[8] == rhs[8] )
        {
            if ( lhs[6] < rhs[6] ) return -1;
            if ( lhs[6] > rhs[6] ) return +1;
        }
    }
}

int compareX(const void *arg1, const void *arg2)
{
    //    if(arg1>arg2){return 1;}
    //    if(arg1<arg2){return -1;}
    //    return (arg1>arg2);
    const int *lhs = static_cast<const  int*>(arg1);
    const int *rhs = static_cast<const  int*>(arg2);
    if ( lhs[0] < rhs[0] ) return -1;
    if ( lhs[0] > rhs[0] ) return +1;
    if ( lhs[0] == rhs[0] ) return 0;
}
int compareY(const void *arg1, const void *arg2)
{
    //    if(arg1>arg2){return 1;}
    //    if(arg1<arg2){return -1;}
    //    return (arg1>arg2);
    const int *lhs = static_cast<const  int*>(arg1);
    const int *rhs = static_cast<const  int*>(arg2);
    if ( lhs[1] < rhs[1] ) return -1;
    if ( lhs[1] > rhs[1] ) return +1;
    if ( lhs[1] == rhs[1] ) return 0;
}


int mapx1(int channel)
{
    float x=0;
    if((channel==58)||(channel==57)||(channel==54)||(channel==52)||(channel==46)||(channel==44)||(channel==38)||(channel==38)){x=0;}
    if((channel==60)||(channel==59)||(channel==56)||(channel==50)||(channel==48)||(channel==42)||(channel==40)||(channel==36)){x=1;}
    if((channel==61)||(channel==55)||(channel==53)||(channel==47)||(channel==45)||(channel==39)||(channel==32)||(channel==35)){x=2;}
    if((channel==63)||(channel==62)||(channel==51)||(channel==49)||(channel==43)||(channel==41)||(channel==34)||(channel==33)){x=3;}
    if((channel==0)||(channel==1)||(channel==12)||(channel==14)||(channel==20)||(channel==22)||(channel==29)||(channel==30)){x=4;}
    if((channel==2)||(channel==8)||(channel==10)||(channel==16)||(channel==18)||(channel==24)||(channel==31)||(channel==28)){x=5;}
    if((channel==3)||(channel==4)||(channel==7)||(channel==13)||(channel==15)||(channel==21)||(channel==23)||(channel==27)){x=6;}
    if((channel==5)||(channel==6)||(channel==9)||(channel==11)||(channel==17)||(channel==19)||(channel==25)||(channel==26)){x=7;}
    //else{x=-1;}
    return x;
}
int mapy1(int channel)
{
    float y=0;
    if((channel==58)||(channel==60)||(channel==61)||(channel==63)||(channel==0)||(channel==2)||(channel==3)||(channel==5)){y=0;}
    if((channel==57)||(channel==59)||(channel==55)||(channel==62)||(channel==1)||(channel==8)||(channel==4)||(channel==6)){y=1;}
    if((channel==54)||(channel==56)||(channel==53)||(channel==51)||(channel==12)||(channel==10)||(channel==7)||(channel==9)){y=2;}
    if((channel==52)||(channel==50)||(channel==47)||(channel==49)||(channel==14)||(channel==16)||(channel==13)||(channel==11)){y=3;}
    if((channel==46)||(channel==48)||(channel==45)||(channel==43)||(channel==20)||(channel==18)||(channel==15)||(channel==17)){y=4;}
    if((channel==44)||(channel==42)||(channel==39)||(channel==41)||(channel==22)||(channel==24)||(channel==21)||(channel==19)){y=5;}
    if((channel==38)||(channel==40)||(channel==32)||(channel==34)||(channel==29)||(channel==31)||(channel==23)||(channel==25)){y=6;}
    if((channel==37)||(channel==36)||(channel==35)||(channel==33)||(channel==30)||(channel==28)||(channel==27)||(channel==37)){y=7;}
    //else{y=-1;}
    return y;
}

int mapxAll(int channel, int sid, int gid)
{
    float x=0;
    if((channel==58)||(channel==57)||(channel==54)||(channel==52)||(channel==46)||(channel==44)||(channel==38)||(channel==38)){x=0;}
    if((channel==60)||(channel==59)||(channel==56)||(channel==50)||(channel==48)||(channel==42)||(channel==40)||(channel==36)){x=1;}
    if((channel==61)||(channel==55)||(channel==53)||(channel==47)||(channel==45)||(channel==39)||(channel==32)||(channel==35)){x=2;}
    if((channel==63)||(channel==62)||(channel==51)||(channel==49)||(channel==43)||(channel==41)||(channel==34)||(channel==33)){x=3;}
    if((channel==0)||(channel==1)||(channel==12)||(channel==14)||(channel==20)||(channel==22)||(channel==29)||(channel==30)){x=4;}
    if((channel==2)||(channel==8)||(channel==10)||(channel==16)||(channel==18)||(channel==24)||(channel==31)||(channel==28)){x=5;}
    if((channel==3)||(channel==4)||(channel==7)||(channel==13)||(channel==15)||(channel==21)||(channel==23)||(channel==27)){x=6;}
    if((channel==5)||(channel==6)||(channel==9)||(channel==11)||(channel==17)||(channel==19)||(channel==25)||(channel==26)){x=7;}
    //else{x=-1;}
    if(sid/2==0){x=x;}
    if(sid/2==1){x= 7-x;}
    x= (gid%2)*16+(sid%2)*8 + x;
    return x;
}
int mapyAll(int channel, int sid, int gid)
{
    float y=0;
    if((channel==58)||(channel==60)||(channel==61)||(channel==63)||(channel==0)||(channel==2)||(channel==3)||(channel==5)){y=0;}
    if((channel==57)||(channel==59)||(channel==55)||(channel==62)||(channel==1)||(channel==8)||(channel==4)||(channel==6)){y=1;}
    if((channel==54)||(channel==56)||(channel==53)||(channel==51)||(channel==12)||(channel==10)||(channel==7)||(channel==9)){y=2;}
    if((channel==52)||(channel==50)||(channel==47)||(channel==49)||(channel==14)||(channel==16)||(channel==13)||(channel==11)){y=3;}
    if((channel==46)||(channel==48)||(channel==45)||(channel==43)||(channel==20)||(channel==18)||(channel==15)||(channel==17)){y=4;}
    if((channel==44)||(channel==42)||(channel==39)||(channel==41)||(channel==22)||(channel==24)||(channel==21)||(channel==19)){y=5;}
    if((channel==38)||(channel==40)||(channel==32)||(channel==34)||(channel==29)||(channel==31)||(channel==23)||(channel==25)){y=6;}
    if((channel==37)||(channel==36)||(channel==35)||(channel==33)||(channel==30)||(channel==28)||(channel==27)||(channel==26)){y=7;}
    //else{y=-1;}
    if(((sid/2)^(gid/2))==0){y=y;}
    if(((sid/2)^(gid/2))==1){y= 7-y;}
    y =  ((sid/2)^(gid/2))*8+y;
    return y;
}


std::string toBinary(int n)
{
    std::string r; int count=15;
    //while(n!=0) {
    while(count>0) {
        r=(n%2==0 ?"0":"1")+r; n/=2;
        count--;
    }
    return r;
}

std::string toBinaryBig(int n)
{
    std::string r; int count=30;
    //while(n!=0) {
    while(count>0) {
        r=(n%2==0 ?"0":"1")+r; n/=2;
        count--;
    }
    return r;
}

std::string toBinarySmall(int n, int count)
{
    std::string r;
    //while(n!=0) {
    while(count>0) {
        r=(n%2==0 ?"0":"1")+r; n/=2;
        count--;
    }
    return r;
}

Double_t CalE(Int_t e, Int_t g,Int_t s, Int_t ch, Double_t par[4][4][64][3])   ////Energy calibration
{
    Double_t energy= par[g][s][ch][1]*log((e-par[g][s][ch][2])/par[g][s][ch][0]);
    return energy;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////MAIN PROGRAM
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
	
	
	///////////////////////////////////////////////////////////      Read arguments /////////////////////////////////////////////////////////////////////////

    cout<<"Start1"<<endl;
    cout<<"argc="<<argc<<endl;
    //cout<<"argv[0]="<<argv[0]<<"\t argv[1]="<<argv[1]<<"\t argv[2]"<<argv[2]<<"\n"<<endl;
    
    ////Variables definition
    char argName[1000],newFolder[1000],openFolder[1000], fileName[1000],filePath[1000],argCal[1000], file[10][1000], fileNameRoot[1000];//file name string
    const int N=20000;
    double xmin=0, xmax=32800,DAQt=1200,sigT=0.06;
    float Eth=400;
    int pos=0; //POSITION TAG
    int  Ecut=2; //number of sigmas + and - about the mean photopeak value (e.g Ecut=2=>+/- 2sigma)
	int bint=0;
	
    if(argc>=4){Eth=strtod(argv[4],NULL);} //Energy threshold (keV)
    if(argc>=6){xmin=strtod(argv[5],NULL); xmax=strtod(argv[6],NULL);} //ADC window
    if(argc>=7){DAQt=strtod(argv[7],NULL);}
    if(argc>=8){bint=strtod(argv[8],NULL);}
	if(argc>=9){sigT=strtod(argv[9],NULL);}
    int const binT = bint;
        	///////////////////////////////////////////////////////////// Energy Calibration ///////////////////////////////////////////////////////////////////////////////////
    sprintf(argCal,"%s",argv[3]);    	
        	
    Double_t caE;     ////energy threshold
    Double_t chi2,par[4][4][64][3]={0},parE[4][4][64][3]={0},Kill1[4][4][64];
    Int_t g2,s2,ch2,kill;
    Double_t p0,pr0,p1,pr1,p2,pr2;
    ifstream inD;
    inD.open(Form(argCal));
    
 //   for(Int_t i=0;i<1024;i++){
		while(1){
		if(!inD.good()) break;  
		inD>>g2>>s2>>ch2>>p0>>pr0>>p1>>pr1>>p2>>pr2>>chi2;
		par[g2][s2][ch2][0]=p0;
		par[g2][s2][ch2][1]=p1;
		par[g2][s2][ch2][2]=p2;
		
		parE[g2][s2][ch2][0]=pr0;
		parE[g2][s2][ch2][1]=pr1;
		parE[g2][s2][ch2][2]=pr2;
		Kill1[g2][s2][ch2]=chi2;
//		cout<<"Killl                               ************************         "<<chi2<<" "<<kill<<endl;
	}
    
    inD.close();
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
    
    //cout<<strtod(argv[4],NULL)<<"\t tDAQ="<<tDAQ<<endl;
    sprintf(argName,"%s",argv[2]);
    sprintf(filePath,"%s",argv[1]);
    sprintf(fileName,"%s/%s.bin",filePath,argName);
    cout<<"Opening file from the argument..\t"<<fileName<<endl;
    
    sprintf(newFolder,"%s/%s_TimeCal1",argv[1], argv[2]);                                ////////////////////////////////////////////////////Creating a new folder for the output
    sprintf(openFolder,"mkdir %s",newFolder);
    system(openFolder);
    sprintf(openFolder,"mkdir %s/EBOX",newFolder);
    system(openFolder);
    sprintf(openFolder,"mkdir %s/TBOX",newFolder);
    system(openFolder);
    
    sprintf(openFolder,"mkdir %s/CoinConstrain",newFolder);
    system(openFolder);
        
  //  ofstream outL;
  //  sprintf(openFolder,"%s/Data-LOR.txt",newFolder);
//	outL.open(Form(openFolder));
        
    const int nevents=1;//100000
    unsigned char buffer[8*nevents]; //char is 1 byte long, so char[8] is 8 bytes long - buffer size for one event
    unsigned int a[8];
    
    unsigned int ecoarse;
    unsigned int tcoarse;
    unsigned int energy;
    unsigned int efine;
    unsigned int tfine;
    unsigned int time;
    unsigned int ebad;
    unsigned int tbad;
    
    unsigned int channel;
    unsigned int frame;
    unsigned int gmslID;
    unsigned int sticID;
    unsigned int boardID;
    unsigned int Data0_Sync1;
    unsigned int RSTcountcc;
    
    
    //HISTOGRAMS
    TH1F *hEc=new TH1F("hEc","E coarse",32770,-1,32769);
    //TH1F *hTc=new TH1F("hTc","time coarse",32770,-1,32769);//32768
    TH1F *hTc=new TH1F("hTc","time coarse",40000,-1,39999);//32768
    TH1F *hTf=new TH1F("hTf","time fine",32,0,32);
    TH1F *hEf=new TH1F("hEf","energy fine",32,0,32);
    TH1F *hT=new TH1F("hT","Time",10000000,-5000000,5000000);
    TH1F *hEbad=new TH1F("hEbad","E-bad",2,0,2);
    TH1F *hTbad=new TH1F("hTbad","T-bad",2,0,2);
    
    TH1F *hFrame=new TH1F("hFrame","Frames",256,0,256);
    TH1F *hCh=new TH1F("hCh","Channels",64,0,64);
    TH1F *hSticID=new TH1F("hSticID","STiC ID",8,0,8);
    TH1F *hGMSLID=new TH1F("hGMSLID","FPGA ID",8,0,8);
    TH1F *hBoardID=new TH1F("hBoardID","Board ID",8,0,8);
    TH2F *h2EventEnergy = new TH2F("h2EventEnergy","h2EventEnergy",1000,0,1000000,33000,0,33000);
    TH2F *h2SticIDGMSLID=new TH2F("hSticIDGMSLID2","SticID-GMSLID",4,0,4,4,0,4); //210822// more info on distribution
    
    TH1F *hRST=new TH1F("hRST","Reset counter",10000000,0,10000000);
    TH1F *hRSTG0=new TH1F("hRSTG0","Reset counter G0",10000,0,100000);
    TH1F *hRSTG1=new TH1F("hRSTG1","Reset counter G1",1000000,0,100000);
    TH1F *hRSTG2=new TH1F("hRSTG2","Reset counter G2",1000000,0,100000);
    TH1F *hRSTG3=new TH1F("hRSTG3","Reset counter G3",1000000,0,100000);
    TH1F *hRSTdeltaG0=new TH1F("hRSTdeltaG0","Reset counter Delta",10000,0,10000);
    TH1F *hRSTdeltaG1=new TH1F("hRSTdeltaG1","Reset counter Delta",10000,0,10000);
    TH1F *hRSTdeltaG2=new TH1F("hRSTdeltaG2","Reset counter Delta",10000,0,10000);
    TH1F *hRSTdeltaG3=new TH1F("hRSTdeltaG3","Reset counter Delta",10000,0,10000);
    
    
    ////////////////////////////////Defining energy histogram for all channel
    TH1F *hE[4][4][64];
    TH1F *hTCi[4][4][64];
    TH1F *hTC[4][4];
    TH1F *h1PPmean[4][4], *h1PPamp[4][4];
    char hname[50],hnameE[50];
    for(int g=0; g<4; g++)
    {
        for(int s=0; s<4; s++)
        {
            sprintf(hname,"h1PPmeanG%dS%d",g,s);
            h1PPmean[g][s]=new TH1F(hname,"E",1000,0,1000);
            sprintf(hname,"h1PPampG%dS%d",g,s);
            h1PPamp[g][s]=new TH1F(hname,"E",1000,0,1000);
            sprintf(hname,"hTC_G%dS%d",g,s);
            hTC[g][s]=new TH1F(hname,hname,int(DAQt+20),0,DAQt+20);
            for(int i=0; i<64; i++)
            {
                sprintf(hname,"G%dS%dCh%d",g,s,i);
                hE[g][s][i]=new TH1F(hname,hname,8192,0,32768); // until 210915-binning one in 4
                sprintf(hname,"hTCi_G%dS%dCh%d",g,s,i);
                hTCi[g][s][i]=new TH1F(hname,hname,int(DAQt+20),0,DAQt+20); // until 210915-binning one in 4
            }
        }
    }
    
    /////////////////////////////////////////////////////////////////////// Defining TFile and tree  for output/////////////////////////////////////////////////////////////////////////////////////////////////////
    sprintf(fileNameRoot,"%s/All_outputTree.root",newFolder);
    //TFile* fileRoot_tree = new TFile("data.root", "RECREATE");
    TFile* fileRoot_tree = new TFile(fileNameRoot, "RECREATE");
    TTree *tree= new TTree("tree", "tree");
    //Setting up the branches..
    tree->Branch("ecoarse", &ecoarse, "ecoarse/I");
    tree->Branch("tcoarse", &tcoarse, "tcoarse/I");
    tree->Branch("efine", &efine, "efine/I");
    tree->Branch("tfine", &tfine, "tfine/I");
    tree->Branch("channel", &channel, "channel/I");
    tree->Branch("frame", &frame, "frame/I");
    tree->Branch("tbad", &tbad, "tbad/I");
    tree->Branch("ebad", &ebad, "ebad/I");
    tree->Branch("sticID", &sticID, "sticID/I");
    tree->Branch("gmslID", &gmslID, "gmslID/I");
    tree->Branch("boardID", &boardID, "boardID/I");
    tree->Branch("Data0_Sync1", &Data0_Sync1, "Data0_Sync1/I");
    tree->Branch("energy", &energy, "energy/I");
    tree->Branch("time", &time, "time/I");
    tree->Branch("RSTcountcc", &RSTcountcc, "RSTcountcc/I");
    
    //char path[200],tag[300], date[50];
    FILE *ptr;
    
    //OPENING FILE; run a loop, get events, close file; OPEN AGAIN
    cout<<"Opening file.."<<fileName<<endl;
    ptr=fopen(fileName,"rb");
    if(!ptr){cout<<"Disaster!"<<endl; return 0;}
    else{cout<<"opening succesful.."<<fileName<<endl;}
    int eventsTOTAL=0;
    while(std::fread(buffer, sizeof(buffer),1,ptr)!='\0')
    {
        eventsTOTAL++;
    }
    const int eventsTOTALmain=eventsTOTAL;
    const int loops = eventsTOTALmain/15320; //ARBITRARY ASSIGNMENT OF ARRAY SIZE LEADING TO MANY LOOPS
    cout<<"TOTAL EVENTS="<<eventsTOTALmain<<endl;
    //if(eventsTOTAL>200000){eventsTOTAL=200000;}
    if(eventsTOTAL>20480){eventsTOTAL=20480;} // Assigned to coin loops later
    fclose(ptr);
    cout<<"ANALYZED EVENTS="<<eventsTOTAL<<endl;
    ptr=fopen(fileName,"rb");
    
    //with an event rate of 1MHz, you will get 1 event in 1us, or 100 events in 1000 events in 1ms on average. So the chance of a true coincidence lying in the next ms is extremely low.
    //with 20K, we further increase this to a 20ms avg window.
  
    //////////////////////////////////////////////////////////////////////////
    //LFSR DECODING****
    //////////////////////////////////////////////////////////////////////////
    int16_t m_lut[ 1 << 15 ], encodedLfsr[1 << 15];
    m_lut[ 0x7FFF ] = -1; // invalid state
    uint16_t lfsr = 0x0000;
    for ( int16_t n = 0; n < ( 1 << 15 ) - 1; ++n )
    {
        m_lut[ lfsr ] = n;
        encodedLfsr[n]=lfsr;
        const uint8_t bits13_14 = lfsr >> 13;
        uint8_t new_bit;
        switch ( bits13_14 )
        { // new_bit = !(bit13 ^ bit14)
            case 0x00:
            case 0x03:
                new_bit = 0x01;
                break;
            case 0x01:
            case 0x02:
                new_bit = 0x00;
                break;
        }// switch
        lfsr = ( lfsr << 1 ) | new_bit; // add the new bit to the right
        lfsr &= 0x7FFF; // throw away the 16th bit from the shift
    }// for
    /////////////////////////////////////
    /////////////////////////////////////
    
    ////////////////////////////////////////////////////////////////////////////////
    ///// EVENT MAIN LOOP: SHORT COIN BUFFERS, RST COUNTERS ETC
    ////////////////////////////////////////////////////////////////////////////////
    //0tcc,1sid,2board,3frame,4ch,5energy, 6eventRecNo, 7time, 8gmslid
    //READING FILE THROUGH THE BUFFER
    int ncount=0, ncountc=0, ncountall=0; // ncounts: stores the number of events, ncountc is a local counter for cm[][]
    int nloopc=0; //number of coincidence loops
    int ncountGood=0; //Stores the number of useful events
    //eventsTOTAL has the total events being analyzed for this task
    int ncoin=0;//Stores the total opposite coincidences registered
    
    int ncountBrdGmslStic[1][4][4];
    int ncountGoodBrdGmslStic[1][4][4];
    
    int RSTcountAbscc=0; // Absolute value of the RST counter as readout qqq
    int RSTcountf=0; // fine value of RST count given by the rolling Efine bins qqq
    int RSTcountccStored[4]; // Counter value as maintained by the number of RST events read by the code
    //long int RSTcountccStoredG=0; //? qqq
    int RSTcountT0[4]; //Start value of the RSTcountAbs or the first stored RST event value
    int oldRSTcountcc0=0,oldRSTcountcc1=0,oldRSTcountcc2=0, oldRSTcountcc3=0;// Previous value
    int RST[4][10000];//1M GOOD FOR 2000S ~33MINS
    int rcount[4][10000];
    int evcount[4][10000];
    float RSTLOR = 0;
    int RSTEventcc[4]; //Holds the latest RSTcc value for the corresponding gID
    int RSTEventff[4]; //RSTff for the current event
    bool RSTflag;
    int RSTnew[4], RSTold[4];//old and current until RSTcountT0 is found for all gmsl.
    //cout<<"SNo\t"<<"frame\t"<<"bID\t"<<"sID\t"<<"ch\t"<<"t\t"<<"e\t"<<"tcc\t"<<"ecc\t"<<"dtcc\t"<<endl;
    
    
    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////COINCIDENCE CONDITIONS, DECLARING VARIABLES, BUFFERS, HISTOGRAMS, ETC
    ////////////////////////////////////////////////////////////////////////////////
    //const int cmsize=eventsTOTAL/8; //cmsize=eventsTOTAL; cmsize=eventsTOTAL/2
    const int cmsize=eventsTOTAL;
    const int cmwidth=11;
    int cm[cmsize][cmwidth]; cout<<"cm[cmsize][cmwidth] array opened"<<endl;
    
    //PLL DATA
    //const int windowTcc=3, windowEvents=2048; //windowEvents=512 // 32736
    //const int windowcluster=2; //No of tcc within which events are considered as clusters
    //NA22 DATA
    const int windowTcc=10, windowEvents=1024; //210526 ans 2106xx dats |windowTcc=10, windowEvents=1024
    //const int windowTcc=3, windowEvents=2048; //windowEvents=512 // 2109xx..
    const int windowcluster=2; //No of tcc within which events are considered as clusters
    
    
    
    //Channel Map | Printing Channel Map
    float x1=0,x2=0,xmean=0,y1=0,y2=0,ymean=0;
    int lch=0, rch=0; //continuous channel map with GID, SID weighted  - 512ch each
    cout<<"Channel"<<"\tmapX"<<"\tmapY"<<endl;
    for(int i=0;i<63;i++)
    {
        cout<<i<<"\t"<<mapx1(i)<<"\t"<<mapy1(i)<<endl;
    }
    cout<<"first few events"<<cm[0][0]<<"\t"<<cm[1][0]<<"\t"<<cm[2][0]<<endl;
    int CFtrans=0, CStrans=0;
    int Cframe=cm[0][3], Cframeold=cm[0][3];
    int CgmslID=cm[0][8], CgmslIDold=cm[0][8];
    int Cevent=0;
    
    int changeS=cm[0][8], changeF=cm[0][3]; //not needed
    
    int CeventStart1=0, CeventStart2=0, CeventEnd=0;
    int oldtcc1=0,oldtcc2=0; int cluster=0;
    ////////////////////////MAPS TIMEFRAME MAPS TIMEFRAME MAPS TIMEFRAME//////////////////
    //DISPLAY OF MAPS BROKEN IN TIMEFRAMES qqq
    float frameTime=1200, totalTime=1200; //1200S TOTAL
    int nframes=totalTime/frameTime;
    
    /////////////////Histogram definition annd fill on the fly
    TH2F *h3Coin =  new TH2F("h3Coin","h3Coin",512,0,512,512,0,512);//This is the raw coincidence map from the detector
    TH2F *h2Coin =  new TH2F("h2Coin","h2Coin",64,0,64,64,0,64); //This is the raw coin for 2x64 channel level
    TH1F *h1Coin = new TH1F("h1Coin","h2Coin",1000,-500,500); //Time difference spectrum L and R
    TH1F *hdEvents = new TH1F("hdEvents","hdEvents",512,-32768,32768);
    //TH2F *h2Map =  new TH2F("h2Map","h2Map",16,0,8,16,0,8); //Mapped distribution - single CHIP
    //TH2F *h2Map =  new TH2F("h2Map","h2Map",32,0,32,16,0,16); //Mapped distribution - ALL CHIPS - SIMPLE BINNING
    TH2F *h2Map =  new TH2F("h2Map","h2Map",64,0,32,32,0,16); //Mapped distribution - ALL CHIPS - OVER BINNING
    TH2F *h2tY =  new TH2F("h2tY","h2MapBP",int(DAQt+20),0,(DAQt+20),32,0,16); //Y vs time
    TH2F *h2tX =  new TH2F("h2tX","h2MapBP",int(DAQt+20),0,(DAQt+20),64,0,32); //X vs time
    char  hnamecoin[50]; 
    //sprintf(hnamecoin,"hCoinCh%d-Ch%d",ChA,ChB);
 //   TH1F *hCoinAB=new TH1F(hnamecoin,hnamecoin,1000,-500,500);// No Energy Sel
 //   sprintf(hnamecoin,"hCoinECh%d-Ch%d",ChA,ChB);
//    TH1F *hCoinABE=new TH1F(hnamecoin,hnamecoin,1000,-500,500);//With Energy Sel
    
    //ANALYSIS - PHOTOPEAKS
    TH2F *hPPamp0 =  new TH2F("hPPamp0","hPPamp0",64,0,32,32,0,16);//PhotoPeak Amp map 2D for the entire detector B0:32X16CH
    TH2F *hPPamp1 =  new TH2F("hPPamp1","hPPamp1",64,0,32,32,0,16);//PhotoPeak Amp map 2D for the entire detector B1:32X16CH
    TH2F *hPPmean0 =  new TH2F("hPPmean0","hPPmean0",64,0,32,32,0,16);//PhotoPeak Amp map 2D for the entire detector B0:32X16CH
    TH2F *hPPmean1 =  new TH2F("hPPmean1","hPPmean1",64,0,32,32,0,16);//PhotoPeak Amp map 2D for the entire detector B1:32X16CH
    TH2F *hSingle0 =  new TH2F("hSingle0","hSingle0",32,0,32,16,0,16);
    TH2F *hSingle1 =  new TH2F("hSingle1","hSingle1",32,0,32,16,0,16);
    
    TH1F *hT1 =  new TH1F("hT1","All singles",int(DAQt+20),0,DAQt+20);
    TH1F *hT2 =  new TH1F("hT2","Coincidences only",int(DAQt+20),0,DAQt+20);
    
    TH1F *hPX =  new TH1F("hPX","Coincidences X projection",64,0,32);
    TH1F *hPY =  new TH1F("hPY","Coincidences Y projection",32,0,16);
    TH1F *hCW =  new TH1F("hCW","Coincidences Window",1000,-500,500);
    
    TH1F *hCWi[16][16];
    for(Int_t i=0;i<16;i++){
		for(Int_t j=0;j<16;j++){
			sprintf(hname,"Board0_%d-Board1_%d",i,j);
			hCWi[i][j] =  new TH1F(hname,hname,binT,-500,500);
		}
	}
    
    TH2F *hLOR1 =  new TH2F("hLOR1","LoR board 0",32,0,32,16,0,16);
    TH2F *hLOR2 =  new TH2F("hLOR2","LoR board 1",32,0,32,16,0,16);
    
    /////////////////Set of Histograms 2D TIMEFRAMES qqq
    TH2F *h2Mapt[nframes];
    char h2MaptTitle[100];
    for(int i=0; i<nframes; i++)
    {
        sprintf(h2MaptTitle,"h2Mapt-%d",i);
        h2Mapt[i] = new TH2F(h2MaptTitle,h2MaptTitle,64,0,32,32,0,16);
        //cout<<"FRAMES - SETTING HIST:"<<i<<endl;
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////
    //FIRST SCAN OF EVENTS - ENERGY HISTOGRAMS
    ////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////
    
    cout<<"ncount"<<"\t"<<"gmslID"<<"\t"<<"sticID"<<"\t"<<"efine"<<"\t"<<"tcoarse"<<"\t"<<"ecoarse"<<"\t"<<"energy"<<"\t"<<"energy"<<endl;
    
    while(std::fread(buffer, sizeof(buffer),1,ptr)!='\0')
    {
        //if(ncount==0){timeOld=0; frameOld=0;}
        for(int j=0; j<nevents; j++)
        {
            for(int i=0; i<8; i++)
            {
                //a[i]=int(buffer[j*8+i]); //TESTER PACK
                //if(j<1000&&i==7){cout<<endl;cout<<"Row"<<ncount<<"\t";}
                //if(j<1000){cout<<toBinary(int(buffer[i]))<<"\t";};
            }
            
            
            if((buffer[j*8+0]/4+buffer[j*8+7]*256/4+(buffer[j*8+6]%2)*256*256/4)!=32767)
            {
                ecoarse=m_lut[ buffer[j*8+3]/32+buffer[j*8+2]*256/32+(buffer[j*8+1]%16)*256*256/32 ];
                tcoarse=m_lut[ (buffer[j*8+0]/4+buffer[j*8+7]*256/4+(buffer[j*8+6]%2)*256*256/4) ];
                //            efine= buffer[j*8+3]%32;
                //            tfine= buffer[j*8+1]/32+(buffer[j*8+0]%4)*256/32;
                channel=buffer[j*8+6]/4;
                //            frame=buffer[j*8+5]%64;
                //            tbad=(buffer[j*8+6]/2)%2;
                //            ebad=(buffer[j*8+1]/16)%2;
                sticID=buffer[j*8+5]/64;
                gmslID=buffer[j*8+4]%4;
                boardID=(buffer[j*8+4]/4)%32;
                //Data0_Sync1=buffer[j*8+4]/128;
                //RSTcountcc=oldRSTcountcc;
                ////////TIME-ENERGY CALCULATION
                //energy=ecoarse-tcoarse;
                if(ecoarse>tcoarse){energy=ecoarse-tcoarse;}
                if(ecoarse<tcoarse){energy=ecoarse+32766-tcoarse;}
                if(energy<=xmin||energy>=xmax){energy=-1;}
                //time=32*tcoarse+tfine;
                caE=CalE(energy,gmslID,sticID,channel,par);
                if(Kill1[gmslID][sticID][channel]==1){
					if(caE>=Eth){
						hE[gmslID][sticID][channel]->Fill(caE);																	///////////// filling energy histogram for each channel
					}
				}
            } //END of if statemnt
        }//END OF J dummy loop
    } // END of WHILE - event scanning
    fclose(ptr);
    cout<<"ENERGY READING COMPLE, FILE WILL BE CLOSED AND OPENED AGAIN"<<eventsTOTAL<<endl;
    ptr=fopen(fileName,"rb");
    
    TCanvas *cE[4][4][4];
    for(int g=0; g<4; g++)
    {
        for(int s=0;  s<4; s++)
        {
            //            h1PPamp[g][s]->Write();
            //            h1PPmean[g][s]->Write();
            for(int j=0; j<4; j++)
            {
                sprintf(hname,"G%dS%dE-ChSet%d",g,s,j);													//// TCanvas name
                cE[g][s][j] = new TCanvas(hname,hname,1000,1000);
                int nx=4, ny=4;
                int number=0;
                cE[g][s][j]->Divide(nx,ny,0,0);
                for(int i=0; i<nx*ny; i++)
                {
                    number++;
                    cE[g][s][j]->cd(number);
                    //h1->FillRandom("gaus",1000);
                    //hE[47+i]->GetXaxis()->SetXLimits();
                    //double xmin=0, xmax=800;
                    hE[g][s][16*j+i]->SetAxisRange(xmin, xmax,"X");
                    hE[g][s][16*j+i]->GetXaxis()->SetLabelFont(53);
                    hE[g][s][16*j+i]->GetXaxis()->SetLabelSize(10);
                    hE[g][s][16*j+i]->GetXaxis()->SetNdivisions(5);
                    hE[g][s][16*j+i]->GetYaxis()->SetLabelFont(53);
                    hE[g][s][16*j+i]->GetYaxis()->SetLabelSize(10);
                    hE[g][s][16*j+i]->GetYaxis()->SetNdivisions(5);
                    hE[g][s][16*j+i]->DrawCopy();
                }
                cE[g][s][j]->Write();
                sprintf(hnameE,"%s/EBOX/%s.png",newFolder,hname);
                cE[g][s][j]->SaveAs(hnameE);
                sprintf(hnameE,"%s/EBOX/%s.pdf",newFolder,hname);
                cE[g][s][j]->SaveAs(hnameE);
            }
        }
    }
     
    
  
    
    ////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////
     //ANALYSIS - TIMING DELAYS
    ////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////
    Double_t CaE1,CaE2;
    double endt=0;
    ncount=0, ncountall=0;
    bool LORflag=0, SinglesFlag=0;
    //cout<<"ncount"<<"\t"<<"gmslID"<<"\t"<<"sticID"<<"\t"<<"efine"<<"\t"<<"tcoarse"<<"\t"<<"ecoarse"<<"\t"<<"energy"<<"\t"<<"energy"<<endl;
    //cout<<"ncount"<<"\t"<<"gmslID"<<"\t"<<"channel"<<"\t"<<"RSTcountcc"<<"\t"<<"RSTevent actual value"<<endl;
    int tempc=0; //qqq delete later
    while(std::fread(buffer, sizeof(buffer),1,ptr)!='\0')                          																																						///// While loop for coincidence buffer#############################
    {
        //if(ncount==0){timeOld=0; frameOld=0;}
        for(int j=0; j<nevents; j++)
        {
            for(int i=0; i<8; i++)
            {
                a[i]=int(buffer[j*8+i]);
                //if(j<1000&&i==7){cout<<endl;cout<<"Row"<<ncount<<"\t";}
                //if(j<1000){cout<<toBinary(int(buffer[i]))<<"\t";};
            }
            
            /////////RESET COUNTER VALUES, GMSL ASSIGNMENTS
            if((buffer[j*8+0]/4+buffer[j*8+7]*256/4+(buffer[j*8+6]%2)*256*256/4)==32767)//==32767  //ncount>2047 //&&ncount>2048,4096?
            {
                efine= buffer[j*8+3]%32;
                tfine= buffer[j*8+1]/32+(buffer[j*8+0]%4)*256/32;																																																///// decoding data
                channel=buffer[j*8+6]/4;
                frame=buffer[j*8+5]%64;
                tbad=(buffer[j*8+6]/2)%2;
                ebad=(buffer[j*8+1]/16)%2;
                sticID=buffer[j*8+5]/64;
                gmslID=buffer[j*8+4]%4;
                boardID=(buffer[j*8+4]/4)%32;
                Data0_Sync1=buffer[j*8+4]/128;
                
                //if(RSTcountT0[gmslID]==0&&RSTcountccStoredG>2048) //NOT CLEAR; RSTcountccStored==0 //
                
                //////////////////////////////
                //FINDING THE RSTCOUNTT0[GMSLID] BASED ON THE FIRST STEP CHANGE
                //VALUES OF RSTCOUNT AND THE COUNT  OF THE NUMBER OF COUNTS&EVENTS
                //LATER USED FOR PRITING GRAPHS TO SEE DISCONTINUITIES AND FLOW.
                if(RSTcountT0[gmslID]==0 && ncountall<10240 && RSTflag==false) //Expect to find RST0 within first 10240 events
                {
                    RSTnew[gmslID]=buffer[j*8+3]+256*buffer[j*8+2]+256*256*buffer[j*8+1]+256*256*256*(buffer[j*8+0]%4);
                    
                    //if(RSTnew[gmslID]-RSTold[gmslID]>1000 &&ncount>1024) //|| RSTnew[gmslID]-RSTold[gmslID]<-1000
                    //THREE CONDITIONS TO REGISTER THE FIRST RST EVENT!
                    //1. EVENTID>2048 &&
                    //2. CORRECT RST0 SHOULD HAVE A MUCH HIGHER VALUE THAN OLD VALUES ||
                    //3. CORRECT RST0 SHOULD HAVE A MUCH LOWER VALUE THAN OLD VALUES }}
                    if((ncountall>=2048)&&((RSTnew[gmslID]-RSTold[gmslID]>1000) || (RSTnew[gmslID]-RSTold[gmslID]<-1000)))
                    {
                        RSTcountT0[gmslID]=buffer[j*8+3]+256*buffer[j*8+2]+256*256*buffer[j*8+1]+256*256*256*(buffer[j*8+0]%4);
                        RSTcountcc=buffer[j*8+3]+256*buffer[j*8+2]+256*256*buffer[j*8+1]+256*256*256*(buffer[j*8+0]%4)-RSTcountT0[gmslID];
                        cout<<"G="<<gmslID<<"\tRSTnew="<<RSTnew[gmslID]<<"\t"<<"RSTold="<<RSTold[gmslID]<<endl;
                        cout<<"----"<<endl;
                        cout<<"FIRST RST COUNT\t giD="<<gmslID<<endl;
                        //cout<<"First RST count="<<RSTcountT0[gmslID]<<"\t RSTcountcc="<<RSTcountcc<<"\t ncount="<<ncount<<"\t GID="<<gmslID<<"\tRSTabs="<<RSTcountAbscc<<endl;
                        cout<<ncountall<<"\t"<<gmslID<<"\t"<<channel<<"\t"<<RSTcountcc<<"\t"<<buffer[j*8+3]+256*buffer[j*8+2]+256*256*buffer[j*8+1]+256*256*256*(buffer[j*8+0]%4)<<endl;
                    }
                    
                    if(RSTcountT0[0]!=0&&RSTcountT0[1]!=0&&RSTcountT0[2]!=0&&RSTcountT0[3]!=0)
                    {
                        RSTflag=true;
                        cout<<"RSTcountT0s for all gmsl found="<<RSTflag<<endl;
                    }
                    RSTold[gmslID]=RSTnew[gmslID];
//                    //TEST TAB FOR CHECKING RST0 INITIALIATON: Prints only RST events
//                    cout<<ncount<<"\t"<<gmslID<<"\t"<<channel<<"\t"<<RSTcountcc<<"\t"<<buffer[j*8+3]+256*buffer[j*8+2]+256*256*buffer[j*8+1]+256*256*256*(buffer[j*8+0]%4)<<endl;
                }
                /////.........
                //////Assigning the RSTcountcc for the given RST event after T0 correction
                if(buffer[j*8+3]+256*buffer[j*8+2]+256*256*buffer[j*8+1]+256*256*256*(buffer[j*8+0]%4)>RSTcountT0[gmslID])
                {
                    RSTcountcc=buffer[j*8+3]+256*buffer[j*8+2]+256*256*buffer[j*8+1]+256*256*256*(buffer[j*8+0]%4)-RSTcountT0[gmslID];
                }
                if(buffer[j*8+3]+256*buffer[j*8+2]+256*256*buffer[j*8+1]+256*256*256*(buffer[j*8+0]%4)<RSTcountT0[gmslID])
                {
                    RSTcountcc=buffer[j*8+3]+256*buffer[j*8+2]+256*256*buffer[j*8+1]+256*256*256*(buffer[j*8+0]%4)+67108864-RSTcountT0[gmslID]; //2^26=67108864
                }
                //.........
                /////TEST TAB FOR CHECKING RST0 INITIALIATON: Prints ALL RST events within first 10,240 events
                if(ncountall<10240)
                {
                    cout<<ncountall<<"\t"<<gmslID<<"\t"<<channel<<"\t"<<RSTcountcc<<"\t"<<buffer[j*8+3]+256*buffer[j*8+2]+256*256*buffer[j*8+1]+256*256*256*(buffer[j*8+0]%4)<<endl;
                }
                
                ///RST HISTOGRAMS FOR CHECKS
                if(ncountall>=2048)
                {
                    if(gmslID==0)
                    {   //cout<<RSTcountccStored<<"\tRST count="<<RSTcountcc<<"\t GID="<<gmslID<<"\t RSTdelta="<<RSTcountcc-oldRSTcountcc0<<endl;
                        hRSTG0->Fill(RSTcountcc);
                        hRSTdeltaG0 ->Fill((RSTcountcc-oldRSTcountcc0)/10);
                        oldRSTcountcc0=RSTcountcc;
                        RSTcountccStored[gmslID]++;
                    }
                    if(gmslID==1)
                    {//cout<<RSTcountccStored<<"\tRST count="<<RSTcountcc<<"\t GID="<<gmslID<<"\t RSTdelta="<<RSTcountcc-oldRSTcountcc0<<endl;
                        hRSTG1->Fill(RSTcountcc);
                        hRSTdeltaG1->Fill((RSTcountcc-oldRSTcountcc1)/10);
                        oldRSTcountcc1=RSTcountcc;
                        RSTcountccStored[gmslID]++;
                    }
                    if(gmslID==2)
                    {//cout<<RSTcountccStored<<"\tRST count="<<RSTcountcc<<"\t GID="<<gmslID<<"\t RSTdelta="<<RSTcountcc-oldRSTcountcc2<<endl;
                        hRSTG2->Fill(RSTcountcc);
                        hRSTdeltaG2->Fill((RSTcountcc-oldRSTcountcc2)/10);
                        oldRSTcountcc2=RSTcountcc;
                        RSTcountccStored[gmslID]++;
                    }
                    if(gmslID==3)
                    {//cout<<RSTcountccStored<<"\tRST count="<<RSTcountcc<<"\t GID="<<gmslID<<"\t RSTdelta="<<RSTcountcc-oldRSTcountcc0<<endl;
                        hRSTG3->Fill(RSTcountcc);
                        hRSTdeltaG3->Fill((RSTcountcc-oldRSTcountcc3)/10);
                        oldRSTcountcc3=RSTcountcc;
                        RSTcountccStored[gmslID]++;
                    }
                }
                 ///RST HISTOGRAMS FOR CHECKS
                
                hRST->Fill(RSTcountcc);
                RSTLOR=(RSTcountcc)/10000; //The global time to be used in LOR
                RSTEventcc[gmslID]=RSTcountcc;
            }
            /////////RESET COUNTER VALUES, GMSL ASSIGNMENTS: END
            
            if((buffer[j*8+0]/4+buffer[j*8+7]*256/4+(buffer[j*8+6]%2)*256*256/4)!=32767)                                    //////////////first if for input cm[][]
            {
                ecoarse=m_lut[ buffer[j*8+3]/32+buffer[j*8+2]*256/32+(buffer[j*8+1]%16)*256*256/32 ];
                tcoarse=m_lut[ (buffer[j*8+0]/4+buffer[j*8+7]*256/4+(buffer[j*8+6]%2)*256*256/4) ];
                efine= buffer[j*8+3]%32;
                tfine= buffer[j*8+1]/32+(buffer[j*8+0]%4)*256/32;
                channel=buffer[j*8+6]/4;
                frame=buffer[j*8+5]%64;
                tbad=(buffer[j*8+6]/2)%2;
                ebad=(buffer[j*8+1]/16)%2;
                sticID=buffer[j*8+5]/64;
                gmslID=buffer[j*8+4]%4;
                boardID=(buffer[j*8+4]/4)%32;
                Data0_Sync1=buffer[j*8+4]/128;
                RSTEventff[gmslID]=efine;
                //RSTcountcc=oldRSTcountcc;
                RSTcountcc=RSTEventcc[gmslID]+RSTEventff[gmslID]; //qqq
                ////////TIME-ENERGY CALCULATION
                //energy=ecoarse-tcoarse;
                if(ecoarse>tcoarse){energy=ecoarse-tcoarse;}
                if(ecoarse<tcoarse){energy=ecoarse+32766-tcoarse;}
                if(energy<=xmin||energy>=xmax){energy=-1;}
                time=32*tcoarse+tfine;
                ncountc=ncount%cmsize;
                
                cm[ncountc][0]=tcoarse; //channel
                cm[ncountc][1]=sticID;
                cm[ncountc][2]=boardID;
                cm[ncountc][3]=frame;
                cm[ncountc][4]=channel; //time
                cm[ncountc][5]=energy;
                cm[ncountc][6]=ncount;
                cm[ncountc][7]=time;
                cm[ncountc][8]=gmslID;
                //cm[ncountc][9]=RSTEventcc[gmslID]+RSTEventff[gmslID]; //qqq
                cm[ncountc][9]=RSTEventcc[gmslID]; //qqq
                cm[ncountc][10]=RSTEventff[gmslID]; //qqq
                if(energy>xmin&&energy<xmax)
                {ncountGood++;ncountGoodBrdGmslStic[0][gmslID][sticID]++;
                  
                    //UNCOMMENT TO SAVE SINGLES FILE
                   endt=(RSTEventcc[gmslID]*1.0)/10000;
                   caE=CalE(energy,gmslID,sticID,channel,par);
                    if((gmslID==0)||(gmslID==1)){
						 if((caE>=Eth)){//&&(Kill1[gmslID][sticID][channel]==1)){						
							hSingle0->Fill(mapxAll(channel,sticID,gmslID),mapyAll(channel,sticID,gmslID));      //////////////////////single map
						}
					}
					if((gmslID==2)||(gmslID==3)){
						if((caE>=Eth)){//&&(Kill1[gmslID][sticID][channel]==1)){
							hSingle1->Fill(mapxAll(channel,sticID,gmslID),mapyAll(channel,sticID,gmslID));
						}
					}
					
					hT1->Fill(1.0*(RSTEventcc[gmslID])/10000.0);
					
                } //Acceptable energy events xmin<Energy<xmax
                 
                if(ncountc==cmsize-1) 																																																				////// if coincidences start
                {
                    ////////////////////////////////////////////////////////
                    //////////////COINCIDENCE BLOCK: INPUTS cm[][], windowtcc, windowevents, clustersize,  hCoin1, hCoin2, hCoin3, h2Map, hCoinAB
                    ////////////////////////////////////////////////////////
                    std::qsort(cm, cmsize,cmwidth*sizeof(int), compare4); //SORT: FRAMES/GMSL/EVENTID
                    //0tcc,1sid,2board,3frame,4ch,5energy, 6eventRecNo, 7time, 8gmslid
                    //COINCIDENCE LOOP START
                    //while(CFtrans==0&&Cevent<eventsTOTAL&&Cframe<15) //
                    //identify and store the start of the frame, the GMSL change and end of frame locations
                    while(CFtrans==0&&Cevent<cmsize&&Cframe<15) //Cframe<15
                    {
                        Cframe=cm[Cevent][3];CgmslID=cm[Cevent][8]/2;CStrans=0;CgmslIDold=CgmslID;
                        CeventStart1=Cevent;
                        //cout<<"CeventStart1="<<CeventStart1<<endl;
                        while(CStrans==0)
                        {
                            //Keeps CgmslID=0 for one set of GMSL (0,1); and 1 for another set GMSL(2,3)
                            CgmslID=cm[Cevent][8]/2;
                            if(CgmslID==CgmslIDold){
                                //cout<<cm[Cevent][1];
                                CStrans=0; Cevent++;
                            }
                            else {CStrans=1; }
                        }
                        CeventStart2=Cevent+1;
                        //cout<<"CeventStart2="<<CeventStart2<<endl; //END OF THE BOARD1; SAME FRAME
                        CStrans=0; //reset the CStrans flag
                        CgmslID=cm[Cevent][8]/2; CgmslIDold=CgmslID;
                        while(CStrans==0)
                        {
                            CgmslID=cm[Cevent][8]/2;
                            if(CgmslID==CgmslIDold){
                                //cout<<cm[Cevent][1];
                                CStrans=0; Cevent++;
                            }
                            else {CStrans=1; }
                        }
                        CStrans=0; //reset the CStrans flag
                        //cout<<endl;
                        CeventEnd=Cevent;
                        //cout<<"CeventEnd="<<CeventEnd<<endl;   //END OF THE BOARD2 AND FRAME
                        //Using the frame start and end, and both boards start and end; coincidences are searched
                        //0TCC,1S-ID,2BRD,3FRAME,4CH,5ENERGY, 6EVENTRECNO, 7TIME, 8 G-ID
                        for(int i1=CeventStart1; i1<CeventStart2;  i1++)                     																										///// 2 loops for coincidences comparison  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
                        {
                            //cout<<"i="<<i<<endl;
                            for(int j1=CeventStart2; j1<CeventEnd+1; j1++)
                            {
                                //cout<<"j="<<j;
                                //MAIN SELECTION CRITERIA windowtcc and windowEvents
                                //if( (abs(cm[i][0]-cm[j][0])<=windowTcc)&&(abs(cm[i][6]-cm[j][6])<=windowEvents) ) //512
                                //PHOTOPEAK SELECTION ****
                                //if( (abs(cm[i1][0]-cm[j1][0])<=windowTcc)&&(abs(cm[i1][6]-cm[j1][6])<=windowEvents)&&
//                                    (cm[i1][5]>=ELChA[cm[i1][8]][cm[i1][1]][cm[i1][4]])&&
//                                    (cm[i1][5]<=EHChA[cm[i1][8]][cm[i1][1]][cm[i1][4]])&&
//                                    (cm[j1][5]>=ELChB[cm[j1][8]][cm[j1][1]][cm[j1][4]])&&
//                                    (cm[j1][5]<=EHChB[cm[j1][8]][cm[j1][1]][cm[j1][4]])
//                                   )
                                //ALL ENERGY SELECTION (REJECT ABNORMAL E values)****
                                if( (abs(cm[i1][0]-cm[j1][0])<=windowTcc)&&(abs(cm[i1][6]-cm[j1][6])<=windowEvents)&&                                                        ////First condition for coincidences
                                        (cm[i1][5]>=xmin)&&
                                        (cm[i1][5]<=xmax)&&
                                        (cm[j1][5]>=xmin)&&
                                        (cm[j1][5]<=xmax)
                                       )
                                {
                                    //SECONDARY CRITERIA TO ELIMINATE CLUSTERS FROM SAME MODULE
                                    if( ((cm[i1][0]-oldtcc1)<windowcluster)||((cm[j1][0]-oldtcc2)<windowcluster) ) {cluster=1;} //eliminate cluster events                   // Second condition for coincidences $$$$$ this need to change
                                    else
                                    {   cluster=0;
                                        x1=mapxAll(cm[i1][4],cm[i1][1],cm[i1][8]);
                                        x2=mapxAll(cm[j1][4],cm[j1][1],cm[j1][8]);
                                        y1=mapyAll(cm[i1][4],cm[i1][1],cm[i1][8]);
                                        y2=mapyAll(cm[j1][4],cm[j1][1],cm[j1][8]);
                                        //cout<<"xmap1,2="<<x1<<","<<x2; cout<<"ymap1,2="<<y1<<","<<y2; cout<<endl;
                                        //dtime SELECTION FOR MAPPING
                                        if((cm[i1][7]-cm[j1][7])<500&&((cm[i1][7]-cm[j1][7])>-500))//FULL COINCIDENCE BAND:-300to300                /////////+-300 original///////Third condition for coincidences///time window////////////////////Coincident condition of 12 ns 50ps each unit
                                            //if((cm[i1][7]-cm[j1][7])<15&&((cm[i1][7]-cm[j1][7])>-5)) //POS L-81MM
                                        {
                                            //WRITE LOR VALUES TO THE OUTPUT TXT FILE.
                                            //Basic pos info, tAbs
                                            //outfileLOR<<x1<<"\t"<<y1<<"\t"<<x2<<"\t"<<y2<<"\t"<<RSTLOR<<endl;
                                            //LOR with full info: for both channels in coincidence from opp modules
                                            //X1,Y1,X2,Y2, timeabs(s), RSTcoarse1, RSTfine1, RSTcoarse2, RSTfine2, eventNo1, eventNo2
                                              hCW->Fill(cm[i1][7]-cm[j1][7]);                                     
                                            CaE1=CalE(cm[i1][5],cm[i1][8],cm[i1][1],cm[i1][4],par);
                                            CaE2=CalE(cm[j1][5],cm[j1][8],cm[j1][1],cm[j1][4],par);                                         
                                            if((CaE1>=Eth)&&(CaE2>=Eth)){//&&(Kill1[cm[j1][8]][cm[j1][1]][cm[j1][4]]==1)&&(Kill1[cm[i1][8]][cm[i1][1]][cm[i1][4]]==1)){
												h2Map->Fill((x1+x2)/2.0,(y1+y2)/2.0) ;
												hPX->Fill((x1+x2)/2.0);
												hPY->Fill((y1+y2)/2.0);			
												if((cm[i1][8]==0)||(cm[i1][8]==1)){
													hLOR1->Fill(x1,y1);
													hLOR2->Fill(x2,y2);
													hCWi[int(int(x1/4)+8*int(y1/8))][int(int(x2/4)+8*int(y2/8))]->Fill(cm[i1][7]-cm[j1][7]);	
												}else{
													hLOR1->Fill(x2,y2);
													hLOR2->Fill(x1,y1);	
													hCWi[int(int(x2/4)+8*int(y2/8))][int(int(x1/4)+8*int(y1/8))]->Fill(cm[i1][7]-cm[j1][7]);	
												}
												hTC[cm[i1][8]][cm[i1][1]]->Fill(RSTLOR);
												hTC[cm[j1][8]][cm[j1][1]]->Fill(RSTLOR);
												hTCi[cm[i1][8]][cm[i1][1]][cm[i1][4]]->Fill(RSTLOR);
												hTCi[cm[j1][8]][cm[j1][1]][cm[j1][4]]->Fill(RSTLOR);
																																						////Coincident map
												h2tY->Fill(float(RSTLOR),(y1+y2)/2.0 );
												h2tX->Fill(float(RSTLOR),(x1+x2)/2.0);
												hT2->Fill(float(RSTLOR));
										//		outL<<cm[i1][8]<<"\t"<<x1<<"\t"<<y1<<"\t"<<cm[j1][8]<<"\t"<<x2<<"\t"<<y2<<"\t"<<RSTLOR<<endl;
											}
                                            ///Filling time framed maps.
                                            int tf=cm[i1][9]/frameTime/10000;
                                            if(tf<nframes)
                                            {h2Mapt[tf]->Fill((x1+x2)/2,(y1+y2)/2); //TFRAMES
                                            }
                                            if(tf>=nframes){cout<<"tf exceeded array size\n BAD RST VALUE="<<cm[i1][9]<<"/10000"<<endl;
                                                //return 0;
                                            }
                                            ncoin++;//increases the coincidence count
                                        }//Ensures the coincidences are within  this peak of the deltatime histogram
                                        //0tcc,1sid,2board,3frame,4ch,5energy, 6eventRecNo, 7time, 8 gid
                                        //ALL CHANNEL 2D COINCIDENCE MAP - DIRECT DETECTOR IMAGE FOR AI
                                        lch=cm[i1][4] + 64*( (cm[i1][1]) + (cm[i1][8])*4);//map the gid and sid to generate a number 1 to9
                                        rch=cm[j1][4] + 64*( (cm[j1][1]) + (cm[j1][8])*4) - 512;//map the gid and sid to generate a number 1 to9
                                        //cout<<"lch"<<lch<<"g"<<cm[i][8]<<"s"<<cm[i][1]<<"\t|\t  rch"<<rch<<"g"<<cm[j][8]<<"s"<<cm[j][1]<<endl;
                                        //FILL HISTOGRAMS
                                        h3Coin->Fill(lch,rch); //Channel Hit Map of Coincidences
                                        h2Coin->Fill(cm[i1][4],cm[j1][4]); //Channel Hit Map of Coincidences
                                        //h1Coin->Fill(cm[i1][7]-cm[j1][7]); //Time difference histogram!
                                        hdEvents->Fill(cm[i1][6]-cm[j1][6]); //Event-event distance
                                    }
                                    oldtcc1=cm[i1][0];
                                    oldtcc2=cm[j1][0];
                                }// END IF COINCIDENCE CONDITION CLEAR
                            } //END FOR j (B1: G2,G3)
                            //cout<<endl;
                        }//END FOR i (B0: G0,G1)
                        //                         cout<<"********************"<<endl;
                        //                         cout<<"End of frame"<<Cframe<<"\t Last event No:"<<CeventEnd<<endl;
                        //                         cout<<"********************"<<endl;
                        Cevent=CeventStart2+1; CFtrans=0; CStrans=0;
                        Cframeold=Cframe;
                        Cframe=cm[Cevent][3];//CHECK THIS MISTER:Cevent+1 has to be a transition of the GMSLset2(2,3), so frame should change
                        //cout<<"WHILE ONE FRAME ROUND ENDS Cevent:"<<Cevent<<endl;
                    }//while loop for COINCIDENCE ALL FRAMES
                    Cframe=0, Cframeold=0, Cevent=0, CeventEnd=0,CStrans=0, CFtrans=0;
                    //COINCIDENCE LOOP END
                    nloopc++; //cout<<"***************************************************COINCIDENCE BLOCK"<<nloopc<<endl;
                    ncountc=cmsize-1;
                }
                
                
                /////// FILLING TREE - HISTOGRAMS
           //     tree->Fill();
            //    hEc->Fill(ecoarse);
           //     hTc->Fill(tcoarse);
           //     hTf->Fill(tfine);
           //     hEf->Fill(efine);
            //    hT->Fill(time);
                
          //      hEbad->Fill(ebad);
           //     hTbad->Fill(tbad);
                
          //      hFrame->Fill(frame);
          //      hCh->Fill(channel);
           //     hSticID->Fill(sticID);
          //      hGMSLID->Fill(gmslID);
          //      hBoardID->Fill(boardID);
          //      h2EventEnergy->Fill(ncount,energy);
                //hGMSLID2->Fill(gmslID);
                //hE[gmslID][sticID][channel]->Fill(energy);
           //     h2SticIDGMSLID->Fill(gmslID,sticID);
                ncount++; ncountBrdGmslStic[0][gmslID][sticID]++; //ALL counts
            } // END IF/ELSE FOR RST/EVENT sel
        }// END DUMMY FOR
        //ncount++; ncountBrdGmslStic[0][gmslID][sticID]++; //ALL counts
        ncountall++;
    }// END WHILE
   char fhistname[100];
    
	////////////
	
	TCanvas *cTi[4][4][4];
	TCanvas *cTg[4][4];
    for(int g=0; g<4; g++)
    {
        for(int s=0;  s<4; s++)
        {
            
            for(int j=0; j<4; j++)
            {
                sprintf(hname,"Timei_G%dS%d-ChSet%d",g,s,j);													//// TCanvas name
                cTi[g][s][j] = new TCanvas(hname,hname,1000,1000);
                
                int nx=4, ny=4;
                int number=0;
                cTi[g][s][j]->Divide(nx,ny,0,0);
                for(int i=0; i<nx*ny; i++)
                {
                    number++;
                    cTi[g][s][j]->cd(number);
                    //h1->FillRandom("gaus",1000);
                    //hE[47+i]->GetXaxis()->SetXLimits();
                    //double xmin=0, xmax=800;
                  //  hTCi[g][s][16*j+i]->SetAxisRange(xmin, xmax,"X");
                    hTCi[g][s][16*j+i]->GetXaxis()->SetLabelFont(53);
                    hTCi[g][s][16*j+i]->GetXaxis()->SetLabelSize(10);
                    hTCi[g][s][16*j+i]->GetXaxis()->SetNdivisions(5);
                    hTCi[g][s][16*j+i]->GetYaxis()->SetLabelFont(53);
                    hTCi[g][s][16*j+i]->GetYaxis()->SetLabelSize(10);
                    hTCi[g][s][16*j+i]->GetYaxis()->SetNdivisions(5);
                    hTCi[g][s][16*j+i]->DrawCopy();
                }
                cTi[g][s][j]->Write();
                sprintf(hnameE,"%s/TBOX/%s.png",newFolder,hname);
                cTi[g][s][j]->SaveAs(hnameE);
                sprintf(hnameE,"%s/TBOX/%s.pdf",newFolder,hname);
                cTi[g][s][j]->SaveAs(hnameE);
                sprintf(hnameE,"%s/TBOX/%s.root",newFolder,hname);
                cTi[g][s][j]->SaveAs(hnameE);
            }
            
            sprintf(fhistname,"TimeG_G%dS%d",g,s);													//// TCanvas name
            cTg[g][s] = new TCanvas(fhistname,fhistname,1000,1000);
            hTC[g][s]->GetXaxis()->SetLabelFont(53);
            hTC[g][s]->GetXaxis()->SetLabelSize(10);
            hTC[g][s]->GetXaxis()->SetNdivisions(5);
            hTC[g][s]->GetYaxis()->SetLabelFont(53);
            hTC[g][s]->GetYaxis()->SetLabelSize(10);
            hTC[g][s]->GetYaxis()->SetNdivisions(5);
            hTC[g][s]->DrawCopy();
            cTg[g][s]->Write();
            sprintf(hnameE,"%s/TBOX/%s_Group.png",newFolder,fhistname);
            cTg[g][s]->SaveAs(hnameE);
            sprintf(hnameE,"%s/TBOX/%s_Group.pdf",newFolder,fhistname);
            cTg[g][s]->SaveAs(hnameE);
            sprintf(hnameE,"%s/TBOX/%s_Group.root",newFolder,fhistname);
            cTg[g][s]->SaveAs(hnameE);
        }
    }
   
   
    fclose(ptr);
   // outL.close();
    cout<<"File reading end"<<endl;
   
    cout<<"Single writing end"<<endl;
   
    cout<<"LOR writing end"<<endl;
    //WRITING INTO TREE..
    cout<<"Writing to tree"<<endl;
    fileRoot_tree->Write();
    
    cout<<"Total Data events="<<ncount<<endl;
    cout<<"Total analyzed events"<<cmsize*nloopc<<"\tTotal good events analyzed="<<ncountGood<<endl;
    cout<<"Percentage of good events="<<float(ncountGood)/float(cmsize)*100<<endl;
    cout<<"Percentage of events analyzed to total collected="<<float(cmsize)/float(ncount)*100<<endl;
    cout<<"The End"<<endl;
    cout<<"Total measured time (sec):   "<<endt<<endl;

  
    

    ////////////// ////////////// ////////////// ////////////// ////////////// ////////////// ////////////// ////////////// //////////////
    //////////////////HISTOGRAMS OF EVENTS, DISTRIBUTIONS, COINCIDENCES ETC
    ////////////// ////////////// ////////////// ////////////// ////////////// ////////////// ////////////// ////////////// //////////////
 //   char fhistname[100];
    

    
    //sprintf(fhistname,"EBOX/h2Map.pdf",windowTcc, windowEvents,windowcluster);
    
    
    gStyle->SetOptStat(55);
    TCanvas *c4 = new TCanvas("c4","c4",1000,500);
    h2Map->Draw("colz");
    h2Map->GetXaxis()->SetTitle("X channels (3.2mm.Ch-1)");
    h2Map->GetYaxis()->SetTitle("Y channels (3.2mm.Ch-1)");
    char pdfname[100]; 
    sprintf(pdfname,"%s/2DCoinicident.png",newFolder);
    c4->SaveAs(pdfname);
     sprintf(pdfname,"%s/2DCoinicident.root",newFolder);
    c4->SaveAs(pdfname);
   
    
    TCanvas *c2tX = new TCanvas("c2tX","c2tX",1000,500);
    h2tX->Draw("CONT4");
    h2tX->GetXaxis()->SetTitle("Second");
    h2tX->GetYaxis()->SetTitle("X channels (3.2mm.Ch-1)");
    sprintf(pdfname,"%s/tX.png",newFolder);
    c2tX->SaveAs(pdfname);
 //   c2tX->Write();
    
    TCanvas *c2tY = new TCanvas("c2tY","c2tY",1000,500);
    h2tY->Draw("CONT4");
    h2tY->GetXaxis()->SetTitle("Second");
    h2tY->GetYaxis()->SetTitle("Y channels (3.2mm.Ch-1)");
    sprintf(pdfname,"%s/tY.png",newFolder);
    c2tY->SaveAs(pdfname);
//    c2tY->Write();
    
    TCanvas *cSall= new TCanvas("cSall","cSall",1000,1000);
    cSall->Divide(1,2);
    cSall->cd(1);
    hSingle0->Draw("colz");
    cSall->cd(2);
    hSingle1->Draw("colz");
    sprintf(pdfname,"%s/SingleAll_v1.png",newFolder);
    cSall->SaveAs(pdfname);
    sprintf(pdfname,"%s/SingleAll_v1.root",newFolder);
    cSall->SaveAs(pdfname);
//    cSall->Write();
    
     TCanvas *cT1 = new TCanvas("cT1","cT1",1000,500);
    hT1->Draw();
    hT1->GetXaxis()->SetTitle("Second");
    sprintf(pdfname,"%s/TimeLine_Single.png",newFolder);    
    cT1->SaveAs(pdfname);
    sprintf(pdfname,"%s/TimeLine_Single.root",newFolder);    
    cT1->SaveAs(pdfname);
    
    TCanvas *cCW = new TCanvas("cCW","cCW",1000,500);
    hCW->Draw();
    hCW->GetXaxis()->SetTitle("1uint = 50ps");
    sprintf(pdfname,"%s/Coincident_window.png",newFolder);    
    cCW->SaveAs(pdfname);
    sprintf(pdfname,"%s/Coincident_window.root",newFolder);    
    cCW->SaveAs(pdfname);
    
    Double_t maxa1=0,maxa2=0;
    Double_t TWM[16][16][7][2]= {0};
    
for(int g=0; g<16; g++)																																							////Coincidence window selection
    {
        for(int s=0;  s<16; s++)
        {		
		 maxa2= hCWi[g][s]->GetEntries();
		 if(maxa1<=maxa2){
			 maxa1=maxa2;			  
		}
		//Peak finding code goes here#########################################################	 
			TSpectrum *sp = new TSpectrum();
                sp->Search( hCWi[g][s],1 ,"",sigT); //option: gon //0.05 toomany peaks, 1 for 1D spectrum and 0.05 for the expected sigma of the peaks
                sp->SetResolution();
                int a = sp->GetNPeaks(); //No of peaks
                if(a<=7){
                Double_t *b = sp->GetPositionX(); //x vector
                Double_t *c = sp->GetPositionY(); //y vector
                 int parray[a][2]; //2D array to  store the x and y values for each of the peaks
                //cout<<"Before sorting: X,Y"<<endl;
                for(int j=0; j<a; j++) //assign x,y  to an array
                {
                    parray[j][0]=b[j];//b[j] is X
                    parray[j][1]=c[j];//c[j] is Y
                    cout<< parray[j][0]<<"\t"<< parray[j][1]<<endl;
                }
                
                  for(int j=0; j<a; j++) //assign x,y  to an array
				{
					double PPleft = float(parray[j][0])-10;
					double PPright = float(parray[j][0])+10;
					TF1 *fG = new TF1("fG","[0]*exp(-0.5*((x-[1])/[2])^2)",PPleft,PPright);
				
					fG->SetParameters(10,parray[j][0],2.0);
					if(j==0){
						hCWi[g][s]->Fit(fG,"RQ");
					}else{
						hCWi[g][s]->Fit(fG,"RQ+");
					}
					cout<<g<<"\t"<<s<<"\t"<<j<<"\t"<<fG->GetParameter(1)<<"\t"<<fG->GetParameter(0)<<"\t"<<fG->GetParameter(2)<<endl;
					
					TWM[g][s][j][0]=fG->GetParameter(1);
					TWM[g][s][j][1]=fG->GetParameter(2);
					
				}
					
                }
                
			}
	
}
    Double_t const maxc=maxa1;
    
    
    TH1F *hCC =  new TH1F("hCC","Coincidences contrain",100,0,maxc);
    
    
    
       TCanvas *cWi[16][16];
    for(int g=0; g<16; g++)
    {
        for(int s=0;  s<16; s++)
        {		
				hCC->Fill(hCWi[g][s]->GetEntries());
				sprintf(hname,"Coincidence_Constrain_%d_%d",g,s);													//// TCanvas name
                cWi[g][s] = new TCanvas(hname,hname,1000,1000);
                hCWi[g][s]->Draw();
                 hCWi[g][s]->GetXaxis()->SetTitle("1uint = 50ps");
				sprintf(pdfname,"%s/CoinConstrain/Coincident_%d_%d.png",newFolder,g,s);    
				cWi[g][s]->SaveAs(pdfname);
				sprintf(pdfname,"%s/CoinConstrain/Coincident_%d_%d.root",newFolder,g,s);     
				cWi[g][s]->SaveAs(pdfname);
                
		}
	}
    ///////////////////////////////////////////////// out time window
    
     ofstream out;
   sprintf(pdfname,"%s/Time_Window_B0_B1.txt",newFolder);
	out.open(Form(pdfname));

for(int g=0; g<16; g++)
    {
        for(int s=0;  s<16; s++)
        {	
			out<<g<<"\t"<<s<<"\t";
			  for(int j=0; j<7; j++) //assign x,y  to an array
				{
				out<<TWM[g][s][j][0]<<"\t"<<TWM[g][s][j][1]<<"\t";
			}
			out<<endl;
	}
}
    
    /////////////////////////////////////////////////////////////
    
    
    
    
    TCanvas *cCC = new TCanvas("cCC","cCC",1000,500);
    hCC->Draw();
    hCC->GetXaxis()->SetTitle("cps");
    sprintf(pdfname,"%s/Coincident_constrain.png",newFolder);    
    cCC->SaveAs(pdfname);
    sprintf(pdfname,"%s/Coincident_constrain.root",newFolder);    
    cCC->SaveAs(pdfname);
    
    
    TCanvas *cT2 = new TCanvas("cT2","cT2",1000,500);
    hT2->Draw();
    hT2->GetXaxis()->SetTitle("Second");
    sprintf(pdfname,"%s/TimeLine_Coincident.png",newFolder);    
    cT2->SaveAs(pdfname);
    sprintf(pdfname,"%s/TimeLine_Coincident.root",newFolder);    
    cT2->SaveAs(pdfname);
 
     //////FRAMES
    for(int i=0; i<nframes; i++)
    {
       // TCanvas *c2Mapt = new TCanvas("c2Mapt","c2Mapt",1000,500);
        h2Mapt[i]->Write();
//        sprintf(h2MaptTitle,"EBOX/MAPS_Timeframe/%d.pdf",i);
//        c2Mapt->SaveAs(h2MaptTitle);
     //   sprintf(h2MaptTitle,"%s/MAPS_Timeframe%d.jpg",i);
      //  c2Mapt->SaveAs(h2MaptTitle);
        //c2Mapt->Write();
    }

    TCanvas *cPall= new TCanvas("cPall","cPall",1000,1000);
    cPall->Divide(1,2);
    cPall->cd(1);
    hLOR1->Draw("colz");
    cPall->cd(2);
    hLOR2->Draw("colz");
    sprintf(pdfname,"%s/LOR_v1.png",newFolder);
    cPall->SaveAs(pdfname);
    sprintf(pdfname,"%s/LOR_v1.root",newFolder);
    cPall->SaveAs(pdfname);

    h2Coin->Write();
    h1Coin->Write();
    h2Map->Write();
    hT1->Write();
    hT2->Write();
    hdEvents->Write();
    hRST->Write();
   hLOR1->Write();
   hLOR2->Write();
    hPX->Write();
    hPY->Write();
   
  
   
   
  
   out.close();
   
 
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    //DELETING HISTOGRAMS
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    delete hEc;
    delete  hTc; delete hTf; delete hEf; delete hT; delete hEbad; delete hTbad; delete hFrame; delete hCh; delete hSticID; delete hGMSLID; delete hBoardID; delete h2EventEnergy;  delete h2SticIDGMSLID;
    //delete hE;
    //delete  h1PPamp, h1PPmean;
    delete hPPamp0; delete hPPamp1; delete hPPmean0; delete hPPmean1;
    

    
    return 0;
}
///////////THE END







