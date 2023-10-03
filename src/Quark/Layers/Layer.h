#ifndef QUARK_LAYER_H
#define QUARK_LAYER_H

#include <iostream>
#include <functional>
#include "../Neurons/Perceptron.h"
#include "../Neurons/IModel.h"

namespace Quark
{
    namespace Layers
    {
    class Layer
        {
        public:
            using Neuron = std::unique_ptr<Neurons::Perceptron<double>>;
            using Samples = const std::vector<std::vector<double>>;
            using Sample = const std::vector<double>;
            using Labels = const std::vector<std::vector<double>>;

            const int units = 0;
            const int inputDim = 0;
            std::vector<Neuron> m_units;
            std::shared_ptr<Functions::Function> m_function;

            Layer(int units, int inputDim, std::shared_ptr<Functions::Function> function) :
                units(units),
                inputDim(inputDim),
                m_function(std::move(function))
            {
                m_units.resize(units);
            }

            void Train(Samples &samples, Labels &labels, const std::function<bool(int, int, double)>& stopFct = [](int epoch, int errorCount, double errorMean) { return epoch >= 1000; })
            {
                for(auto i = 0; i < m_units.size(); i ++)
                {
                    std::vector<double> labelsToSend;
                    for (int j = 0; j < labels.size(); ++j)
                    {
                        labelsToSend.push_back(labels[j][i]);
                    }
                    m_units[i]->Train(samples, labelsToSend, stopFct);
                }
            }

            std::vector<std::vector<double>> GetWeights()
            {
                std::vector<std::vector<double>> toReturn;
                for(auto i = 0; i < m_units.size(); i ++)
                {
                    toReturn.push_back(m_units[i]->GetWeights());
                }
                return toReturn;
            }

            std::tuple<std::vector<double>, std::vector<double>> KernelPredict(Sample &sample)
            {
                std::vector<double> potentials(m_units.size(), 0);
                std::vector<double> predictions(m_units.size(), 0);
                for(int i = 0; i < m_units.size(); i++)
                {
                    potentials[i] = m_units[i]->ComputePotential(sample);
                    predictions[i] = m_units[i]->Activation(potentials[i]);
                }
                return { predictions, potentials };
            }

            std::vector<double> Predict(Sample &sample)
            {
                std::vector<double> predictions;
                for(const auto & m_unit : m_units)
                    predictions.push_back(m_unit->Predict(sample));
                return predictions;
            }

            inline const std::shared_ptr<Functions::Function>& GetActivationFunction() const { return m_function; }

            inline std::vector<std::vector<double>> GetQuadraticMeans() const
            {
                std::vector<std::vector<double>> mses;

                for(const auto& neuron : m_units)
                    mses.emplace_back(neuron->GetQuadraticMeans());

                return mses;
            }
        };
    }
}

#endif //QUARK_LAYER_H
