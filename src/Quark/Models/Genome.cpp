#include "Genome.h"
#include "../Functions/Sigmoid.h"

Quark::Models::Genome::Genome(std::vector<Neurons::NodeGene> genes, const std::vector<Neurons::ConnectionGene>& connections) :
    m_genes(std::move(genes)),
    m_connections()
{
    for (auto i = 0; i < connections.size(); i ++)
        m_connections.emplace_back();

    Initialize(connections);
}

void Quark::Models::Genome::Initialize(const std::vector<Neurons::ConnectionGene>& connections)
{
    for (auto& connection : connections)
    {
        m_connections[connection.GetTo() - 1].emplace_back(connection);
    }
}

std::vector<double> Quark::Models::Genome::Evaluate(const std::vector<double> &inputs) const
{
    Functions::Sigmoid sigmoid(4.9);
    std::vector<double> outputs(m_connections.size());
    std::vector<double> finalOutputs;
    for (auto i = 0; i < m_connections.size(); i ++)
        outputs.emplace_back(0.0);

    for (auto i = 0; i < m_connections.size(); i ++)
    {
        if (m_genes[i] == Neurons::GeneType::INPUT)
        {
            outputs[i] = inputs[i];
        }
        else
        {
            for (const auto& connection : m_connections[i])
                outputs[i] += outputs[connection.GetFrom() - 1] * connection.GetWeight();

            outputs[i] = sigmoid.F(outputs[i]);
        }

        if (m_genes[i] == Neurons::GeneType::OUTPUT)
            finalOutputs.emplace_back(outputs[i]);
    }

    return finalOutputs;
}
