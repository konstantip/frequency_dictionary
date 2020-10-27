#include "frequency_dictionary.hpp"

namespace frequency_dictionary
{
std::vector<FrequencyDictionary::WordWithCount> FrequencyDictionary::getResults()
{
  std::vector<WordWithCount> result;
  result.reserve(words.size());

  for (auto it = std::begin(words); it != std::end(words); ++it)
  {
    result.push_back({it->second, it->first});
  }

  std::sort(std::begin(result), std::end(result), [](const WordWithCount w1, const WordWithCount w2) noexcept {
    return w1.count > w2.count || (w1.count == w2.count && w1.word > w2.word);
  });

  return result;
}

namespace
{
void toLower(std::string& str) noexcept
{
  std::transform(std::cbegin(str), std::cend(str), std::begin(str),
                 [](unsigned char c) noexcept { return std::tolower(c); });
}
}

void FrequencyDictionary::addWord(std::string& word)
{
  toLower(word);

  ++words[word];
}
}