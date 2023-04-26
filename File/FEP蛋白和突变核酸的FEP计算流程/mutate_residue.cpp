#include "./basic.h"
/*
string NA_R[]={"A","G","A5","G5","A3","G3","6MA"};
string NA_Y[]={"C","U","C5","U5","C3","U3"};
string PROTEIN[]={"GLY","ALA","LEU","ILE","VAL","PRO","PHE","MET","TRP",
                  "SER","GLN","THR","CYS","ASN","TYR","ASP","GLU","LYS",
                  "ARG","HIS","HIE","HID","HIP"};
string SOL_ION[]={"WAT","HOH","NA","CL","K"};
string COMMON_ATOM_NA[]={"P","OP1","OP2","O5'","C5'","C4'","O4'","C1'","N1",
                         "C6","C2","N9","C8","C4","C3'","C2'","O2'","O3'"};
string COMMON_ATOM_P[]={"N","CA","CB","C","O"};
*/
string COMMON_ATOM[]={"N","H","C","O","P","OP1","OP2","O5'","C5'","H5'","H5''","C4'",
                      "H4'","O4'","C3'","H3'","C2'","H2'","O2'","HO2'","O3'",
                      "OXT","HO5'","HO3'"};
string WAT_ION[]={"K","NA","CL","WAT"};

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

class Atom {
  public:
   int id,ntid;
   string head,chainid,ntseq,atomname,atomalt;
   char iCode;
   double x,y,z;

   Atom(string &);
   //friend ostream& ::operator <<(ostream &, Atom &);
};

Atom::Atom(string &sline) {  //sline in pdb format
  if(sline.size()<54) {
    string str="Not standard PDB format --> "+sline;
    cout<<str<<endl;
    exit(0);
  }

  string serial,aname,altLoc,resName,chainID,resSeq,sx,sy,sz,ahead,occupy,tempfactor;

  ahead=sline.substr(0,6);
  //string str0=sline.substr(0,6);
  //for(size_t i=0;i<str0.size();i++)
    //if(!isspace(str0[i])) head+=str0[i];

  string str1=sline.substr(6,5);
  for(size_t i=0;i<str1.size();i++)
    if(!isspace(str1[i])) {serial+=str1[i];}

  string str2=sline.substr(12,4);
  for(size_t i=0;i<str2.size();i++)
    if(!isspace(str2[i])) {aname+=str2[i];}

  altLoc=sline[16];

  string str3=sline.substr(17,3);
  for(size_t i=0;i<str3.size();i++)
    if(!isspace(str3[i])) {resName+=str3[i];}

  chainID=sline[21];

  string str4=sline.substr(22,4);
  for(size_t i=0;i<str4.size();i++)
    if(!isspace(str4[i])) {resSeq+=str4[i];}

  iCode=sline[26];
  //resSeq+=iCode;

  string str5=sline.substr(30,8);
  for(size_t i=0;i<str5.size();i++)
    if(!isspace(str5[i])) {sx+=str5[i];}

  string str6=sline.substr(38,8);
  for(size_t i=0;i<str6.size();i++)
    if(!isspace(str6[i])) {sy+=str6[i];}

  string str7=sline.substr(46,8);
  for(size_t i=0;i<str7.size();i++)
    if(!isspace(str7[i])) {sz+=str7[i];}

  if(sline.size()>=54) {
    string str8=sline.substr(54,6);
    for(size_t i=0;i<str8.size();i++)
      if(!isspace(str8[i])) {occupy+=str8[i];}
  }

  if(sline.size()>=60) {
    string str9=sline.substr(60,6);
    for(size_t i=0;i<str9.size();i++)
      if(!isspace(str9[i])) {tempfactor+=str9[i];}
  }

  head=ahead;
  id=stoi(serial);
  atomname=aname;
  ntseq=resName;
  chainid=chainID;
  ntid=stoi(resSeq);
  x=stod(sx);
  y=stod(sy);
  z=stod(sz);
  atomalt=altLoc;
}

string outstr(Atom& atom) {
  char head[7]="      ";
  for(int ll=0;ll<6;ll++)
    head[ll]=atom.head[ll];
  char atomname[5]="    ";
  int len=atom.atomname.size();
  if(len==4) {
    for(int ll=0;ll<len;ll++)
      atomname[ll]=atom.atomname[ll];
  }
  else {
    for(int ll=0;ll<len;ll++)
      atomname[ll+1]=atom.atomname[ll];
  }
  char resname[4]="   ";
  len=atom.ntseq.size();
  for(int ii=len-1;ii>=0;ii--)
  resname[3-(len-ii)]=atom.ntseq[ii];
  char str[128];
  sprintf(str,"%s%5d %s %s %c%4d%c   %8.3f%8.3f%8.3f  1.00  0.00           %c\n",
              head,atom.id,atomname,resname,atom.chainid[0],atom.ntid,atom.iCode,atom.x,atom.y,atom.z,atom.atomname[0]);
  return str;
}

struct Mutant {
  string chainid,wt,mut;
  int ntid;
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
    cout<<"No input!"<<endl;
    exit(0);
  }
  if(Mcmd.count("-o")==0) {
    cout<<"No output!"<<endl;
    exit(0);
  }
  if(Mcmd.count("-m")==0) {
    cout<<"No mutant!"<<endl;
    exit(0);
  }
  if(Mcmd.count("-map")==0) {
    cout<<"No atom map!"<<endl;
    exit(0);
  }

  string inPDB=Mcmd["-i"];
  string outName=Mcmd["-o"];
  string inAtommap=Mcmd["-map"];

  int flag_leap=0;
  string fleap;
  if(Mcmd.count("-leap")) {
    flag_leap=1;
    fleap=Mcmd["-leap"];
  }
  
  int flag_pause=0;
  if(Mcmd.count("-p"))
    flag_pause=1;

  int flag_PR=0;
  if(Mcmd.count("-pr"))
    flag_PR=1;
  
  int flag_ext=0;
  if(Mcmd.count("-e"))
    flag_ext=1;

  vector<Mutant> Vmut;
  vector<string> stoken=split_string(Mcmd["-m"],"_");
  cout<<"Mutants:"<<endl;
  for(int ii=0;ii<stoken.size();ii++) {
    vector<string> mstr=split_string(stoken[ii],".");
    if(mstr.size()!=2 && mstr.size()!=3) {
      cout<<"Wrong mutant string! --> "<<stoken[ii]<<endl;
      exit(0);
    }
    Mutant mi;
    if(mstr.size()==2) {
      mi.chainid=" ";
      mi.ntid=stoi(mstr[0]);
      string str=mstr[1];
      int found=str.find("to");
      str.replace(found,2," ");
      vector<string> res=split_string(str);
      mi.wt=res[0];
      mi.mut=res[1];
    }
    else if(mstr.size()==3) {
      mi.chainid=mstr[0];
      mi.ntid=stoi(mstr[1]);
      string str=mstr[2];
      int found=str.find("to");
      str.replace(found,2," ");
      vector<string> res=split_string(str);
      mi.wt=res[0];
      mi.mut=res[1];
    }
    Vmut.push_back(mi);
    cout<<" "<<mi.chainid<<"."<<mi.ntid<<"."<<mi.wt<<" -> "<<mi.mut<<endl;
  }

  vector<string> Vlines=read_file(inAtommap);
  map<string,vector<string> > Amap;
  vector<string> Vtmp;
  string tname="";
  for(size_t ii=0;ii<Vlines.size();ii++) {
    if(Vlines[ii].size()<2) continue;
    if(Vlines[ii].find("#")!=string::npos) continue;
    if(Vlines[ii].find("[")!=string::npos && Vlines[ii].find("]")!=string::npos) {
      if(tname!="" && Vtmp.size()>0) {
        if(Amap.count(tname)==0) {
          Amap.emplace(tname,Vtmp);
          tname=Vlines[ii];
          Vtmp.clear();
        }
        else {
          cout<<"Mutant name has been used in atom map --> "<<Vlines[ii]<<endl;
          return 1;
        }
      }
      else
        tname=Vlines[ii];
      continue;
    }
    else
      Vtmp.push_back(Vlines[ii]);
  }
  if(Amap.count(tname)==0)
    Amap.emplace(tname,Vtmp);
  else {
    cout<<"Mutant name has been used in atom map --> "<<tname<<endl;
    return 1;
  }
  map<string,string> atommap;
  for(auto& it : Amap) {
    string mname=it.first;
    vector<string> mlist=it.second;
    int found=mname.find("->");
    mname.replace(found,2," ");
    vector<string> stoken=split_string(mname,"[]");
    if(stoken.size()!=2) {
      cout<<"Wrong mutant name --> "<<mname<<endl;
      return 1;
    }
    string A2B=stoken[0]+"->"+stoken[1];
    string B2A=stoken[1]+"->"+stoken[0];
    for(auto line : mlist) {
      stoken=split_string(line,":");
      if(stoken.size()!=2) {
        cout<<"Wrong atom map --> "<<mname<<" : "<<line<<endl;
      }
      string atom1=A2B+"."+stoken[0];
      if(atommap.count(atom1)) {
        cout<<"Atom has been used in atom map --> "<<atom1<<endl;
        return 1;
      }
      atommap.emplace(atom1,stoken[1]);
      atom1=B2A+"."+stoken[1];
      if(atommap.count(atom1)) {
        cout<<"Atom has been used in atom map --> "<<atom1<<endl;
        return 1;
      }
      atommap.emplace(atom1,stoken[0]);
    }
  }
  
  vector<int> mark_mut;
  Vlines.clear();
  Vlines=read_file(inPDB);
  string fPR=outName+"_PR.pdb";
  ofstream outPR (fPR.c_str());
  //vector<string> Vsol;
  string Fline="";
  for(size_t ii=0;ii<Vlines.size();ii++) {
    if(Vlines[ii].find("CRYST")!=string::npos) 
      Fline=Vlines[ii];
  }
  for(size_t ii=0;ii<Vlines.size();ii++) {
    string str=Vlines[ii].substr(0,6);
    if(str.find("ATOM")==string::npos && str.find("HETATM")==string::npos) {
      outPR<<Vlines[ii]<<endl;
      continue;
    }
    Atom atom1(Vlines[ii]);
    int check=0;
    for(int mm=0;mm<Vmut.size();mm++) {
      if(atom1.chainid==Vmut[mm].chainid && atom1.ntid==Vmut[mm].ntid && atom1.ntseq==Vmut[mm].wt) {
        check=1;
        if(count(mark_mut.begin(),mark_mut.end(),mm)==0) 
          mark_mut.push_back(mm);
        string mname=Vmut[mm].wt+"->"+Vmut[mm].mut+"."+atom1.atomname;
        if(atommap.count(mname)) {
          string atomname=atommap[mname];
          if(atomname[0]!='D') {
            atom1.atomname=atomname;
            atom1.ntseq=Vmut[mm].mut;
            outPR<<outstr(atom1);
          }
        }
        else if(atommap.count(mname)==0 && count(begin(COMMON_ATOM),end(COMMON_ATOM),atom1.atomname)) {
          atom1.ntseq=Vmut[mm].mut;
          outPR<<outstr(atom1);
        }
        else {
          cout<<"Undefined atom map for mutation "<<mname<<endl;
          return 1;
        }
        break;
      }
    }
    if(check==0) 
      outPR<<Vlines[ii]<<endl;
    /*
    int check_NA=0;
    int check_P=0;
    int check_S=0;
    if(count(begin(NA_R),end(NA_R),ntseq)==1 || count(begin(NA_Y),end(NA_Y),ntseq)==1)
      check_NA=1; 
    else if(count(begin(PROTEIN),end(PROTEIN),ntseq)==1)
      check_P=1;
    else if(count(begin(SOL_ION),end(SOL_ION),ntseq)==1)
      check_S=1;
    else {
      cout<<"Unknown residue name --> "<<ntseq<<endl;
      return 1;
    }
    if(check_S)
      Vsol.push_back(Vlines[ii]);
    else if(check_P) {
      int check=0;
      for(int mm=0;mm<Vmut.size();mm++) {
        if(atom1.chainid==Vmut[mm].chainid && atom1.ntid==Vmut[mm].ntid && atom1.ntseq==Vmut[mm].wt) {
          check=1;
          if(count(mark_mut.begin(),mark_mut.end(),mm)==0) 
            mark_mut.push_back(mm);
          atom1.ntseq=Vmut[mm].mut;
          if(count(begin(COMMON_ATOM_P),end(COMMON_ATOM_P),atom1.atomname)==1)
            outPR<<outstr(atom1);
        } 
      }
      if(check==0) outPR<<Vlines[ii]<<endl;
    }
    else if(check_NA) {
      int check=0;
      for(int mm=0;mm<Vmut.size();mm++) {
        if(atom1.chainid==Vmut[mm].chainid && atom1.ntid==Vmut[mm].ntid && atom1.ntseq==Vmut[mm].wt) {
          check=1;
          if(count(mark_mut.begin(),mark_mut.end(),mm)==0) 
            mark_mut.push_back(mm);
          atom1.ntseq=Vmut[mm].mut;
          if(count(begin(COMMON_ATOM_NA),end(COMMON_ATOM_NA),atom1.atomname)==1) {
            if(count(begin(NA_R),end(NA_R),Vmut[mm].wt)==1 && count(begin(NA_R),end(NA_R),Vmut[mm].mut)==1)
              outPR<<outstr(atom1);
            else if(count(begin(NA_Y),end(NA_Y),Vmut[mm].wt)==1 && count(begin(NA_Y),end(NA_Y),Vmut[mm].mut)==1)
              outPR<<outstr(atom1);  
            else if(count(begin(NA_R),end(NA_R),Vmut[mm].wt)==1 && count(begin(NA_Y),end(NA_Y),Vmut[mm].mut)==1) {
              if(atom1.atomname=="N1" || atom1.atomname=="C6" || atom1.atomname=="C2" || atom1.atomname=="C4") continue;
              else if(atom1.atomname=="N9") atom1.atomname="N1";
              else if(atom1.atomname=="C8") atom1.atomname="C6";
              outPR<<outstr(atom1);
            }
            else if(count(begin(NA_Y),end(NA_Y),Vmut[mm].wt)==1 && count(begin(NA_R),end(NA_R),Vmut[mm].mut)==1) {
              if(atom1.atomname=="C2" || atom1.atomname=="C4") continue;
              else if(atom1.atomname=="N1") atom1.atomname="N9";
              else if(atom1.atomname=="C6") atom1.atomname="C8";
              outPR<<outstr(atom1);
            }
            else {
              cout<<"Unknown mutant residue --> "<<Vmut[mm].mut<<endl;
              return 1;
            }
          }
          continue;
        }
      }
      if(check==0) outPR<<Vlines[ii]<<endl;
    }
    */
  }
  outPR.close(); outPR.clear();

  for(int jj=0;jj<Vmut.size();jj++) {
    if(count(mark_mut.begin(),mark_mut.end(),jj)==0) {
      cout<<"Cannot find residue for mutant: "<<Vmut[jj].chainid<<"."<<Vmut[jj].ntid<<"."<<Vmut[jj].wt<<endl;
    }
  }

  string cmd="pdb4amber -i "+fPR+" -o "+outName+"_amber.pdb --add-missing-atoms";
  system(cmd.c_str());

  string fstr=outName+"_amber.pdb";
  Vlines.clear();
  Vlines=read_file(fstr);
  ofstream outP (fstr.c_str());
  if(Fline!="") outP<<Fline<<endl;
  for(int ll=0;ll<Vlines.size();ll++) {
    if(Vlines[ll].find("END")!=string::npos) continue;
    if(flag_ext) {
      string tmp=Vlines[ll];
      if(tmp.find("TER")!=string::npos && (tmp.find(" G ")!=string::npos || tmp.find("M6A")!=string::npos)) 
        continue;
    }
    outP<<Vlines[ll]<<endl;
  }
  outP.close(); outP.clear();

  if(flag_pause) pause();

  if(flag_leap==0) {
    fleap="./leap.in";
    ofstream outF (fleap.c_str());
    outF<<"source leaprc.protein.ff14SB"<<endl;
    outF<<"source leaprc.DNA.OL15"<<endl;
    outF<<"source leaprc.RNA.OL3"<<endl;
    // outF<<"source leaprc.user"<<endl;
    outF<<"source leaprc.water.tip3p"<<endl;
    outF<<"source leaprc.gaff2"<<endl;
    outF<<"loadamberparams frcmod.ionsjc_tip3p"<<endl;
    outF<<endl;
    outF<<"mol = loadpdb "<<fstr<<endl;
    outF<<"saveamberparm mol "<<outName<<".prmtop "<<outName<<".inpcrd"<<endl;
    outF<<"quit"<<endl;
    outF.close(); outF.clear();
  }
  
  cmd="tleap -s -f "+fleap+" > leap.out";
  system(cmd.c_str());

  cmd="amb2gro_top_gro.py -p "+outName+".prmtop -c "+outName+".inpcrd -t "+outName;
  cmd+=".top -g "+outName+".gro -b "+outName+"_water_ions.pdb";
  system(cmd.c_str());

  fstr=outName+".gro";
  Vlines.clear();
  Vlines=read_file(fstr);
  ofstream outG (fstr.c_str());
  for(size_t jj=0;jj<Vlines.size()-1;jj++)
    outG<<Vlines[jj]<<endl;
  stoken=split_string(Fline);
  if(stoken.size()>=7) {
    double x=stod(stoken[1])/10.0;
    double y=stod(stoken[2])/10.0;
    double z=stod(stoken[3])/10.0;
    outG<<setw(10)<<fixed<<setprecision(5)<<x<<setw(10)<<fixed<<setprecision(5)<<y<<setw(10)<<fixed<<setprecision(5)<<z<<endl;
  }
  else
    outG<<Vlines.back()<<endl;
  outG.close(); outG.clear();

  cmd="rm leap.out leap.log "+fPR;
  system(cmd.c_str());

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