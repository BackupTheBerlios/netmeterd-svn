#include <fstream>
#include <iostream>
#include <string>
#include <iomanip>
#include <vector>

using namespace std;

void trim(std::string &);
void read_data(vector <string> &,vector <vector <unsigned int> > &);

int main() {

  std::string line,prop,value;
  std::vector <std::string> devs;
  std::vector <std::vector <unsigned int> > data;
  /*
  fstream archivo ("hola.txt");
  while (getline(archivo,line)) {
    if (line.find("=",0) == string::npos) { 
      std::cout << "Device: " << line << std::endl;
    } else { 
      std::cout << "   ";
      
      prop=line.substr(0,line.find("=",0));
      value=line.substr(line.find("=",0)+1,line.size()-line.find("=",0)-2);
      trim(value); trim(prop);
      std::cout << prop << " -> " << value << std::endl;
    }
    
    
  } 
  std::cout << "<<---<--------<->-------->--->>" << std::endl; */
  read_data(devs,data);
  std::cout << data[0][0];  
  return 0;
}

//Erase whitespaces from the beginning and end of a string, and modify it
void trim(std::string & str) {
  while (str[str.size()-1]==' ') str=str.substr(0,str.size()-2);
  while (str[0]==' ') str=str.substr(1,str.size()-1);
}


/*
We have to parse a file like that:
Inter-|   Receive                                                |  Transmit
 face |bytes    packets errs drop fifo frame compressed multicast|bytes    packets errs drop fifo colls carrier compressed
    lo: 1622114   18934    0    0    0     0          0         0  1622114   18934    0    0    0     0       0          0
  eth0:241657198 2347068    0    0    0     0          0         0 250050213  748964    0    0    0     0       0          0

... no comments...
*/
void read_data(std::vector <string> &devs,std::vector <vector <unsigned int> > &data) {
  int i=0,j=1;
  std::string temp,temp2;
  unsigned int pos,number;
  bool was_space=false;
  
  fstream file ("/proc/net/dev", ios::in );
  while (getline(file,temp)) { //read data
    if (i>=2) { //First 2 lines are headers
      temp2 = temp.substr(0,temp.find(':',0));
      trim(temp2);
      devs[i]=temp2; //Save the device name
      temp=temp.substr(temp.find(':',0)+1,temp.size()-temp.find(':',0)-2);
      trim(temp);
      for(pos=0;pos<=temp.size();pos++) {
        if (temp[pos]!=' ') {
          number=number*10 + temp[pos] - '0';
          was_space=false;
        }
        else {
          if (!was_space) { 
            if (j==1) data[i][0]=number; //save data1 (received)
            if (j==9) data[i][1]=number; //save data2 (sent)
            number=0;  
            j++;
          } 
          was_space=true;
        }
        pos++;
      }
    }
    i++;
  }
}
