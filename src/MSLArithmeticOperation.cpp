#include "MSLArithmeticOperation.h"

namespace MSL {

ArithmeticOperation::ArithmeticOperation()
{

}

Number ArithmeticOperation::add(Number operandA, Number operandB)
{
    if ((operandA.isPositive() && operandB.isPositive()) || (!operandA.isPositive() && !operandB.isPositive()))
    {
        // DO the Add Function
    }
    else
    {
        // Substract and check for sign
    }
}

};