#include <fstream>
#include <iostream>
#include <string>
#include <iomanip>
#include <vector>

using namespace std;



int main() {

  std::string line,prop,value;
  std::vector <std::string> devs;
  std::vector <std::vector <unsigned int> > data;

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

   
  return 0;
}

