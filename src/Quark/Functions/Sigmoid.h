#ifndef PYQUARK_SIGMOID_H
#define PYQUARK_SIGMOID_H

#include <string>
#include "Function.h"

namespace Quark
{
    namespace Functions
    {
        class Sigmoid : public Functions::Function
        {
        public:
            explicit Sigmoid(double c);

            double F(double p) override;
            double D(double p) override;

        private:
            const double m_c;
        };
    }
}

#endif //PYQUARK_SIGMOID_H
