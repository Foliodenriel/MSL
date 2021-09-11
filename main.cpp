# include "src/MSL.h"

int main(int argc, char **argv)
{
    MSL::Number a = MSL::Number("50803");
    MSL::Number b = MSL::Number("92021.684");
    MSL::Number res;

    MSL::Number::Precision(20);
    
    MSL::ArithmeticOperation ao;

    res = ao.divide(a, b);
    //res.print();

    return 0;
}