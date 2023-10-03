#include "Step.h"
#include "../Exceptions/NotDerivableException.h"

namespace Quark::Functions
{
    double Step::F(double p)
    {
        return p >= 0 ? 1 : -1;
    }

    double Step::D(double p)
    {
        throw NotDerivableException("Step function is not derivable");
    }
}