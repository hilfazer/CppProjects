#pragma once

#include <QString>

namespace GameDataXml
{

QString const GameDataSchemaFilename = "game_data.xsd";

const struct {
	struct {
		QString game  = "GameData";
		QString price = "PriceData";
		QString mine  = "GoldMineData";
		QString shop  = "ShopData";
		QString ruins = "RuinsData";
	} Data;

	struct {
		QString mine    = "MinePriceData";
		QString house   = "HousePriceData";
		QString shop    = "ShopPriceData";
		QString easyMod = "EasyDiffModifier";
		QString apocMod = "ApoDiffModifier";
	} Price;

	struct {
		QString build    = "Build";
		QString maximum  = "MaxBuildings";
		QString starting = "StartingBuildings";
		QString houses   = "houses";
		QString mines    = "mines";
		QString shop     = "shop";
		QString ruins1   = "ruins1";
		QString ruins2   = "ruins2";
		QString ruins3   = "ruins3";
		QString ruins4   = "ruins4";
	} Building;

	struct {
		QString const rewards    = "NightRewards";
		QString const itemValues = "ItemValues";
		QString const item       = "item";
		QString const night      = "night";
		QString const gold       = "Gold";
	} Night;

	struct {
		QString production = "production";
		QString workers    = "workers";
		QString uses       = "uses";
		QString gold       = "gold";
		QString expansion  = "expansion";
		QString itemSell   = "sellingPrices";
	} Upgrade;

	QString levelPrefix = "lvl";

	static QString level(unsigned x) { return "lvl" + QString::number(x); }
} Elements;

const struct {
	struct {
		QString production = "production";
		QString workers    = "workers";
		QString uses       = "uses";
		QString gold       = "gold";
	} Upgrade;

	QString minRank = "min_rank";
	QString value   = "value";

	static QString level(unsigned x) { return "lvl" + QString::number(x); }
} Attributes;

} // namespace GameDataXml
