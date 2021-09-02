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
    ~Number();

    // GETTER / SETTER
    void            setValue(const char *s);                            // Call init if constructor wasn't called
    bool            hasFlag(Number::InfoFlags flags) const;             // Checks for flags
    bool            isNull() const;                                     // Checks if number is equal to 0
    bool            hasNullDecimal() const;                             // Checks if decimal is equal to 0
    bool            hasNullInteger() const;                             // Checks if integer is equal to 0
    char            *getInteger() const;                                // Gives a pointer to integer
    char            *getDecimal() const;                                // Gives a pointer to decimal
    bool            isPositive() const;                                 // Checks if number is positive
    bool            isDecimal() const;                                  // Checks if number is a decimal number

    // ACTION
    void            opposite();                                         // Set number to it's opposite (1 opposite of -1)
    void            abs();                                              // Set number to it's absolute value

    void            printInfo();                                        // Prints number's info verbose
    void            print();                                            // Prints number

    // OPERATOR
    bool            operator>(const Number& n);
    bool            operator>=(const Number& n);
    bool            operator<(const Number& n);
    bool            operator<=(const Number& n);
    bool            operator==(const Number& n);

    // STATIC
    static bool     compareSign(const Number& a, const Number& b);      // Compares two numbers by sign
    static int      compareDecimal(const Number& a, const Number& b);   // Compares decimal of two numbers

private:

    void            init(const char *str);
    void            clear();
    bool            isValidStr(const char *str) const;                  // Checks if str is valid must be -.0123456789
    void            checkInfo(const char *str);                         // Checks and fill info_flags
    void            parseIntegerPart(const char *str);
    void            parseDecimalPart(const char *str);
    void            reformat();                                         // Removes useless char
    bool            isNumberNull(char *str) const;

    void            addInfoFlag(Number::InfoFlags flags);
    void            removeInfoFlag(Number::InfoFlags flags);

    char            *integer;
    char            *decimal;
    int             number_set;
    int             info_flag;
    bool            validity;                                           // DEPRECATED
};

bool                str_contains(const char *str, char c);

};

/*

ERROR CODE:

101: Invalid character/format



*/