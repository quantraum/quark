#ifndef PYQUARK_LAYERFACTORY_H
#define PYQUARK_LAYERFACTORY_H

#include <Quark/Layers/GradientLayer.h>

namespace LayerFactory
{
    std::unique_ptr<Quark::Layers::Layer> Create(const std::string& name, int units,
                                                        int inputDim, double learningRate,
                                                        const std::shared_ptr<Quark::Functions::Function>& activation,
                                                        bool randomize)
    {
        if(name == "Adaline")
            return std::make_unique<Quark::Layers::AdalineLayer>(units, inputDim, learningRate, activation, randomize);
        else if(name == "Gradient")
            return std::make_unique<Quark::Layers::GradientLayer>(units, inputDim, learningRate, activation, randomize);

        return nullptr;
    }
}

#endif //PYQUARK_LAYERFACTORY_H
