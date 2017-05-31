#include "Ngrams.h"

void ngram_load_structures( std::string &hist, 
                            std::string &freq, 
                            Ngrams<int> & ng) {
  std::cout << "Loading hist from: " << hist << std::endl;
  std::ifstream of1(hist);
  for (std::string line; std::getline(of1, line);) {
    auto vline = cutil_string_split(line, ',');
    if (vline.size() > 1)
      ng.hashObj_[stoi(vline[0])] = stoi(vline[1]);
  }
  of1.close();

  //..........................................................................
  std::ifstream of2(freq);
  std::cout << "Loading frequency table from: " << freq << std::endl;
  for (std::string line; std::getline(of2, line);) {
    auto vline = cutil_string_split(line, ',');
    if (vline.size() > 1) {
      auto a = stoi(vline[0]);
      auto b = stoi(vline[1]);
      ng.hashFreq_[a][b] = stoi(vline[2]);
    }
  }
  of2.close();
}