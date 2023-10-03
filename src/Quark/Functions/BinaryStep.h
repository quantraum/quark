#ifndef PYQUARK_BINARYSTEP_H
#define PYQUARK_BINARYSTEP_H

#include <string>
#include "Function.h"

namespace Quark
{
    namespace Functions
    {
        class BinaryStep : public Functions::Function
        {
        public:
            double F(double p) override;
            double D(double p) override;
        };
    }
}

#endif //PYQUARK_BINARYSTEP_H
