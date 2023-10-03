#ifndef QUARK_GRADIENTLAYER_H
#define QUARK_GRADIENTLAYER_H

#include "Layer.h"
#include "../Functions/Function.h"

namespace Quark
{
    namespace Layers
    {
        class GradientLayer : public Layer
        {
        public:
            GradientLayer(int units, int inputDim, double learningRate, const std::shared_ptr<Functions::Function>& activation, bool randomize = false);
            GradientLayer(int units, int inputDim, double learningRate, const std::shared_ptr<Functions::Function>& activation, std::vector<std::vector<double>> weights);
        };
    }
}

#endif //QUARK_GRADIENTLAYER_H
