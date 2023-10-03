#ifndef QUARK_RAND_H
#define QUARK_RAND_H

#include <vector>
#include <random>
#include <functional>

namespace Quark
{
    namespace Utils
    {
        template<typename T, typename D> concept IsDistribution =
        std::is_base_of_v<std::uniform_real_distribution<T>, D> ||
        std::is_base_of_v<std::uniform_int_distribution<T>, D>;

        template<typename T, typename D>
        requires IsDistribution<T, D>
        void RandInitialize(std::vector<T> &vector, D distribution)
        {
            std::random_device dev;
            std::mt19937 rng(dev());

            std::generate(std::begin(vector), std::end(vector), [&distribution, &rng]() -> T
            {
                return distribution(rng);
            });
        }
    }
}
#endif //QUARK_RAND_H
