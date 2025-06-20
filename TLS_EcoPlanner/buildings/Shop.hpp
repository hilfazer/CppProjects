#pragma once

#include <game_data/GameData.hpp>

#include <QString>

class Shop
{
public:
	static unsigned const DefaultPercentage = 30;

public:
	Shop(ShopUpgradesData const& upgrades);

	bool upgradePrices();
	bool downgradePrices();

	QString  getDescription() const;
	unsigned getPricePercentage() const;
	unsigned getPriceLevel() const { return m_pricesLevel; }

private:
	unsigned         m_pricesLevel = 0;
	ShopUpgradesData m_upgradesData;
};
