#pragma once

#include "game_data/GameData.hpp"
#include "source/Misc.hpp"

#include <QMap>
#include <QString>
#include <QVariant>

class History;

QString const PlanElement           = "Plan";
QString const GameDataFileElement   = "GameDataFile";
QString const EasyDifficultyElement = "EasyModifier";
QString const ApoDifficultyElement  = "ApoModifier";

QString const ActionsGroupElement  = "Actions";
QString const PanicRankAttribute   = "rank";
QString const UpgradePathAttribute = "upgrade_path";
QString const MineIndexAttribute   = "mine_index";
QString const HouseIndexAttribute  = "house_index";
QString const RuinsSizeAttribute   = "ruins_size";

enum class ActionElement {
	INVALID = 0,
	DAY_START,
	BUILD_MINE,
	UPGRADE_MINE,
	WORK_MINE,
	BUILD_HOUSE,
	UPGRADE_HOUSE,
	BUILD_SHOP,
	UPGRADE_SHOP,
	SCAVENGE_RUINS,
};

struct SerializedHistoryAction {
	ActionElement           actionType;
	QMap<QString, QVariant> arguments;
};

struct SerializedHistory {
	QString                          gameDataFile;
	PriceModsFlags                   priceModifiers;
	QVector<SerializedHistoryAction> actions;
};

StringError writePlanToFile(
        QString const& filename, History const& history, QString const& gameDataFilename,
        PriceModsFlags modifiers);

StringError readPlanFromFile(QString const& filename, SerializedHistory& serializedActions);

QString     upgradePathToString(UpgradePath path);
UpgradePath stringToUpgradePath(QString str);
