#include "Linear.h"

namespace Quark::Functions
{
    double Linear::F(double p)
    {
        return p;
    }

    double Linear::D(double p)
    {
        return 1;
    }
}
