#include "source/FileOperations.hpp"

#include <QtCore/QFile>
#include <QtXmlPatterns/QXmlSchema>
#include <QtXmlPatterns/QXmlSchemaValidator>

StringError validateXmlFile(QString const& xmlFilename, QString const& schemaFilename)
{
	QFile schemaFile(schemaFilename);
	if (schemaFile.open(QIODevice::ReadOnly) == false) {
		return CouldNotOpenFileForReading.arg(schemaFile.fileName());
	}

	QXmlSchema schema;
	schema.load(&schemaFile);
	schemaFile.close();
	if (schema.isValid() == false)
		return InvalidSchemaFile.arg(schemaFile.fileName());

	QFile xmlFile(xmlFilename);
	if (xmlFile.open(QIODevice::ReadOnly) == false) {
		return CouldNotOpenFileForReading.arg(xmlFile.fileName());
	}

	StringError         error;
	QXmlSchemaValidator validator(schema);
	if (validator.validate(&xmlFile, QUrl::fromLocalFile(xmlFile.fileName())) == false) {
		error = FileValidationFailed.arg(xmlFile.fileName());
	}

	xmlFile.close();
	return error;
}
