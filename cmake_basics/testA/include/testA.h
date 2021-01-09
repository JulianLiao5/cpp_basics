#pragma once

#include<string>

class TestA  {
 public:
  TestA(const int &type);

  ~TestA();

  bool GetString(std::string &data_str);

  bool InitType(const int &type);

 private:
  int m_type_;
};
