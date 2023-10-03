#ifndef PYQUARK_LINEAR_H
#define PYQUARK_LINEAR_H

#include "Function.h"

namespace Quark
{
    namespace Functions
    {
        class Linear : public Functions::Function
        {
        public:
            double F(double p) override;
            double D(double p) override;
        };
    }
}

#endif //PYQUARK_LINEAR_H
