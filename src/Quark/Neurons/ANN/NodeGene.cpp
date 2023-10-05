#include "NodeGene.h"

Quark::Neurons::NodeGene::NodeGene(Quark::Neurons::GeneType type) :
    m_type(type)
{

}

bool Quark::Neurons::NodeGene::operator==(Quark::Neurons::GeneType type) const
{
    return GetType() == type;
}
