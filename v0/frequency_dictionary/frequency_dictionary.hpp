#pragma once

#include <algorithm>
#include <cctype>
#include <iterator>
#include <list>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_set>

#include <boost/intrusive/list.hpp>
#include <boost/range/adaptor/reversed.hpp>

namespace frequency_dictionary
{

class FrequencyDictionary final
{
  class WordsCount;

  struct Word final : boost::intrusive::list_base_hook<>
  {
    explicit Word(std::string word) : word{std::move(word)}
    {}

    std::string word;
    std::list<WordsCount>::iterator current_counter;

    class Hasher final
    {
     public:
      std::size_t operator()(const Word& w) const noexcept;
    };

    class EqualityChecker final
    {
     public:
      bool operator()(const Word& w1, const Word& w2) const noexcept;
    };
  };

  class WordsCount final
  {
   public:
    WordsCount(const std::size_t count, Word& word);

    explicit WordsCount(const std::size_t count);

    [[nodiscard]] bool empty() const noexcept;

    void setCount(const std::size_t new_count) noexcept;

    [[nodiscard]] std::size_t count() const noexcept;

    void remove(const Word& word) noexcept;

    void insert(Word& word) noexcept;

    void getSortedWordsPointers(std::vector<Word*>& output_vector);

   private:
    std::size_t count_;
    boost::intrusive::list<Word, boost::intrusive::constant_time_size<false>> words_;
  };

 public:
  struct WordWithCount final
  {
    std::size_t count;
    std::string_view word;
  };

  FrequencyDictionary() = default;

  //It is not exception-safe, but only std::bad_alloc can be thrown, and in our task to handle it is meaningless
  void addWord(std::string& word);

  std::vector<WordWithCount> getResults();

 private:
  static void toLower(std::string& str) noexcept;

  std::list<WordsCount>::iterator
  createCountBeforeIt(const std::size_t count, const std::list<WordsCount>::iterator it);

  static void setCountToWord(Word& word, const std::list<WordsCount>::iterator count_it) noexcept;

  void unsetCount(Word& word) noexcept;

  void addNew(Word& new_word);

  void
  addNewCountForWordBeforeIterator(const std::size_t count, Word& word, const std::list<WordsCount>::iterator it);

  bool itPointsToCount(std::list<WordsCount>::const_iterator it, const std::size_t count) const noexcept;

  void increaseWordCount(Word& word);

  std::unordered_set<Word, Word::Hasher, Word::EqualityChecker> words_;
  std::list<WordsCount> free_counters_pool_;
  std::list<WordsCount> counters_;
};

}
