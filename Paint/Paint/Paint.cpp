#include "Paint.hpp"
#include "ScribbleArea.hpp"

#include <QtGlobal> // dla Q_UNUSED
#include <QByteArray>
#include <QCloseEvent>
#include <QColorDialog>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>


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
        QString fileName = QFileDialog::getCurrentDirectory(this, tr("Open File"), QDir::currentPath());

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
    // Empty implementation (stub)
}

void Paint::createMenus()
{
    // Empty implementation (stub)
}

bool Paint::maybeSave()
{
    // Default to allowing close. Adjust logic in future as needed.
    return true;
}

bool Paint::saveFile(const QByteArray& fileFormat)
{
    Q_UNUSED(fileFormat);
    // Stub: actual save logic to be implemented later.
    return false;
}

