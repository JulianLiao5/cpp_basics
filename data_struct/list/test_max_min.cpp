/*************************************************************************
	> File Name: test_max_min.cpp
	> Author: 
	> Mail: 
	> Created Time: 2017年08月11日 星期五 10时58分00秒
 ************************************************************************/

#include<iostream>
#include<vector>    // vector
#include<string>    // string
#include<algorithm>  // std::max_element
using namespace std;

struct S {
  string myWord;
  int a;
  int b;
};



static bool CompForMax_a(S&, S&);



int main(int argc, char *argv[]) {
  vector<S> vec;
  for(int j = 0; j < 10; j++) {
    S tempS;
    tempS.a = j;
    tempS.b = 5+j;
    tempS.myWord = "aWord";
    vec.push_back(tempS);
  }

  vector<S>::iterator result;
  result = max_element(vec.begin(), vec.end(), CompForMax_a);
  cout<<"*(result).a: "<<(*result).a<<endl;

  int diff_index = result - vec.begin();
  cout<<"Maximum element S.a of vector<S> is at: "<<std::distance(vec.begin(), result)<<endl;
  cout<<"diff_index: "<<diff_index<<endl;

  cout<<"--------------------------------"<<endl;

  size_t range = 3;

  for(vector<S>::iterator itr=(vec.begin() + 6); itr != vec.end(); itr++) {
    size_t index = itr - vec.begin();
    vector<S>::iterator first = itr - range;
    vector<S>::iterator last = itr + 1;
    result = std::max_element(first, last, CompForMax_a);

    cout<<"max element of vec[].a between slot " << index - range << " and slot " << index << " is: " << (*result).a << endl;
    cout<<"vec["<<index<<"].a=" << (*itr).a<<endl;
  }

  return 0;
}


static bool CompForMax_a(S& S_0, S& S_1) {
  return (S_0.a < S_1.a);
}
