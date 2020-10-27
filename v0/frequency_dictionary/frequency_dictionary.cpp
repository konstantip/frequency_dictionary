#include "frequency_dictionary.hpp"

namespace frequency_dictionary
{

std::size_t FrequencyDictionary::Word::Hasher::operator()(const Word& w) const noexcept
{
  return std::hash<std::string>{}(w.word);
}

bool FrequencyDictionary::Word::EqualityChecker::operator()(const Word& w1, const Word& w2) const noexcept
{
  return w1.word == w2.word;
}

FrequencyDictionary::WordsCount::WordsCount(Word& word)
{
  insert(word);
}

bool FrequencyDictionary::WordsCount::empty() const noexcept
{
  return words_.empty();
}

void FrequencyDictionary::WordsCount::remove(const Word& word) noexcept
{
  words_.erase(words_.iterator_to(word));
}

void FrequencyDictionary::WordsCount::insert(Word& word) noexcept
{
  words_.push_back(word);
}

void FrequencyDictionary::WordsCount::getSortedWordsPointers(std::vector<Word*>& output_vector)
{
  assert(output_vector.empty());

  std::transform(std::begin(words_), std::end(words_), std::back_inserter(output_vector),
                 [](Word& word) { return &word; });

  std::sort(std::begin(output_vector), std::end(output_vector),
            [](const Word* w1, const Word* w2) noexcept { return w1->word < w2->word; });
}

void FrequencyDictionary::addWord(std::string& word)
{
  toLower(word);

  //Sorry for this, it will be fixed
  const auto[it, inserted] = [&words = words_, &word]() {
    Word tmp{std::move(word)};
    const auto it_ = words.find(tmp);
    word = std::move(tmp.word);
    if (it_ != std::cend(words))
    {
      return std::pair{it_, false};
    }
    return words.emplace(word);
  }();

  //It is needed because (*it) is const-qualified, it might be fixed wia using boost::intrusive::unordered_set
  if (Word& element = const_cast<Word&>(*it); inserted)
  {
    addNew(element);
  }
  else
  {
    increaseWordCount(element);
  }
}

std::vector<FrequencyDictionary::WordWithCount> FrequencyDictionary::getResults()
{
  std::vector<WordWithCount> result;
  result.reserve(words_.size());

  std::vector<Word*> buffer_for_sort;
  //I guess, it is enough even for animal language to have max frequency 0.5 for 1 word
  buffer_for_sort.reserve(words_.size() / 2);
  for (int i = counters_.size() - 1; i >= 0; --i)
  {
    auto& el = counters_[i];
    if (el.empty())
    {
      continue;
    }
    buffer_for_sort.resize(0);
    el.getSortedWordsPointers(buffer_for_sort);

    std::
    transform(std::begin(buffer_for_sort), std::end(buffer_for_sort), std::back_inserter(result),
              [i](Word* const word_pointer) -> FrequencyDictionary::WordWithCount {
                return {static_cast<std::size_t>(i + 1), word_pointer->word};
              });
  }

  return result;
}

void FrequencyDictionary::toLower(std::string& str) noexcept
{
  std::transform(std::cbegin(str), std::cend(str), std::begin(str),
                 [](unsigned char c) noexcept { return std::tolower(c); });
}

void FrequencyDictionary::setCountToWord(Word& word, const std::size_t pos) noexcept
{
  if (pos == counters_.size())
  {
    counters_.push_back({});
  }
  counters_[pos].insert(word);
  word.current_counter = pos;
}

void FrequencyDictionary::unsetCount(Word& word) noexcept
{
  counters_[word.current_counter].remove(word);
}

void FrequencyDictionary::addNew(Word& new_word)
{
  setCountToWord(new_word, 0);
}

void FrequencyDictionary::increaseWordCount(Word& word)
{
  const auto current_count = word.current_counter;

  unsetCount(word);

  setCountToWord(word, current_count + 1);
}

}