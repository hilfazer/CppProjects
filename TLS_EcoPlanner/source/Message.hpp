#pragma once

#include <QString>

struct Message {
	enum class Severity { INFO, WARNING, ERROR, CODE_ERROR };

	QString  text;
	Severity severity = Severity::INFO;
};
