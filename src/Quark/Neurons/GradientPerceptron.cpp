#include "../Utils/Rand.h"
#include "GradientPerceptron.h"
#include <random>
#include <cassert>
#include <iostream>
#include <utility>

namespace Quark::Neurons
{
    GradientPerceptron::GradientPerceptron(size_t inputDim, double learningRate, bool randomize, std::shared_ptr<Functions::Function> activation) :
            Perceptron<double>(inputDim, learningRate, std::move(activation)),
            m_corrections(inputDim + 1, 0)
    {
        if (randomize)
            Utils::RandInitialize(m_weights, std::uniform_real_distribution<double>(-1, 1));
    }

    GradientPerceptron::GradientPerceptron(size_t inputDim, double learningRate, std::vector<double> weights, std::shared_ptr<Functions::Function> activation) :
            Perceptron<double>(inputDim, learningRate, std::move(weights), std::move(activation)),
            m_corrections(inputDim + 1, 0)
    {

    }

    std::tuple<int, double> GradientPerceptron::Epoch(Perceptron::Samples &samples, Perceptron::Labels &labels)
    {
        std::fill(m_corrections.begin(), m_corrections.end(), 0);
        int errorCount = 0;
        double quadraticMean = 0;
        for (int i = 0; i < samples.size() && i < labels.size(); i++)
        {
            double potential = ComputePotential(samples[i]);
            double prediction = m_activationFct->F(potential);
            errorCount += prediction == labels[i] ? 0: 1;
            quadraticMean += pow(labels[i] - potential, 2.0);
            UpdateCorrections(samples[i], labels[i] - potential);
        }

        UpdateWeights();

        quadraticMean /= static_cast<double>((2 * samples.size()));

        return { errorCount, quadraticMean };
    }

    void GradientPerceptron::UpdateCorrections(Sample &sample, double error)
    {
        m_corrections[0] += m_learningRate * error * 1;
        for (int j = 0; j < sample.size(); j++)
        {
            m_corrections[j + 1] += m_learningRate * error * sample[j];
        }
    }

    void GradientPerceptron::UpdateWeights()
    {
        for (int i = 0; i < m_weights.size(); i++)
        {
            m_weights[i] += m_corrections[i];
        }
    }

    nlohmann::json GradientPerceptron::Serialize() const
    {
        return Perceptron::Serialize();
    }

    void GradientPerceptron::Deserialize(const nlohmann::json &document)
    {
        Perceptron::Deserialize(document);
    }
}