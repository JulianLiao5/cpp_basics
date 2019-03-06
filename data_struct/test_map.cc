
#include <cmath>
#include <map>
#include <iostream>
#include <vector>
#include <numeric>

#define prev_range_num_maintained 4
#define diff_range_thresh 0.8  // unit: meter

int main(int argc, char *argv[]) {
  std::map<int, std::vector<float>> objs;
  std::vector<float> dists;
  dists.push_back(4.18);
  objs.insert(std::pair<int, std::vector<float>>(4, dists));
    std::cout << __LINE__ << std::endl;
  for (std::map<int, std::vector<float>>::iterator it = objs.begin(); it != objs.end(); ++it) {
      std::cout << "        " << it->first << " => {";
      for (float& dist_item : it->second) {
          std::cout << dist_item << ", ";
      }
      std::cout << "}" << std::endl;
  }
  std::vector<float> dists_4 = objs.at(4);
  dists_4.push_back(4.32);
  dists_4.push_back(4.17);
  objs.at(4) = dists_4;
  std::cout << __LINE__ << std::endl;
  for (std::map<int, std::vector<float>>::iterator it = objs.begin(); it != objs.end(); ++it) {
      std::cout << "        " << it->first << " => {";
      for (float& dist_item : it->second) {
          std::cout << dist_item << ", ";
      }
      std::cout << "}" << std::endl;
  }
  float input = 2.81;
  // calculate existing average range
  if (dists_4.size() > 0) {
    float avg_4 = std::accumulate(dists_4.begin(), dists_4.end(), 0.0) / dists_4.size();
    std::cout << "avg_4: " << avg_4 << std::endl;
    if (std::fabs(input - avg_4) > diff_range_thresh) {
        std::cout << "[filtered], avg_4: " << avg_4 << ", input: " << input << ", diff: " << (avg_4 - input) << std::endl;
    } else if (dists_4.size() < prev_range_num_maintained) {
        dists_4.push_back(input);
    } else if (dists_4.size() >= prev_range_num_maintained) {
        for (unsigned int i = 0; i < dists_4.size(); i++) {
            if (i != (dists_4.size() -1)) {
              dists_4[i] = dists_4[i+1];
            } else {
              dists_4[i] = input;
            }
        }
    }
  }

  std::vector<float> another_dists;
  another_dists.push_back(11.34);
  objs.insert(std::pair<int, std::vector<float>>(5, another_dists));
  std::cout << __LINE__ << std::endl;
  for (std::map<int, std::vector<float>>::iterator it = objs.begin(); it != objs.end(); ++it) {
      std::cout << "        " << it->first << " => {";
      for (float& dist_item : it->second) {
          std::cout << dist_item << ", ";
      }
      std::cout << "}" << std::endl;
  }

  return 0;
}
