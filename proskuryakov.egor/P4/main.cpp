#include <cstddef>
#include <iostream>
#include <limits>
#include <stdexcept>

namespace proskuryakov {
  const int error = 1;
  const std::size_t table_size = std::numeric_limits< unsigned char >::max() + 1;

  char * readLine(std::istream & in, std::size_t & out_length);
  char * commonSymbols(char * dest, const char * src1, const char * src2) noexcept;
  int hasRepeats(const char * src) noexcept;
}

char * proskuryakov::readLine(std::istream & in, std::size_t & out_length)
{
  const std::size_t initial_capacity = 32;
  const std::size_t growth_factor = 2;
  const std::size_t max_capacity = std::numeric_limits< std::size_t >::max();

  std::size_t capacity = initial_capacity;
  char * buffer = new char[capacity];
  std::size_t length = 0;

  char current = '\0';
  while (in.get(current) && (current != '\n')) {
    if (length + 1 >= capacity) {
      if (capacity == max_capacity) {
        delete[] buffer;
        throw std::length_error("Превышен максимальный размер строки");
      }

      std::size_t new_capacity = max_capacity;
      if (capacity <= (max_capacity / growth_factor)) {
        new_capacity = capacity * growth_factor;
      }

      char * new_buffer = nullptr;
      try {
        new_buffer = new char[new_capacity];
      } catch (...) {
        delete[] buffer;
        throw;
      }

      for (std::size_t i = 0; i < length; ++i) {
        new_buffer[i] = buffer[i];
      }
      delete[] buffer;
      buffer = new_buffer;
      capacity = new_capacity;
    }
    buffer[length] = current;
    ++length;
  }

  if ((length == 0) && (!in)) {
    delete[] buffer;
    return nullptr;
  }

  buffer[length] = '\0';
  out_length = length;
  return buffer;
}

char * proskuryakov::commonSymbols(char * dest, const char * src1, const char * src2) noexcept
{
  if ((dest == nullptr) || (src1 == nullptr) || (src2 == nullptr)) {
    return dest;
  }

  int in_second[table_size] = {};
  int already_added[table_size] = {};

  for (std::size_t i = 0; src2[i] != '\0'; ++i) {
    in_second[static_cast< unsigned char >(src2[i])] = 1;
  }

  std::size_t dest_index = 0;
  for (std::size_t i = 0; src1[i] != '\0'; ++i) {
    const unsigned char ch = static_cast< unsigned char >(src1[i]);
    if ((in_second[ch] == 1) && (already_added[ch] == 0)) {
      already_added[ch] = 1;
      dest[dest_index] = src1[i];
      ++dest_index;
    }
  }
  dest[dest_index] = '\0';
  return dest;
}

int proskuryakov::hasRepeats(const char * src) noexcept
{
  if (src == nullptr) {
    return 0;
  }

  int seen[table_size] = {};

  for (std::size_t i = 0; src[i] != '\0'; ++i) {
    const unsigned char ch = static_cast< unsigned char >(src[i]);
    if (seen[ch] == 1) {
      return 1;
    }
    seen[ch] = 1;
  }
  return 0;
}

int main()
{
  const char * const second_literal = "abc_ef";

  std::size_t length = 0;
  char * input_str = nullptr;

  try {
    input_str = proskuryakov::readLine(std::cin, length);
  } catch (const std::exception & e) {
    std::cerr << "Ошибка выделения памяти при чтении строки: " << e.what() << "\n";
    return proskuryakov::error;
  }

  if (input_str == nullptr) {
    std::cerr << "Ошибка чтения потока или конец файла\n";
    return proskuryakov::error;
  }

  if (length == 0) {
    delete[] input_str;
    std::cerr << "Ошибка: введена пустая строка\n";
    return proskuryakov::error;
  }

  char * common_result = nullptr;
  try {
    common_result = new char[length + 1];
  } catch (const std::exception & e) {
    delete[] input_str;
    std::cerr << "Ошибка выделения памяти под результат: " << e.what() << "\n";
    return proskuryakov::error;
  }

  proskuryakov::commonSymbols(common_result, input_str, second_literal);
  const int repeats_result = proskuryakov::hasRepeats(input_str);

  std::cout << common_result << "\n";
  std::cout << repeats_result << "\n";

  delete[] common_result;
  delete[] input_str;

  return 0;
}
