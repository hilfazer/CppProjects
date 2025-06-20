#include "Shop.hpp"

namespace
{

QString const ShopText = "Shop [prices lvl: %1]";

unsigned const MaxPricesLvl = 2;

} // namespace

Shop::Shop(ShopUpgradesData const& upgrades) : m_upgradesData(upgrades)
{
}

bool Shop::upgradePrices()
{
	if (m_pricesLevel < MaxPricesLvl) {
		++m_pricesLevel;
		return true;
	}
	else {
		return false;
	}
}

bool Shop::downgradePrices()
{
	if (m_pricesLevel > 0) {
		--m_pricesLevel;
		return true;
	}
	else {
		return false;
	}
}

QString Shop::getDescription() const
{
	return ShopText.arg(m_pricesLevel);
}

unsigned Shop::getPricePercentage() const
{
	switch (m_pricesLevel) {
	default:
	case 0:
		return m_upgradesData.percentage0;
	case 1:
		return m_upgradesData.percentage1;
	case 2:
		return m_upgradesData.percentage2;
	}
}
