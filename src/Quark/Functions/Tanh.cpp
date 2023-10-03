#include <cmath>
#include "Tanh.h"

namespace Quark::Functions
{
    double Tanh::F(double p)
    {
        return (exp(p) - exp(-p)) / (exp(p) + (exp(-p)));
    }

    double Tanh::D(double p)
    {
        return 1 - pow(F(p), 2);
    }
}