#ifndef PYQUARK_LRELU_H
#define PYQUARK_LRELU_H


#include "Function.h"

namespace Quark
{
    namespace Functions
    {
        class LReLU : public Functions::Function // Leaky ReLU
        {
        public:
            double F(double p) override;
            double D(double p) override;
        };
    }
}


#endif //PYQUARK_LRELU_H
