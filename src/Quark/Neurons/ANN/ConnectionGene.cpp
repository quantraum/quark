#include "ConnectionGene.h"

Quark::Neurons::ConnectionGene::ConnectionGene(unsigned int from, unsigned int to, double weight) :
    m_from(from),
    m_to(to),
    m_weight(weight)
{

}

Quark::Neurons::ConnectionGene::ConnectionGene() :
    m_from(0),
    m_to(0),
    m_weight(0.0)
{

}

bool Quark::Neurons::ConnectionGene::operator==(const Quark::Neurons::ConnectionGene &other)
{
    return m_from == other.m_from && m_to == other.m_to && m_weight == other.m_weight;
}
