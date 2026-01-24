#pragma once

#include "game_data/GameData.hpp"
#include "history/HistorySerialization.hpp"
#include "source/Misc.hpp"
#include "stage/Stage.hpp"

#include <QMap>
#include <QString>
#include <QVariant>

int applyModifiers(unsigned cost, PriceData const& priceData, PriceModsFlags modifiers);

class IHistoryAction
{
public:
	virtual ~IHistoryAction();

public:
	virtual StageError execute() = 0;

	virtual void undo() = 0;

	virtual bool isExecuted() const = 0;

	virtual int getCost(PriceData const& priceData, PriceModsFlags modifiers) const = 0;

	virtual QString getDescription() const = 0;

	virtual QString getTooltip() const = 0;

	virtual SerializedHistoryAction serialize() const = 0;

protected:
	IHistoryAction() = default;
};

class HistoryActionBase : public IHistoryAction
{
public:
	HistoryActionBase(Stage& stage);
	HistoryActionBase(HistoryActionBase const&) = delete;
	HistoryActionBase(HistoryActionBase&&)      = delete;
	void operator==(HistoryActionBase const&)   = delete;
	void operator==(HistoryActionBase&&)        = delete;

	// IHistoryAction interface
public:
	StageError execute() final;
	bool       isExecuted() const final { return m_executed; }
	QString    getTooltip() const override { return {}; }

protected:
	virtual StageError doImpl()     = 0;
	virtual void       postDoInit() = 0; // reading from Stage goes here

protected:
	Stage& m_stage;

private:
	bool m_executed = false;
};
