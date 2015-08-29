#define FORMATTER_THROW_ON_ERROR
#include <xll/pnt.hpp>
#include <utility>
#define CATCH_CONFIG_MAIN
#include <catch.hpp>

using namespace xll::pnt;

template <typename... Args>
void testCase(const std::string& expected, const char* format,
    Args... args)
{
  std::stringbuf sb;
  writef(sb, format, args...);
  SCOPED_INFO("format string: " << format);
  CHECK(expected == sb.str());
}

template <typename... Args>
void testCase(const std::wstring& expected, const wchar_t* format,
    Args... args)
{
  std::wstringbuf sb;
  writef(sb, format, args...);
  CHECK(expected == sb.str());
}

TEST_CASE("noarg", "only a text argument")
{
  testCase("", "");
  testCase("test", "test");
  testCase("a very long long long test", "a very long long long test");
}

TEST_CASE("noarg/%", "only a text argument with %%")
{
  testCase("%", "%%");
  testCase("test % ", "test %% ");
}

TEST_CASE("s/int", "%s with int argument")
{
  testCase("0", "%s", 0);
  testCase("10", "%s", 10);
  testCase("-10", "%s", -10);
  testCase("-10019874929812798", "%s", -10019874929812798ll);
  testCase("9223372036854775808", "%s", 9223372036854775808ull);
}

TEST_CASE("s/int/flags", "%s with int argument")
{
  testCase("+005", "%+0.3s", 5);
  testCase("-005", "%+0.3s", -5);
}

TEST_CASE("s/int/text", "%s with int argument with text")
{
  testCase("aa 0 bb", "aa %s bb", 0);
  testCase("aa 10 bb", "aa %s bb", 10);
  testCase("aa -10 bb", "aa %s bb", -10);
}

TEST_CASE("s/bool", "%s with bool argument")
{
  testCase("aa true bb", "aa %s bb", true);
  testCase("aa false bb", "aa %s bb", false);
}

TEST_CASE("s/bool/fill", "%s with bool and fill")
{
  testCase("aa   true bb", "aa %6s bb", true);
  testCase("aa false  bb", "aa %-6s bb", false);
}

TEST_CASE("int/hexadecimal", "%x and %X")
{
  testCase("aa 123456789ABCDEF bb", "aa %X bb", 0x123456789ABCDEF);
  testCase("aa 123456789abcdef bb", "aa %x bb", 0x123456789ABCDEF);
  testCase("aa 0 bb", "aa %x bb", 0);
}

TEST_CASE("int/hexadecimal/unsignedness", "%x considered as unsigned")
{
  testCase("aa ffffffff bb", "aa %x bb", -1);
}

TEST_CASE("int/octal", "%o")
{
  testCase("aa 1234 bb", "aa %o bb", 01234);
  testCase("aa 0 bb", "aa %o bb", 0);
}

TEST_CASE("int/binary", "%b")
{
  testCase("aa 11010111 bb", "aa %b bb", 0xd7);
  testCase("aa 0 bb", "aa %b bb", 0);
}

TEST_CASE("int/decimal", "%d")
{
  testCase("aa 10 bb", "aa %d bb", 10);
  testCase("aa 0 bb", "aa %d bb", 0);
}

TEST_CASE("int/decimal/negative", "%d with negative")
{
  testCase("aa -100 bb", "aa %d bb", -100);
}

TEST_CASE("int/decimal/big", "%d with big number")
{
  testCase("aa 4294967295 bb", "aa %d bb", 4294967295u);
  testCase("aa 2147483647 bb", "aa %d bb", 2147483647);
  testCase("aa 4294967295 bb", "aa %d bb", 4294967295u);
  testCase("aa 18446744073709551615 bb", "aa %d bb", 18446744073709551615llu);
  testCase("aa 9223372036854775807 bb", "aa %d bb", 9223372036854775807ll);
}

TEST_CASE("int/decimal/big/negative", "%d with big negative number")
{
  testCase("aa -2147483648 bb", "aa %d bb", -2147483647 - 1);
  testCase("aa -9223372036854775808 bb", "aa %d bb", -9223372036854775807ll - 1);
}

TEST_CASE("int/decimal/fill", "%d with filling")
{
  testCase("aa    15 bb", "aa %5d bb", 15);
  testCase("aa 00015 bb", "aa %05d bb", 15);
}

TEST_CASE("int/decimal/fill/too small", "%d with width too small to fit")
{
  testCase("aa 1000000 bb", "aa %5d bb", 1000000);
  testCase("aa -1000000 bb", "aa %5d bb", -1000000);
}

TEST_CASE("int/decimal/precision", "%d with precision")
{
  testCase("aa 00010 bb", "aa %.5d bb", 10);
  testCase("aa -00010 bb", "aa %.5d bb", -10);
}

TEST_CASE("int/decimal/precision/too small",
    "%d with precision too small to fit")
{
  testCase("aa 1000 bb", "aa %.3d bb", 1000);
  testCase("aa -1000 bb", "aa %.3d bb", -1000);
}

TEST_CASE("int/decimal/precision/zero", "%d with zero precision")
{
  testCase("aa 10 bb", "aa %.d bb", 10);
  testCase("aa  bb", "aa %.d bb", 0);
}

TEST_CASE("int/decimal/sign", "%d with sign or space")
{
  testCase("aa +15 bb", "aa %+d bb", 15);
  testCase("aa +15 bb", "aa %+ d bb", 15);
  testCase("aa  15 bb", "aa % d bb", 15);
  testCase("aa -15 bb", "aa %+d bb", -15);
  testCase("aa -15 bb", "aa % d bb", -15);
}

TEST_CASE("int/decimal/sign/fill", "%d with sign or space")
{
  testCase("aa   +15 bb", "aa %+5d bb", 15);
  testCase("aa    15 bb", "aa % 5d bb", 15);
  testCase("aa +0015 bb", "aa %+05d bb", 15);
  testCase("aa  0015 bb", "aa % 05d bb", 15);
}

TEST_CASE("int/decimal/fill/negative", "%d with filling and negative number")
{
  testCase("aa   -15 bb", "aa %5d bb", -15);
  testCase("aa -0015 bb", "aa %05d bb", -15);
}

TEST_CASE("int/decimal/fill and precision", "%d with precision and fill")
{
  testCase("aa   010 bb", "aa %5.3d bb", 10);
  testCase("aa 00010 bb", "aa %05.3d bb", 10);
}

TEST_CASE("int/decimal/fill and precision/fill smaller than precision",
    "%d with precision and fill but fill smaller than precision")
{
  testCase("aa 00010 bb", "aa %3.5d bb", 10);
  testCase("aa 00010 bb", "aa %03.5d bb", 10);
}

TEST_CASE("int/decimal/sign/fill and precision",
    "%d with sign, precision and fill")
{
  testCase("aa  +010 bb", "aa %+5.3d bb", 10);
  testCase("aa +0010 bb", "aa %+05.3d bb", 10);
  testCase("aa +00010 bb", "aa %+3.5d bb", 10);
  testCase("aa +00010 bb", "aa %+03.5d bb", 10);
}

TEST_CASE("int/decimal/left justify",
    "%d with left justify")
{
  testCase("aa +010  bb", "aa %-+5.3d bb", 10);
  testCase("aa 010   bb", "aa %-05.3d bb", 10);
  testCase("aa -010  bb", "aa %-5.3d bb", -10);
}

TEST_CASE("int/non decimal/flags", "other than %d with flags")
{
  testCase("aa f bb", "aa %+x bb", 15);
  testCase("aa f bb", "aa % x bb", 15);
  testCase("aa f bb", "aa % +x bb", 15);
}

TEST_CASE("int/non decimal/explicit base", "other than %d with explicit base")
{
  testCase("aa 0xf bb", "aa %#x bb", 15);
  testCase("aa 0XF bb", "aa %#X bb", 15);
  testCase("aa 017 bb", "aa %#o bb", 15);
}

TEST_CASE("int/hexa/explicit base/null",
    "%x with explicit base but null argument")
{
  testCase("aa 0 bb", "aa %#x bb", 0);
  testCase("aa 0 bb", "aa %#X bb", 0);
}

TEST_CASE("int/non decimal/explicit base/fill",
    "other than %d with explicit base and fill")
{
  testCase("aa   0xf bb", "aa %#5x bb", 15);
  testCase("aa   0XF bb", "aa %#5X bb", 15);
  testCase("aa   017 bb", "aa %#5o bb", 15);
  testCase("aa 0x00f bb", "aa %#05x bb", 15);
  testCase("aa 0X00F bb", "aa %#05X bb", 15);
  testCase("aa 00017 bb", "aa %#05o bb", 15);
}

TEST_CASE("char", "char argument")
{
  testCase("aa a bb", "aa %c bb", 'a');
  testCase("aa \x03 bb", "aa %c bb", '\x03');
}

TEST_CASE("char/fill", "char argument with fill")
{
  testCase("aa   a bb", "aa %3c bb", 'a');
  testCase("aa \x03   bb", "aa %-3c bb", '\x03');
}

TEST_CASE("s/char", "char argument with fill")
{
  testCase("aa   a bb", "aa %3s bb", 'a');
  testCase("aa \x03   bb", "aa %-3s bb", '\x03');
}

TEST_CASE("s/string", "string argument")
{
  testCase("aa let me be bb", "aa %s bb", "let me be");
  testCase("aa let me be bb", "aa %s bb", std::string("let me be"));
}

TEST_CASE("s/string/fill", "string with fill")
{
  testCase("aa       let me be bb", "aa %15s bb", "let me be");
  testCase("aa let me be       bb", "aa %-15s bb", "let me be");
}

TEST_CASE("s/string/castable object", "%s with string-castable argument")
{
  struct Obj
  {
    operator std::string() const
    { return std::string("let me be"); }
  };
  testCase("aa let me be bb", "aa %s bb", Obj());
}

TEST_CASE("pointer", "pointer argument")
{
  int* ptr = reinterpret_cast<int*>(0x02345678);
  if (sizeof(ptr) == 4)
  {
    testCase("aa 0x02345678 bb", "aa %s bb", ptr);
    testCase("aa           0x02345678 bb", "aa %20p bb", ptr);
  }
  else if (sizeof(ptr) == 8)
  {
    testCase("aa 0x0000000002345678 bb", "aa %s bb", ptr);
    testCase("aa   0x0000000002345678 bb", "aa %20p bb", ptr);
  }
  else
  {
    std::cout << "can't test pointers, unknown architecture" << std::endl;
  }
}

TEST_CASE("float/%f", "float argument with %f")
{
  testCase("aa 3.500000 bb", "aa %f bb", 3.5);
  testCase("aa -3.500000 bb", "aa %f bb", -3.5);
}

TEST_CASE("float/%f/width and precision",
    "float argument with %f, width and precision")
{
  testCase("aa  -3.5 bb", "aa %5.1f bb", -3.5);
  testCase("aa -03.5 bb", "aa %05.1f bb", -3.5);
  testCase("aa -3.50 bb", "aa %.2f bb", -3.5);
  testCase("aa 3 bb", "aa %.0f bb", 3.);
  testCase("aa 3   bb", "aa %-3.0f bb", 3.);
}

TEST_CASE("float/%e", "float argument with %e")
{
  testCase("aa 3.550000e+01 bb", "aa %e bb", 35.5);
  testCase("aa 3.550000E+01 bb", "aa %E bb", 35.5);
  testCase("aa -3.550000e+01 bb", "aa %e bb", -35.5);
  testCase("aa -3e+00 bb", "aa %.0e bb", -3.);
  testCase("aa   -3.550e+01 bb", "aa %12.3e bb", -35.5);
  testCase("aa -003.550e+01 bb", "aa %012.3e bb", -35.5);
  testCase("aa 3.550e+01    bb", "aa % -12.3e bb", 35.5);
}

TEST_CASE("float/%g", "float argument with %g")
{
  testCase("aa 5E-09 bb", "aa %G bb", .000000005);
  testCase("aa 5e-09 bb", "aa %g bb", .000000005);
  testCase("aa 0.0005 bb", "aa %g bb", .0005);
  testCase("aa 5 bb", "aa %g bb", 5.);
  testCase("aa 5e+11 bb", "aa %g bb", 500000000000.);
  testCase("aa 500000000000 bb", "aa %.500g bb", 500000000000.);
}

TEST_CASE("s/float", "float argument with %s")
{
  testCase("aa 5e-09 bb", "aa %s bb", .000000005);
  testCase("aa 0.0005 bb", "aa %s bb", .0005);
  testCase("aa 5 bb", "aa %s bb", 5.);
  testCase("aa 5e+11 bb", "aa %s bb", 500000000000.);
  testCase("aa 500000000000 bb", "aa %.500s bb", 500000000000.);
}

TEST_CASE("s/multiarg/intonly", "%s with multiple arguments, only ints")
{
  testCase("aa 0 bb -10 ", "aa %s bb %s ", 0, -10);
}

TEST_CASE("s/multiarg/positional", "%s with multiple positional arguments")
{
  testCase("aa -10 bb 0 ", "aa %1$s bb %0$s ", 0, -10);
}

TEST_CASE("s/multiarg/positional/mixed",
    "%s with multiple positional and non-positional arguments")
{
  testCase("-10 -10 0 0", "%1$s %s %0$s %s", 0, -10);
}

TEST_CASE("container")
{
  std::vector<int> v = {1, 2, 3};
  testCase("123", "%(%d%)", v);
}

TEST_CASE("container/separator")
{
  std::vector<int> v = {1, 2, 3};
  testCase("1, 2, 3", "%(%d, %)", v);
}

TEST_CASE("container/separator and prefix")
{
  std::vector<int> v = {1, 2, 3};
  testCase("=1, =2, =3", "%(=%d, %)", v);
}

TEST_CASE("container/separator, prefix and suffix")
{
  std::vector<int> v = {1, 2, 3};
  testCase("=1=, =2=, =3=", "%(=%d=%|, %)", v);
}

TEST_CASE("container/escaping")
{
  std::vector<int> v = {1, 2, 3};
  testCase("%1%, %2%, %3%", "%(%%%d%%%|, %)", v);
  testCase("%1%% %2%% %3%", "%(%%%d%%%|% %)", v);
}

TEST_CASE("container/positionnal")
{
  std::vector<int> v = {1, 2, 3};
  testCase("0 123", "%1$d %0$(%d%)", v, 0);
}

TEST_CASE("container/empty")
{
  std::vector<int> v;
  testCase("", "%(%d%)", v);
}

TEST_CASE("container/nested")
{
  std::vector<std::vector<int>> v = {{1, 2}, {3, 4}, {5, 6}};
  testCase("{[=1=-=2=], [=3=-=4=], [=5=-=6=]}", "{%([%(=%d=%|-%)]%|, %)}", v);
}

TEST_CASE("s/container")
{
  std::vector<std::vector<int>> v = {{1, 2}, {3, 4}, {5, 6}};
  testCase("{{1, 2}, {3, 4}, {5, 6}}", "%s", v);
}

TEST_CASE("complex", "complex formatting")
{
  testCase("aa 10 0x14  30 0x28 +40 00040 bb", "aa %d %#x %3d %#x %3$+d %05s bb", 10, 20, 30, 40);
}

TEST_CASE("unicode", "unicode strings")
{
  testCase(L"aa hello bb", L"aa %s bb", L"hello");
  testCase(L"aa true bb", L"aa %s bb", true);
  testCase(L"aa x bb", L"aa %c bb", L'x');
}

TEST_CASE("error/not enough arguments", "not enough arguments")
{
  CHECK_THROWS(testCase("", "%d %d %d", 1, 2));
  CHECK_THROWS(testCase("", "%3$d", 1, 2));
}

TEST_CASE("error/invalid type", "invalid type")
{
  CHECK_THROWS(testCase("", "%d", "test"));
  CHECK_THROWS(testCase("", "%c", nullptr));
}

TEST_CASE("error/invalid format string", "invalid format string")
{
  CHECK_THROWS(testCase("", "%y", "test"));
  CHECK_THROWS(testCase("", "%..s", "test"));
  CHECK_THROWS(testCase("", "%$s", "test"));
}

TEST_CASE("error/invalid format string/containers")
{
  std::vector<int> v;
  CHECK_THROWS(testCase("", "%(", v));
  CHECK_THROWS(testCase("", "%)", v));
  CHECK_THROWS(testCase("", "%(%)", v));
  CHECK_THROWS(testCase("", "%(%|%)", v));
  CHECK_THROWS(testCase("", "%(%|%d%)", v));
}

// vim: ts=2:sw=2:sts=2:expandtab
