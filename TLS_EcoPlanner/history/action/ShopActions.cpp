#include "ShopActions.hpp"

namespace
{

QString const ShopBuiltText          = "Shop built";
QString const ShopPricesUpgradedText = "Shop prices upgraded to lvl %1";

} // namespace

BuildShopAction::BuildShopAction(Stage& stage) : HistoryActionBase(stage)
{
}

StageError BuildShopAction::doImpl()
{
	return m_stage.buildShop();
}

void BuildShopAction::undo()
{
	m_stage.undoBuildShop();
}

void BuildShopAction::postDoInit()
{
	// empty
}

int BuildShopAction::getCost(PriceData const& priceData, PriceModsFlags modifiers) const
{
	return applyModifiers(priceData.shop.build, priceData, modifiers);
}

QString BuildShopAction::getDescription() const
{
	return ShopBuiltText;
}

SerializedHistoryAction BuildShopAction::serialize() const
{
	return { ActionElement::BUILD_SHOP, {} };
}

//---------------------------------------------------------------------------

UpgradeShopSellingPricesAction::UpgradeShopSellingPricesAction(Stage& stage) :
        HistoryActionBase(stage)
{
}

StageError UpgradeShopSellingPricesAction::doImpl()
{
	return m_stage.upgradeShopSellingPrices();
}

void UpgradeShopSellingPricesAction::undo()
{
	m_stage.downgradeShopSellingPrices();
}

void UpgradeShopSellingPricesAction::postDoInit()
{
	m_upgradeLevel = m_stage.getShop()->getPriceLevel();
}

int UpgradeShopSellingPricesAction::getCost(
        PriceData const& priceData, PriceModsFlags modifiers) const
{
    assert(m_upgradeLevel > 0 && m_upgradeLevel <= 2);

	unsigned cost = m_upgradeLevel == 1 ? priceData.shop.upgrade1 : priceData.shop.upgrade2;
	return applyModifiers(cost, priceData, modifiers);
}

QString UpgradeShopSellingPricesAction::getDescription() const
{
	return ShopPricesUpgradedText.arg(m_upgradeLevel);
}

SerializedHistoryAction UpgradeShopSellingPricesAction::serialize() const
{
	return { ActionElement::UPGRADE_SHOP, {} };
}
