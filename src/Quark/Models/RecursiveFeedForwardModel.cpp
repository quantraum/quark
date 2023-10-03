#include "../Utils/Rand.h"
#include "RecursiveFeedForwardModel.h"
#include "FitOptions.h"

namespace Quark::Models
{
    RecursiveFeedForwardModel::RecursiveFeedForwardModel(std::vector<Layer> layers)
    {
        m_layers = std::move(layers);
    }

    void RecursiveFeedForwardModel::AddLayer(Layer layer)
    {
        m_layers.push_back(std::move(layer));
    }

    void RecursiveFeedForwardModel::Fit(Model::Samples &samples, Model::Labels &labels, const FitOptions& options)
    {
        for(int i = 0; i < options.maxIterations; i++)
        {
            m_quadraticMeans.push_back(0);
            for(int j = 0; j < samples.size(); j++)
            {
                m_errorSignals.resize(m_layers.size());
                for(auto& v : m_errorSignals)
                    v.clear();
                RecursiveErrorBackPropagation(samples[j], labels[j], i);
                RecursiveWeightsCorrection(samples[j]);
            }
            m_quadraticMeans[i] /= 2.0 * static_cast<double>(samples.size());
            if(m_quadraticMeans[i] < options.minQuadraticMean) return;
        }
    }


    void RecursiveFeedForwardModel::RecursiveErrorBackPropagation(Model::Sample &sample, Model::Label &label, int currentIteration, int currentLayer)
    {
        // Data Propagation
        auto [predictions, potentials] = m_layers[currentLayer]->KernelPredict(sample);
        if(currentLayer != m_layers.size() - 1) // If we are in a deep layer, propagate forward
            RecursiveErrorBackPropagation(predictions, label, currentIteration, currentLayer + 1);
        else  // If we are at the output layer, compute quadratic mean
            for(int i = 0; i < predictions.size() && i < label.size(); i++)
                m_quadraticMeans[currentIteration] += pow(label[i] - predictions[i], 2.0);

        // Error Signal Computation
        m_errorSignals[currentLayer] = ComputeErrorSignals(predictions, potentials, label, currentLayer);
    }

    std::vector<double> RecursiveFeedForwardModel::ComputeErrorSignals(std::vector<double> predictions, std::vector<double> potentials, Model::Label &label, int currentLayer)
    {
        std::vector<double> errorSignals(m_layers[currentLayer]->units);
        for(int i = 0; i < predictions.size(); i++)
            errorSignals[i] = (currentLayer == m_layers.size() - 1) ?
                              (label[i] - predictions[i]) * m_layers[currentLayer]->GetActivationFunction()->D(potentials[i]) : // Output layer error signal computation
                              m_layers[currentLayer]->GetActivationFunction()->D(potentials[i]) * ComputeErrorSignalPotential(m_errorSignals[currentLayer + 1], i, currentLayer); // Deep layer error signal computation
        return errorSignals;
    }


    double RecursiveFeedForwardModel::ComputeErrorSignalPotential(std::vector<double> & successorErrorSignals, int currentNeuron, int currentLayer)
    {
        double potential = 0;
        for(int i = 0; i < m_layers[currentLayer + 1]->m_units.size(); i++)
            potential += successorErrorSignals[i] * m_layers[currentLayer + 1]->m_units[i]->GetWeights()[currentNeuron + 1];
        return potential;
    }

    void RecursiveFeedForwardModel::RecursiveWeightsCorrection(Model::Sample &sample, int currentLayer)
    {
        if(currentLayer != m_layers.size() - 1)
            RecursiveWeightsCorrection(m_layers[currentLayer]->Predict(sample), currentLayer + 1);
        for(int i = 0; i < m_layers[currentLayer]->m_units.size(); i++)
            m_layers[currentLayer]->m_units[i]->UpdateWeights(sample, m_errorSignals[currentLayer][i]);
    }


    std::vector<double> RecursiveFeedForwardModel::Predict(Model::Sample &sample)
    {
        std::vector<double> currentInput = sample;
        for(const auto & m_layer : m_layers)
            currentInput = m_layer->Predict(currentInput);
        return currentInput;
    }

    std::vector<double> RecursiveFeedForwardModel::GetQuadraticMeans()
    {
        return m_quadraticMeans;
    }
}
