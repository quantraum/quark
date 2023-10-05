#ifndef PYQUARK_GENOME_H
#define PYQUARK_GENOME_H

#include "Model.h"
#include "../Neurons/ANN/NodeGene.h"
#include "../Neurons/ANN/ConnectionGene.h"

namespace Quark
{
    namespace Models
    {
        class Genome
        {
        public:
            explicit Genome(std::vector<Neurons::NodeGene> genes, const std::vector<Neurons::ConnectionGene>& connections);
            [[nodiscard]] std::vector<double> Evaluate(const std::vector<double>& inputs) const;
            [[nodiscard]] inline const std::vector<std::vector<Neurons::ConnectionGene>>& GetConnections() const { return m_connections; }
        private:
            std::vector<Quark::Neurons::NodeGene> m_genes;
            std::vector<std::vector<Quark::Neurons::ConnectionGene>> m_connections;

            void Initialize(const std::vector<Neurons::ConnectionGene>& connections);
        };
    }
}


#endif //PYQUARK_GENOME_H
