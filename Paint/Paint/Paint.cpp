#include "Paint.hpp"
#include "ScribbleArea.hpp"

#include <QtGlobal> // dla Q_UNUSED
#include <QCloseEvent>
#include <QColorDialog>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QByteArray>
#include <QImageWriter>


Paint::Paint(QWidget *parent)
    : QMainWindow(parent)
{
	scribbleArea = new ScribbleArea;
	setCentralWidget(scribbleArea);
	createActions();
	createMenus();
	setWindowTitle(tr("Scribble"));
    resize(500, 500);
    //setupUi(this);
}

Paint::~Paint()
{}

void Paint::closeEvent(QCloseEvent* event)
{
    if(maybeSave()) {
        event->accept();
    } else {
        event->ignore();
	}
}

void Paint::open()
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());

        if (!fileName.isEmpty()) {
            scribbleArea->openImage(fileName);
        }
    }
}

void Paint::save()
{
	QAction* action = qobject_cast<QAction*>(sender());
    QByteArray fileFormat = action->data().toByteArray();
	saveFile(fileFormat);
}

void Paint::penColor()
{
	QColor newColor = QColorDialog::getColor(scribbleArea->penColor());
    if (newColor.isValid()) {
        scribbleArea->setPenColor(newColor);
	}
}

void Paint::penWidth()
{
	bool ok;
    int newWidth = QInputDialog::getInt(this, tr("Scribble"), tr("Select pen width:"),
        scribbleArea->penWidth(), 1, 50, 1, &ok);
    if (ok) {
        scribbleArea->setPenWidth(newWidth);
	}

}

void Paint::about()
{
    QMessageBox::about(this, tr("About Scribble"),
        tr("<p>The <b>Scribble</b> example demonstrates how to "
           "use QMainWindow as the base widget for an application, and how "
           "to reimplement some of QWidget's event handlers to receive "
           "the events generated for the application.</p><p>In addition it "
			"demonstrates painting on widgets using QPainter.</p>"));
}

void Paint::createActions()
{
	openAct = new QAction(tr("Open"), this);
	openAct->setShortcuts(QKeySequence::Open);
	connect(openAct, &QAction::triggered, this, &Paint::open);
	foreach(QByteArray format, QImageWriter::supportedImageFormats()) {
		QString text = tr("%1...").arg(QString(format)).toUpper();
		QAction* action = new QAction(text, this);
		action->setData(format);
		connect(action, &QAction::triggered, this, &Paint::save);
		saveAsActs.append(action);
	}

	printAct = new QAction(tr("&Print..."), this);
	connect(printAct, &QAction::triggered, scribbleArea, &ScribbleArea::print);

	exitAct = new QAction(tr("E&xit"), this);
	exitAct->setShortcuts(QKeySequence::Quit);
	connect(exitAct, &QAction::triggered, this, &Paint::close);

	penColorAct = new QAction(tr("&Pen Color..."), this);
	connect(penColorAct, &QAction::triggered, scribbleArea, &ScribbleArea::penColor);

	penWidthAct = new QAction(tr("&Pen Width..."), this);
	connect(penWidthAct, &QAction::triggered, scribbleArea, &ScribbleArea::penWidth);

	clearScreenAct = new QAction(tr("&Clear Screen..."), this);
	clearScreenAct->setShortcut(tr("Ctrl+L"));
	connect(clearScreenAct, &QAction::triggered, scribbleArea, &ScribbleArea::penColor);

	aboutAct = new QAction(tr("&About..."), this);
	connect(aboutAct, &QAction::triggered, &Paint::about);

	aboutQtAct = new QAction(tr("About &Qt..."), this);
	connect(aboutQtAct, &QAction::triggered, qApp, &QApplication::aboutQt);
}

void Paint::createMenus()
{
	saveAsMenu = new QMenu(tr("&Save As"), this);
	foreach(QAction* action, saveAsActs) {
		saveAsMenu->addAction(action);
	}
	fileMenu = new QMenu(tr("&File"), this);
	fileMenu->addAction(openAct);
	fileMenu->addMenu(saveAsMenu);
	fileMenu->addAction(printAct);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAct);

	optionMenu = new QMenu(tr("&Options"), this);
	optionMenu->addAction(penColorAct);
	optionMenu->addAction(penWidthAct);
	optionMenu->addSeparator();
	optionMenu->addAction(clearScreenAct);
	
	helpMenu = new QMenu(tr("&Help"), this);
	helpMenu->addAction(aboutAct);
	helpMenu->addAction(aboutQtAct);
	
	QMainWindow::menuBar()->addMenu(fileMenu);
	QMainWindow::menuBar()->addMenu(optionMenu);
	QMainWindow::menuBar()->addMenu(helpMenu);
}

bool Paint::maybeSave()
{
	if(scribbleArea->isModified()) {
		QMessageBox::StandardButton ret;
		ret = QMessageBox::warning(this, tr("Scribble"),
			tr("The image has been modified.\n"
			   "Do you want to save your changes?"),
			QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

		if (ret == QMessageBox::Save) {
			return saveFile("png");
		} else if (ret == QMessageBox::Cancel) {
			return false;
		}
	}
	return true;
}

bool Paint::saveFile(const QByteArray& fileFormat)
{
	QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;
	QString fileName = QFileDialog::getSaveFileName(
		this,
		tr("Save As"),
		initialPath,
		tr("%1 Files (*.%2);; All Files (*)")
			.arg(QString::fromLatin1(fileFormat.toUpper()))
			.arg(QString::fromLatin1(fileFormat))
		);

	if(fileName.isEmpty()) {
		return false;
	} else {
		return scribbleArea->saveImage(fileName, fileFormat.constData());
	}
}
