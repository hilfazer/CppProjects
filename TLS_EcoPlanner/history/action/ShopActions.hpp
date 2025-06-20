#pragma once

#include "history/action/HistoryAction.hpp"

class BuildShopAction : public HistoryActionBase
{
public:
	BuildShopAction(Stage& stage);

	// IHistoryAction interface
public:
	void    undo() override;
	int     getCost(PriceData const& priceData, PriceModsFlags modifiers) const override;
	QString getDescription() const override;
	SerializedHistoryAction serialize() const override;

	// HistoryActionBase interface
protected:
	StageError doImpl() override;
	void       postDoInit() override;
};

class UpgradeShopSellingPricesAction : public HistoryActionBase
{
public:
	UpgradeShopSellingPricesAction(Stage& stage);

	// HistoryActionBase interface
public:
	int     getCost(PriceData const& priceData, PriceModsFlags modifiers) const override;
	QString getDescription() const override;
	SerializedHistoryAction serialize() const override;
	void                    undo() override;

protected:
	StageError doImpl() override;
	void       postDoInit() override;

private:
	unsigned m_upgradeLevel = 0;
};
