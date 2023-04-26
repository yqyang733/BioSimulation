#include "./basic.h"

string PROTEIN[]={"GLY","ALA","LEU","ILE","VAL","PRO","PHE","MET","TRP",
                  "SER","GLN","THR","CYS","ASN","TYR","ASP","GLU","LYS",
                  "ARG","HIS","HIE","HID","HIP","CYX"};
string RNA[]={"A","C","G","U","A5","A3","C5","C3","G5","G3","U5","U3","6MA","M6A"};
string ION[]={"NA","CL","K"};
string WATER[]={"WAT"};
string ATOM_NA[]={"P","OP1","OP2","O5'","C5'","C4'","O4'","C1'","C3'","C2'","O3'","HO5'"};
string ATOM_PROTEIN[]={"N","CA","C","O"};

int main(int narg, char **arg) {
  map<string,string> Mcmd;
  int ni=1;
  while(ni<narg) {
    string style="";
    string value="";
    if(arg[ni][0]=='-') {
      style=arg[ni];
      if(ni<narg-1 && arg[ni+1][0]!='-') {
        value=arg[ni+1];
        ni+=2;
      }
      else {
        value="NULL";
        ni+=1;
      }
      if(Mcmd.count(style)) {
        cout<<"Wrong commands!"<<endl;
        exit(0);
      }
      Mcmd.emplace(style,value);
    }
    else {
      cout<<"Wrong commands!"<<endl;
      exit(0);
    }
  }
  if(Mcmd.count("-i")==0) {
    cout<<"No input file!"<<endl;
    exit(0);
  }
  if(Mcmd.count("-o")==0) {
    cout<<"No output!"<<endl;
    exit(0);
  }

  int flag_RNA=1;
  int flag_Protein=1;
  int flag_Protein_RNA=1;
  int flag_Water=1;
  int flag_Ion=1;
  int flag_Water_ions=1;

  string inFile=Mcmd["-i"];
  string outFile=Mcmd["-o"];

  map<string,vector<int> > ndx;
  vector<string> Vlines=read_file(inFile);
  vector<string> stoken=split_string(Vlines[1]);
  int n_tot=stoi(stoken[0]);
  vector<int> vtmp;
  ndx.emplace("System",vtmp);
  int check_atom=0;
  int resid_p=0;
  for(int ii=2;ii<Vlines.size()-1;ii++) {
    string sline=Vlines[ii].substr(0,5);
    stoken=split_string(sline);
    int resid=stoi(stoken[0]);
    if(resid!=resid_p) {
      check_atom=0;
      resid_p=resid;
    }
    sline=Vlines[ii].substr(5,4);
    stoken=split_string(sline);
    string resname=stoken[0];
    sline=Vlines[ii].substr(15,5);
    stoken=split_string(sline);
    int id=stoi(stoken[0]);
    ndx["System"].push_back(id);
    string name="";
    if(count(begin(PROTEIN),end(PROTEIN),resname)) name="Protein";
    else if(count(begin(RNA),end(RNA),resname)) name="RNA";
    else if(resname=="MUT") {
      string str=Vlines[ii].substr(9,6);
      stoken=split_string(str);
      string atomname=stoken[0];
      if(count(begin(ATOM_NA),end(ATOM_NA),atomname)) check_atom=1;
      else if(count(begin(ATOM_PROTEIN),end(ATOM_PROTEIN),atomname)) check_atom=2;
      if(check_atom==1) name="RNA";
      else if(check_atom==2) name="Protein";
      else {
        cout<<"Unknown residue --> "<<Vlines[ii]<<endl;
        return 1;
      }
    }
    else if(count(begin(ION),end(ION),resname)) name="Ion";
    else if(count(begin(WATER),end(WATER),resname)) name="Water";
    else {
      cout<<"Unknown residue --> "<<Vlines[ii]<<endl;
      return 1;
    }
    if(ndx.count(name)) {
      ndx[name].push_back(id);
    }
    else {
      vtmp.clear();
      vtmp.push_back(id);
      ndx.emplace(name,vtmp);
    }
  }
  
  ofstream outF (outFile.c_str());
  outF<<"[ System ]"<<endl;
  vector<int> ids=ndx["System"];
  if(n_tot!=ids.size()) {
    cout<<"Number of total atoms is not matched --> "<<n_tot<<" / "<<ids.size()<<endl; 
    return 1;
  }

  for(int ii=0;ii<ids.size();ii++) {
    outF<<setw(4)<<ids[ii];
    if((ii+1)%15==0) outF<<endl;
    else outF<<" "; 
  }
  if(((int) ids.size())%15) outF<<endl;

  if(ndx.count("Protein") && flag_Protein) {
    ids=ndx["Protein"];
    outF<<"[ Protein ]"<<endl;
    for(int ii=0;ii<ids.size();ii++) {
      outF<<setw(4)<<ids[ii];
      if((ii+1)%15==0) outF<<endl;
      else outF<<" "; 
    }
    if(((int) ids.size())%15) outF<<endl;
  }

  if(ndx.count("RNA") && flag_RNA) {
    ids=ndx["RNA"];
    outF<<"[ RNA ]"<<endl;
    for(int ii=0;ii<ids.size();ii++) {
      outF<<setw(4)<<ids[ii];
      if((ii+1)%15==0) outF<<endl;
      else outF<<" "; 
    }
    if(((int) ids.size())%15) outF<<endl;
  }

  if(ndx.count("Protein") && ndx.count("RNA") && flag_Protein_RNA) {
    ids=ndx["Protein"];
    vtmp=ndx["RNA"];
    ids.insert(ids.end(),vtmp.begin(),vtmp.end());
    outF<<"[ Protein_RNA ]"<<endl;
    for(int ii=0;ii<ids.size();ii++) {
      outF<<setw(4)<<ids[ii];
      if((ii+1)%15==0) outF<<endl;
      else outF<<" "; 
    }
    if(((int) ids.size())%15) outF<<endl;
  }

  if(ndx.count("Ion") && flag_Ion) {
    ids=ndx["Ion"];
    outF<<"[ Ion ]"<<endl;
    for(int ii=0;ii<ids.size();ii++) {
      outF<<setw(4)<<ids[ii];
      if((ii+1)%15==0) outF<<endl;
      else outF<<" "; 
    }
    if(((int) ids.size())%15) outF<<endl;
  }

  if(ndx.count("Water") && flag_Water) {
    ids=ndx["Water"];
    outF<<"[ Water ]"<<endl;
    for(int ii=0;ii<ids.size();ii++) {
      outF<<setw(4)<<ids[ii];
      if((ii+1)%15==0) outF<<endl;
      else outF<<" "; 
    }
    if(((int) ids.size())%15) outF<<endl;
  }

  if(ndx.count("Ion") && ndx.count("Water") && flag_Water_ions) {
    ids=ndx["Ion"];
    vtmp=ndx["Water"];
    ids.insert(ids.end(),vtmp.begin(),vtmp.end());
    outF<<"[ Water_and_ions ]"<<endl;
    for(int ii=0;ii<ids.size();ii++) {
      outF<<setw(4)<<ids[ii];
      if((ii+1)%15==0) outF<<endl;
      else outF<<" "; 
    }
    if(((int) ids.size())%15) outF<<endl;
  }
  outF.close(); outF.clear();
}

