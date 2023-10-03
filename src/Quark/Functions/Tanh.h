#ifndef PYQUARK_TANH_H
#define PYQUARK_TANH_H

#include "Function.h"

namespace Quark
{
    namespace Functions
    {
        class Tanh : public Functions::Function
        {
        public:
            double F(double p) override;
            double D(double p) override;
        };
    }
}

#endif //PYQUARK_TANH_H
