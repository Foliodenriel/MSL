# include "src/MSL.h"

int main(int argc, char **argv)
{
    MSL::Number a = MSL::Number("0.125");
    MSL::Number b = MSL::Number("0.125");
    MSL::Number res;
    
    MSL::ArithmeticOperation ao;

    res = a + b;
    res.print();

    return 0;
}