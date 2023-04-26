#include "./basic.h"

string WAT_ION[]={"K","NA","CL","WAT"};   # string数组

struct Molecule {
  vector<string> heads;
  vector<vector<string> > residues;
  vector<string> bonds,pairs,angles,dihedrals;
  vector<string> sequence;
  vector<string> others;
};

struct Topol {
  vector<string> heads;
  vector<string> atomtypes;
  vector<Molecule> molecules;
  vector<string> tails;
};

Topol read_topol(string fstr) {
  Topol top;
  vector<string> Vlines=read_file(fstr);
  int lindex1=0;
  for(int ii=0;ii<Vlines.size();ii++) {
    if(Vlines[ii].find("atomtypes")!=string::npos || Vlines[ii].find("moleculetype")!=string::npos) {
      lindex1=ii;
      break;
    }
  }
  top.heads.clear();
  for(int ii=0;ii<lindex1;ii++)
    top.heads.push_back(Vlines[ii]);
  
  int lindex2=0;
  for(int ii=lindex1;ii<Vlines.size();ii++) {
    if(Vlines[ii].find("moleculetype")!=string::npos) {
      lindex2=ii;
      break;
    }
  }
  top.atomtypes.clear();
  for(int ii=lindex1;ii<lindex2;ii++) 
    top.atomtypes.push_back(Vlines[ii]);

  int lindex3=0;
  for(int ii=Vlines.size()-1;ii>=lindex2;ii--) {
    if(Vlines[ii].find("system")!=string::npos && Vlines[ii].find("[")!=string::npos &&
       Vlines[ii].find("]")!=string::npos) {
      lindex3=ii;
      break;
    }
  }
  top.tails.clear();
  for(int ii=lindex3;ii<Vlines.size();ii++) 
    top.tails.push_back(Vlines[ii]);

  vector<int> Vbegin,Vend;
  for(int ii=lindex2;ii<lindex3;ii++) {
    if(Vlines[ii].find("moleculetype")!=string::npos) {
      Vbegin.push_back(ii);
      if(Vbegin.size()>=2) Vend.push_back(ii-1);
    }
  }
  Vend.push_back(lindex3-1);
  
  for(int jj=0;jj<Vbegin.size();jj++) {
    Molecule mol;
    int ni=Vbegin[jj];
    mol.heads.push_back(Vlines[ni]);
    ni++;
    while(1) {
      if(Vlines[ni].find("atoms")!=string::npos) break;
      mol.heads.push_back(Vlines[ni]);
      ni++;
    }
    vector<string> res;
    for(int ll=0;ll<2;ll++)
      res.push_back(Vlines[ni+ll]);
    mol.residues.push_back(res);
    ni+=2;
    res.clear();
    while(1) {
      if(Vlines[ni].find("[")!=string::npos && Vlines[ni].find("]")!=string::npos) break;
      if(Vlines[ni].find("#ifdef")!=string::npos) break;
      if(Vlines[ni].find("; residue")!=string::npos) {
        if(res.size()>0) {
          mol.residues.push_back(res);
          res.clear();
        }
        vector<string> stoken=split_string(Vlines[ni]);
        mol.sequence.push_back(stoken[3]);
      }
      if(Vlines[ni].size()>2) res.push_back(Vlines[ni]);
      ni++;
    }
    mol.residues.push_back(res);
    
    if(Vlines[ni].find("bonds")!=string::npos) {
      mol.bonds.push_back(Vlines[ni]);
      ni++;
      while(1) {
        if(Vlines[ni].find("[")!=string::npos && Vlines[ni].find("]")!=string::npos) break;
        if(Vlines[ni].find("#ifdef")!=string::npos) break;
        if(Vlines[ni].size()>2) mol.bonds.push_back(Vlines[ni]);
        ni++;
      }
    }

    if(Vlines[ni].find("pairs")!=string::npos) {
      mol.pairs.push_back(Vlines[ni]);
      ni++;
      while(1) {
        if(Vlines[ni].find("[")!=string::npos && Vlines[ni].find("]")!=string::npos) break;
        if(Vlines[ni].find("#ifdef")!=string::npos) break;
        if(Vlines[ni].size()>2) mol.pairs.push_back(Vlines[ni]);
        ni++;
      }
    }
    
    if(Vlines[ni].find("angles")!=string::npos) {
      mol.angles.push_back(Vlines[ni]);
      ni++;
      while(1) {
        if(Vlines[ni].find("[")!=string::npos && Vlines[ni].find("]")!=string::npos) break;
        if(Vlines[ni].find("#ifdef")!=string::npos) break;
        if(Vlines[ni].size()>2) mol.angles.push_back(Vlines[ni]);
        ni++;
      }
    }
    
    if(Vlines[ni].find("dihedrals")!=string::npos) {
      mol.dihedrals.push_back(Vlines[ni]);
      ni++;
      while(1) {
        if(Vlines[ni].find("[")!=string::npos && Vlines[ni].find("]")!=string::npos) break;
        if(Vlines[ni].find("#ifdef")!=string::npos) break;
        if(Vlines[ni][0]==';' && Vlines[ni].find("Include")!=string::npos) break;
        if(Vlines[ni].size()>2) mol.dihedrals.push_back(Vlines[ni]);
        ni++;
      }
    }

    for(int ll=ni;ll<=Vend[jj];ll++) 
      mol.others.push_back(Vlines[ll]);
    
    top.molecules.push_back(mol);
  }

  return top;
}

int main(int narg, char **arg) {
  map<string,string> Mcmd;  // store the parameters input by users.
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
      if(Mcmd.count(style)) {              // prevent duplicate input.
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
    cout<<"No input!"<<endl;
    exit(0);
  }
  if(Mcmd.count("-o")==0) {
    cout<<"No output!"<<endl;
    exit(0);
  }

  string inPDB=Mcmd["-i"];
  string outName=Mcmd["-o"];
  
  double d=12;
  double conc=0.15;
  string p_ion_name="NA";
  string n_ion_name="CL";  

  if(Mcmd.count("-d"))
    d=stod(Mcmd["-d"]);
  if(Mcmd.count("-c"))
    conc=stod(Mcmd["-c"]);
  
  if(Mcmd.count("-pi"))
    p_ion_name=Mcmd["-pi"];
  if(p_ion_name=="Na" || p_ion_name=="na")
    p_ion_name="NA";  
  int flag_pause=0;
  if(Mcmd.count("-p"))
    flag_pause=1;
  int flag_r=1;
  if(Mcmd.count("-r"))
    flag_r=stoi(Mcmd["-r"]);
  int flag_PR=0;
  if(Mcmd.count("-pr"))
    flag_PR=1;

  string cmd="pdb4amber -i "+inPDB+" -o "+outName+"_amber.pdb -y --reduce --add-missing-atoms";
  system(cmd.c_str());

  if(flag_pause) pause();

  string fstr="./leap.in";
  ofstream outF (fstr.c_str());
  outF<<"source leaprc.protein.ff14SB"<<endl;
  outF<<"source leaprc.DNA.OL15"<<endl;
  outF<<"source leaprc.RNA.OL3"<<endl;
  // outF<<"source leaprc.user"<<endl;
  outF<<"source leaprc.water.tip3p"<<endl;
  outF<<"source leaprc.gaff2"<<endl;
  outF<<"loadamberparams frcmod.ionsjc_tip3p"<<endl;
  outF<<endl;
  outF<<"mol = loadpdb "<<outName<<"_amber.pdb"<<endl;
  outF<<"solvatebox mol TIP3PBOX "<<d<<endl;
  outF<<"quit"<<endl;
  outF.close(); outF.clear();
  cmd="tleap -s -f ./leap.in > leap.out";
  system(cmd.c_str());
  cmd="tail leap.out -n 4 > ./tail.dat";
  system(cmd.c_str());
  vector<string> sline=read_file("./tail.dat");
  cout<<sline[0]<<endl;
  vector<string> stoken=split_string(sline[0]);
  int nwater=stoi(stoken[1]);
  int nion=round(nwater*0.0187*conc);

  outF.open(fstr.c_str());
  outF<<"source leaprc.protein.ff14SB"<<endl;
  outF<<"source leaprc.DNA.OL15"<<endl;
  outF<<"source leaprc.RNA.OL3"<<endl;
  // outF<<"source leaprc.user"<<endl;
  outF<<"source leaprc.water.tip3p"<<endl;
  outF<<"source leaprc.gaff2"<<endl;
  outF<<"loadamberparams frcmod.ionsjc_tip3p"<<endl;
  outF<<endl;
  outF<<"mol = loadpdb "<<outName<<"_amber.pdb"<<endl;
  outF<<"solvatebox mol TIP3PBOX "<<d<<endl;
  outF<<"addions mol "<<p_ion_name<<" 0"<<endl;
  outF<<"addions mol "<<n_ion_name<<" 0"<<endl;
  outF<<"addions mol "<<p_ion_name<<" "<<nion<<endl;
  outF<<"addions mol "<<n_ion_name<<" 0"<<endl;
  //outF<<"savepdb mol "<<outName<<"_water_ions.pdb"<<endl;
  outF<<"saveamberparm mol "<<outName<<".prmtop "<<outName<<".inpcrd"<<endl;
  outF<<"quit"<<endl;
  outF.close(); outF.clear();
  cmd="tleap -s -f ./leap.in > leap.out";
  system(cmd.c_str());

  cmd="amb2gro_top_gro.py -p "+outName+".prmtop -c "+outName+".inpcrd -t "+outName;
  cmd+=".top -g "+outName+".gro -b "+outName+"_water_ions.pdb";
  system(cmd.c_str());

  if(flag_r) {
    cmd="rm reduce_info.log leap.in leap.out leap.log tail.dat";
    system(cmd.c_str());
  }

  if(flag_PR==0) return 0;
  
  double fc=1000;
  fstr=outName+".top";
  Topol top1=read_topol(fstr);

  //fstr=outName+"1.top";
  ofstream outT(fstr.c_str());
  for(auto sline : top1.heads)
    outT<<sline<<endl;
  for(auto sline : top1.atomtypes)
    outT<<sline<<endl;

  for(int ii=0;ii<top1.molecules.size();ii++) {
    vector<int> heavyatoms;
    string moleculename;
    vector<string> vtmp=split_string(top1.molecules[ii].heads[2]);
    moleculename=vtmp[0];
    for(auto sline : top1.molecules[ii].heads) {
      outT<<sline<<endl;
    }
    for(auto sline : top1.molecules[ii].residues[0])
      outT<<sline<<endl;
    for(int jj=1;jj<top1.molecules[ii].residues.size();jj++) {  
      for(auto sline : top1.molecules[ii].residues[jj]) 
        outT<<sline<<endl;
      for(int kk=1;kk<top1.molecules[ii].residues[jj].size();kk++) {
        vector<string> stoken=split_string(top1.molecules[ii].residues[jj][kk]);
        int id=stoi(stoken[0]);
        string atomname=stoken[4];
        if(atomname[0]!='H') 
          heavyatoms.push_back(id);
      }
    }
    outT<<endl;
    int nbond=top1.molecules[ii].bonds.size();
    if(nbond>0) {
      for(auto sline : top1.molecules[ii].bonds) {
        outT<<sline<<endl;
      }
      outT<<endl;
    }
    int npair=top1.molecules[ii].pairs.size();
    if(npair>0) {
      for(auto sline : top1.molecules[ii].pairs) {
        outT<<sline<<endl;
      }
      outT<<endl;
    }
    int nangle=top1.molecules[ii].angles.size();
    if(nangle>0) {
      for(auto sline : top1.molecules[ii].angles) {
        outT<<sline<<endl;
      }
      outT<<endl;
    }
    int ndihedral=top1.molecules[ii].dihedrals.size();
    if(ndihedral>0) {
      for(auto sline : top1.molecules[ii].dihedrals) {
        outT<<sline<<endl;
      }
      outT<<endl;
    }
    int nother=top1.molecules[ii].others.size();
    if(nother>0) {
      for(auto sline : top1.molecules[ii].others)
        outT<<sline<<endl;
    }
    if(nbond==0 && npair==0 && nother==0)
      outT<<endl;
    
    if(count(begin(WAT_ION),end(WAT_ION),moleculename)==0) {
      outT<<"#ifdef POSRES"<<endl;
      outT<<"[ position_restraints ]"<<endl;
      outT<<";   i funct       fcx        fcy        fcz"<<endl;
      for(auto id : heavyatoms) {
        outT<<setw(5)<<id<<"    1"<<setw(11)<<fixed<<setprecision(1)<<fc<<setw(11)<<fixed<<setprecision(1)<<fc;
        outT<<setw(11)<<fixed<<setprecision(1)<<fc<<endl;
      }
      outT<<"#endif"<<endl;
      outT<<endl<<endl;
    }
  }
  
  for(auto sline: top1.tails)
    outT<<sline<<endl; 
  outT.close(); outT.clear();

  return 0;
}

