#include "BinaryStep.h"
#include "../Exceptions/NotDerivableException.h"

namespace Quark::Functions
{
    double BinaryStep::F(double p)
    {
        return p >= 0 ? 1 : 0;
    }

    double BinaryStep::D(double p)
    {
        throw NotDerivableException("Step function is not derivable");
    }
}