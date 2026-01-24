#pragma once

#include <QtCore/QString>

#include <optional>

// TODO 3 levels of apo modifiers (20, 40 and 60 %)
struct PriceModsFlags {
	bool easy = false;
	bool apoc = false;
};

using StringError = std::optional<QString>;
