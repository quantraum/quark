#ifndef QUARK_ADALINELAYER_H
#define QUARK_ADALINELAYER_H

#include "Layer.h"
#include "../Functions/Function.h"

namespace Quark
{
    namespace Layers
    {
        class AdalineLayer : public Layer
        {
        public:
            AdalineLayer(int units, int inputDim, double learningRate, const std::shared_ptr<Functions::Function>& activation, bool randomize = false);
            AdalineLayer(int units, int inputDim, double learningRate, const std::shared_ptr<Functions::Function>& activation, std::vector<std::vector<double>> weights);
        };
    }
}

#endif //QUARK_ADALINELAYER_H
