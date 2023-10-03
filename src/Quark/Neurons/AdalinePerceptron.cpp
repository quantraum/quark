#include "../Utils/Rand.h"
#include "AdalinePerceptron.h"

#include <random>
#include <cassert>
#include <iostream>

namespace Quark::Neurons
{
    AdalinePerceptron::AdalinePerceptron(size_t inputDim, double learningRate, bool randomize, std::shared_ptr<Functions::Function> activation) :
            Perceptron<double>(inputDim, learningRate, std::move(activation))
    {
        if (randomize)
            Utils::RandInitialize(m_weights, std::uniform_real_distribution<double>(-1, 1));
    }

    AdalinePerceptron::AdalinePerceptron(size_t inputDim, double learningRate, std::vector<double> weights, std::shared_ptr<Functions::Function> activation) :
            Perceptron<double>(inputDim, learningRate, std::move(weights), std::move(activation))
    {

    }

    std::tuple<int, double> AdalinePerceptron::Epoch(Perceptron::Samples &samples, Perceptron::Labels &labels)
    {
        std::fill(m_corrections.begin(), m_corrections.end(), 0);
        int errorCount = 0;
        for (int i = 0; i < samples.size() && i < labels.size(); i++)
        {
            double potential = ComputePotential(samples[i]);
            double prediction = m_activationFct->F(potential);
            errorCount += prediction == labels[i] ? 0: 1;
            UpdateWeights(samples[i], labels[i] - potential);
        }

        return { errorCount, ComputeQuadtraticMean(samples, labels) };
    }

    nlohmann::json Neurons::AdalinePerceptron::Serialize() const
    {
        return Perceptron::Serialize();
    }

    void AdalinePerceptron::Deserialize(const nlohmann::json &document)
    {
        Perceptron::Deserialize(document);
    }

    double AdalinePerceptron::ComputeQuadtraticMean(Perceptron::Samples &samples, Perceptron::Labels &labels)
    {
        double quadraticMean = 0;
        for (int i = 0; i < samples.size() && i < labels.size(); i++)
        {
            double potential = ComputePotential(samples[i]);
            quadraticMean += pow(labels[i] - potential, 2.0);
        }
        quadraticMean /= static_cast<double>((2 * samples.size()));
        return quadraticMean;
    }
}