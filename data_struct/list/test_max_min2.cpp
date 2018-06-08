/*************************************************************************
	> File Name: test_max_min_2.cpp
	> Author: 
	> Mail: 
	> Created Time: 2017年08月11日 星期五 11时53分49秒
 ************************************************************************/

#include<iostream>
#include<algorithm>  // std::max_element | std::min_element
using namespace std;


bool myfn(int i, int j) {
  return i < j;
}

struct myclass {
  bool operator() (int i, int j) {return i < j;}
} myobj;

int main(int argc, char *argv) {
  int myints[] = {3, 7, 2, 5, 6, 4, 9};

  // using default comparison:
  cout<<"The min is: "<<*std::min_element(myints, myints+7) << "\n";
  cout<<"The max is: "<<*std::max_element(myints, myints+7) << "\n";

  // using function myfn as comparison
  cout<<"The min is: "<<*std::min_element(myints, myints+7, myfn)<<"\n";
  cout<<"The max is: "<<*std::max_element(myints, myints+7, myfn)<<"\n";

  // using object myobj as comparison
  cout<<"The min is: "<<*std::min_element(myints, myints+7, myobj)<<"\n";
  cout<<"The max is: "<<*std::max_element(myints, myints+7, myobj)<<"\n";

  return 0;
}
