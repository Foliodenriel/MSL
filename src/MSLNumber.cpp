# include "MSLNumber.h"

namespace MSL {

size_t Number::sPrecision = 4;

Number::Number() {
    init(NULL);
}

Number::Number(const char *s)
{
    init(s);
}

Number::Number(std::string s)
{
    init(s.c_str());
}

Number::Number(int n)
{
    std::string s;

    s = std::to_string(n);
    init(s.c_str());
}

Number::~Number()
{

}

// PRIVATE ----------------------

void Number::init(const char *str)
{
    integer = NULL;
    decimal = NULL;
    info_flag = static_cast<int>(Number::InfoFlags::NONE);
    number_set = static_cast<int>(Number::NumberSet::NONE);

    if (isValidStr(str))
    {
        checkInfo(str);
        parseIntegerPart(str);
        if (hasFlag(Number::InfoFlags::DECIMAL))
            parseDecimalPart(str);
        reformat();
    }
    else
        parseIntegerPart("0");
}

void Number::clear()
{
    if (integer) delete integer;
    if (decimal) delete decimal;
}

void Number::addInfoFlag(Number::InfoFlags flags)
{
    info_flag = info_flag | static_cast<int>(flags);
}

void Number::removeInfoFlag(Number::InfoFlags flags)
{
    if (hasFlag(flags))
        info_flag -= static_cast<int>(flags);
}

bool Number::isValidStr(const char *str) const
{
    int i = 0;
    int negative_char = 0;
    int dot_char = 0;

    if (!str)
        return false;
    while (str[i])
    {
        if (!str_contains("-.0123456789", str[i]))
            return false;
        if (str[i] == '-' && i > 0)
            return false;
        if ((str[i] == '.') && (i < 1 || (i > 0 && str[i-1] == '-')))
            return false;
        i++;
    }
    if (negative_char > 1 || dot_char > 1)
        return false;
    return true;
}

void Number::checkInfo(const char *str)
{
    int i = 0;

    while (str[i])
    {
        if (str[i] == '-')
            addInfoFlag(Number::InfoFlags::NEGATIVE);
        if (str[i] == '.')
            addInfoFlag(Number::InfoFlags::DECIMAL);
        i++;
    }
}

void Number::parseIntegerPart(const char *str)
{
    int len = 0;
    int i = 0;
    int j = 0;

    // Go to begining
    i = hasFlag(Number::InfoFlags::NEGATIVE) ? 1 : 0;
    while (str[i])
    {
        if (str[i] == '.')
            break ;
        i++;
        len++;
    }
    
    integer = new char[len + 1];
    memset(integer, 0, len + 1);

    i = hasFlag(Number::InfoFlags::NEGATIVE) ? 1 : 0;
    while (str[i])
    {
        if (str[i] == '.')
            break ;
        else {
            integer[j] = str[i];
            i++;
            j++;
        }
    }
}

void Number::parseDecimalPart(const char *str)
{
    int len = 0;
    int i = 0;
    int i_tmp = 0;
    int j = 0;

    while (str[i] != '.')
        i++;
    if (!str[i + 1])
    {
        decimal = new char[2];
        memset(decimal, 0, 2);
        decimal[0] = '0';
        return ;
    }
    i++;
    i_tmp = i;
    i = 0;
    while (str[i_tmp + i] && i < Number::sPrecision)
    {
        i++;
        len++;
    }

    decimal = new char[len + 1];
    memset(decimal, 0, len + 1);

    i = 0;
    while (str[i_tmp + i] && i < Number::sPrecision)
    {
        decimal[j] = str[i_tmp + i];
        i++;
        j++;
    }
}

void Number::reformat()
{
    char *tmp;
    int i = 0;
    int start = 0;
    int len = strlen(integer);

    // Integer removing zero
    while (integer[start] && integer[start] == '0')
        start++;
    if (start == len)
    {
        tmp = new char[2];
        memset(tmp, 0, 2);
        tmp[0] = '0';
    }
    else
    {
        tmp = new char[len - start + 1];
        memset(tmp, 0, len - start + 1);
        while (start != len)
        {
            tmp[i] = integer[start];
            start++;
            i++;
        }
    }
    delete integer;
    integer = tmp;

    // Decimal removing zero
    if (isDecimal())
    {
        tmp = NULL;
        i = start = 0;
        len = strlen(decimal);
        while (len >= 0 && decimal[len - 1] == '0')
            len--;
        if (len == 0)
        {
            tmp = new char[2];
            memset(tmp, 0, 2);
            tmp[0] = '0';
        }
        else
        {
            tmp = new char[len + 1];
            memset(tmp, 0, len + 1);
            while (decimal[i] && i != len)
            {
                tmp[i] = decimal[i];
                i++;
            }
        }
        delete decimal;
        decimal = tmp;
    }
}

bool Number::isNumberNull(char *str) const
{
    int i = 0;

    if (str == NULL)
        return true;
    while (str[i])
    {
        if (str[i] != '0')
            return false;
        i++;
    }
    return true;
}

// PUBLIC -----------------------

bool Number::operator>(const Number& n)
{
    char *nInteger;
    char *nDecimal;
    int lenA;
    int lenB;
    int i;
    int cmpRes;


    if (isPositive() && !n.isPositive())
        return true;
    else if (!isPositive() && n.isPositive())
        return false;
    else
    {
        nInteger = n.getInteger();
        lenA = strlen(integer);
        lenB = strlen(nInteger);
        i = 0;
        if (isPositive())
        {
            if (lenA > lenB)
                return true;
            else if (lenA < lenB)
                return false;
        }
        else
        {
            if (lenA > lenB)
                return false;
            else if (lenA < lenB)
                return true;
        }
        // lenA == lenB
        cmpRes = strcmp(integer, nInteger);
        if (cmpRes > 0)
        {
            if (isPositive())
                return true;
            return false;
        }
        else if (cmpRes < 0)
        {
            if (isPositive())
                return false;
            return true;
        }
        else
        {
            // Check decimal difference
            if (hasNullDecimal() && n.hasNullDecimal())
                return false;
            else if (hasNullDecimal())
            {
                if (isPositive())
                    return false;
                return true;
            }
            else if (n.hasNullDecimal())
            {
                if (isPositive())
                    return true;
                return false;
            }
            nDecimal = n.getDecimal();
            cmpRes = strcmp(decimal, nDecimal);
            if (cmpRes > 0)
            {
                if (isPositive())
                    return true;
                return false;
            }
            else if (cmpRes < 0)
            {
                if (isPositive())
                    return false;
                return true;
            }
            else
                return false;
        }
    }
}

bool Number::operator>=(const Number& n)
{
    if ((*this > n) || (*this == n))
        return true;
    return false;
}

bool Number::operator<(const Number& n)
{
    bool res;

    if (isPositive() && !n.isPositive())
        return false;
    else if (!isPositive() && n.isPositive())
        return true;
    if (*this == n)
        return false;
    res = (*this > n);
    if (res)
        return false;
    return true;

}

bool Number::operator<=(const Number& n)
{
    if (*this < n || (*this == n))
        return true;
    return false;
}

bool Number::operator==(const Number& n)
{
    char *nInteger;
    char *nDecimal;
    int cmpRes;

    if (Number::compareSign(*this, n))
    {
        nInteger = n.getInteger();
        cmpRes = strcmp(integer, nInteger);
        if (!cmpRes)
        {
            nDecimal = n.getDecimal();
            cmpRes = Number::compareDecimal(*this, n);
            if (cmpRes == 0)
                return true;
            return false;
        }
        else
            return false;
    }
    return false;
}

void Number::setValue(const char *s)
{
    // DO CLEAR FIRST -- CODE8 --
    clear();
    init(s);
}

char *Number::getInteger() const
{
    return integer;
}

char *Number::getDecimal() const
{
    return decimal;
}

bool Number::isPositive() const
{
    if (hasFlag(Number::InfoFlags::NEGATIVE))
        return false;
    return true;
}

bool Number::isDecimal() const
{
    if (!hasNullDecimal())
        return true;
    return false;
}

bool Number::hasFlag(Number::InfoFlags flags) const
{
    return info_flag & static_cast<int>(flags);
}

bool Number::isNull() const
{
    return hasNullInteger() && hasNullDecimal();
}

bool Number::hasNullInteger() const
{
    return isNumberNull(integer);
}

bool Number::hasNullDecimal() const
{
    return isNumberNull(decimal);
}

void Number::printInfo()
{
    std::cout << "------ Number " << validity << " ------" << std::endl;

    if (hasFlag(Number::InfoFlags::NEGATIVE)) std::cout << "Sign : NEGATIVE -" << std::endl;
    else std::cout << "Sign : POSITIVE +" << std::endl;

    if (hasFlag(Number::InfoFlags::DECIMAL)) std::cout << "Type : DECIMAL" << std::endl;
    else std::cout << "Type : INTEGER" << std::endl;

    std::cout << "Number : ";
    if (hasFlag(Number::InfoFlags::NEGATIVE)) std::cout << "-";
    std::cout << integer;
    if (hasFlag(Number::InfoFlags::DECIMAL)) std::cout << ".";
    std::cout << decimal;

    std::cout << std::endl;

    std::cout << "-----     END     -----" << std::endl;
}

void Number::print()
{
    if (!isPositive())
        std::cout << "-";
    std::cout << integer;
    if (isDecimal())
        std::cout << "." << decimal;
    std::cout << std::endl;
}

void Number::opposite()
{
    if (isPositive())
        addInfoFlag(Number::InfoFlags::NEGATIVE);
    else
        removeInfoFlag(Number::InfoFlags::NEGATIVE);
}

void Number::abs()
{
    if (!isPositive())
        removeInfoFlag(Number::InfoFlags::NEGATIVE);
}

// STATIC

bool Number::compareSign(const Number& a, const Number& b)
{
    if ((a.isPositive() && b.isPositive()) || (!a.isPositive() && !b.isPositive()))
        return true;
    return false;
}

int Number::compareDecimal(const Number& a, const Number& b)
{
    char *aDecimal = a.getDecimal();
    char *bDecimal = b.getDecimal();
    int i = 0;

    if (a.hasNullDecimal() && b.hasNullDecimal())
        return 0;
    else if (a.hasNullDecimal() && !b.hasNullDecimal())
        return -1;
    else if (!a.hasNullDecimal() && b.hasNullDecimal())
        return 1;
    return strcmp(aDecimal, bDecimal);
}

size_t Number::Precision()
{
    return Number::sPrecision;
}

size_t Number::Precision(size_t n)
{
    Number::sPrecision = n;
    return Number::sPrecision;
}

// FILE

bool str_contains(const char *str, char c)
{
    int i = 0;
    bool ret = false;

    while (str[i])
    {
        if (str[i] == c)
            ret = true;
        i++;
    }
    return ret;
}

};