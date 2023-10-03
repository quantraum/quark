#include "../Utils/Rand.h"
#include "IterativeFeedForwardModel.h"
#include "FitOptions.h"

namespace Quark::Models
{
    IterativeFeedForwardModel::IterativeFeedForwardModel(std::vector<Layer> layers)
    {
        m_layers = std::move(layers);
    }

    void IterativeFeedForwardModel::AddLayer(Layer layer)
    {
        m_layers.push_back(std::move(layer));
    }

    void IterativeFeedForwardModel::Fit(Model::Samples &samples, Model::Labels &labels, const FitOptions& options)
    {
        for(int i = 0; i < options.maxIterations; i++)
        {
            m_quadraticMeans.push_back(0);
            for(int j = 0; j < samples.size(); j++)
            {
                m_errorSignals.resize(m_layers.size());
                for(auto& v : m_errorSignals)
                    v.clear();
                ErrorBackPropagation(samples[j], labels[j], i);
                WeightsCorrection(samples[j]);
            }
            m_quadraticMeans[i] /= 2.0 * static_cast<double>(samples.size());
            if(m_quadraticMeans[i] < options.minQuadraticMean) return;
        }
    }

    void IterativeFeedForwardModel::ErrorBackPropagation(Model::Sample &sample, Model::Label &label, int currentIteration)
    {
        std::vector<std::vector<double>> predictions(m_layers.size());
        std::vector<std::vector<double>> potentials(m_layers.size());

        for(int i = 0; i < m_layers.size(); i++)
        {
            std::tie(predictions[i], potentials[i]) = (i == 0) ? m_layers[i]->KernelPredict(sample) : m_layers[i]->KernelPredict(predictions[i - 1]);
            if(i == m_layers.size() - 1)
                for(int j = 0; j < predictions[i].size() && j < label.size(); j++)
                    m_quadraticMeans[currentIteration] += pow(label[j] - predictions[i][j], 2.0);
        }

        for(int i = m_layers.size() - 1; i >= 0; i--)
            m_errorSignals[i] = std::move(ComputeErrorSignals(predictions[i], potentials[i], label, i));
    }

    std::vector<double> IterativeFeedForwardModel::ComputeErrorSignals(std::vector<double> predictions, std::vector<double> potentials, Model::Label &label, int currentLayer)
    {
        std::vector<double> errorSignals(m_layers[currentLayer]->units);
        for(int i = 0; i < predictions.size(); i++)
            errorSignals[i] = (currentLayer == m_layers.size() - 1) ?
                              (label[i] - predictions[i]) * m_layers[currentLayer]->GetActivationFunction()->D(potentials[i]) : // Output layer error signal computation
                              m_layers[currentLayer]->GetActivationFunction()->D(potentials[i]) * ComputeErrorSignalPotential(m_errorSignals[currentLayer + 1], i, currentLayer); // Deep layer error signal computation
        return errorSignals;
    }


    double IterativeFeedForwardModel::ComputeErrorSignalPotential(std::vector<double> & successorErrorSignals, int currentNeuron, int currentLayer)
    {
        double potential = 0;
        for(int i = 0; i < m_layers[currentLayer + 1]->m_units.size(); i++)
            potential += successorErrorSignals[i] * m_layers[currentLayer + 1]->m_units[i]->GetWeights()[currentNeuron + 1];
        return potential;
    }

    void IterativeFeedForwardModel::WeightsCorrection(Model::Sample &sample)
    {
        std::vector<std::vector<double>> predictions(m_layers.size());
        for(int i = 0; i < m_layers.size(); i++)
            (i == 0) ? predictions[i] = m_layers[i]->Predict(sample) : predictions[i] = m_layers[i]->Predict(predictions[i - 1]);

        for(int i = 0; i < m_layers.size(); i++)
            for(int j = 0; j < m_layers[i]->m_units.size(); j++)
                (i == 0) ? m_layers[i]->m_units[j]->UpdateWeights(sample, m_errorSignals[i][j]) : m_layers[i]->m_units[j]->UpdateWeights(predictions[i - 1], m_errorSignals[i][j]);
    }


    std::vector<double> IterativeFeedForwardModel::Predict(Model::Sample &sample)
    {
        std::vector<double> currentInput = sample;
        for(const auto & m_layer : m_layers)
            currentInput = m_layer->Predict(currentInput);
        return currentInput;
    }

    std::vector<double> IterativeFeedForwardModel::GetQuadraticMeans()
    {
        return m_quadraticMeans;
    }
}
