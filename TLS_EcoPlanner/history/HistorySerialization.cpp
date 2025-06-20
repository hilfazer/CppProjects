#include "HistorySerialization.hpp"
#include "history/History.hpp"
#include "history/action/HistoryAction.hpp"
#include "source/FileOperations.hpp"

#include <QFile>
#include <QVector>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#include <boost/bimap.hpp>

#include <memory>
#include <optional>

namespace
{

QString const PlanSchemaFilename = "plan_schema.xsd";

QVector<SerializedHistoryAction> serializeHistoryActions(History const& history);
void writeAction(SerializedHistoryAction const& action, QXmlStreamWriter& writer);
QVector<SerializedHistoryAction>       parseHistoryActions(QXmlStreamReader& xmlReader);
std::optional<SerializedHistoryAction> parseAction(QXmlStreamReader const& xmlReader);
ActionElement                          stringToActionElement(QString const& stringElement);
QString                                actionElementToString(ActionElement element);

} // namespace

StringError writePlanToFile(
        QString const& filename, History const& history, QString const& gameDataFilename,
        PriceModsFlags modifiers)
{
	QFile file;
	file.setFileName(filename);

	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		return CouldNotOpenFileForWriting.arg(filename);
	}

	QXmlStreamWriter xmlWriter(&file);
	xmlWriter.setAutoFormatting(true);

	xmlWriter.writeStartDocument();
	xmlWriter.writeStartElement(PlanElement);

	xmlWriter.writeTextElement(GameDataFileElement, gameDataFilename);
	xmlWriter.writeTextElement(EasyDifficultyElement, QString::number(modifiers.easy));
	xmlWriter.writeTextElement(ApoDifficultyElement, QString::number(modifiers.apoc));

	xmlWriter.writeStartElement(ActionsGroupElement);
	{
		auto serializedActions = serializeHistoryActions(history);
		for (SerializedHistoryAction const& serializedAction : serializedActions) {
			writeAction(serializedAction, xmlWriter);
		}
	}
	xmlWriter.writeEndElement();

	xmlWriter.writeEndElement();
	xmlWriter.writeEndDocument();
	file.close();
	return {};
}

StringError readPlanFromFile(QString const& filename, SerializedHistory& serializedHistory)
{
	assert(serializedHistory.actions.size() == 0);

	auto error = validateXmlFile(filename, PlanSchemaFilename);
	if (error) {
		return *error;
	}

	QFile file;
	file.setFileName(filename);

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		return CouldNotOpenFileForReading.arg(filename);
	}

	QXmlStreamReader xmlReader(&file);
	while (!xmlReader.atEnd()) {
		xmlReader.readNext();

		if (xmlReader.tokenType() == QXmlStreamReader::TokenType::StartElement) {
			if (xmlReader.qualifiedName() == GameDataFileElement) {
				serializedHistory.gameDataFile = xmlReader.readElementText();
			}
			else if (xmlReader.qualifiedName() == EasyDifficultyElement) {
				serializedHistory.priceModifiers.easy = xmlReader.readElementText().toInt();
			}
			else if (xmlReader.qualifiedName() == ApoDifficultyElement) {
				serializedHistory.priceModifiers.apoc = xmlReader.readElementText().toInt();
			}
			else if (xmlReader.qualifiedName() == ActionsGroupElement) {
				serializedHistory.actions = parseHistoryActions(xmlReader);
			}
		}
	}

	file.close();
	if (xmlReader.hasError())
		return xmlReader.errorString();

	return {};
}

//----------------------------------------------------------------------------

namespace
{

QVector<SerializedHistoryAction> serializeHistoryActions(History const& history)
{
	auto const&                      actions = history.getActions();
	QVector<SerializedHistoryAction> serializedActions;

	for (auto const action : actions) {
		serializedActions.push_back(action->serialize());
	}

	return serializedActions;
}

QVector<SerializedHistoryAction> parseHistoryActions(QXmlStreamReader& xmlReader)
{
	using Type = QXmlStreamReader::TokenType;
	assert(xmlReader.qualifiedName() == ActionsGroupElement);

	QVector<SerializedHistoryAction> serializedActions;
	while (!xmlReader.atEnd() && !(xmlReader.tokenType() == Type::EndElement &&
	                               xmlReader.qualifiedName() == ActionsGroupElement)) {

		auto const tokenType = xmlReader.readNext();
		if (tokenType != QXmlStreamReader::StartElement)
			continue;

		auto action = parseAction(xmlReader);
		if (action)
			serializedActions.append(*action);
	}

	return serializedActions;
}

void writeAction(SerializedHistoryAction const& action, QXmlStreamWriter& writer)
{
	writer.writeStartElement(actionElementToString(action.actionType));

	for (auto keyValue = action.arguments.constKeyValueBegin();
	     keyValue != action.arguments.constKeyValueEnd();
	     ++keyValue) {
		auto argumentName  = keyValue->first;
		auto argumentValue = keyValue->second.toString();
		writer.writeAttribute(argumentName, argumentValue);
	}
	writer.writeEndElement();
}

std::optional<SerializedHistoryAction> parseAction(QXmlStreamReader const& xmlReader)
{
	using AE = ActionElement;

	AE const element = stringToActionElement(xmlReader.qualifiedName().toString());

	switch (element) {
	case AE::DAY_START: {
		auto attributes = xmlReader.attributes();
		if (!attributes.hasAttribute(PanicRankAttribute))
			return {};

		QString rank = attributes.value(PanicRankAttribute).toString();

		return SerializedHistoryAction{ element, { { PanicRankAttribute, rank } } };
	}
	case AE::BUILD_MINE: {
		return SerializedHistoryAction{ element, {} };
	}
	case AE::UPGRADE_MINE: {
		auto attributes = xmlReader.attributes();
		if (!attributes.hasAttribute(MineIndexAttribute) ||
		    !attributes.hasAttribute(UpgradePathAttribute))
			return {};

		QString path  = attributes.value(UpgradePathAttribute).toString();
		QString index = attributes.value(MineIndexAttribute).toString();

		return SerializedHistoryAction{
			element, {{ UpgradePathAttribute, path }, { MineIndexAttribute, index }}
		};
	}
	case AE::WORK_MINE: {
		auto attributes = xmlReader.attributes();
		if (!attributes.hasAttribute(MineIndexAttribute))
			return {};

		QString index = attributes.value(MineIndexAttribute).toString();

		return SerializedHistoryAction{ element, { { MineIndexAttribute, index } } };
	}
	case AE::BUILD_HOUSE: {
		return SerializedHistoryAction{ element, {} };
	}
	case AE::UPGRADE_HOUSE: {
		auto attributes = xmlReader.attributes();
		if (!attributes.hasAttribute(HouseIndexAttribute))
			return {};

		QString index = attributes.value(HouseIndexAttribute).toString();

		return SerializedHistoryAction{ element, { { HouseIndexAttribute, index } } };
	}
	case AE::BUILD_SHOP: {
		return SerializedHistoryAction{ element, {} };
	}
	case AE::UPGRADE_SHOP: {
		return SerializedHistoryAction{ element, {} };
	}
	case AE::SCAVENGE_RUINS: {
		auto attributes = xmlReader.attributes();
		if (!attributes.hasAttribute(RuinsSizeAttribute))
			return {};

		QString size = attributes.value(RuinsSizeAttribute).toString();

		return SerializedHistoryAction{ element, { { RuinsSizeAttribute, size } } };
	}
	case AE::INVALID: {
		assert(!bool("invalid action"));
		return {};
	}
	}

	return {};
}

boost::bimap<QString, ActionElement> const ElementQStringBimap = []() {
	using AE = ActionElement;
	boost::bimap<QString, ActionElement> bm;
	bm.insert({ "DayStart", AE::DAY_START });
	bm.insert({ "BuildGoldMine", AE::BUILD_MINE });
	bm.insert({ "UpgradeGoldMine", AE::UPGRADE_MINE });
	bm.insert({ "WorkGoldMine", AE::WORK_MINE });
	bm.insert({ "BuildHouse", AE::BUILD_HOUSE });
	bm.insert({ "UpgradeHouse", AE::UPGRADE_HOUSE });
	bm.insert({ "BuildShop", AE::BUILD_SHOP });
	bm.insert({ "UpgradeShopPrices", AE::UPGRADE_SHOP });
	bm.insert({ "ScavengeRuins", AE::SCAVENGE_RUINS });
	return bm;
}();

ActionElement stringToActionElement(QString const& stringElement)
{
	auto it = ElementQStringBimap.left.find(stringElement);
	return it == ElementQStringBimap.left.end() ? ActionElement::INVALID : it->second;
}

QString actionElementToString(ActionElement element)
{
	auto it = ElementQStringBimap.right.find(element);
	assert(it != ElementQStringBimap.right.end());
	return it->second;
}

} // namespace

QString upgradePathToString(UpgradePath path)
{
	if (path == UpgradePath::PRODUCTION)
		return "PRODUCTION";
	else if (path == UpgradePath::WORKERS)
		return "WORKERS";
	else
		assert(false);
	return {};
}

UpgradePath stringToUpgradePath(QString str)
{
	if (str == "PRODUCTION")
		return UpgradePath::PRODUCTION;
	else if (str == "WORKERS")
		return UpgradePath::WORKERS;
	else
		assert(false);
	return {};
}
