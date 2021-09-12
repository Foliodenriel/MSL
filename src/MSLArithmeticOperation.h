#pragma once

# include <iostream>
# include <vector>

# include "MSLNumber.h"

namespace MSL {

class ArithmeticOperation {

    public:

        enum class OperationType : int {
            NONE            = 0,
            ADDITION        = 1,
            SUBSTRACTION    = 2,
            MULTIPLICATION  = 4,
            DIVISION        = 8,
            MODULO          = 16
        };

        ArithmeticOperation();

        // ACTION
        Number              add(Number addendA, Number addendB);
        Number              substract(Number addendA, Number addendB);
        Number              multiplicate(Number numberA, Number numberB);
        Number              divide(Number numberA, Number numberB);
        


    private:

        int                 getIntFromChar(char c);
        char                getCharFromInt(int n);

        // + | -
        char                *addProcedure(Number addendA, Number addendB);
        char                *subProcedure(Number a, Number b);
        char                *subProcedureDecimalHelper(char *decA, char *decB, int *ec, int cmp_res);
        char                *subProcedureAction(char *a, char *b, int extra_carry = 0);
        char                *multProcedure(char *a, char *b);
        char                *divProcedureInteger(Number& a, Number& b);
        char                *divProcedureDecimal(Number& a, Number& b);
        bool                divProcedureVectorContains(std::vector<Number> list, Number n);              // Check if n is contained in list
        void                divProcedurePrepare(char **copy, char **remain, Number& b);
        int                 divProcedureClosest(Number a, Number& b);                                    // How many times they are b in a ( a > b)

        char                *placeDecimal(char *src, int nb_decimal);
        bool                isAbsSuperior(Number a, Number b);
        char                *allocateSupUnit(char *s); // Allocate a new *10 number that is one unit superior to given string, ex: 6672 => 10000
        void                fillChar(char **stra, char **strb, bool fillBefore);
        void                reverseStr(char *str);
        void                commonIntegeriser(Number& a, Number& b);

        Number              result;
};

Number                      operator+(Number& a, Number& b);
Number                      operator-(Number& a, Number& b);
Number                      operator*(Number& a, Number& b);

char                        *strjoin(const char *a, const char *b);     // Concat two string and return newly allocated result
char                        *strcjoin(const char *a, char c);


};