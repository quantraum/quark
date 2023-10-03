#ifndef PYQUARK_FUNCTION_H
#define PYQUARK_FUNCTION_H

#include <memory>

namespace Quark
{
    namespace Functions
    {
        class Function : public std::enable_shared_from_this<Function>
        {
        public:
            virtual ~Function() = default;
            virtual double F(double p) = 0;
            virtual double D(double p) = 0;
        };
    }
}

#endif //PYQUARK_FUNCTION_H
