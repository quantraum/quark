#ifndef QUARK_IPERCEPTRON_H
#define QUARK_IPERCEPTRON_H

#include <cstddef>
#include <string>
#include <utility>
#include <vector>
#include <functional>
#include <algorithm>
#include <iterator>
#include <tuple>
#include "IModel.h"
#include "nlohmann/json.hpp"
#include "../Functions/Function.h"
#include "../Functions/Linear.h"

namespace Quark
{
    namespace Neurons
    {
        template<typename T, typename = typename std::enable_if<std::is_same<T, int>::value || std::is_same<T, double>::value>::type>
        class Perceptron : public IModel
        {
        public:

            using Samples = const std::vector<std::vector<T>>;
            using Sample = const std::vector<T>;
            using Labels = const std::vector<T>;

            Perceptron() = default;

            Perceptron(size_t inputDim, double learningRate, std::shared_ptr<Functions::Function> activation):
                m_inputDim(inputDim),
                m_learningRate(learningRate),
                m_weights(inputDim + 1, 0),
                m_activationFct(std::move(activation))
            {

            }

            Perceptron(size_t inputDim, double learningRate, std::vector<double> weights, std::shared_ptr<Functions::Function> activation):
                    m_inputDim(inputDim),
                    m_learningRate(learningRate),
                    m_weights(std::move(weights)),
                    m_activationFct(std::move(activation))
            {

            }

            ~Perceptron() override = default;

            std::vector<T> Predict(Samples &samples)
            {
                std::vector<T> predictions;
                for(const Sample &s : samples)
                    predictions.push_back(Predict(s));
                return predictions;
            }

            T Predict(Sample &sample) { return Activation(ComputePotential(sample)); }

            T Activation(double potential) { return m_activationFct->F(potential); }

            int Train(Samples &samples, Labels &labels, const std::function<bool(int, int, double)>& stopFct = [](int epoch, int errorCount, double errorMean){ return epoch >= 1000; })
            {
                assert(samples.size() == labels.size() && "Expectations and Samples sizes must be equal.");
                int epoch = 0;
                m_quadraticMeans.clear();
                while(true)
                {
                    epoch++;
                    auto error = Epoch(samples, labels);
                    auto errorCount = std::get<0>(error);
                    auto errorMean = std::get<1>(error);

                    m_quadraticMeans.emplace_back(errorMean);

                    if(stopFct(epoch, errorCount, errorMean))
                        break;
                }
                return epoch;
            }

            inline std::vector<double> &GetWeights() const { return m_weights; }

            double ComputePotential(Sample &sample)
            {
                double potential = m_weights[0];
                for(int i = 0; i < sample.size(); i++)
                    potential += sample[i] * m_weights[i+1];
                return potential;
            }

            nlohmann::json Serialize() const override
            {
                nlohmann::json document;
                document["weights"] = m_weights;
                document["inputDim"] = m_inputDim;
                document["learningRate"] = m_learningRate;

                return document;
            }

            void Deserialize(const nlohmann::json& document) override
            {
                m_inputDim = document["inputDim"];
                m_learningRate = document["learningRate"];
                m_weights = document["weights"].get<std::vector<double>>();
            }

            void UpdateWeights(Sample &sample, double error)
            {
                m_weights[0] += m_learningRate * error * 1;
                for (int j = 0; j < sample.size(); j++)
                    m_weights[j + 1] += m_learningRate * error * sample[j];
            }

            inline std::vector<double> GetQuadraticMeans() const { return m_quadraticMeans; }

        protected:
            size_t m_inputDim{};
            double m_learningRate{};
            mutable std::vector<double> m_weights;
            std::shared_ptr<Functions::Function> m_activationFct;
            std::vector<double> m_quadraticMeans;

            virtual std::tuple<int, double> Epoch(Samples &samples, Labels &labels) = 0;
        };
    }
}

#endif //QUARK_IPERCEPTRON_H
