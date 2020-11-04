#pragma once

#include <algorithm>
#include <string>
#include <vector>
#include <unordered_map>

namespace frequency_dictionary
{

class FrequencyDictionary final
{
 public:
  struct WordWithCount final
  {
    std::size_t count;
    std::string_view word;
  };

  FrequencyDictionary();

  void addWord(std::string& word);

  std::vector<WordWithCount> getResults();

 private:
  std::unordered_map<std::string, std::size_t> words;
};
}