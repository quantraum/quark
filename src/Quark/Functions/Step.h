#ifndef PYQUARK_STEP_H
#define PYQUARK_STEP_H

#include "Function.h"

namespace Quark
{
    namespace Functions
    {
        class Step : public Functions::Function
        {
        public:
            double F(double p) override;
            double D(double p) override;
        };
    }
}

#endif //PYQUARK_STEP_H
