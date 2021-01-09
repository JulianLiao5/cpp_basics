#include <deque>
#include <stdio.h>
#include <string>

#include <iostream>
#include <limits.h>

int main(int argc, char *argv[]) {

  printf("line: %d, void_*_size: %ld\n", __LINE__, sizeof(void *));
  std::cout << __LINE__ << "  sizeof_void* " << sizeof(void *) << std::endl;
  /**------------------BEGIN char-----------------------**/
  /**
  char cc2;
  signed char sc1;
  unsigned char usc1;
  printf("line: %d, char_size: %ld, signed_char_size: %ld, unsigned_char_size: %ld\n", __LINE__, sizeof(cc2), sizeof(sc1), sizeof(usc1));
  std::cout << __LINE__ << "  min_signed_char: " << SCHAR_MIN << ", max_signed_char: " << SCHAR_MAX << std::endl;
  std::cout << __LINE__ << "  min_char: " << CHAR_MIN << ", max_char: " << CHAR_MAX << std::endl;
  std::cout << __LINE__ << "  max_unsigned_char: " << UCHAR_MAX << std::endl;
  **/
  /**------------------END char-----------------------**/
  /**------------------BEGIN short-----------------------**/
  /**
  unsigned short us33;
  short int cc;
  short dd;
  printf("unsigned short_size: %ld, short int size: %ld, short_size: %ld\n", sizeof(us33), sizeof(cc), sizeof(dd));
  std::cout << __LINE__ << "  min_short_int: " << SHRT_MIN << ", max_short_int: " << SHRT_MAX << std::endl;
  std::cout << __LINE__ << "  max_unsigned_short_int: " << USHRT_MAX << std::endl;
  **/
  /**------------------END short-----------------------**/
  /**------------------BEGIN int-----------------------**/
  /**
  int aa;
  unsigned int bb;
  printf("int_size: %ld, unsigned int size: %ld\n", sizeof(aa), sizeof(bb));
  std::cout << __LINE__ << "  min_int: " << INT_MIN << ", max_int: " << INT_MAX << std::endl;
  std::cout << __LINE__ << "  max_unsigned_int: " << UINT_MAX << std::endl;
  **/
  /**------------------END int-----------------------**/
  /**------------------BEGIN long-----------------------**/
  /**
  long laa;
  unsigned long lbb;
  printf("long_size: %ld, unsigned long size: %ld\n", sizeof(laa), sizeof(lbb));
  std::cout << __LINE__ << "  min_long_int: " << LONG_MIN << ", max_long_int: " << LONG_MAX << std::endl;
  std::cout << __LINE__ << "  max_unsigned_long_int: " << ULONG_MAX << std::endl;
  **/
  /**------------------END long-----------------------**/
  /**------------------BEGIN long long-----------------------**/
  /**
  long long laal;
  unsigned long long lbbl;
  printf("long_long size: %ld, unsigned long long size: %ld\n", sizeof(laal), sizeof(lbbl));
  std::cout << __LINE__ << "  min_long_long_int: " << LLONG_MIN << ", max_long_long_int: " << LLONG_MAX << std::endl;
  std::cout << __LINE__ << "  max_unsigned_long_long_int: " << ULLONG_MAX << std::endl;
  **/
  /**------------------END long long-----------------------**/

  /**------------------BEGIN x-bytes int-----------------------**/
  /**
  uint8_t a1;
  int8_t a2;
  uint16_t b1;
  int16_t b2;
  uint32_t c1;
  int32_t c2;
  uint64_t d1;
  int64_t d2;
  printf("uint8_t_size: %ld, int8_t size: %ld, uint16_t_size: %ld, int16_t size: %ld\n", sizeof(a1), sizeof(a2), sizeof(b1), sizeof(b2));
  printf("uint32_t_size: %ld, int32_t size: %ld, uint64_t_size: %ld, int64_t size: %ld\n", sizeof(c1), sizeof(c2), sizeof(d1), sizeof(d2));
  **/
  /**------------------END x-bytes int-----------------------**/

  /**------------------BEGIN float/double-----------------------**/
  /**
  float f1;
  double d1;
  printf("float_size: %ld, double size: %ld\n", sizeof(f1), sizeof(d1));
  **/
  /**------------------END float/double-----------------------**/

/**
  printf("min_int8_t: %d, max_int8_t: %d\n", MIN_OF(int8_t), MAX_OF(int8_t));
  printf("min_int16_t: %d, max_int16_t: %d\n", MIN_OF(int16_t), MAX_OF(int16_t));
  printf("min_int32_t: %d, max_int32_t: %d\n", MIN_OF(int32_t), MAX_OF(int32_t));
**/

  std::string tmp_str = "d8:12:65:65:08:7d-H001";
  std::string delimiter = "-";
  std::string decrypt_mac_addr = tmp_str.substr(0, tmp_str.find(delimiter));
  std::string decrypt_car_name = tmp_str.substr(tmp_str.find(delimiter)+delimiter.length());
  std::cout << "decrypt_mac_addr: " << decrypt_mac_addr << ", decrypt_car_name: " << decrypt_car_name << std::endl;

  std::string s1 = "0051";
  std::string s2 =  "00 01 00 00 00 00 00 00 ";
  printf("s1.size: %ld, s2.size: %ld\n", s1.size(), s2.size());

  printf("**************test case 1****\n");
  std::deque<int> d = {7, 5, 16, 8};

  // Add an integer to the beginning of the deque
  d.push_front(13);
  // Add an integer to the end of the deque
  d.push_back(25);

    printf("[");
    for(int n : d) {
      printf("%d ", n);
    }
    printf("]\n");





    for(std::deque<int>::iterator it = d.begin(); it != d.end(); ++it) {
      printf("%d, ", *it);
    }
    printf("\n");





}
