#include "./basic.h"

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
    cout<<"No mdp files!"<<endl;
    exit(0);
  }
  if(Mcmd.count("-o")==0) {
    cout<<"No output!"<<endl;
    exit(0);
  }
  if(Mcmd.count("-l")==0) {
    cout<<"No lambdas!"<<endl;
    exit(0);
  }

  string inMDP=Mcmd["-i"];
  string outDIR=Mcmd["-o"];
  string inLambda=Mcmd["-l"];

  double lambda_cut=0.25;
  int ndup=5;
  if(Mcmd.count("-lc")!=0) {
    lambda_cut=stod(Mcmd["-lc"]);
  }
  if(Mcmd.count("-d")!=0) {
    ndup=stoi(Mcmd["-d"]);
  }

  if(inMDP.back()!='/') inMDP+='/';
  if(outDIR.back()!='/') outDIR+='/';
  string fstr=inMDP+"em.mdp";
  vector<string> em_mdp=read_file(fstr);
  fstr=inMDP+"nvt.mdp";
  vector<string> nvt_mdp=read_file(fstr);
  fstr=inMDP+"npt.mdp";
  vector<string> npt_mdp=read_file(fstr);
  fstr=inMDP+"md.mdp";
  vector<string> md_mdp=read_file(fstr);
  vector<string> lambdas=read_file(inLambda);

  int nlambda=-1;
  string llambda;
  for(auto sline : lambdas) {
    if(sline.find(";")!=string::npos) continue;
    int found0=sline.find("=");
    string str=sline.substr(found0+1);
    vector<string> stoken=split_string(str);
    int nl=stoken.size();
    if(nlambda==-1) {
      nlambda=nl;
      llambda=sline;
      continue;
    }
    else if(nlambda!=nl) {
      cout<<"Number of lambdas is not matched:"<<endl;
      cout<<llambda<<endl<<sline<<endl;
      return 1;
    }
  }

  cout<<"There are "<<nlambda<<" lambdas!"<<endl;
  
  vector<double> Vlambda;  //detemine constraints of bond type
  int flag_mass=0;
  int flag_fep=0;
  for(auto sline : lambdas) {
    if(sline.find("mass-lambdas")!=string::npos) flag_mass=1;
    if(sline.find("fep-lambdas")!=string::npos) flag_fep=1;
  }
  if(flag_mass) {
    for(auto sline : lambdas) {
      if(sline.find("mass-lambdas")!=string::npos) {
        int found0=sline.find("=");
        string str=sline.substr(found0+1);
        vector<string> stoken=split_string(str);
        for(int jj=0;jj<stoken.size();jj++)
          Vlambda.push_back(stod(stoken[jj]));
        break;
      }
    }
  }
  else if(flag_mass==0 && flag_fep==1) {
    for(auto sline : lambdas) {
      if(sline.find("fep-lambdas")!=string::npos) {
        int found0=sline.find("=");
        string str=sline.substr(found0+1);
        vector<string> stoken=split_string(str);
        for(int jj=0;jj<stoken.size();jj++)
          Vlambda.push_back(stod(stoken[jj]));
        break;
      }
    }
  }
  else {
    cout<<"Warning: mass-lambdas not is given!"<<endl;
    for(int jj=0;jj<nlambda;jj++)
      Vlambda.push_back(0.5);
  }

  for(int dd=1;dd<=ndup;dd++) {
    string sdir=outDIR+"dup"+to_string(dd);
    string cmd="mkdir "+sdir;
    system(cmd.c_str());
    for(int ii=0;ii<nlambda;ii++) {
      string dir=sdir+"/lambda"+to_string(ii);
      cmd="mkdir "+dir;
      system(cmd.c_str());
      string bond_type=" h-bonds";
      //if(Vlambda[ii]<0.1 || Vlambda[ii]>0.9) bond_type=" all-bonds";
      fstr=dir+"/em.mdp";
      ofstream outF (fstr.c_str());
      int ncount=0;
      for(auto sline : em_mdp) {
        if(sline.find("init-lambda-state")!=string::npos && sline.find("=")!=string::npos && sline.find(";")==string::npos) {
          int found0=sline.find("=");
          sline=sline.substr(0,found0+1);
          sline+=" "+to_string(ii);
          outF<<sline<<endl;
          continue;
        }
        if(sline.find("constraints")!=string::npos && sline.find("=")!=string::npos && sline.find(";")==string::npos) {
          int found0=sline.find("=");
          sline=sline.substr(0,found0+1);
          sline+=bond_type;
          outF<<sline<<endl;
          continue;
        }
        if(sline.find("-lambdas")!=string::npos && sline.find(";")==string::npos) {
          ncount++;
          if(ncount==1) {
            for(auto ltmp : lambdas)
              outF<<ltmp<<endl;
          }
          continue;
        }
        outF<<sline<<endl;
      }
      outF.close(); outF.clear();

      fstr=dir+"/nvt.mdp";
      outF.open(fstr.c_str());
      ncount=0;
      for(auto sline : nvt_mdp) {
        if(sline.find("init-lambda-state")!=string::npos && sline.find("=")!=string::npos && sline.find(";")==string::npos) {
          int found0=sline.find("=");
          sline=sline.substr(0,found0+1);
          sline+=" "+to_string(ii);
          outF<<sline<<endl;
          continue;
        }
        if(sline.find("constraints")!=string::npos && sline.find("=")!=string::npos && sline.find(";")==string::npos) {
          int found0=sline.find("=");
          sline=sline.substr(0,found0+1);
          sline+=bond_type;
          outF<<sline<<endl;
          continue;
        }
        if(sline.find("-lambdas")!=string::npos && sline.find(";")==string::npos) {
          ncount++;
          if(ncount==1) {
            for(auto ltmp : lambdas)
              outF<<ltmp<<endl;
          }
          continue;
        }
        outF<<sline<<endl;
      }
      outF.close(); outF.clear();

      fstr=dir+"/npt.mdp";
      outF.open(fstr.c_str());
      ncount=0;
      for(auto sline : npt_mdp) {
        if(sline.find("init-lambda-state")!=string::npos && sline.find("=")!=string::npos && sline.find(";")==string::npos) {
          int found0=sline.find("=");
          sline=sline.substr(0,found0+1);
          sline+=" "+to_string(ii);
          outF<<sline<<endl;
          continue;
        }
        if(sline.find("constraints")!=string::npos && sline.find("=")!=string::npos && sline.find(";")==string::npos) {
          int found0=sline.find("=");
          sline=sline.substr(0,found0+1);
          sline+=bond_type;
          outF<<sline<<endl;
          continue;
        }
        if(sline.find("-lambdas")!=string::npos && sline.find(";")==string::npos) {
          ncount++;
          if(ncount==1) {
            for(auto ltmp : lambdas)
              outF<<ltmp<<endl;
          }
          continue;
        }
        outF<<sline<<endl;
      }
      outF.close(); outF.clear();

      fstr=dir+"/md.mdp";
      outF.open(fstr.c_str());
      ncount=0;
      vector<string> Vstep;
      for(auto sline : md_mdp) {
        if((sline.find("nsteps ")!=string::npos || sline.find("dt ")!=string::npos) && sline[0]!=';') {
          Vstep.push_back(sline);
          if(Vstep.size()==2) {
            int nsteps;
            double dt;
            string snsteps,sdt;
            for(int kk=0;kk<Vstep.size();kk++) {
              int found0=Vstep[kk].find("=");
              string str=Vstep[kk].substr(0,found0+1);
              string tmp=Vstep[kk].substr(found0+1);
              vector<string> stoken=split_string(tmp);
              if(str.find("nsteps")!=string::npos) {
                nsteps=stoi(stoken[0]);
                snsteps=str;
              }
              if(str.find("dt")!=string::npos) {
                dt=stod(stoken[0]);
                sdt=str;
              }
            }
            int fa=round(dt/0.001);
            if(Vlambda[ii]<lambda_cut || Vlambda[ii]>1-lambda_cut) {
              nsteps*=fa;
              outF<<snsteps<<" "<<nsteps<<endl;
              outF<<sdt<<" 0.001     ; 1 fs"<<endl;
            }
            else {
              outF<<snsteps<<" "<<nsteps<<endl;
              outF<<sdt<<" "<<dt<<endl;
            }
          }
          continue;
        }
        if(sline.find("init-lambda-state ")!=string::npos && sline.find("=")!=string::npos && sline.find(";")==string::npos) {
          int found0=sline.find("=");
          sline=sline.substr(0,found0+1);
          sline+=" "+to_string(ii);
          outF<<sline<<endl;
          continue;
        }
        if(sline.find("constraints ")!=string::npos && sline.find("=")!=string::npos && sline.find(";")==string::npos) {
          int found0=sline.find("=");
          sline=sline.substr(0,found0+1);
          sline+=bond_type;
          outF<<sline<<endl;
          continue;
        }
        if(sline.find("-lambdas ")!=string::npos && sline.find(";")==string::npos) {
          ncount++;
          if(ncount==1) {
            for(auto ltmp : lambdas)
              outF<<ltmp<<endl;
          }
          continue;
        }
        outF<<sline<<endl;
      }
      if(Vstep.size()!=2) {
        cout<<" nsteps or dt is not given in md.mdp"<<endl;
        for(auto sline : Vstep)
          cout<<sline<<endl;
        return 1;
      }
      outF.close(); outF.clear();
    }
  }
}

