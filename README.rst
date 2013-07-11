=====
 pnt
=====

What is pnt?
============

pnt is a library to format text the printf way.

Example 1 - integer formatting
------------------------------

If you are tired of lines like::

    std::cout << "The value is " << std::setw(10) << std::setfill('0') << std::showpos << std::internal << value << " which is, in hexa, " << std::setw(8) << std::setfill('0') << std::showbase << std::hex << value << '.' << std::endl;

Maybe you would prefer writing it as::

    pnt::writef("The value is %0$+010d which is, in hexa, %0$#08x.\n", value);

As a note, here is the output for 12345::

    The value is +000012345 which is, in hexa, 0x003039.

Example 2 - container formatting
--------------------------------

Now, let's do something more sexy::

    pnt::writef("Here is the list: %([%#x]%|, %).\n", std::vector<int>{1, 2, 3, 4});

Which would write out::

    Here is the list: [0x1], [0x2], [0x3], [0x4].

Okay, let's decompose this one.

- The vector (or anything iterable) is formatted by what's between %( and %).
- The %#x defines how we format the elements of the container.
- The brackets are printed as-is.
- What's after the %| is used as a separator

Features
========

- Single header-only library
- Type safe
- No heap allocation (except for floats and unless a cast to std::string is needed)
- Fast (except for floats)
- No dependencies apart from std headers
- C++11-only compatible
- Widechar support

TODO:

- %a and %A
- List formatting (nested lists and %s)
- Width and precision given as arguments

Benchmarks
==========

These benches are made with bench.cpp executed with ./bench > /dev/null to keep the terminal overhead to 0.

With ``"%d"``:

=========== =================
Function    Time (in seconds)
=========== =================
pnt         0.831953
printf      0.984046
cout        1.01145
tinyformat  1.45267
=========== =================

With ``"Positive value: %+12.8d, negative value: %+12.8d\n"`` (I couldn't translate this to cout, so cout uses 8 width only, which explains why it is faster):

=========== =================
Function    Time (in seconds)
=========== =================
cout        2.57762
printf      2.65869
pnt         3.65454
tinyformat  3.98099
=========== =================

With ``"%g"``:

=========== =================
Function    Time (in seconds)
=========== =================
printf      4.58373
cout        6.55518
tinyformat  6.83606
pnt         12.3644
=========== =================

How to install
==============

Just copy pnt.hpp in your include path and you are ready to go!

Documentation
=============

All methods are in the pnt namespace.

::

    template <typename Streambuf, typename... Args>
    void writef(Streambuf& sb, const Streambuf::char_type* fmt, Args... args);

Note: This documentation is copied and adapted from the std.format.formattedWriter documentation of the D language. This documentation is licensed under the Boost Version 1.0 license. See license section.

Interprets variadic argument list args, formats them according to fmt, and sends the resulting characters to sb.

The variadic arguments are normally consumed in order. POSIX-style positional parameter syntax is also supported. Each argument is formatted into a sequence of chars according to the format specification, and the characters are passed to sb. As many arguments as specified in the format string are consumed and formatted. If there are fewer arguments than format specifiers, an error is raised. If there are more remaining arguments than needed by the format specification and there are no positionnal argument used in fmt, an error is raised, otherwise they are ignored.

Parameters
----------

:sb:     Output is sent to this streambuf.
:fmt:    Format string.
:args:   Variadic argument list.

Returns
-------

Nothing.

Error handling
--------------

Mismatched arguments and formats result in an error.

If PNT_THROW_ON_ERROR is defined before including pnt.hpp, FormatError will be thrown, otherwise an assertion will fail. Note that if PNT_THROW_ON_ERROR is not defined and NDEBUG is defined, the behaviour is undefined but the program should not crash.

Streambuf object
----------------

The streambuf object must not be a real streambuf, it must only define char_type, type_traits and the functions sputc and sputn. Whatever object which satisfies this definition may be used.

Format String
-------------

Format strings consist of characters interspersed with format specifications. Characters are simply copied to the output after any necessary conversion to the corresponding sequence.

The format string has the following grammar::

    FormatString:
        FormatStringItem*
    FormatStringItem:
        '%%'
        '%' Position Flags Width Precision FormatChar
        '%' Position '(' FormatString Separator '%)'
        OtherCharacterExceptPercent
    Separator:
        empty
        '%|' Chars
    Position:
        empty
        Integer '$'
    Flags:
        empty
        '-' Flags
        '+' Flags
        '#' Flags
        '0' Flags
        ' ' Flags
    Width:
        empty
        Integer
        '*'
    Precision:
        empty
        '.'
        '.' Integer
        '.*'
    Integer:
        Digit
        Digit Integer
    Digit:
        '0'|'1'|'2'|'3'|'4'|'5'|'6'|'7'|'8'|'9'
    FormatChar:
        's'|'c'|'b'|'d'|'o'|'x'|'X'|'p'|'e'|'E'|'f'|'F'|'g'|'G'|'a'|'A'

Flags
*****

============ ====================== ==============
Flag         Types affected         Semantics
============ ====================== ==============
'-'          numeric                Left justify the result in the field. It overrides any 0 flag.
'+'          numeric                Prefix positive numbers in a signed conversion with a +. It overrides any space flag.
'#'          integral ('o')         Add to precision as necessary so that the first digit of the octal formatting is a '0', even if both the argument and the Precision are zero.
'#'          integral ('x', 'X')    If non-zero, prefix result with 0x (0X).
'#'          floating               Always insert the decimal point and print trailing zeros.
'0'          numeric                Use leading zeros to pad rather than spaces (except for the floating point values nan and infinity). Ignore if there's a Precision.
' '          numeric                Prefix positive numbers in a signed conversion with a space.
============ ====================== ==============

Width
*****

Specifies the minimum field width. If the width is a \*, the next argument, which must be of type int, is taken as the width. If the width is negative, it is as if the - was given as a Flags character.

Precision
*********

Gives the precision for numeric conversions. If the precision is a \*, the next argument, which must be of type int, is taken as the precision. If it is negative, it is as if there was no Precision.

FormatChar
**********

's'
    The corresponding argument is formatted in a manner consistent with its type:

    bool
        The result is 'true' or 'false'. 
    integral types
        The %d format is used. 
    floating point types
        The %g format is used. 
    const char_type*
        The string is printed

'c'
    The corresponding argument must be a character type.

'b','d','o','x','X'
    The corresponding argument must be an integral type and is formatted as an integer. If the argument is a signed type and the FormatChar is d it is converted to a signed string of characters, otherwise it is treated as unsigned. An argument of type bool is formatted as '1' or '0'. The base used is binary for b, octal for o, decimal for d, and hexadecimal for x or X. x formats using lower case letters, X uppercase. If there are fewer resulting digits than the Precision, leading zeros are used as necessary. If the Precision is 0 and the number is 0, no digits result.

'e','E'
    A floating point number is formatted as one digit before the decimal point, Precision digits after, the FormatChar, ±, followed by at least a two digit exponent: d.dddddde±dd. If there is no Precision, six digits are generated after the decimal point. If the Precision is 0, no decimal point is generated.

'f','F'
    A floating point number is formatted in decimal notation. The Precision specifies the number of digits generated after the decimal point. It defaults to six. At least one digit is generated before the decimal point. If the Precision is zero, no decimal point is generated.

'g','G'
    A floating point number is formatted in either e or f format for g; E or F format for G. The f format is used if the exponent for an e format is greater than -5 and less than the Precision. The Precision specifies the number of significant digits, and defaults to six. Trailing zeros are elided after the decimal point, if the fractional part is zero then no decimal point is generated.

'a','A'
    A floating point number is formatted in hexadecimal exponential notation 0xh.hhhhhhp±d. There is one hexadecimal digit before the decimal point, and as many after as specified by the Precision. If the Precision is zero, no decimal point is generated. If there is no Precision, as many hexadecimal digits as necessary to exactly represent the mantissa are generated. The exponent is written in as few digits as possible, but at least one, is in decimal, and represents a power of 2 as in h.hhhhhh*2±d. The exponent for zero is zero. The hexadecimal digits, x and p are in upper case if the FormatChar is upper case. 

Floating point NaN's are formatted as nan if the FormatChar is lower case, or NAN if upper. Floating point infinities are formatted as inf or infinity if the FormatChar is lower case, or INF or INFINITY if upper. 

Containers
**********

What we call here a container or an iterable is a class which defines begin and
end. They are formated using %( and %). Only one formatter must be between %(
and %). What's before this formatter will be repeated before each element,
what's after will be used as a separator unless %| is used. If %| is used,
anything between the formatter and %| will be repeated after each element and
what's after %| will be used as a separator. '%' characters must be escaped
when used as prefix or suffix but not when used as separator. Look at the
examples.

::

    template <typename... Args>
    void writef(const char* fmt, Args... args);

This method is the same as above but prints on stdout.

License
=======

The code is licensed under the FreeBSD license provided in the COPYING file.

As stated above, the documentation is licensed under the Boost 1.0 license, provided in the file Boost_license. Copyright: Copyright Digital Mars 2000-.

Authors
=======

All code is written from scratch by Philippe Daouadi <p.daouadi@free.fr>
