/*************************************************************************
	> File Name: test_fstream_mode.cpp
	> Author: 
	> Mail: 
	> Created Time: 2018年08月21日 星期二 20时59分19秒
 ************************************************************************/

#include<iostream>
#include<sstream>
using namespace std;


// fstream constructor.
#include <fstream>      // std::fstream

int main (int argc, char **argv) {
  std::ostringstream oss;

  for (int n=0; n<100; ++n)
  {
    oss << n;
    oss.str("");
    // outfile.str().clear();
  }
  std::cout << oss.str() << std::endl;

/**
  ofstream radar77File;
  radar77File.open("./radar77_data.txt", ios::out);
  // radar77File << "This is a test" << std::endl;
  radar77File << "what is wrong!" << std::endl;
  radar77File.close();


    std::fstream fs ("./test.txt", std::fstream::out | std::fstream::trunc);

  fs << 1;
  fs << 2;
  fs << 3;
  // i/o operations here

  fs.close();


  fs << 4;
  fs << 5;
  fs << 6;
  // i/o operations here

  fs.close();  **/
  return 0;
}
