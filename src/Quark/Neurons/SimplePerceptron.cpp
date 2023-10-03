#include "SimplePerceptron.h"
#include "../Utils/Rand.h"
#include <random>
#include <cassert>
#include <iostream>

namespace Quark::Neurons
{
    SimplePerceptron::SimplePerceptron(size_t inputDim, double learningRate, bool randomize, std::shared_ptr<Functions::Function> activation) :
            Perceptron<int>(inputDim, learningRate, std::move(activation))
    {
        if (randomize)
            Utils::RandInitialize(m_weights, std::uniform_real_distribution<double>(-100, 100));
    }

    SimplePerceptron::SimplePerceptron(size_t inputDim, double learningRate, std::vector<double> weights, std::shared_ptr<Functions::Function> activation) :
            Perceptron<int>(inputDim, learningRate, std::move(weights), std::move(activation))
    {

    }

    std::tuple<int, double> SimplePerceptron::Epoch(Perceptron::Samples &samples, Perceptron::Labels &labels)
    {
        int errorCount = 0;
        for(int i = 0; i < samples.size() && i < labels.size(); i++)
        {
            double prediction = Predict(samples[i]);
            int error = labels[i] - prediction;
            if(error)
            {
                errorCount++;
                UpdateWeights(samples[i], error);
            }
        }

        return std::make_tuple(errorCount, 0);
    }

    void SimplePerceptron::UpdateWeights(Perceptron::Sample & sample, int error)
    {
        m_weights[0] += m_learningRate * error;
        for (int i = 0; i < sample.size(); i++)
        {
            m_weights[i + 1] += m_learningRate * error * sample[i];
        }
    }

    nlohmann::json SimplePerceptron::Serialize() const
    {
        return Perceptron::Serialize();
    }

    void SimplePerceptron::Deserialize(const nlohmann::json &document)
    {
        Perceptron::Deserialize(document);
    }
}