#include "Ruins.hpp"

namespace
{

QString const RuinsDescription = "Ruins (size %1) #%2";

}

Ruins::Ruins(unsigned size) : m_size(size)
{
	assert(size >= 1u && size <= 4);
}

QString Ruins::getDescription(unsigned index) const
{
	return RuinsDescription.arg(m_size).arg(index);
}
