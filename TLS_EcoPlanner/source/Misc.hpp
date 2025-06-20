#pragma once

#include <QtCore/QString>

#include <optional>

struct PriceModsFlags {
	bool easy = false;
	bool apoc = false;
};

using StringError = std::optional<QString>;
