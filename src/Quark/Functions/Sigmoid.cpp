#include <valarray>
#include "Sigmoid.h"

namespace Quark::Functions
{
    Sigmoid::Sigmoid(double c): m_c(c) {}

    double Sigmoid::F(double p)
    {
        return 1 / (1 + std::exp(-m_c * p));
    }

    double Sigmoid::D(double p)
    {
        return m_c * F(p) * (1 - F(p));
    }
}