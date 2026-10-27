#include <cstddef>
#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <new>

namespace proskuryakov
{
  const int err_args = 1;
  const int err_input = 2;
  const size_t max_fixed_size = 10000;

  int parseMode(const char *text)
  {
    if (text[0] == '1' && text[1] == '\0')
    {
      return 1;
    }
    if (text[0] == '2' && text[1] == '\0')
    {
      return 2;
    }
    throw std::invalid_argument("num must be 1 or 2");
  }

  void readSize(std::ifstream &in, size_t &rows, size_t &cols)
  {
    int r = 0;
    int c = 0;
    if (!(in >> r >> c) || r < 0 || c < 0)
    {
      throw std::runtime_error("cannot read matrix size");
    }
    rows = static_cast< size_t >(r);
    cols = static_cast< size_t >(c);
    if (cols != 0 && rows > std::numeric_limits< size_t >::max() / cols)
    {
      throw std::runtime_error("matrix is too large");
    }
  }

  void readElements(std::ifstream &in, int *data, size_t count)
  {
    for (size_t i = 0; i < count; ++i)
    {
      if (!(in >> data[i]))
      {
        throw std::runtime_error("cannot read matrix elements");
      }
    }
  }

  long long diagonalSum(const int *data, size_t rows, size_t cols, size_t row, size_t col)
  {
    long long sum = 0;
    while (row < rows && col < cols)
    {
      sum += data[row * cols + col];
      ++row;
      ++col;
    }
    return sum;
  }

  long long maxSumSdg(const int *data, size_t rows, size_t cols)
  {
    if (rows == 0 || cols == 0)
    {
      return 0;
    }
    bool found = false;
    long long best = 0;
    for (size_t k = 1; k < rows + cols - 1; ++k)
    {
      const size_t row = (k < cols) ? 0 : k - cols + 1;
      const size_t col = (k < cols) ? k : 0;
      const long long sum = diagonalSum(data, rows, cols, row, col);
      if (!found || sum > best)
      {
        best = sum;
        found = true;
      }
    }
    return best;
  }

  size_t numColLsr(const int *data, size_t rows, size_t cols)
  {
    size_t best_column = 0;
    size_t best_length = 0;
    for (size_t col = 0; col < cols; ++col)
    {
      size_t length = 0;
      for (size_t row = 0; row < rows; ++row)
      {
        if (row > 0 && data[row * cols + col] == data[(row - 1) * cols + col])
        {
          ++length;
        }
        else
        {
          length = 1;
        }
        if (length > best_length)
        {
          best_length = length;
          best_column = col + 1;
        }
      }
    }
    return best_column;
  }

  void processFixed(std::ifstream &in, long long &sdg, size_t &lsr)
  {
    size_t rows = 0;
    size_t cols = 0;
    readSize(in, rows, cols);
    if (rows * cols > max_fixed_size)
    {
      throw std::runtime_error("matrix does not fit into 10000 elements");
    }
    int data[max_fixed_size];
    readElements(in, data, rows * cols);
    sdg = maxSumSdg(data, rows, cols);
    lsr = numColLsr(data, rows, cols);
  }

  void processDynamic(std::ifstream &in, long long &sdg, size_t &lsr)
  {
    size_t rows = 0;
    size_t cols = 0;
    readSize(in, rows, cols);
    int *const data = new int[rows * cols];
    try
    {
      readElements(in, data, rows * cols);
      sdg = maxSumSdg(data, rows, cols);
      lsr = numColLsr(data, rows, cols);
    }
    catch (...)
    {
      delete[] data;
      throw;
    }
    delete[] data;
  }
}

int main(int argc, char *argv[])
{
  try
  {
    if (argc != 4)
    {
      throw std::invalid_argument("expected 3 arguments: ./lab num input output");
    }
    const int mode = proskuryakov::parseMode(argv[1]);

    std::ifstream in(argv[2]);
    if (!in)
    {
      throw std::runtime_error("cannot open the input file");
    }
    long long sdg = 0;
    size_t lsr = 0;
    if (mode == 1)
    {
      proskuryakov::processFixed(in, sdg, lsr);
    }
    else
    {
      proskuryakov::processDynamic(in, sdg, lsr);
    }

    std::ofstream out(argv[3]);
    if (!out)
    {
      throw std::invalid_argument("cannot open the output file");
    }
    out << sdg << '\n' << lsr << '\n';
  }
  catch (const std::invalid_argument &e)
  {
    std::cerr << "Error: " << e.what() << '\n';
    return proskuryakov::err_args;
  }
  catch (const std::runtime_error &e)
  {
    std::cerr << "Error" << e.what() << '\n';
    return proskuryakov::err_input;
  }
  catch (const std::bad_alloc &e)
  {
    std::cerr << "Error: " << e.what() << '\n';
    return proskuryakov::err_input;
  }
  return 0;
}
