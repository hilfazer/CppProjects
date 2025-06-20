#pragma once

#include "source/Misc.hpp"

#include <optional>

#include <QString>

QString const FileValidationFailed       = "Validation of %1 file has failed";
QString const InvalidSchemaFile          = "XML Schema file %1 is invalid";
QString const CouldNotOpenFileForReading = "Could not open file %1 for reading";
QString const CouldNotOpenFileForWriting = "Could not open file %1 for writing";

StringError validateXmlFile(QString const& xmlFilename, QString const& schemaFilename);
