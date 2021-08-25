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

        Number           add(Number operandA, Number operandB);
        Number           substract(Number operandA, Number operandB);
        


    private:

        Number           result;
};

};