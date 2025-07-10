#include <QFile>
#include <QXmlStreamWriter>

#include "game_data/GameData.hpp"
#include "game_data/GameDataXml.hpp"
#include "source/FileOperations.hpp"
#include "source/Misc.hpp"

namespace GameDataXml
{

namespace
{

// TODO: write omens

void writeHousePriceData(HousePriceData const& data, QXmlStreamWriter& xmlWriter);
void writeMinePriceData(MinePriceData const& data, QXmlStreamWriter& xmlWriter);
void writeShopPriceData(ShopPriceData const& data, QXmlStreamWriter& xmlWriter);
void writeMaxBuildingsData(MaxBuildingsData const& data, QXmlStreamWriter& xmlWriter);
void writeStartingBuildingsData(StartingBuildingsData const& data, QXmlStreamWriter& xmlWriter);
void writeGoldMineData(GoldMineUpgrades const& data, QXmlStreamWriter& xmlWriter);
void writeShopData(ShopUpgradesData const& data, QXmlStreamWriter& xmlWriter);
void writeRuinsData(RuinsData const data, QXmlStreamWriter& xmlWriter);
void writeNightRewardData(NightRewardData const& data, QXmlStreamWriter& xmlWriter);

} // namespace

StringError writeGameDataToXml(GameData const& data, QString const& fileName)
{
	QFile file(fileName);

	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		return CouldNotOpenFileForWriting.arg(fileName);
	}

	QXmlStreamWriter xmlWriter(&file);
	xmlWriter.setAutoFormatting(true);

	xmlWriter.writeStartDocument();
	{
		xmlWriter.writeStartElement(Elements.Data.game);
		{
			// TODO write omens

			xmlWriter.writeStartElement(Elements.Data.price);
			{
				xmlWriter.writeTextElement(
				        Elements.Price.easyMod,
				        QString::number(static_cast<double>(data.prices.easyDifficultyModifier)));
				xmlWriter.writeTextElement(
				        Elements.Price.apocMod,
				        QString::number(static_cast<double>(data.prices.apocDifficultyModifier)));
				writeHousePriceData(data.prices.house, xmlWriter);
				writeMinePriceData(data.prices.mine, xmlWriter);
				writeShopPriceData(data.prices.shop, xmlWriter);
			}
			xmlWriter.writeEndElement();

			writeMaxBuildingsData(data.maxBuildings, xmlWriter);
			writeStartingBuildingsData(data.startingBuildings, xmlWriter);
			writeGoldMineData(data.goldMineUpgrades, xmlWriter);
			writeShopData(data.shopUpgrades, xmlWriter);
			writeRuinsData(data.ruinsValues, xmlWriter);
			writeNightRewardData(data.night, xmlWriter);
		}
		xmlWriter.writeEndElement();
	}
	xmlWriter.writeEndDocument();
	file.close();
	assert(!validateXmlFile(fileName, GameDataSchemaFilename));
	return {};
}

namespace
{

void writeHousePriceData(HousePriceData const& data, QXmlStreamWriter& xmlWriter)
{
	xmlWriter.writeStartElement(Elements.Price.house);
	{
		xmlWriter.writeTextElement(Elements.Building.build, QString::number(data.build));
		xmlWriter.writeStartElement(Elements.Upgrade.expansion);
		xmlWriter.writeAttribute(Attributes.level(1), QString::number(data.upgrade1));
		xmlWriter.writeAttribute(Attributes.level(2), QString::number(data.upgrade2));
		xmlWriter.writeEndElement();
	}
	xmlWriter.writeEndElement();
}

void writeMinePriceData(MinePriceData const& data, QXmlStreamWriter& xmlWriter)
{
	xmlWriter.writeStartElement(Elements.Price.mine);
	{
		xmlWriter.writeTextElement(Elements.Building.build, QString::number(data.build));

		xmlWriter.writeStartElement(Elements.Upgrade.production);
		xmlWriter.writeAttribute(Attributes.level(1), QString::number(data.productionUpgrades[0]));
		xmlWriter.writeAttribute(Attributes.level(2), QString::number(data.productionUpgrades[1]));
		xmlWriter.writeAttribute(Attributes.level(3), QString::number(data.productionUpgrades[2]));
		xmlWriter.writeEndElement();

		xmlWriter.writeStartElement(Elements.Upgrade.workers);
		xmlWriter.writeAttribute(Attributes.level(1), QString::number(data.workerUpgrades[0]));
		xmlWriter.writeAttribute(Attributes.level(2), QString::number(data.workerUpgrades[1]));
		xmlWriter.writeEndElement();
	}
	xmlWriter.writeEndElement();
}

void writeShopPriceData(ShopPriceData const& data, QXmlStreamWriter& xmlWriter)
{
	xmlWriter.writeStartElement(Elements.Price.shop);
	{
		xmlWriter.writeTextElement(Elements.Building.build, QString::number(data.build));
		xmlWriter.writeStartElement(Elements.Upgrade.itemSell);
		{
			xmlWriter.writeAttribute(Attributes.level(1), QString::number(data.upgrade1));
			xmlWriter.writeAttribute(Attributes.level(2), QString::number(data.upgrade2));
		}
		xmlWriter.writeEndElement();
	}
	xmlWriter.writeEndElement();
}

void writeMaxBuildingsData(MaxBuildingsData const& data, QXmlStreamWriter& xmlWriter)
{
	xmlWriter.writeStartElement(Elements.Building.maximum);
	{
		xmlWriter.writeTextElement(Elements.Building.houses, QString::number(data.house));
		xmlWriter.writeTextElement(Elements.Building.mines, QString::number(data.mine));
	}
	xmlWriter.writeEndElement();
}

void writeStartingBuildingsData(StartingBuildingsData const& data, QXmlStreamWriter& xmlWriter)
{
	xmlWriter.writeStartElement(Elements.Building.starting);
	{
		xmlWriter.writeTextElement(Elements.Building.houses, QString::number(data.house));
		xmlWriter.writeTextElement(Elements.Building.mines, QString::number(data.mine));
		xmlWriter.writeTextElement(Elements.Building.shop, QString::number(data.shop));
		xmlWriter.writeTextElement(Elements.Building.ruins1, QString::number(data.ruins1));
		xmlWriter.writeTextElement(Elements.Building.ruins2, QString::number(data.ruins2));
		xmlWriter.writeTextElement(Elements.Building.ruins3, QString::number(data.ruins3));
		xmlWriter.writeTextElement(Elements.Building.ruins4, QString::number(data.ruins4));
	}
	xmlWriter.writeEndElement();
}

void writeGoldMineData(GoldMineUpgrades const& data, QXmlStreamWriter& xmlWriter)
{
	using P = GoldMineParam;

	xmlWriter.writeStartElement(Elements.Data.mine);

	for (unsigned level = 0; level < data.size(); ++level) {
		xmlWriter.writeStartElement(Elements.level(level));

		if (data[level].find(P::PRODUCTION) != data[level].end()) {
			xmlWriter.writeAttribute(
			        Attributes.Upgrade.production, QString::number(data[level].at(P::PRODUCTION)));
		}
		if (data[level].find(P::GOLD) != data[level].end()) {
			xmlWriter.writeAttribute(
			        Attributes.Upgrade.gold, QString::number(data[level].at(P::GOLD)));
		}
		if (data[level].find(P::WORKER_COST) != data[level].end()) {
			xmlWriter.writeAttribute(
			        Attributes.Upgrade.workers, QString::number(data[level].at(P::WORKER_COST)));
		}
		if (data[level].find(P::USES) != data[level].end()) {
			xmlWriter.writeAttribute(
			        Attributes.Upgrade.uses, QString::number(data[level].at(P::USES)));
		}

		xmlWriter.writeEndElement();
	}

	xmlWriter.writeEndElement();
}

void writeShopData(ShopUpgradesData const& data, QXmlStreamWriter& xmlWriter)
{
	xmlWriter.writeStartElement(Elements.Data.shop);
	{
		xmlWriter.writeStartElement(Elements.Upgrade.itemSell);
		xmlWriter.writeAttribute(Attributes.level(0), QString::number(data.percentage0));
		xmlWriter.writeAttribute(Attributes.level(1), QString::number(data.percentage1));
		xmlWriter.writeAttribute(Attributes.level(2), QString::number(data.percentage2));
		xmlWriter.writeEndElement();
	}
	xmlWriter.writeEndElement();
}

void writeRuinsData(RuinsData const data, QXmlStreamWriter& xmlWriter)
{
	xmlWriter.writeStartElement(Elements.Data.ruins);
	{
		xmlWriter.writeTextElement(Elements.Building.ruins1, QString::number(data.size1gold));
		xmlWriter.writeTextElement(Elements.Building.ruins2, QString::number(data.size2gold));
		xmlWriter.writeTextElement(Elements.Building.ruins3, QString::number(data.size3gold));
		xmlWriter.writeTextElement(Elements.Building.ruins4, QString::number(data.size4gold));
	}
	xmlWriter.writeEndElement();
}

void writeNightRewardData(NightRewardData const& data, QXmlStreamWriter& xmlWriter)
{
	xmlWriter.writeStartElement(Elements.Night.rewards);
	{
		xmlWriter.writeStartElement(Elements.Night.gold);
		for (auto const& night : data.panicRewards) {
			xmlWriter.writeEmptyElement(Elements.Night.night);
			for (auto const& rankValue : night)
				xmlWriter.writeAttribute(
				        QString::fromStdString(rankToString(rankValue.first)),
				        QString::number(rankValue.second));
		}
		xmlWriter.writeEndElement();

		xmlWriter.writeStartElement(Elements.Night.itemValues);
		xmlWriter.writeAttribute(
		        Attributes.minRank, QString::fromStdString(rankToString(data.minimumRankForItem)));
		for (auto const& value : data.itemValues) {
			xmlWriter.writeStartElement(Elements.Night.item);
			xmlWriter.writeAttribute(Attributes.value, QString::number(value));
			xmlWriter.writeEndElement();
		}
		xmlWriter.writeEndElement();
	}
	xmlWriter.writeEndElement();
}

} // namespace

} // namespace GameDataXml
