#pragma once

# include <iostream>
# include <cstring>

namespace MSL {

class Number {

public:

    enum class NumberSet : int {
        NONE        = 0,
        NATURAL     = 1,
        INTEGER     = 2,
        RATIONAL    = 4,
        IRRATIONAL  = 8,
        ALGEBRAIC   = 16,
        REAL        = 32,
        IMAGINARY   = 64
    };

    enum class InfoFlags : int {

        NONE        = 0,
        NEGATIVE    = 1,
        DECIMAL     = 2
    };

    Number(const char *s);
    Number(std::string s);
    Number();

    // GETTER / SETTER
    bool            hasFlag(Number::InfoFlags flags) const;
    bool            hasNullDecimal() const;
    bool            hasNullInteger() const;
    char            *getInteger() const;
    char            *getDecimal() const;
    bool            isPositive() const;
    bool            isDecimal() const;

    // ACTION
    void            opposite();

    void            printInfo();

    // OPERATOR
    bool            operator>(const Number& n);
    bool            operator<(const Number& n);
    bool            operator==(const Number& n);

    // STATIC
    static bool     compareSign(const Number& a, const Number& b);
    static int      compareDecimal(const Number& a, const Number& b);

private:

    void            init(const char *str);
    bool            is_valid(const char *str) const;   // Check if char * passed is valid must be -.0123456789
    void            check_info(const char *str);       // Check for info flags
    void            parse_integer_part(const char *str);
    void            parse_decimal_part(const char *str);
    void            reformat();
    bool            is_number_null(char *str) const;

    void            addInfoFlag(Number::InfoFlags flags);
    void            removeInfoFlag(Number::InfoFlags flags);

    char            *integer;
    char            *decimal;
    int             number_set;
    int             info_flag;
    bool            validity;
};

bool                str_contains(const char *str, char c);

};

/*

ERROR CODE:

101: Invalid character/format



*/