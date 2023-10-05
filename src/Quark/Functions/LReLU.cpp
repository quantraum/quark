#include "LReLU.h"
#include "../Exceptions/NotDerivableException.h"

double Quark::Functions::LReLU::F(double p)
{
    if (p >= 0)
        return p;

    return 0.01 * p;
}

double Quark::Functions::LReLU::D(double p)
{
    throw NotDerivableException("Step function is not derivable");
}
