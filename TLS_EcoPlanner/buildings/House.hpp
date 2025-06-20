#pragma once

#include <QString>

class House
{
public:
	House();

	bool upgradeWorkers();
	bool downgradeWorkers();

	unsigned getWorkerCount() const { return m_level + 1; }
	unsigned getUpgradeLevel() const { return m_level; }
	QString  getDescription(unsigned index) const;

private:
	unsigned m_level = 0;
};
