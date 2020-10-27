#include <fstream>
#include <ios>
#include <iostream>
#include <vector>

#include <frequency_dictionary/frequency_dictionary.hpp>

void
saveResults(std::ofstream& output, const std::vector<frequency_dictionary::FrequencyDictionary::WordWithCount>& results)
{
  for (const auto&[count, word] : results)
  {
    try
    {
      output << count << " " << word << "\n";
    }
    catch (const std::exception& ex)
    {
      std::cerr << "Error while writing file: " << ex.what();
      std::exit(-4);
    }
  }
}

bool isSpace(const char c) noexcept
{
  return !((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

void countFromFile(std::ifstream& input, frequency_dictionary::FrequencyDictionary& dictionary)
{
  static constexpr auto buffer_size{100000};

  char buffer[buffer_size];

  //In real languages word size limited, so we could use buffer on stack
  std::string tmp_string;

  for (;;)
  {
    try
    {
      input.read(buffer, buffer_size);
    }
    catch (const std::ios_base::failure& ex)
    {
      std::cerr << "Error reading file: " << ex.what();
      std::exit(-3);
    }
    std::size_t num_of_chars = input.gcount();

    for (std::size_t i = 0; i < num_of_chars; ++i)
    {
      if (!isSpace(buffer[i]))
      {
        tmp_string.push_back(buffer[i]);
        continue;
      }

      if (tmp_string.empty())
      {
        continue;
      }

      dictionary.addWord(tmp_string);
      tmp_string.resize(0);
    }

    if (input.eof())
    {
      std::cout << "EOF occured\n";
      if (!tmp_string.empty())
      {
        dictionary.addWord(tmp_string);
      }

      break;
    }
  }
}


int main(const int argc, const char* const argv[])
{
  if (argc < 3)
  {
    std::cerr << "Invalid arguments! The first argument must be input file, and the second must be output file"
              << std::endl;
    std::exit(-1);
  }


  frequency_dictionary::FrequencyDictionary dictionary;

  std::ifstream input{argv[1]};
  if (input.fail())
  {
    std::cerr << "Can not open file" << argv[1] << "! May be name is wrong?" << std::endl;
    std::exit(-2);
  }

  std::ofstream output{argv[2]};
  if (input.fail())
  {
    std::cerr << "Can not open file " << argv[2] << "! May be name is wrong?" << std::endl;
    std::exit(-2);
  }

  countFromFile(input, dictionary);
  const auto result = dictionary.getResults();

  saveResults(output, result);

  return 0;
}
