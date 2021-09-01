# include "src/MSL.h"

int main(int argc, char **argv)
{
    MSL::Number a = MSL::Number("1");
    MSL::Number b = MSL::Number("2");
    MSL::Number c = MSL::Number("3");
    MSL::Number d = MSL::Number("4");
    MSL::Number res;
    
    MSL::ArithmeticOperation ao;

    res = a + b;
    res = res + c;
    res = res + d;
    res.print();

    return 0;
}