# include "src/MSL.h"

int main(int argc, char **argv)
{
    MSL::Number na = MSL::Number("5.00006");
    MSL::Number nb = MSL::Number("5.00006");

    std::cout << "EQUAL : " << (na == nb) << std::endl;

//    std::cout << (na > nb) << std::endl;

    return 0;
}