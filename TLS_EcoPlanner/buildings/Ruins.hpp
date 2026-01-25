#pragma once

#include <QString>

class Ruins
{
public:
	Ruins(unsigned size);

public:
	unsigned getSize() const { return m_size; }
	QString  getDescription(unsigned index) const;

private:
	unsigned m_size;
};
