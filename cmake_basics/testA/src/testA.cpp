#include "testA.h"
#include <iostream>

TestA::TestA(const int &_type) { m_type_ = _type; }

TestA::~TestA() {}

bool TestA::InitType(const int &type) { m_type_ = type; }

bool TestA::GetString(std::string &data_str) {
  switch (m_type_) {
  case 1: {
    data_str = "AAAA";
    break;
  }
  case 2: {
    data_str = "BBBB";
    break;
  }
  case 3: {
    data_str = "CCCC";
    break;
  }
  default: {
    data_str = "DDDD";
    break;
  }
  }

  std::cout << "m_type_: " << m_type_ << ", data_str: " << data_str
            << std::endl;
  return true;
}
