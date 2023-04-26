#ifndef _BASIC_H_
#define _BASIC_H_

#include <cstddef>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <list>
#include <map>
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <cstdio>
#include <ctype.h>
#include <iomanip>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <iterator>
#include <time.h>
#include <assert.h>
#include <limits>
using namespace std;

static const double PI  = 3.14159265358979323846;
static const double EPS = 1E-6;

inline string to_string (char aa) {
  string bb;
  stringstream ss;
  ss << aa;
  ss >> bb;

  return bb;
}

inline vector<string> split_string(string sline, string sp=" ") {
  size_t found=sline.find_first_of(sp);
  while(found!=string::npos) {
    sline[found]=' ';
    found=sline.find_first_of(sp,found+1);
  }

  istringstream ss (sline);
  string buf;
  vector<string> token;
  while(ss >> buf) token.push_back(buf);

  return token;
}

template <class T>
inline int get_array_size(const T &arr) {
  return (sizeof(arr)/sizeof(arr[0]));
}

inline vector<string> read_file(string fstr) {
  vector<string> Vstr;
  string sline;
  ifstream inF (fstr.c_str());
  if(!inF.is_open()) {
    cout<<"Failed to read file: "<<fstr<<endl;
    exit(0);
  }
  while(getline(inF,sline)) {
    //if(sline.size()<1) continue;
    Vstr.push_back(sline);
  }
  inF.close(); inF.clear();

  return Vstr;
}

inline vector<string> get_dir_files (string dir) {
  vector<string> files;
  DIR *dp, *dp_tmp;
  struct dirent *dirp;
  if((dp  = opendir(dir.c_str())) == NULL) {
    cout << "Error(" << errno << ") opening " << dir << endl; //return errno;
  }

  while ((dirp = readdir(dp)) != NULL) {
    string dir_tmp = string(dirp->d_name);
    if(dir_tmp != "." && dir_tmp != "..") {
      if((dp_tmp  = opendir(dir_tmp.c_str())) == NULL) files.push_back(dir_tmp);
      else closedir(dp_tmp);
    }
  }
  closedir(dp);

  return files;
}

inline bool is_file_exist(const std::string& filename) {
  struct stat buf;
  if (stat(filename.c_str(), &buf) != -1)
    return true;
  else
    return false;
}

inline void pause() {
  using std::numeric_limits;
  using std::streamsize;
  cout << "Press ENTER to continue..." << flush;
  cin.clear();  // use only if a human is involved
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

#endif
