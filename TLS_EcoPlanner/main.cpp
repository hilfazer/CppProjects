#include "source/mainwindow.hpp"

#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
	QFile styleSheetFile("resources/Combinear/Combinear.qss");
	styleSheetFile.open(QFile::ReadOnly);
	QString styleSheet = QLatin1String(styleSheetFile.readAll());

	QApplication a(argc, argv);
	//	a.setStyleSheet(styleSheet);
	MainWindow   w;
	w.show();
	return a.exec();
}
