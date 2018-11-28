/*************************************************************************
	> File Name: main.cpp
	> Author: 
	> Mail: 
	> Created Time: 2018年11月27日 星期二 21时19分37秒
 ************************************************************************/

#include<iostream>
#include <QApplication>
#include <QLabel>
using namespace std;

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  QLabel hello("<center>Welcome to my first Wiki, How Qt program</center>");
  hello.setWindowTitle("My First Wiki, How Qt program");
  hello.resize(400, 400);
  hello.show();
  return app.exec();
}
