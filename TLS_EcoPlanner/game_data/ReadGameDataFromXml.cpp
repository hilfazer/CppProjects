#include "game_data/GameData.hpp"
#include "game_data/GameDataXml.hpp"
#include "source/FileOperations.hpp"

#include <QByteArray>
#include <QFile>
#include <QString>
#include <QXmlStreamReader>

#include <array>

extern GameData createDefaultGameData();

namespace GameDataXml
{

namespace
{

HousePriceData        readHousePriceData(QXmlStreamReader& xmlReader);
MinePriceData         readMinePriceData(QXmlStreamReader& xmlReader);
ShopPriceData         readShopPriceData(QXmlStreamReader& xmlReader);
MaxBuildingsData      readMaxBuildingsData(QXmlStreamReader& xmlReader);
StartingBuildingsData readStartingBuildingsData(QXmlStreamReader& xmlReader);
GoldMineUpgrades      readGoldMineData(QXmlStreamReader& xmlReader);
ShopUpgradesData      readShopData(QXmlStreamReader& xmlReader);
RuinsData             readRuinsData(QXmlStreamReader& xmlReader);
NightRewardData       readNightRewardData(QXmlStreamReader& xmlReader);
void                  assignUIntAttribute(
                         unsigned& target, QXmlStreamAttributes const& attributes, QString const& name);

} // namespace

GameData readGameDataFromXml(QString const& filename, QString& error)
{
	auto optionalError = validateXmlFile(filename, GameDataSchemaFilename);
	if (optionalError) {
		error = *optionalError;
		return {};
	}

	QFile file(filename);

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		error = CouldNotOpenFileForReading.arg(filename);
		return {};
	}

	QXmlStreamReader xmlReader(&file);
	GameData         gameData = createDefaultGameData();

	while (!xmlReader.atEnd()) {
		xmlReader.readNext();
		if (xmlReader.isStartElement()) {
			if (xmlReader.qualifiedName() == Elements.Price.house) {
				gameData.prices.house = readHousePriceData(xmlReader);
			}
			else if (xmlReader.qualifiedName() == Elements.Price.mine) {
				gameData.prices.mine = readMinePriceData(xmlReader);
			}
			else if (xmlReader.qualifiedName() == Elements.Price.shop) {
				gameData.prices.shop = readShopPriceData(xmlReader);
			}
			else if (xmlReader.qualifiedName() == Elements.Building.maximum) {
				gameData.maxBuildings = readMaxBuildingsData(xmlReader);
			}
			else if (xmlReader.qualifiedName() == Elements.Building.starting) {
				gameData.startingBuildings = readStartingBuildingsData(xmlReader);
			}
			else if (xmlReader.qualifiedName() == Elements.Night.rewards) {
				gameData.night = readNightRewardData(xmlReader);
			}
			else if (xmlReader.qualifiedName() == Elements.Price.easyMod) {
				gameData.prices.easyDifficultyModifier = xmlReader.readElementText().toFloat();
			}
			else if (xmlReader.qualifiedName() == Elements.Price.apocMod) {
				gameData.prices.apocDifficultyModifier = xmlReader.readElementText().toFloat();
			}
			else if (xmlReader.qualifiedName() == Elements.Data.mine) {
				gameData.goldMineUpgrades = readGoldMineData(xmlReader);
			}
			else if (xmlReader.qualifiedName() == Elements.Data.shop) {
				gameData.shopUpgrades = readShopData(xmlReader);
			}
			else if (xmlReader.qualifiedName() == Elements.Data.shop) {
				gameData.ruinsValues = readRuinsData(xmlReader);
			}
		}
	}
	if (xmlReader.hasError()) {
		error = xmlReader.error();
		return {};
	}

	file.close();
	return gameData;
}

namespace
{

HousePriceData readHousePriceData(QXmlStreamReader& xmlReader)
{
	assert(xmlReader.qualifiedName() == Elements.Price.house);

	HousePriceData data;
	while (!xmlReader.atEnd()) {
		xmlReader.readNext();
		if (xmlReader.isEndElement() && xmlReader.qualifiedName() == Elements.Price.house) {
			break;
		}
		else if (
		        xmlReader.isStartElement() &&
		        xmlReader.qualifiedName() == Elements.Building.build) {
			data.build = xmlReader.readElementText().toUInt();
		}
		else if (
		        xmlReader.isStartElement() &&
		        xmlReader.qualifiedName() == Elements.Upgrade.expansion) {
			auto attrs    = xmlReader.attributes();
			data.upgrade1 = attrs.value(Attributes.level(1)).toUInt();
			data.upgrade2 = attrs.value(Attributes.level(2)).toUInt();
		}
	}

	return data;
}

MinePriceData readMinePriceData(QXmlStreamReader& xmlReader)
{
	assert(xmlReader.qualifiedName() == Elements.Price.mine);

	MinePriceData data;
	while (!xmlReader.atEnd()) {
		xmlReader.readNext();
		if (xmlReader.isEndElement() && xmlReader.qualifiedName() == Elements.Price.mine) {
			break;
		}
		else if (
		        xmlReader.isStartElement() &&
		        xmlReader.qualifiedName() == Elements.Building.build) {
			data.build = xmlReader.readElementText().toUInt();
		}
		else if (
		        xmlReader.isStartElement() &&
		        xmlReader.qualifiedName() == Elements.Upgrade.production) {
			auto attrs = xmlReader.attributes();
			data.productionUpgrades.resize(0);
			data.productionUpgrades.push_back(attrs.value(Attributes.level(1)).toUInt());
			data.productionUpgrades.push_back(attrs.value(Attributes.level(2)).toUInt());
			data.productionUpgrades.push_back(attrs.value(Attributes.level(3)).toUInt());
		}
		else if (
		        xmlReader.isStartElement() &&
		        xmlReader.qualifiedName() == Elements.Upgrade.workers) {
			auto attrs = xmlReader.attributes();
			data.workerUpgrades.resize(0);
			data.workerUpgrades.push_back(attrs.value(Attributes.level(1)).toUInt());
			data.workerUpgrades.push_back(attrs.value(Attributes.level(2)).toUInt());
		}
	}

	return data;
}

ShopPriceData readShopPriceData(QXmlStreamReader& xmlReader)
{
	assert(xmlReader.qualifiedName() == Elements.Price.shop);

	ShopPriceData data;
	while (!xmlReader.atEnd()) {
		xmlReader.readNext();
		if (xmlReader.isEndElement() && xmlReader.qualifiedName() == Elements.Price.shop) {
			break;
		}
		else if (
		        xmlReader.isStartElement() &&
		        xmlReader.qualifiedName() == Elements.Building.build) {
			data.build = xmlReader.readElementText().toUInt();
		}
		else if (
		        xmlReader.isStartElement() &&
		        xmlReader.qualifiedName() == Elements.Upgrade.itemSell) {
			auto attrs    = xmlReader.attributes();
			data.upgrade1 = attrs.value(Attributes.level(1)).toUInt();
			data.upgrade2 = attrs.value(Attributes.level(2)).toUInt();
		}
	}

	return data;
}

MaxBuildingsData readMaxBuildingsData(QXmlStreamReader& xmlReader)
{
	assert(xmlReader.qualifiedName() == Elements.Building.maximum);

	MaxBuildingsData data;
	while (!xmlReader.atEnd()) {
		xmlReader.readNext();
		if (xmlReader.isEndElement() && xmlReader.qualifiedName() == Elements.Building.maximum) {
			break;
		}
		else if (
		        xmlReader.isStartElement() &&
		        xmlReader.qualifiedName() == Elements.Building.houses) {
			data.house = xmlReader.readElementText().toUInt();
		}
		else if (
		        xmlReader.isStartElement() &&
		        xmlReader.qualifiedName() == Elements.Building.mines) {
			data.mine = xmlReader.readElementText().toUInt();
		}
	}

	return data;
}

std::map<PanicRank, unsigned> getRankToGoldMap(QXmlStreamAttributes const& attrs)
{
	static std::array<std::string, 5> const StringRanks{ "S", "A", "B", "C", "D" };
	std::map<PanicRank, unsigned>           mapToReturn;

	for (auto const& rank : StringRanks) {
		mapToReturn[stringToRank(rank)] = attrs.value(QString::fromStdString(rank)).toUInt();
	}

	return mapToReturn;
}

NightRewardData readNightRewardData(QXmlStreamReader& xmlReader)
{
	assert(xmlReader.qualifiedName() == Elements.Night.rewards);

	NightRewardData data;
	assert(data.panicRewards.size() == 0);
	assert(data.itemValues.size() == 0);

	while (!xmlReader.atEnd()) {
		xmlReader.readNext();
		if (xmlReader.isEndElement() && xmlReader.qualifiedName() == Elements.Night.rewards) {
			break;
		}
		else if (
		        xmlReader.isStartElement() &&
		        xmlReader.qualifiedName() == Elements.Night.itemValues) {
			auto        attrs       = xmlReader.attributes();
			std::string rank        = attrs.value(Attributes.minRank).toString().toStdString();
			data.minimumRankForItem = stringToRank(rank);
		}
		else if (xmlReader.isStartElement() && xmlReader.qualifiedName() == Elements.Night.night) {
			auto attrs = xmlReader.attributes();
			data.panicRewards.push_back(getRankToGoldMap(attrs));
		}
		else if (xmlReader.isStartElement() && xmlReader.qualifiedName() == Elements.Night.item) {
			data.itemValues.push_back(xmlReader.attributes().value(Attributes.value).toUInt());
		}
	}

	return data;
}

StartingBuildingsData readStartingBuildingsData(QXmlStreamReader& xmlReader)
{
	assert(xmlReader.qualifiedName() == Elements.Building.starting);

	StartingBuildingsData data;
	memset(&data, 0, sizeof(data));

	while (!xmlReader.atEnd()) {
		xmlReader.readNext();
		if (xmlReader.isEndElement() && xmlReader.qualifiedName() == Elements.Building.starting) {
			break;
		}
		else if (
		        xmlReader.isStartElement() &&
		        xmlReader.qualifiedName() == Elements.Building.houses) {
			data.house = xmlReader.readElementText().toUInt();
		}
		else if (
		        xmlReader.isStartElement() &&
		        xmlReader.qualifiedName() == Elements.Building.mines) {
			data.mine = xmlReader.readElementText().toUInt();
		}
		else if (
		        xmlReader.isStartElement() && xmlReader.qualifiedName() == Elements.Building.shop) {
			data.shop = xmlReader.readElementText().toUInt();
		}
		else if (
		        xmlReader.isStartElement() &&
		        xmlReader.qualifiedName() == Elements.Building.ruins1) {
			data.ruins1 = xmlReader.readElementText().toUInt();
		}
		else if (
		        xmlReader.isStartElement() &&
		        xmlReader.qualifiedName() == Elements.Building.ruins2) {
			data.ruins2 = xmlReader.readElementText().toUInt();
		}
		else if (
		        xmlReader.isStartElement() &&
		        xmlReader.qualifiedName() == Elements.Building.ruins3) {
			data.ruins3 = xmlReader.readElementText().toUInt();
		}
		else if (
		        xmlReader.isStartElement() &&
		        xmlReader.qualifiedName() == Elements.Building.ruins4) {
			data.ruins4 = xmlReader.readElementText().toUInt();
		}
	}

	return data;
}

GoldMineUpgrades readGoldMineData(QXmlStreamReader& xmlReader)
{
	assert(xmlReader.qualifiedName() == Elements.Data.mine);

	GoldMineUpgrades data;

	while (!xmlReader.atEnd()) {
		xmlReader.readNext();
		if (xmlReader.isEndElement() && xmlReader.qualifiedName() == Elements.Data.mine) {
			break;
		}
		else if (
		        xmlReader.isStartElement() &&
		        xmlReader.qualifiedName().startsWith(Elements.levelPrefix)) {
			data.push_back({});
			auto attrs = xmlReader.attributes();

			auto const& goldStringRef = attrs.value(Attributes.Upgrade.gold);
			if (!goldStringRef.isEmpty())
				data.back()[GoldMineParam::GOLD] = goldStringRef.toUInt();

			auto const& productionStringRef = attrs.value(Attributes.Upgrade.production);
			if (!productionStringRef.isEmpty())
				data.back()[GoldMineParam::PRODUCTION] = productionStringRef.toUInt();

			auto const& usesStringRef = attrs.value(Attributes.Upgrade.uses);
			if (!usesStringRef.isEmpty())
				data.back()[GoldMineParam::USES] = usesStringRef.toUInt();

			auto const& workersStringRef = attrs.value(Attributes.Upgrade.workers);
			if (!workersStringRef.isEmpty())
				data.back()[GoldMineParam::WORKER_COST] = workersStringRef.toUInt();
		}
	}

	return data;
}

ShopUpgradesData readShopData(QXmlStreamReader& xmlReader)
{
	assert(xmlReader.qualifiedName() == Elements.Data.shop);
	ShopUpgradesData data;

	while (!xmlReader.atEnd()) {
		xmlReader.readNext();
		if (xmlReader.isEndElement() && xmlReader.qualifiedName() == Elements.Data.shop) {
			break;
		}
		else if (
		        xmlReader.isStartElement() &&
		        xmlReader.qualifiedName() == Elements.Upgrade.itemSell) {
			auto attrs       = xmlReader.attributes();
			data.percentage0 = attrs.value(Attributes.level(0)).toUInt();
			data.percentage1 = attrs.value(Attributes.level(1)).toUInt();
			data.percentage2 = attrs.value(Attributes.level(2)).toUInt();
		}
	}

	return data;
}

RuinsData readRuinsData(QXmlStreamReader& xmlReader)
{
	assert(xmlReader.qualifiedName() == Elements.Data.ruins);

	RuinsData data;

	while (!xmlReader.atEnd()) {
		xmlReader.readNext();
		if (xmlReader.isEndElement() && xmlReader.qualifiedName() == Elements.Data.ruins) {
			break;
		}
		else if (
		        xmlReader.isStartElement() &&
		        xmlReader.qualifiedName() == Elements.Building.ruins1) {
			data.size1gold = xmlReader.readElementText().toUInt();
		}
		else if (
		        xmlReader.isStartElement() &&
		        xmlReader.qualifiedName() == Elements.Building.ruins2) {
			data.size2gold = xmlReader.readElementText().toUInt();
		}
		else if (
		        xmlReader.isStartElement() &&
		        xmlReader.qualifiedName() == Elements.Building.ruins3) {
			data.size3gold = xmlReader.readElementText().toUInt();
		}
		else if (
		        xmlReader.isStartElement() &&
		        xmlReader.qualifiedName() == Elements.Building.ruins4) {
			data.size4gold = xmlReader.readElementText().toUInt();
		}
	}

	return data;
}

void assignUIntAttribute(
        unsigned& target, QXmlStreamAttributes const& attributes, QString const& name)
{
	QStringRef attribute = attributes.value(name);
	if (!attribute.isEmpty())
		target = attribute.toUInt();
}

} // namespace

} // namespace GameDataXml
