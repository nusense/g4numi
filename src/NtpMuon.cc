
// C++
#include <iostream>
#include <string>
#include <stdio.h>
#include <sstream>

// ROOT



// Local
#include "NtpMuon.hh"

using namespace std;

//------------------------------------------------------------------------------------------
NtpMuon::NtpMuon()
   :evtno(-999),
    muvx(-999.0),
    muvy(-999.0),
    muvz(-999.0),
    mupx(-999.0),
    mupy(-999.0),
    mupz(-999.0),
    muweight(-999.0),
    tpx(-999.0),
    tpy(-999.0),
    tpz(-999.0),
    tptype(-999),
    nimpwt(-999.0),
    ptype(-999),
    evtnoS(65535),
    muvxF(-999.0),
    muvyF(-999.0),
    muvzF(-999.0),
    mupxF(-999.0),
    mupyF(-999.0),
    mupzF(-999.0),
    tpxF(-999.0),
    tpyF(-999.0),
    tpzF(-999.0),
    tptypeS(-999),
    ptypeS(-999),
    ftree(0)
{
}

//------------------------------------------------------------------------------------------
NtpMuon::~NtpMuon()
{
}

//------------------------------------------------------------------------------------------
bool NtpMuon::SetTree(TTree* tree)
{
   cout << "NtpMuon::SetTree executed" << endl;
   
   ftree = tree;
   SetBranchesStatus();
   return SetBranches();
}

//------------------------------------------------------------------------------------------
void NtpMuon::SetBranchesStatus()
{
//   cout << "NtpMuon::SetBranchesStatus executed" << endl;

   
   ftree -> SetBranchStatus("evtno", 1, 0);
   ftree -> SetBranchStatus("muvx", 1, 0);
   ftree -> SetBranchStatus("muvy", 1, 0);
   ftree -> SetBranchStatus("muvz", 1, 0);
   ftree -> SetBranchStatus("mupx", 1, 0);
   ftree -> SetBranchStatus("mupy", 1, 0);
   ftree -> SetBranchStatus("mupz", 1, 0);
   ftree -> SetBranchStatus("muweight", 1, 0);
   ftree -> SetBranchStatus("tpx", 1, 0);
   ftree -> SetBranchStatus("tpy", 1, 0);
   ftree -> SetBranchStatus("tpz", 1, 0);
   ftree -> SetBranchStatus("tptype", 1, 0);
   ftree -> SetBranchStatus("nimpwt", 1, 0);
   ftree -> SetBranchStatus("ptype", 1, 0);
   
}



//------------------------------------------------------------------------------------------
bool NtpMuon::SetBranches()
{
//   cout << "NtpMuon::SetBranches executed" << endl;

      
   SetBranch(&evtnoS, "evtno");
   SetBranch(&muvxF, "muvx");
   SetBranch(&muvyF, "muvy");
   SetBranch(&muvzF, "muvz");
   SetBranch(&mupxF, "mupx");
   SetBranch(&mupyF, "mupy");
   SetBranch(&mupzF, "mupz");
   SetBranch(&muweight, "muweight");
   SetBranch(&tpxF, "tpx");
   SetBranch(&tpyF, "tpy");
   SetBranch(&tpzF, "tpz");
   SetBranch(&tptypeS, "tptype");
   SetBranch(&nimpwt, "nimpwt");
   SetBranch(&ptypeS, "ptype");

   return true;
}

//------------------------------------------------------------------------------------------
bool NtpMuon::SetBranch(void *var, const std::string &name)
{
//   cout << "NtpMuon::SetBranch executed" << endl;

   
   if (!ftree)
   {
      return false;
   }
   
   /* TBranch *branch = ftree -> GetBranch(name.c_str());
   
   if(!branch)
   {
      return false;
   }

   if(fBranch.find(name) != fBranch.end())
   {
      cerr << "NtpMuon::SetBranch - Branch " << name << " already set" << endl;
      return false;
   }

   //cout<<"branch = "<<branch<<endl;
    fBranch[name] = branch;
    if (fBranch.find(name)!= fBranch.end())
    {
       //cout<< "found branch "<< name<<endl;
    }
    //cout<< "1no of leaves = " << fBranch[name] -> GetNleaves() << endl;
    */

    ftree -> SetBranchAddress(name.c_str(), var);

/*    char* str = ftree -> GetTree() -> GetBranch(name.c_str()) -> GetAddress();
    
    stringstream straddress;
    straddress << str << 0;
    cout<< "0 branch "<<name <<" address= "<< straddress.str() <<endl;
   cout<< name << "  = "<< var << endl;
*/
   //cout<< "no of leaves = " << fBranch[name] -> GetNleaves() << endl;
   
  

   //cout<< "1 branch Ntype address= "<< fBranch["run"] -> GetAddress() <<endl;

   

   return true;
}

//------------------------------------------------------------------------------------------
int NtpMuon::GetNEntries()
{
   cout << "NtpMuon::GetNEnetries executed" << endl;

   return ftree -> GetEntries();
}

//------------------------------------------------------------------------------------------
void NtpMuon::GetEntry(unsigned int entry)
{
   //cout << "NtpMuon::FillTree executed" << endl;

   ftree -> GetEntry(entry);

   evtno = (Int_t)evtnoS;
   muvx = (Double_t)muvxF;
   muvy = (Double_t)muvyF;
   muvz = (Double_t)muvzF;
   mupx = (Double_t)mupxF;
   mupy = (Double_t)mupyF;
   mupz = (Double_t)mupzF;
   tpx = (Double_t)tpxF;
   tpy = (Double_t)tpyF;
   tpz = (Double_t)tpzF;
   tptype = (Int_t)tptypeS;
   ptype = (Int_t)ptypeS;
    
}

//------------------------------------------------------------------------------------------
void NtpMuon::GetBranchEntry(std::string name, unsigned int entry)
{
   cout << "NtpMuon::FillBranch executed" << endl;


  ftree -> GetBranch(name.c_str()) -> GetEntry(entry);
  
}




//------------------------------------------------------------------------------------------
void NtpMuon::Clear()
{
   //cout << "NtpMuon::Clear executed" << endl;

   evtno = -999;
   muvx = -999.0;
   muvy = -999.0;
   muvz = -999.0;
   mupx = -999.0;
   mupy = -999.0;
   mupz = -999.0;
   muweight = -999.0;
   tpx = -999.0;
   tpy = -999.0;
   tpz = -999.0;
   tptype = -999;
   nimpwt = -999.0;
   ptype = -999;

   evtnoS = 65535;
   muvxF = -999.0;
   muvyF = -999.0;
   muvzF = -999.0;
   mupxF = -999.0;
   mupyF = -999.0;
   mupzF = -999.0;
   tpxF = -999.0;
   tpyF = -999.0;
   tpzF = -999.0;
   tptypeS = -999;
   ptypeS = -999;

}












































































