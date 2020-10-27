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

FrequencyDictionary::WordsCount::WordsCount(const std::size_t count) : count_{count}
{}

FrequencyDictionary::WordsCount::WordsCount(const std::size_t count, Word& word) : count_{count}
{
  insert(word);
}

bool FrequencyDictionary::WordsCount::empty() const noexcept
{
  return words_.empty();
}

void FrequencyDictionary::WordsCount::setCount(const std::size_t new_count) noexcept
{
  count_ = new_count;
}

std::size_t FrequencyDictionary::WordsCount::count() const noexcept
{
  return count_;
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
  for (auto& el : boost::adaptors::reverse(counters_))
  {
    buffer_for_sort.resize(0);
    el.getSortedWordsPointers(buffer_for_sort);

    std::
    transform(std::begin(buffer_for_sort), std::end(buffer_for_sort), std::back_inserter(result),
              [&el](Word* const word_pointer) -> FrequencyDictionary::WordWithCount {
                return {el.count(), word_pointer->word};
              });
  }

  return result;
}

void FrequencyDictionary::toLower(std::string& str) noexcept
{
  std::transform(std::cbegin(str), std::cend(str), std::begin(str),
                 [](unsigned char c) noexcept { return std::tolower(c); });
}

void FrequencyDictionary::setCountToWord(Word& word, const std::list<WordsCount>::iterator count_it) noexcept
{
  count_it->insert(word);
  word.current_counter = count_it;
}

void FrequencyDictionary::unsetCount(Word& word) noexcept
{
  word.current_counter->remove(word);
  if (word.current_counter->empty())
  {
    free_counters_pool_.splice(std::cend(free_counters_pool_), counters_, word.current_counter,
                               std::next(word.current_counter));
  }
}

void FrequencyDictionary::addNew(Word& new_word)
{
  if (counters_.empty())
  {
    counters_.emplace_back(1);
    setCountToWord(new_word, std::begin(counters_));
  }
  else
  {
    if (counters_.front().count() != 1)
    {
      addNewCountForWordBeforeIterator(1, new_word, std::begin(counters_));
    }
    else
    {
      setCountToWord(new_word, std::begin(counters_));
    }
  }
}

std::list<FrequencyDictionary::WordsCount>::iterator
FrequencyDictionary::createCountBeforeIt(const std::size_t count, const std::list<WordsCount>::iterator it)
{
  if (free_counters_pool_.empty())
  {
    return counters_.emplace(it, count);
  }
  else
  {
    free_counters_pool_.front().setCount(count);
    counters_.splice(it, free_counters_pool_, std::begin(free_counters_pool_),
                     std::next(std::begin(free_counters_pool_)));
    return std::prev(it);
  }
}

void
FrequencyDictionary::addNewCountForWordBeforeIterator(const std::size_t count, Word& word,
                                                      const std::list<WordsCount>::iterator it)
{
  setCountToWord(word, createCountBeforeIt(count, it));
}

bool FrequencyDictionary::itPointsToCount(std::list<FrequencyDictionary::WordsCount>::const_iterator it,
                                          const std::size_t count) const noexcept
{
  return it != std::cend(counters_) && it->count() == count;
}

void FrequencyDictionary::increaseWordCount(Word& word)
{
  auto next_it = std::next(word.current_counter);
  const auto current_count = word.current_counter->count();

  unsetCount(word);

  if (itPointsToCount(next_it, current_count + 1))
  {
    setCountToWord(word, next_it);
    return;
  }

  addNewCountForWordBeforeIterator(current_count + 1, word, next_it);
}

}