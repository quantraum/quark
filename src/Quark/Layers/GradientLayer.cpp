#include "Layer.h"
#include "GradientLayer.h"
#include "../Neurons/GradientPerceptron.h"
#include "../Functions/Function.h"

namespace Quark::Layers
{
    GradientLayer::GradientLayer(int units, int inputDim, double learningRate, const std::shared_ptr<Functions::Function>& activation, bool randomize) : Layer(
            units, inputDim, activation)
    {
        for (auto it = m_units.begin(); it < m_units.end(); it++)
            *it = std::make_unique<Neurons::GradientPerceptron>(inputDim, learningRate, randomize, activation);
    }

    GradientLayer::GradientLayer(
            int units,
            int inputDim,
            double learningRate,
            const std::shared_ptr<Functions::Function>& activation,
            std::vector<std::vector<double>> weights
            ) : Layer(units, inputDim, activation)
    {
        for(int i = 0; i < m_units.size(); i++)
            m_units[i] = std::make_unique<Neurons::GradientPerceptron>(inputDim, learningRate, weights[i], activation);
    }
}
