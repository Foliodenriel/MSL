#include "MSLArithmeticOperation.h"

namespace MSL {

ArithmeticOperation::ArithmeticOperation()
{

}

// PUBLIC


Number ArithmeticOperation::add(Number addendA, Number addendB)
{
    Number ret;
    char *new_number = NULL;
    bool isNegative = false;

    if ((addendA.isPositive() && addendB.isPositive()) || (!addendA.isPositive() && !addendB.isPositive()))
    {
        // Calculate decimal
        new_number = addProcedure(addendA, addendB);
        if (!addendB.isPositive())
            isNegative = true;
    }
    else
    {
        // Substract and check for sign
        if (ArithmeticOperation::isAbsSuperior(addendA, addendB))
        {
            new_number = subProcedure(addendA, addendB);
            if (!addendA.isPositive())
                isNegative = true;
        }
        else
        {
            new_number = subProcedure(addendB, addendA);
            if (!addendB.isPositive())
                isNegative = true;
        }
    }
    ret.setValue(new_number);
    // Check if negative and NULL to change sign
    if (isNegative && !ret.isNull())
        ret.opposite();
    if (new_number)
        delete new_number;
    return ret;
}

Number ArithmeticOperation::substract(Number numberA, Number numberB)
{
    Number ret;
    char *new_number = NULL;
    bool isNegative = false;

    if (numberA.isPositive() && !numberB.isPositive())
        new_number = addProcedure(numberA, numberB);
    else if (!numberA.isPositive() && numberB.isPositive())
    {
        new_number = addProcedure(numberA, numberB);
        isNegative = true;
    }
    else if (numberA.isPositive() && numberB.isPositive())
    {
        if (ArithmeticOperation::isAbsSuperior(numberA, numberB))
            new_number = subProcedure(numberA, numberB);
        else
        {
            new_number = subProcedure(numberB, numberA);
            isNegative = true;
        }
    }
    else if (!numberA.isPositive() && !numberB.isPositive())
    {
        if (ArithmeticOperation::isAbsSuperior(numberA, numberB))
        {
            new_number = subProcedure(numberA, numberB);
            isNegative = true;
        }
        else
            new_number = subProcedure(numberB, numberA);
    }
    ret.setValue(new_number);
    if (isNegative)
        ret.opposite();
    if (new_number)
        delete new_number;
    if (ret.isNull())
        ret.abs();
    return ret;
}

Number ArithmeticOperation::multiplicate(Number numberA, Number numberB)
{
    Number ret;
    char *new_number = NULL;
    char *tmp = NULL;
    size_t len;
    bool isNegative = false;

    // Classic handmade multiplication (cF. picture in folder)
    len = 0;
    if ((numberA.isPositive() && !numberB.isPositive()) || (!numberA.isPositive() && numberB.isPositive()))
        isNegative = true;
    new_number = multProcedure(strjoin(numberA.getInteger(), numberA.getDecimal()), strjoin(numberB.getInteger(), numberB.getDecimal()));
    if (numberA.getDecimal())
        len += strlen(numberA.getDecimal());
    if (numberB.getDecimal())
        len += strlen(numberB.getDecimal());
    tmp = new_number;
    new_number = placeDecimal(tmp, len);
    ret.setValue(new_number);
    if (isNegative)
        ret.opposite();
    if (new_number)
        delete new_number;
    return ret;
}

Number ArithmeticOperation::divide(Number numberA, Number numberB)
{
    Number ret;
    size_t len;
    char *res;
    char *integer;
    char *decimal;

    res = NULL;
    integer = NULL;
    decimal = NULL;
    numberA.abs();
    numberB.abs();
    commonIntegeriser(numberA, numberB);

    integer = divProcedureInteger(numberA, numberB);
    if (!numberA.isNull())
        decimal = divProcedureDecimal(numberA, numberB);

    if (decimal)
    {
        len = strlen(integer) + strlen(decimal);
        res = new char[len + 2];
        memset(res, 0, len + 2);
        strcpy(res, integer);
        strcpy(&res[strlen(integer)], ".");
        strcpy(&res[strlen(integer) + 1], decimal);
    }
    else
    {
        len = strlen(integer);
        res = new char[len + 1];
        memset(res, 0, len + 1);
        strcpy(res, integer);
    }

    ret = Number(res);
    if ((numberA.isPositive() && !numberB.isPositive()) || (!numberA.isPositive() && numberB.isPositive()))
        ret.opposite();

    return ret;
}

// PRIVATE

int ArithmeticOperation::getIntFromChar(char c)
{
    return static_cast<int>(c) - 48;
}

char ArithmeticOperation::getCharFromInt(int n)
{
    if (n >= 0 && n < 10)
        return static_cast<char>(n + 48);
    return 0;
}

bool ArithmeticOperation::isAbsSuperior(Number a, Number b)
{
    a.abs();
    b.abs();
    if (a > b)
        return true;
    return false;
}

char *ArithmeticOperation::addProcedure(Number addendA, Number addendB)
{
    char *new_decimal = NULL;
    char *new_integer = NULL;
    char *tmp;
    int extra_carry = 0;
    int len;
    size_t res;

    // Decimal Addition
    if (addendA.hasNullDecimal() && !addendB.hasNullDecimal()) // If !A and B : get B decimal
        new_decimal = addendB.getDecimal();
    else if (!addendA.hasNullDecimal() && addendB.hasNullDecimal()) // If !B and A : get A decimal
        new_decimal = addendA.getDecimal();
    else if (!addendA.hasNullDecimal() && !addendB.hasNullDecimal())
    {
        char *decA = addendA.getDecimal();
        char *decB = addendB.getDecimal();

        fillChar(&decA, &decB, false);
        
        len = strlen(decA);
        new_decimal = new char[len + 1];
        memset(new_decimal, 0, len + 1);
        len--;
        while (len >= 0)
        {
            res = (getIntFromChar(decA[len]) + getIntFromChar(decB[len])) + extra_carry;
            new_decimal[len] = getCharFromInt(res % 10);
            extra_carry = (res > 9) ? 1 : 0;
            len--;
        }
    }

    char *intA = addendA.getInteger();
    char *intB = addendB.getInteger();

    fillChar(&intA, &intB, true);
    len = strlen(intA);
    new_integer = new char[len + 1];
    memset(new_integer, 0, len + 1);
    len--;
    while (len >= 0)
    {
        res = (getIntFromChar(intA[len]) + getIntFromChar(intB[len])) + extra_carry;
        new_integer[len] = getCharFromInt(res % 10);
        extra_carry = (res > 9) ? 1 : 0;
        len--;
    }
    if (extra_carry > 0)
    {
        len = strlen(new_integer) + 1;
        tmp = new char[len + 1];
        memset(tmp, 0, len + 1);
        tmp[0] = '1';
        strcat(tmp, new_integer);
        delete(new_integer);
        new_integer = tmp;
        tmp = NULL;
    }
    len = new_decimal != NULL ? strlen(new_decimal): 0;
    len = strlen(new_integer) + len + 2;
    tmp = new char[len];
    memset(tmp, 0, len);
    strcpy(tmp, new_integer);
    if (new_decimal)
        strcat(strcat(tmp, "."), new_decimal);
    if (new_integer)
        delete new_integer;
    if (new_decimal)
        delete new_decimal;
    return tmp;
}

char *ArithmeticOperation::subProcedure(Number a, Number b)
{
    char *new_integer;
    char *new_decimal;
    char *aInteger;
    char *bInteger;
    char *tmp;
    int ec = 0;

    aInteger = a.getInteger();
    bInteger = b.getInteger();
    new_decimal = subProcedureDecimalHelper(a.getDecimal(), b.getDecimal(), &ec, Number::compareDecimal(a, b));
    fillChar(&aInteger, &bInteger, true);
    new_integer = subProcedureAction(aInteger, bInteger, ec);

    // Using ec as new len
    ec = new_decimal != NULL ? strlen(new_decimal): 0;
    ec = strlen(new_integer) + ec + 2;
    tmp = new char[ec];
    memset(tmp, 0, ec);
    strcpy(tmp, new_integer);
    if (new_decimal)
        strcat(strcat(tmp, "."), new_decimal);
    delete new_integer;
    delete new_decimal;
    return tmp;
}

char *ArithmeticOperation::subProcedureDecimalHelper(char *decA, char *decB, int *ec, int cmp_res)
{
    char *new_decimal = NULL;
    char *tmp_decimal = NULL;
    char *tmp_upunit = NULL;

    if (cmp_res > 0)
    {
        fillChar(&decA, &decB, false);
        new_decimal = subProcedureAction(decA, decB);
    }
    else if (cmp_res < 0)
    {
        // B - A    => Because Substraction is only possible when the subtrahend is smaller than minuend
        fillChar(&decA, &decB, false);
        tmp_decimal = subProcedureAction(decB, decA);
        fillChar(&tmp_upunit, &tmp_decimal, false);
        new_decimal = subProcedureAction(tmp_upunit, tmp_decimal);
        delete(tmp_upunit);
        delete(tmp_decimal);
        *ec = 1;
    }
    return new_decimal;
}

char *ArithmeticOperation::subProcedureAction(char *a, char *b, int extra_carry)
{
    char *ret;
    int res;
    int len;
    int ec = extra_carry;

    len = strlen(a);
    ret = new char[len + 1];
    memset(ret, 0, len + 1);
    len--;
    while (len >= 0)
    {
        res = (getIntFromChar(a[len]) - getIntFromChar(b[len]) - ec);
        ec = 0;
        if (res >= 0 )
            ret[len] = getCharFromInt(res);
        else
        {
            ret[len] = getCharFromInt(10 + res);
            ec = 1;
        }
        len--;
    }
    return ret;
}

char *ArithmeticOperation::multProcedure(char *a, char *b)
{
    // 1.46  |  2
    Number res;
    Number addend;
    char *line;
    int x, y;
    int i;
    int m;
    int aLen;
    int ec;

    y = 0;
    line = NULL;
    aLen = strlen(a);
    reverseStr(a);
    reverseStr(b);
    while (b[y])
    {
        i = 0;
        x = 0;
        ec = 0;
        if (line)
            delete line;
        line = new char[aLen + y + 2];
        memset(line, 0, aLen + y + 2);
        // Adding begining 0
        while (i < y)
        {
            line[i] = '0';
            i++;
        }
        while (a[x])
        {
            m = (getIntFromChar(a[x]) * getIntFromChar(b[y])) + ec;
            line[x + i] = getCharFromInt(m % 10);
            ec = m / 10;
            x++;
        }
        line[x + i] = getCharFromInt(ec);
        reverseStr(line);
        addend.setValue(line);
        res = res + addend;
        y++;
    }
    if (line)
        delete line;
    return res.getInteger();
}

char *ArithmeticOperation::divProcedureInteger(Number& a, Number& b)
{
    Number mult;
    Number subA;
    Number subB;
    char *aRemain;
    char *tmpRes;
    char *aCopy;
    char *res;
    char resChar;
    size_t i;

    i = 0;
    res = NULL;
    aCopy = NULL;
    tmpRes = NULL;
    aRemain = NULL;
    aCopy = a.getInteger();
    mult = Number("10");
    if (a < b)
    {
        res = new char[2];
        memset(res, 0, 2);
        res[0] = '0';
        a = a * mult;
    }
    else
    {
        while (aCopy[i]) // We increase each time we are going through
        {
            if (i == 0)
                subA = Number(getIntFromChar(aCopy[i]));
            subB = b;
            resChar = divProcedureClosest(subA, subB);
            if (resChar > 0)
            {
                Number add;

                if (aCopy[i + 1])
                    add = Number(getIntFromChar(aCopy[i + 1]));
                tmpRes = strcjoin(res, getCharFromInt(resChar));
                if (res)
                    delete res;
                res = tmpRes;
                if (aCopy[i])
                {
                    subA = subA - subB;
                    subA = subA * mult;
                    subA = subA + add;
                }
            }
            else
            {
                Number add;

                if (aCopy[i + 1])
                    add = Number(getIntFromChar(aCopy[i + 1]));
                tmpRes = strcjoin(res, getCharFromInt(resChar));
                if (res)
                    delete res;
                res = tmpRes;
                if (aCopy[i])
                {
                    subA = subA * mult;
                    subA = subA + add;
                }
            }
            i++;
        }
        a = subA;
    }
    return res;
}

char *ArithmeticOperation::divProcedureDecimal(Number& a, Number& b)
{
    std::vector<Number> pattern;
    int i;
    Number mult;
    Number subA;
    Number subB;
    char *res;
    char *aRemain;
    char *tmpRes;
    char resChar;
    
    i = 0;
    subA = a;
    res = NULL;
    mult = Number("10");
    while (!a.isNull() && i < Number::Precision())
    {
        if (divProcedureVectorContains(pattern, subA))
            break ;
        else
            pattern.push_back(subA);
        subB = b;
        if (subA >= subB)   // Then divclosest in next number
        {
            resChar = divProcedureClosest(subA, subB);
            tmpRes = strcjoin(res, getCharFromInt(resChar));
            if (res)
                delete res;
            res = tmpRes;
            subA = subA - subB;
            subA = subA * mult;
        }
        else                // Then '0' is next number
        {
            resChar = 0;
            tmpRes = strcjoin(res, getCharFromInt(resChar));
            if (res)
                delete res;
            res = tmpRes;
            subA = subA * mult;
        }
        i++;
    }
    a = subA;
    return res;
}

bool ArithmeticOperation::divProcedureVectorContains(std::vector<Number> list, Number n)
{
    for (auto &i : list)
    {
        if (i == n)
            return true;
    }
    return false;
}

int ArithmeticOperation::divProcedureClosest(Number a, Number& b)
{
    Number add;
    int i;

    i = 0;
    add = b;
    if (a < b || b.isNull())
        return 0;
    while (b <= a)
    {
        b = b + add;
        i++;
    }
    b = b - add;
    return i;
}

// Fill char for them to have same size
void ArithmeticOperation::fillChar(char **stra, char **strb, bool fillBefore)
{
    char *new_str;
    size_t len_a = *stra != NULL ? strlen(*stra) : 0;
    size_t len_b = *strb != NULL ? strlen(*strb) : 0;
    size_t fill_len;

    if (len_a == len_b)
        return ;
    if (len_a > len_b)
    {
        fill_len = len_a - len_b;
        new_str = new char[len_a + 1];
        memset(new_str, '0', len_a + 1);
        new_str[len_a] = '\0';
        if (fillBefore)
            strncpy(&new_str[fill_len], *strb, len_b);
        else
            strncpy(new_str, *strb, len_b);
        *strb = new_str;
    }
    else
    {
        fill_len = len_b - len_a;
        new_str = new char[len_b + 1];
        memset(new_str, '0', len_b + 1);
        new_str[len_b] = '\0';
        if (fillBefore)
            strncpy(&new_str[fill_len], *stra, len_a);
        else
            strncpy(new_str, *stra, len_a);
        *stra = new_str;
    }
}

void ArithmeticOperation::reverseStr(char *str)
{
    char tmp;
    size_t len;
    int i;

    i = 0;
    len = strlen(str) - 1;
    while (str[i] && (i < len))
    {
        tmp = str[i];
        str[i] = str[len];
        str[len] = tmp;
        len--;
        i++;
    }
}

void ArithmeticOperation::commonIntegeriser(Number& a, Number& b)
{
    Number n = Number("10");
    
    while (a.isDecimal() || b.isDecimal())
    {
        a = a * n;
        b = b * n;
    }
}

char *ArithmeticOperation::placeDecimal(char *src, int nb_decimal)
{
    char *ret;
    size_t len;

    ret = NULL;
    len = strlen(src);
    if (nb_decimal > len)
    {
        ret = new char[nb_decimal + 3];
        memset(ret, '0', nb_decimal + 3);
        ret[1] = '.';
        ret[nb_decimal + 2] = '\0';
        strcpy(&ret[nb_decimal + 2 - len], src);
        return ret;
    }
    ret = strjoin(".", &src[len - nb_decimal]);
    src[len - nb_decimal] = '\0';
    if (len == nb_decimal)
        ret = strjoin("0", ret);
    else
        ret = strjoin(src, ret);
    return ret;
}

// STATIC

Number operator+(Number& a, Number& b)
{
    ArithmeticOperation ao;
    return ao.add(a, b);
}

Number operator-(Number& a, Number& b)
{
    ArithmeticOperation ao;
    return ao.substract(a, b);
}

Number operator*(Number& a, Number& b)
{
    ArithmeticOperation ao;
    return ao.multiplicate(a, b);
}

char *strjoin(const char *a, const char *b)
{
    char *ret;
    size_t len;

    ret = NULL;
    if (a && b)
    {
        len = strlen(a) + strlen(b) + 1;
        ret = new char[len];
        memset(ret, 0, len);
        strcpy(ret, a);
        strcpy(&ret[strlen(a)], b);
    }
    else if (a && !b)
    {
        len = strlen(a) + 1;
        ret = new char[len];
        memset(ret, 0, len);
        strcpy(ret, a);
    }
    else if (!a && b)
    {
        len = strlen(b) + 1;
        ret = new char[len];
        memset(ret, 0, len);
        strcpy(ret, b);
    }
    return ret;
}

char *strcjoin(const char *a, char c)
{
    char *ret;
    size_t len;

    len = 0;
    ret = NULL;
    if (a)
    {
        len = strlen(a);
        ret = new char[len + 2];
        memset(ret, 0, len + 2);
        strcpy(ret, a);
        ret[len] = c;
    }
    else
    {
        ret = new char[2];
        memset(ret, 0, 2);
        ret[0] = c;
    }
    return ret;
}

};