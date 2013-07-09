#include <iostream>
#include <pnt.hpp>
#include <tinyformat/tinyformat.h>
#include <sys/time.h>
#include <cstdio>
#include <iomanip>

using namespace pnt;

class FileStreambuf : public std::streambuf
{
  public:
    FileStreambuf(FILE* file) :
      m_file(file)
    {}

    int_type overflow(int_type ch)
    {
      if (!traits_type::eq_int_type(ch, traits_type::eof()))
      {
        sync();
        fputc(ch, m_file);
        return ch;
      }
      return traits_type::eof();
    }

    int sync()
    {
      fwrite(pbase(), pptr() - pbase(), 1, m_file);
      pbump(pptr() - pbase());

      return 0;
    }

  private:
    FILE* m_file;
};

class FileStreambuf2 : public std::streambuf
{
  public:
    FileStreambuf2(FILE* file) :
      m_file(file)
    {}

    int_type sputc(char_type ch)
    {
      fputc(ch, m_file);
    }

    std::streamsize sputn(const char_type* s, std::streamsize count)
    {
      fwrite(s, count, 1, m_file);
    }

  private:
    FILE* m_file;
};

class ScopedTimer
{
  public:
    ScopedTimer(const std::string& name) :
      m_name(name)
    {
      struct timeval tv;
      gettimeofday(&tv, nullptr);
      m_start = tv.tv_sec * 1000000 + tv.tv_usec;
    }

    ~ScopedTimer()
    {
      struct timeval tv;
      gettimeofday(&tv, nullptr);
      uint64_t stop = tv.tv_sec * 1000000 + tv.tv_usec;

      uint64_t time = stop - m_start;

      std::cerr << std::setw(12) << std::left << m_name << ((double)time)/1000000 << std::endl;
    }

  private:
    std::string m_name;
    uint64_t m_start;
};

int main()
{
  const int nbPrints = 10000000;

  std::cerr << "int" << std::endl;

  {
    ScopedTimer t("printf");
    for (int i = 0; i < nbPrints; ++i)
      printf("%d\n", i);
  }

  {
    ScopedTimer t("cout");
    for (int i = 0; i < nbPrints; ++i)
      std::cout << i << "\n";
  }

  {
    ScopedTimer t("tinyformat");
    for (int i = 0; i < nbPrints; ++i)
      tfm::format(std::cout, "%d\n", i);
  }

  {
    FileStreambuf2 sb(stdout);
    ScopedTimer t("pnt");
    Formatter<FileStreambuf2> fmt(sb);
    for (int i = 0; i < nbPrints; ++i)
      fmt.print("%d\n", i);
  }

  std::cerr << "ints, negative, precision, padding" << std::endl;

  {
    ScopedTimer t("printf");
    for (int i = 0; i < nbPrints; ++i)
      printf("Positive value: %+12.8d, negative value: %+12.8d\n", i, -i);
  }

  {
    ScopedTimer t("cout");
    for (int i = 0; i < nbPrints; ++i)
      std::cout << "Positive value: "
        << std::showpos << std::setw(8) << std::internal
        << std::setfill('0') << i
        << ", negative value: "
        << std::showpos << std::setw(8) << std::internal << std::setfill('0')
        << -i << "\n";
  }

  {
    ScopedTimer t("tinyformat");
    for (int i = 0; i < nbPrints; ++i)
      tfm::format(std::cout, "Positive value: %+12.8d, negative value: %+12.8d\n", i, -i);
  }

  {
    FileStreambuf2 sb(stdout);
    ScopedTimer t("pnt");
    Formatter<FileStreambuf2> fmt(sb);
    for (int i = 0; i < nbPrints; ++i)
      fmt.print("Positive value: %+12.8d, negative value: %+12.8d\n", i, -i);
  }

  return 0;
}

// vim: ts=2:sw=2:sts=2:expandtab
