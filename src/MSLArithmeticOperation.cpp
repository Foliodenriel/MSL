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
    if (isNegative)
        ret.opposite();
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
    return ret;
}

Number ArithmeticOperation::multiplicate(Number numberA, Number numberB)
{
    bool isNegative = false;

    if ((numberA.isPositive() && !numberB.isPositive()) || (!numberA.isPositive() && numberB.isPositive()))
        isNegative = true;
    // Classic handmade multiplication (cF. picture in folder)
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
        tmp = new char[len] + 1;
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

//    a.abs();
//    b.abs();
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

};