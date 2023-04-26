#include "./basic.h"

string WAT_ION[]={"K","NA","CL","WAT"};
string COMMON_ATOM[]={"N","H","C","O","P","OP1","OP2","O5'","C5'","H5'","H5''","C4'",
                      "H4'","O4'","C3'","H3'","C2'","H2'","O2'","HO2'","O3'",
                      "OXT","HO5'","HO3'"};

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

struct Atom {
  int id,resid;
  string type,resname,name;
  double charge,mass;
  string other;
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
  //for(int jj=0;jj<Vbegin.size();jj++) 
    //cout<<Vbegin[jj]<<" : "<<Vend[jj]<<endl;
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

    //if(ni>=Vend[jj]) continue;

    for(int ll=ni;ll<=Vend[jj];ll++) 
      mol.others.push_back(Vlines[ll]);
    
    top.molecules.push_back(mol);
  }
  
  /*
  cout<<fstr<<":"<<endl;
  //cout<<"      heads "<<top.heads.size()<<endl;
  //cout<<"  atomtypes "<<top.atomtypes.size()<<endl;
  //cout<<"      tails "<<top.tails.size()<<endl;
  for(int jj=0;jj<top.molecules.size();jj++) {
    vector<string> stoken=split_string(top.molecules[jj].heads[2]);
    cout<<"  "<<stoken[0]<<":"<<endl;
    cout<<"     residues "<<top.molecules[jj].residues.size()-1<<endl;
    int nbond=0;
    if(top.molecules[jj].bonds.size()>0) nbond=top.molecules[jj].bonds.size()-2;
    cout<<"        bonds "<<nbond<<endl;
    int npair=0;
    if(top.molecules[jj].pairs.size()>0) npair=top.molecules[jj].pairs.size()-2;
    cout<<"        pairs "<<npair<<endl;
    int nangle=0;
    if(top.molecules[jj].angles.size()>0) nangle=top.molecules[jj].angles.size()-2;
    cout<<"       angles "<<nangle<<endl;
    int ndihedral=0;
    if(top.molecules[jj].dihedrals.size()>0) ndihedral=top.molecules[jj].dihedrals.size()-2;
    cout<<"    dihedrals "<<ndihedral<<endl;
    cout<<"       others "<<top.molecules[jj].others.size()<<endl;
  }
  */
 
  return top;
}

void rename_molecule(Molecule &mol) {
  map<int,string> atomindex;
  for(int ii=1;ii<mol.residues.size();ii++) {
    for(int jj=1;jj<mol.residues[ii].size();jj++) {
      vector<string> stoken=split_string(mol.residues[ii][jj]);
      if(stoken.size()<=1) continue;
      int id=stoi(stoken[0]);
      string atom=stoken[2]+"."+stoken[3]+"."+stoken[4];
      atomindex.emplace(id,atom);
    }
  }
  
  for(int ii=2;ii<mol.bonds.size();ii++) {
    vector<string> stoken=split_string(mol.bonds[ii]);
    int id1=stoi(stoken[0]);
    int id2=stoi(stoken[1]);
    if(atomindex.count(id1)==0 || atomindex.count(id2)==0) {
      cout<<"Cannot find atom name for id --> "<<id1<<" || "<<id2<<endl;
      exit(0);
    }
    string str=atomindex[id1]+" "+atomindex[id2];
    for(int jj=2;jj<stoken.size();jj++)
      str+=" "+stoken[jj];
    mol.bonds[ii]=str;
  }
  
  for(int ii=2;ii<mol.pairs.size();ii++) {
    vector<string> stoken=split_string(mol.pairs[ii]);
    int id1=stoi(stoken[0]);
    int id2=stoi(stoken[1]);
    if(atomindex.count(id1)==0 || atomindex.count(id2)==0) {
      cout<<"Cannot find atom name for id --> "<<id1<<" || "<<id2<<endl;
      exit(0);
    }
    string str=atomindex[id1]+" "+atomindex[id2];
    for(int jj=2;jj<stoken.size();jj++)
      str+=" "+stoken[jj];
    mol.pairs[ii]=str;
  }
  
  for(int ii=2;ii<mol.angles.size();ii++) {
    vector<string> stoken=split_string(mol.angles[ii]);
    int id1=stoi(stoken[0]);
    int id2=stoi(stoken[1]);
    int id3=stoi(stoken[2]);
    if(atomindex.count(id1)==0 || atomindex.count(id2)==0 || atomindex.count(id3)==0) {
      cout<<"Cannot find atom name for id --> "<<id1<<" || "<<id2<<" || "<<id3<<endl;
      exit(0);
    }
    string str=atomindex[id1]+" "+atomindex[id2]+" "+atomindex[id3];
    for(int jj=3;jj<stoken.size();jj++) {
      str+=" "+stoken[jj];
    }
    mol.angles[ii]=str;
  }
  
  for(int ii=2;ii<mol.dihedrals.size();ii++) {
    vector<string> stoken=split_string(mol.dihedrals[ii]);
    int id1=stoi(stoken[0]);
    int id2=stoi(stoken[1]);
    int id3=stoi(stoken[2]);
    int id4=stoi(stoken[3]);
    if(atomindex.count(id1)==0 || atomindex.count(id2)==0 || atomindex.count(id3)==0 || atomindex.count(id4)==0) {
      cout<<"Cannot find atom name for id --> "<<id1<<" || "<<id2<<" || "<<id3<<" || "<<id4<<endl;
      exit(0);
    }
    string str=atomindex[id1]+" "+atomindex[id2]+" "+atomindex[id3]+" "+atomindex[id4];
    for(int jj=4;jj<stoken.size();jj++) {
      str+=" "+stoken[jj];
    }
    mol.dihedrals[ii]=str;
  }
}

Molecule read_gro(string fstr) {
  Molecule mol;

  vector<string> Vlines=read_file(fstr);

  mol.heads.clear();
  for(int ii=0;ii<2;ii++)
    mol.heads.push_back(Vlines[ii]);
  mol.others.clear();
  mol.others.push_back(Vlines.back());

  mol.residues.clear();
  mol.sequence.clear();
  int nres=1;
  vector<string> residue;
  string resname="";
  for(int ii=2;ii<Vlines.size()-1;ii++) {
    string str=Vlines[ii].substr(0,5);
    //cout<<str<<" "<<Vlines[ii]<<endl;
    int resid=stoi(str);
    str=Vlines[ii].substr(5,3);
    vector<string> stoken=split_string(str);
    if(resid!=nres) {
      if(residue.size()>0) {
        mol.residues.push_back(residue);
        residue.clear();
        mol.sequence.push_back(resname);
      }
      nres=resid;
    }
    resname=stoken[0];
    residue.push_back(Vlines[ii]);
  }
  if(residue.size()>0) {
    mol.residues.push_back(residue);
    mol.sequence.push_back(resname);
  }
  
  cout<<fstr<<":"<<endl;
  cout<<"  residues "<<mol.residues.size()<<endl;
  cout<<"     atoms "<<mol.heads[1]<<endl;

  return mol;
}

int main(int narg, char **arg) {
  int fposres=1;
  double fc=1000.0;
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
  if(Mcmd.count("-it1")==0 || Mcmd.count("-it2")==0) {
    cout<<"No input!"<<endl;
    exit(0);
  }
  if(Mcmd.count("-ig1")==0 || Mcmd.count("-ig2")==0) {
    cout<<"No input!"<<endl;
    exit(0);
  }
  if(Mcmd.count("-ot")==0 || Mcmd.count("-og")==0) {
    cout<<"No output!"<<endl;
    exit(0);
  }
  if(Mcmd.count("-map")==0) {
    cout<<"No atom map!"<<endl;
    exit(0);
  }

  string intop1=Mcmd["-it1"];
  string intop2=Mcmd["-it2"];
  string ingro1=Mcmd["-ig1"];
  string ingro2=Mcmd["-ig2"];
  string outtop=Mcmd["-ot"];
  string outgro=Mcmd["-og"];
  string inAtommap=Mcmd["-map"];
  
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

  //for(auto& it : atommap)
    //cout<<it.first<<" --> "<<it.second<<endl;
  
  Topol topA=read_topol(intop1);
  Topol topB=read_topol(intop2);

  ofstream outT (outtop);
  for(int ii=0;ii<topA.heads.size();ii++) {
    outT<<topA.heads[ii]<<endl;
  }

  vector<string> atomtypes;
  for(int ii=2;ii<topA.atomtypes.size();ii++) {
    vector<string> stoken=split_string(topA.atomtypes[ii]);
    if(stoken.size()==0) continue;
    atomtypes.push_back(stoken[0]);
  }
  for(int ii=0;ii<topA.atomtypes.size();ii++) {
    if(topA.atomtypes[ii].size()<2) continue;
    outT<<topA.atomtypes[ii]<<endl;
  }
  for(int ii=2;ii<topB.atomtypes.size();ii++) {
    string sline=topB.atomtypes[ii];
    if(sline.size()<2) continue;
    vector<string> stoken=split_string(sline);
    if(count(atomtypes.begin(),atomtypes.end(),stoken[0])==0)
      outT<<sline<<endl;
  }
  outT<<"DUM            0   1.000000  0.00000000  A              0              0"<<endl;
  outT<<endl<<endl;

  for(int ii=0;ii<topA.molecules.size();ii++) {
    vector<int> heavyatom;
    string moleculename;
    vector<string> vtmp=split_string(topA.molecules[ii].heads[2]);
    moleculename=vtmp[0];
    for(int jj=0;jj<topA.molecules[ii].heads.size();jj++) {
      outT<<topA.molecules[ii].heads[jj]<<endl;
    }
    for(int jj=0;jj<topA.molecules[ii].residues[0].size();jj++)
      outT<<topA.molecules[ii].residues[0][jj]<<endl;
    if(topA.molecules[ii].sequence.size()!=topB.molecules[ii].sequence.size()) {
      cout<<"Number of residues in Molecule "<<ii+1<<" is not matched in topA/topB"<<endl;
      return 1;
    }
    map<int,string> mutations;
    for(int jj=0;jj<topA.molecules[ii].sequence.size();jj++) {
      string wt=topA.molecules[ii].sequence[jj];
      string mut=topB.molecules[ii].sequence[jj];
      if(wt!=mut) {
        string str=wt+"->"+mut;
        mutations.emplace(jj+1,str);
      }
    }
    if(mutations.size()==0) {
      for(int jj=1;jj<topA.molecules[ii].residues.size();jj++) {
        if(topA.molecules[ii].residues[jj].size()!=topB.molecules[ii].residues[jj].size()) {
          cout<<"Number of atoms in Molecule "<<ii+1<<" Residue "<<jj<<" is not matched in topA/topB"<<endl;
          return 1;
        }
        for(int kk=0;kk<topA.molecules[ii].residues[jj].size();kk++) 
          outT<<topA.molecules[ii].residues[jj][kk]<<endl;
        for(int kk=1;kk<topA.molecules[ii].residues[jj].size();kk++) {
          vector<string> stoken=split_string(topA.molecules[ii].residues[jj][kk]);
          int id=stoi(stoken[0]);
          string atomname=stoken[4];
          if(atomname[0]!='H') 
            heavyatom.push_back(id);
        }
      }
      outT<<endl;
      int nbond=topA.molecules[ii].bonds.size();
      if(nbond>0) {
        for(auto sline : topA.molecules[ii].bonds) {
          outT<<sline;
          //if(sline[0]=='[' || sline[0]==';') {
            outT<<endl;
            //continue;
          //}
          //vector<string> stoken=split_string(sline);
          //outT<<"  "<<stoken[3]<<" "<<stoken[4]<<endl;
        }
          
        outT<<endl;
      }
      int npair=topA.molecules[ii].pairs.size();
      if(npair>0) {
        for(auto sline : topA.molecules[ii].pairs) {
          outT<<sline<<endl;
        }
        outT<<endl;
      }
      int nangle=topA.molecules[ii].angles.size();
      if(nangle>0) {
        for(auto sline : topA.molecules[ii].angles) {
          outT<<sline;
          //if(sline[0]=='[' || sline[0]==';') {
            outT<<endl;
            //continue;
          //}
          //vector<string> stoken=split_string(sline);
          //outT<<"  "<<stoken[4]<<" "<<stoken[5]<<endl;
        }
        outT<<endl;
      }
      int ndihedral=topA.molecules[ii].dihedrals.size();
      if(ndihedral>0) {
        for(auto sline : topA.molecules[ii].dihedrals) {
          outT<<sline;
          //if(sline[0]=='[' || sline[0]==';') {
            outT<<endl;
            //continue;
          //}
          //vector<string> stoken=split_string(sline);
          //outT<<"  "<<stoken[5]<<"  "<<stoken[6]<<"  "<<stoken[7]<<endl;
        }
        outT<<endl;
      }
      int nother=topA.molecules[ii].others.size();
      if(nother>0) {
        for(auto sline : topA.molecules[ii].others)
          outT<<sline<<endl;
      }
      if(nbond==0 && npair==0 && nother==0)
        outT<<endl;
    }
    else {
      cout<<"Mutations in Molecule "<<moleculename<<":"<<endl;
      for(auto& it : mutations)
        cout<<" "<<it.first<<" "<<it.second<<endl;
      Molecule molA=topA.molecules[ii];
      Molecule molB=topB.molecules[ii];
      rename_molecule(molA);
      rename_molecule(molB);
      int id_offset=0;
      map<string,int> atomid;
      map<string,string> mut_wt_atoms;
      for(int jj=1;jj<molA.residues.size();jj++) {
        if(mutations.count(jj)==0) {
          if(molA.residues[jj].size()!=molB.residues[jj].size()) {
            cout<<"Number of atoms in Molecule "<<ii+1<<" Residue "<<jj<<" is not matched in topA/topB"<<endl;
            return 1;
          }
          outT<<molA.residues[jj][0]<<endl;
          for(int kk=1;kk<molA.residues[jj].size();kk++) {
            string sline=molA.residues[jj][kk];
            vector<string> stoken=split_string(sline);
            int id=stoi(stoken[0]);
            string atomname=stoken[4];
            id+=id_offset;
            ostringstream ostr;
            ostr<<setw(5)<<id;
            string tmp=ostr.str();
            sline.replace(0,5,tmp);
            sline.replace(39,5,tmp);
            outT<<sline<<endl;
            string aname=stoken[2]+"."+stoken[3]+"."+stoken[4];
            atomid.emplace(aname,id);
            if(atomname[0]!='H')
              heavyatom.push_back(id);
          }
        }
        else {
          string sline=molA.residues[jj][0].substr(0,15);
          vector<string> stoken=split_string(molA.residues[jj][0]);
          sline+="MUT rtp MUT q "+stoken[7];
          outT<<sline<<endl;
          vector<Atom> residueA;
          for(int kk=1;kk<molA.residues[jj].size();kk++) {
            string sline=molA.residues[jj][kk];
            stoken=split_string(sline);
            Atom atom1;
            int id=stoi(stoken[0]);
            atom1.id=id+id_offset;
            atom1.type=stoken[1];
            atom1.resid=stoi(stoken[2]);
            atom1.resname=stoken[3];
            atom1.name=stoken[4];
            atom1.charge=stod(stoken[6]);
            atom1.mass=stod(stoken[7]);
            atom1.other="";
            for(int nn=8;nn<stoken.size();nn++)
              atom1.other+=" "+stoken[nn];
            residueA.push_back(atom1);
          }
          vector<Atom> residueB;
          for(int kk=1;kk<molB.residues[jj].size();kk++) {
            string sline=molB.residues[jj][kk];
            stoken=split_string(sline);
            Atom atom1;
            int id=stoi(stoken[0]);
            atom1.id=id+id_offset;
            atom1.type=stoken[1];
            atom1.resid=stoi(stoken[2]);
            atom1.resname=stoken[3];
            atom1.name=stoken[4];
            atom1.charge=stod(stoken[6]);
            atom1.mass=stod(stoken[7]);
            atom1.other="";
            for(int nn=8;nn<stoken.size();nn++)
              atom1.other+=" "+stoken[nn];
            residueB.push_back(atom1);
          }
          for(int kk=0;kk<residueA.size();kk++) {
            string mname=residueA[kk].resname+"->"+residueB[0].resname+"."+residueA[kk].name;
            if(atommap.count(mname)) {
              string nameB=atommap[mname];
              string wt_atom=to_string(residueA[kk].resid)+"."+residueA[kk].resname+"."+residueA[kk].name;
              string mut_atom=to_string(residueB[0].resid)+"."+residueB[0].resname+"."+nameB;
              mut_wt_atoms.emplace(mut_atom,wt_atom);
              mut_wt_atoms.emplace(wt_atom,mut_atom);
              atomid.emplace(wt_atom,residueA[kk].id);
              atomid.emplace(mut_atom,residueA[kk].id);
              int idB=-1;
              for(int nn=0;nn<residueB.size();nn++) {
                if(residueB[nn].name==nameB) {
                  idB=nn;
                  break;
                }
              }
              if(idB<0) {
                residueA[kk].other=" DUM  0.00000000   1.000000  "+residueA[kk].other;
              }
              else {
                ostringstream ostr;
                ostr<<setw(4)<<residueB[idB].type<<" "<<setw(11)<<fixed<<setprecision(8)<<residueB[idB].charge;
                ostr<<"  "<<setw(9)<<fixed<<setprecision(6)<<residueB[idB].mass;
                string tmp=ostr.str();
                residueA[kk].other=tmp+"  "+residueA[kk].other;
              }
              continue;
            }
            else if(count(begin(COMMON_ATOM),end(COMMON_ATOM),residueA[kk].name)) {
              string aname=to_string(residueA[kk].resid)+"."+residueA[kk].resname+"."+residueA[kk].name;
              atomid.emplace(aname,residueA[kk].id);
              aname=to_string(residueB[0].resid)+"."+residueB[0].resname+"."+residueA[kk].name;
              atomid.emplace(aname,residueA[kk].id);
              continue;
            }
            else {
              cout<<"Undefined atom map for mutation "<<mname<<endl;
              return 1;
            }
          }
          for(int kk=0;kk<residueB.size();kk++) {  
            string mname=residueB[kk].resname+"->"+residueA[0].resname+"."+residueB[kk].name;
            if(atommap.count(mname)) {
              string nameA=atommap[mname];
              int idA=-1;
              for(int nn=0;nn<residueA.size();nn++) {
                if(residueA[nn].name==nameA) {
                  idA=nn;
                  break;
                }
              }
              if(idA<0) {
                Atom atom1;
                atom1.id=residueA.back().id+1;
                id_offset++;
                atom1.type="DUM";
                atom1.resid=residueA[0].resid;
                atom1.resname="MUT";
                atom1.name=nameA;
                atom1.charge=0.0;
                atom1.mass=1.0;
                ostringstream ostr;
                ostr<<setw(4)<<residueB[kk].type<<" "<<setw(11)<<fixed<<setprecision(8)<<residueB[kk].charge;
                ostr<<"  "<<setw(9)<<fixed<<setprecision(6)<<residueB[kk].mass;
                atom1.other=ostr.str();
                residueA.push_back(atom1);
                string wt_atom=to_string(residueA[0].resid)+"."+residueA[0].resname+"."+nameA;
                string mut_atom=to_string(residueB[kk].resid)+"."+residueB[kk].resname+"."+residueB[kk].name;
                mut_wt_atoms.emplace(mut_atom,wt_atom);
                mut_wt_atoms.emplace(wt_atom,mut_atom);
                atomid.emplace(wt_atom,atom1.id);
                atomid.emplace(mut_atom,atom1.id);
              }
              continue;
            }
            else if(count(begin(COMMON_ATOM),end(COMMON_ATOM),residueB[kk].name)) {
              continue;
            }
            else {
              cout<<"Undefined atom map for mutation "<<mname<<endl;
              return 1;
            }
          }
          for(int kk=0;kk<residueA.size();kk++) {
            outT<<setw(5)<<residueA[kk].id<<"      "<<setw(5)<<residueA[kk].type<<"  "<<setw(5)<<residueA[kk].resid;
            outT<<"    MUT  "<<setw(5)<<residueA[kk].name<<"  "<<setw(5)<<residueA[kk].id<<" ";
            outT<<setw(11)<<fixed<<setprecision(8)<<residueA[kk].charge<<"  "<<setw(9)<<fixed<<setprecision(6)<<residueA[kk].mass;
            outT<<"  "<<residueA[kk].other<<endl;
            string atomname=residueA[kk].name;
            //cout<<residueA[kk].id<<" "<<residueA[kk].name<<" "<<atomname<<" "<<heavyatom.size()<<endl;
            //if(atomname[0]!='H' || !(atomname.size()>1 && atomname[0]=='D' && atomname[1]=='H'))
            if(atomname[0]=='H') continue;
            if(atomname.size()>1 && atomname[0]=='D' && atomname[1]=='H') continue;
              heavyatom.push_back(residueA[kk].id);
          }
        }
      }
      outT<<endl;

      //for(auto& it : atomid)
        //cout<<it.first<<" "<<it.second<<endl;
      //for(auto& it : mut_wt_atoms)
        //cout<<it.first<<" "<<it.second<<endl;
      int nbond=molA.bonds.size();
      if(nbond>0) {
        for(int jj=0;jj<2;jj++)
          outT<<molA.bonds[jj]<<endl;
        struct Bond {
          int ai,aj,funct;
          string coeff;
        };
        vector<Bond> bondA;
        for(int jj=2;jj<nbond;jj++) {
          vector<string> stoken=split_string(molA.bonds[jj]);
          if(stoken.size()!=5) {
            cout<<"Wrong line in [ bonds ] in topA --> "<<topA.molecules[ii].bonds[jj]<<endl;
            return 1;
          }
          string atom1=stoken[0];
          string atom2=stoken[1];
          int funct=stoi(stoken[2]);
          string coeff=stoken[3]+" "+stoken[4];
          if(atomid.count(atom1)==0 || atomid.count(atom2)==0) {
            cout<<"Cannot find id for atom --> "<<atom1<<" || "<<atom2<<endl;
            return 1;
          }
          int ai=atomid[atom1];
          int aj=atomid[atom2];
          if(mut_wt_atoms.count(atom1) || mut_wt_atoms.count(atom2)) {
            Bond bond1;
            bond1.ai=ai;
            bond1.aj=aj;
            bond1.funct=funct;
            bond1.coeff=coeff;
            bondA.push_back(bond1);
          }
          else {
            outT<<setw(7)<<ai<<setw(7)<<aj<<setw(6)<<funct<<"   "<<coeff<<"  "<<coeff<<endl;
          }
        }
        vector<Bond> bondB;
        for(int jj=2;jj<molB.bonds.size();jj++) {
          vector<string> stoken=split_string(molB.bonds[jj]);
          if(stoken.size()!=5) {
            cout<<"Wrong line in [ bonds ] in topB --> "<<topB.molecules[ii].bonds[jj]<<endl;
            return 1;
          }
          string atom1=stoken[0];
          string atom2=stoken[1];
          if(mut_wt_atoms.count(atom1) || mut_wt_atoms.count(atom2)) {
            Bond bond1;
            if(atomid.count(atom1)==0 || atomid.count(atom2)==0) {
              cout<<"Cannot find id for atom --> "<<atom1<<" || "<<atom2<<endl;
              return 1;
            }
            int ai=atomid[atom1];
            int aj=atomid[atom2];
            if(ai>aj) swap(ai,aj);
            bond1.ai=ai;
            bond1.aj=aj;
            bond1.funct=stoi(stoken[2]);
            bond1.coeff=stoken[3]+" "+stoken[4];
            bondB.push_back(bond1);
          }
        }
        for(int kk=0;kk<bondA.size();kk++) {
          int ai=bondA[kk].ai;
          int aj=bondA[kk].aj;
          outT<<setw(7)<<ai<<setw(7)<<aj<<setw(6)<<bondA[kk].funct<<"   "<<bondA[kk].coeff;
          int ni=-1;
          for(int nn=0;nn<bondB.size();nn++) {
            if(ai==bondB[nn].ai && aj==bondB[nn].aj) {
              ni=nn;
              break;
            }
          }
          if(ni<0) {
            outT<<"  "<<bondA[kk].coeff<<endl;
          }
          else {
            outT<<"  "<<bondB[ni].coeff<<endl;
            bondB.erase(bondB.begin()+ni);
          }
        }
        for(int kk=0;kk<bondB.size();kk++) {
          outT<<setw(7)<<bondB[kk].ai<<setw(7)<<bondB[kk].aj<<setw(6)<<bondB[kk].funct<<"   "<<bondB[kk].coeff;
          outT<<"  "<<bondB[kk].coeff<<endl;
        }
        outT<<endl;
      }

      int npair=molA.pairs.size();
      if(npair>0) {
        for(int jj=0;jj<2;jj++)
          outT<<molA.pairs[jj]<<endl;
        vector<int> Vai,Vaj;
        for(int jj=2;jj<npair;jj++) {
          vector<string> stoken=split_string(molA.pairs[jj]);
          if(stoken.size()!=3) {
            cout<<"Wrong line in [ pairs ] in topA --> "<<topA.molecules[ii].pairs[jj]<<endl;
            return 1;
          }
          string atom1=stoken[0];
          string atom2=stoken[1];
          int funct=stoi(stoken[2]);
          if(atomid.count(atom1)==0 || atomid.count(atom2)==0) {
            cout<<"Cannot find id for atom --> "<<atom1<<" || "<<atom2<<endl;
            return 1;
          }
          int ai=atomid[atom1];
          int aj=atomid[atom2];
          Vai.push_back(ai);
          Vaj.push_back(aj);
          outT<<setw(7)<<ai<<setw(7)<<aj<<setw(6)<<funct<<endl;
        }
        for(int jj=2;jj<molB.pairs.size();jj++) {
          vector<string> stoken=split_string(molB.pairs[jj]);
          if(stoken.size()!=3) {
            cout<<"Wrong line in [ pairs ] in topB --> "<<topB.molecules[ii].pairs[jj]<<endl;
            return 1;
          }
          string atom1=stoken[0];
          string atom2=stoken[1];
          int funct=stoi(stoken[2]);
          if(atomid.count(atom1)==0 || atomid.count(atom2)==0) {
            cout<<"Cannot find id for atom --> "<<atom1<<" || "<<atom2<<endl;
            return 1;
          }
          int ai=atomid[atom1];
          int aj=atomid[atom2];
          if(ai>aj) swap(ai,aj);
          int flag=0;
          for(int nn=0;nn<Vai.size();nn++) {
            if(ai==Vai[nn] && aj==Vaj[nn]) {
              flag=1;
              break;
            }
          }
          if(!flag) {
            outT<<setw(7)<<ai<<setw(7)<<aj<<setw(6)<<funct<<endl;
          }
        }
        outT<<endl;
      }  

      int nangle=molA.angles.size();
      if(nangle>0) {
        for(int jj=0;jj<2;jj++) 
          outT<<molA.angles[jj]<<endl;
        struct Angle {
          int ai,aj,ak,funct;
          string coeff;
        };
        vector<Angle> angleA;
        for(int jj=2;jj<nangle;jj++) {
          vector<string> stoken=split_string(molA.angles[jj]);
          if(stoken.size()!=6) {
            cout<<"Wrong line in [ angles ] in topA --> "<<topA.molecules[ii].angles[jj]<<endl;
            return 1;
          }
          string atom1=stoken[0];
          string atom2=stoken[1];
          string atom3=stoken[2];
          int funct=stoi(stoken[3]);
          string coeff=stoken[4]+" "+stoken[5];
          if(atomid.count(atom1)==0 || atomid.count(atom2)==0 || atomid.count(atom3)==0) {
            cout<<"Cannot find id for atom --> "<<atom1<<" || "<<atom2<<" || "<<atom3<<endl;
            return 1;
          }
          int ai=atomid[atom1];
          int aj=atomid[atom2];
          int ak=atomid[atom3];
          if(mut_wt_atoms.count(atom1) || mut_wt_atoms.count(atom2) || mut_wt_atoms.count(atom3)) {
            Angle ang1;
            ang1.ai=ai;
            ang1.aj=aj;
            ang1.ak=ak;
            ang1.funct=funct;
            ang1.coeff=coeff;
            angleA.push_back(ang1);
          }
          else {
            outT<<setw(7)<<ai<<setw(7)<<aj<<setw(7)<<ak<<setw(6)<<funct<<"   "<<coeff<<"  "<<coeff<<endl;
          }
        }
        vector<Angle> angleB;
        for(int jj=2;jj<molB.angles.size();jj++) {
          vector<string> stoken=split_string(molB.angles[jj]);
          if(stoken.size()!=6) {
            cout<<"Wrong line in [ angles ] in topB --> "<<topB.molecules[ii].angles[jj]<<endl;
            return 1;
          }
          string atom1=stoken[0];
          string atom2=stoken[1];
          string atom3=stoken[2];
          if(mut_wt_atoms.count(atom1) || mut_wt_atoms.count(atom2) || mut_wt_atoms.count(atom3)) {
            Angle ang1;
            if(atomid.count(atom1)==0 || atomid.count(atom2)==0 || atomid.count(atom3)==0) {
              cout<<"Cannot find id for atom --> "<<atom1<<" || "<<atom2<<" || "<<atom3<<endl;
              return 1;
            }
            int ai=atomid[atom1];
            int aj=atomid[atom2];
            int ak=atomid[atom3];
            if(ai>ak) swap(ai,ak);
            ang1.ai=ai;
            ang1.aj=aj;
            ang1.ak=ak;
            ang1.funct=stoi(stoken[3]);
            ang1.coeff=stoken[4]+" "+stoken[5];
            angleB.push_back(ang1);
          }
        }
        for(int kk=0;kk<angleA.size();kk++) {
          int ai=angleA[kk].ai;
          int aj=angleA[kk].aj;
          int ak=angleA[kk].ak;
          outT<<setw(7)<<ai<<setw(7)<<aj<<setw(7)<<ak<<setw(6)<<angleA[kk].funct<<"   "<<angleA[kk].coeff;
          int ni=-1;
          for(int nn=0;nn<angleB.size();nn++) {
            if(ai==angleB[nn].ai && aj==angleB[nn].aj && ak==angleB[nn].ak) {
              ni=nn;
              break;
            }
          }
          if(ni<0) {
            outT<<"  "<<angleA[kk].coeff<<endl;
          }
          else {
            outT<<"  "<<angleB[ni].coeff<<endl;
            angleB.erase(angleB.begin()+ni);
          }
        }
        for(int kk=0;kk<angleB.size();kk++) {
          outT<<setw(7)<<angleB[kk].ai<<setw(7)<<angleB[kk].aj<<setw(7)<<angleB[kk].ak<<setw(6)<<angleB[kk].funct;
          outT<<"   "<<angleB[kk].coeff<<"  "<<angleB[kk].coeff<<endl;
        }
        outT<<endl;
      }

      int ndihedral=molA.dihedrals.size();
      if(ndihedral>0) {
        for(int jj=0;jj<2;jj++) 
          outT<<molA.dihedrals[jj]<<endl;
        struct Dihedral {
          int ai,aj,ak,al,funct;
          string coeff;
          int m;
        };
        vector<Dihedral> dihedralA;
        for(int jj=2;jj<ndihedral;jj++) {
          vector<string> stoken=split_string(molA.dihedrals[jj]);
          if(stoken.size()!=8) {
            cout<<"Wrong line in [ dihedrals ] in topA --> "<<topA.molecules[ii].dihedrals[jj]<<endl;
            return 1;
          }
          string atom1=stoken[0];
          string atom2=stoken[1];
          string atom3=stoken[2];
          string atom4=stoken[3];
          int funct=stoi(stoken[4]);
          string coeff=stoken[5]+"  "+stoken[6]+"  ";
          int m=stoi(stoken[7]);
          if(atomid.count(atom1)==0 || atomid.count(atom2)==0 || atomid.count(atom3)==0 || atomid.count(atom4)==0) {
            cout<<"Cannot find id for atom --> "<<atom1<<" || "<<atom2<<" || "<<atom3<<" || "<<atom4<<endl;
            return 1;
          }
          int ai=atomid[atom1];
          int aj=atomid[atom2];
          int ak=atomid[atom3];
          int al=atomid[atom4];
          if(mut_wt_atoms.count(atom1) || mut_wt_atoms.count(atom2) || mut_wt_atoms.count(atom3) || mut_wt_atoms.count(atom4)) {
            Dihedral dih1;
            dih1.ai=ai;
            dih1.aj=aj;
            dih1.ak=ak;
            dih1.al=al;
            dih1.funct=funct;
            dih1.coeff=coeff;
            dih1.m=m;
            dihedralA.push_back(dih1);
          }
          else {
            outT<<setw(7)<<ai<<setw(7)<<aj<<setw(7)<<ak<<setw(7)<<al<<setw(6)<<funct<<"  "<<coeff<<m<<"  "<<coeff<<m<<endl;
          }
        }
        vector<Dihedral> dihedralB;
        for(int jj=2;jj<molB.dihedrals.size();jj++) {
          vector<string> stoken=split_string(molB.dihedrals[jj]);
          if(stoken.size()!=8) {
            cout<<"Wrong line in [ dihedrals ] in topB --> "<<topB.molecules[ii].dihedrals[jj]<<endl;
            return 1;
          }
          string atom1=stoken[0];
          string atom2=stoken[1];
          string atom3=stoken[2];
          string atom4=stoken[3];
          if(mut_wt_atoms.count(atom1) || mut_wt_atoms.count(atom2) || mut_wt_atoms.count(atom3) || mut_wt_atoms.count(atom4)) {
            Dihedral dih1;
            if(atomid.count(atom1)==0 || atomid.count(atom2)==0 || atomid.count(atom3)==0 || atomid.count(atom4)==0) {
              cout<<"Cannot find id for atom --> "<<atom1<<" || "<<atom2<<" || "<<atom3<<" || "<<atom4<<endl;
              return 1;
            }
            int ai=atomid[atom1];
            int aj=atomid[atom2];
            int ak=atomid[atom3];
            int al=atomid[atom4];
            if(ai>al) {
              swap(ai,al);
              swap(aj,ak);
            }
            dih1.ai=ai;
            dih1.aj=aj;
            dih1.ak=ak;
            dih1.al=al;
            dih1.funct=stoi(stoken[4]);
            dih1.coeff=stoken[5]+"  "+stoken[6]+"  ";
            dih1.m=stoi(stoken[7]);
            dihedralB.push_back(dih1);
          }
        }
        for(int kk=0;kk<dihedralA.size();kk++) {
          int ai=dihedralA[kk].ai;
          int aj=dihedralA[kk].aj;
          int ak=dihedralA[kk].ak;
          int al=dihedralA[kk].al;
          int m=dihedralA[kk].m;
          outT<<setw(7)<<ai<<setw(7)<<aj<<setw(7)<<ak<<setw(7)<<al<<setw(6)<<dihedralA[kk].funct<<"  "<<dihedralA[kk].coeff<<m;
          int ni=-1;
          int check_id=0;
          for(int nn=0;nn<dihedralB.size();nn++) {
            if(ai==dihedralB[nn].ai && aj==dihedralB[nn].aj && ak==dihedralB[nn].ak && al==dihedralB[nn].al) {
              check_id=1;
              if(m==dihedralB[nn].m) {
                ni=nn;
                break;
              }
            }
          }
          if(ni<0 && check_id==0) {
            outT<<"  "<<dihedralA[kk].coeff<<m<<endl;
          }
          else if(ni<0 && check_id) {
            outT<<"  0.0000000  0.0000000  "<<m<<endl;
          }
          else {
            outT<<"  "<<dihedralB[ni].coeff<<dihedralB[ni].m<<endl;
            dihedralB.erase(dihedralB.begin()+ni);
          }
        }
        for(int kk=0;kk<dihedralB.size();kk++) {
          int ai=dihedralB[kk].ai;
          int aj=dihedralB[kk].aj;
          int ak=dihedralB[kk].ak;
          int al=dihedralB[kk].al;
          int m=dihedralB[kk].m;
          int ni=-1;
          int check_id=0;
          for(int nn=0;nn<dihedralA.size();nn++) {
            if(ai==dihedralA[nn].ai && aj==dihedralA[nn].aj && ak==dihedralA[nn].ak && al==dihedralA[nn].al) {
              check_id=1;
              if(m==dihedralA[nn].m) {
                ni=nn;
                break;
              }
            }
          }

          if(ni>=0) {
            cout<<"Error in write dihedral coeff"<<endl;
            return 1;
          }
          
          outT<<setw(7)<<dihedralB[kk].ai<<setw(7)<<dihedralB[kk].aj<<setw(7)<<dihedralB[kk].ak<<setw(7)<<dihedralB[kk].al;
          outT<<setw(6)<<dihedralB[kk].funct<<"  ";
          if(check_id==0)
            outT<<dihedralB[kk].coeff<<m<<"  "<<dihedralB[kk].coeff<<m<<endl;
          else
            outT<<"0.0000000  0.0000000  "<<m<<"  "<<dihedralB[kk].coeff<<m<<endl;
        }
        outT<<endl;
      }

      int nother=molA.others.size();
      if(nother>0) {
        for(auto sline : molA.others)
          outT<<sline<<endl;
        outT<<endl;
      }
    }

    if(fposres && count(begin(WAT_ION),end(WAT_ION),moleculename)==0) {
      outT<<"#ifdef POSRES"<<endl;
      outT<<"[ position_restraints ]"<<endl;
      outT<<";   i funct       fcx        fcy        fcz"<<endl;
      for(auto id : heavyatom) {
        outT<<setw(5)<<id<<"    1"<<setw(11)<<fixed<<setprecision(1)<<fc<<setw(11)<<fixed<<setprecision(1)<<fc;
        outT<<setw(11)<<fixed<<setprecision(1)<<fc<<endl;
      }
      outT<<"#endif"<<endl;
      outT<<endl;
    }
  }

  for(auto sline : topA.tails)
    outT<<sline<<endl;
  outT.close(); outT.clear();

  Molecule groA=read_gro(ingro1);
  Molecule groB=read_gro(ingro2);
  if(groA.sequence.size()!=groB.sequence.size()) {
    cout<<"Number of residues is not matched in groA/groB --> "<<groA.sequence.size()<<" / "<<groB.sequence.size()<<endl;
    return 1;
  }
  vector<string> mutations;
  for(int jj=0;jj<groA.sequence.size();jj++) {
    //cout<<jj<<" : "<<groA.residues[jj][0].substr(0,8)<<" "<<groA.sequence[jj]<<" --> ";
    //cout<<groB.residues[jj][0].substr(0,8)<<" "<<groB.sequence[jj]<<endl;
    if(groA.sequence[jj]!=groB.sequence[jj]) {
      string mut=groB.sequence[jj]+"->"+groA.sequence[jj];
      string str=groA.residues[jj][0].substr(0,5)+"  "+groA.sequence[jj]+"->"+groB.sequence[jj];
      mutations.push_back(str);
      vector<string> atomA;
      string resid_A=groA.residues[jj][0].substr(0,5);
      for(int kk=0;kk<groA.residues[jj].size();kk++) {
        str=groA.residues[jj][kk].substr(10,5);
        vector<string> stoken=split_string(str);
        atomA.push_back(stoken[0]);
        groA.residues[jj][kk].replace(5,3,"MUT");
      }
      for(int kk=0;kk<groB.residues[jj].size();kk++) {
        str=groB.residues[jj][kk].substr(10,5);
        vector<string> stoken=split_string(str);
        string atomname=stoken[0];
        string aname=mut+"."+atomname;
        if(atommap.count(aname)) {
          string atomBA=atommap[aname];
          if(count(atomA.begin(),atomA.end(),atomBA)==0) {
            string sline=groB.residues[jj][kk];
            sline.replace(5,3,"MUT");
            ostringstream ostr;
            ostr<<setw(5)<<atomBA;
            atomBA=ostr.str();
            sline.replace(10,5,atomBA);
            sline.replace(0,5,resid_A);
            groA.residues[jj].push_back(sline);
          }
          continue;
        }
        else if(count(begin(COMMON_ATOM),end(COMMON_ATOM),atomname)) continue;
        else {
          cout<<"Undefined atom map for mutation "<<aname<<endl;
          return 1;
        }
      }
    }
  }

  cout<<"Mutations in gro A/B:"<<endl;
  for(auto sline : mutations)
    cout<<sline<<endl;

  int ntot=0;
  for(int jj=0;jj<groA.residues.size();jj++)
    ntot+=groA.residues[jj].size();
  cout<<"Number of atoms: "<<groA.heads[1]<<" --> "<<ntot<<endl;
  ofstream outG (outgro.c_str());
  outG<<groA.heads[0]<<endl;
  outG<<ntot<<endl;
  int natom=0;
  for(int jj=0;jj<groA.residues.size();jj++) {
    for(int kk=0;kk<groA.residues[jj].size();kk++) {
      natom++;
      ostringstream ostr;
      ostr<<setw(5)<<natom;
      string str=ostr.str();
      string sline=groA.residues[jj][kk];
      sline.replace(15,5,str);
      outG<<sline<<endl;
    }
  }
  outG<<groA.others[0]<<endl;
  outG.close(); outG.clear();

  return 0;
}

