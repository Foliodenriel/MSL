# include "MSLNumber.h"

namespace MSL {

Number::Number(const char *s)
{
    init(s);
}

Number::Number(std::string s)
{
    init(s.c_str());
}

// PRIVATE ----------------------

void Number::init(const char *str)
{
    integer = NULL;
    decimal = NULL;
    info_flag = static_cast<int>(Number::InfoFlags::NONE);
    number_set = static_cast<int>(Number::NumberSet::NONE);

    if (is_valid(str))
    {
        validity = true;
        check_info(str);
        parse_integer_part(str);
        if (hasFlag(Number::InfoFlags::DECIMAL))
            parse_decimal_part(str);
        reformat();

    } else validity = false;
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

bool Number::is_valid(const char *str) const
{
    int i = 0;
    int negative_char = 0;
    int dot_char = 0;

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

void Number::check_info(const char *str)
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

void Number::parse_integer_part(const char *str)
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

void Number::parse_decimal_part(const char *str)
{
    int len = 0;
    int i = 0;
    int i_tmp = 0;
    int j = 0;

    while (str[i] != '.')
        i++;
    if (!str[i+1]) // If 
    {
        decimal = new char[2];
        memset(decimal, 0, 2);
        decimal[0] = '0';
        return ;
    }
    i++;
    i_tmp = i;
    while (str[i])
    {
        i++;
        len++;
    }

    decimal = new char[len + 1];
    memset(decimal, 0, len + 1);

    i = i_tmp;
    while (str[i])
    {
        decimal[j] = str[i];
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
    delete(integer);
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
        delete(decimal);
        decimal = tmp;
    }
}

bool Number::is_number_null(char *str) const
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
    }
}

bool Number::operator<(const Number& n)
{
    if (isPositive() && !n.isPositive())
        return false;
    else if (!isPositive() && n.isPositive())
        return true;
}

bool Number::operator==(const Number& n)
{
    char *nInteger;
    char *nDecimal;
    int cmpRes;

    if (Number::compareSign(*this, n))
    {
        //std::cout << "Same sign" << std::endl;
        nInteger = n.getInteger();
        cmpRes = strcmp(integer, nInteger);
        //std::cout << "integer cmp : " << cmpRes << std::endl;
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
    if (hasFlag(Number::InfoFlags::DECIMAL))
        return true;
    return false;
}

bool Number::hasFlag(Number::InfoFlags flags) const
{
    return info_flag & static_cast<int>(flags);
}

bool Number::hasNullInteger() const
{
    return is_number_null(integer);
}

bool Number::hasNullDecimal() const
{
    return is_number_null(decimal);
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

void Number::opposite()
{
    if (isPositive())
        addInfoFlag(Number::InfoFlags::NEGATIVE);
    else
        removeInfoFlag(Number::InfoFlags::NEGATIVE);
}

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
    {
        std::cout << "A" << std::endl;
        return 0;
    }
    else if (a.hasNullDecimal() && !b.hasNullDecimal())
    {
        std::cout << "B" << std::endl;
        return -1;
    }
    else if (!a.hasNullDecimal() && b.hasNullDecimal())
    {
        std::cout << "C" << std::endl;
        return 1;
    }
    return strcmp(aDecimal, bDecimal);
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