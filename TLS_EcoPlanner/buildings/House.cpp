#include "House.hpp"

namespace
{

QString const  HouseDescription = "House #%1 [worker lvl: %2]";
unsigned const MaxWorkerLevel   = 2;

} // namespace

House::House()
{
}

bool House::upgradeWorkers()
{
	if (m_level >= MaxWorkerLevel)
		return false;

	++m_level;
	return true;
}

bool House::downgradeWorkers()
{
	if (m_level > 0) {
		--m_level;
		return true;
	}
	else {
		return false;
	}
}

QString House::getDescription(unsigned index) const
{
	return HouseDescription.arg(index).arg(m_level);
}
