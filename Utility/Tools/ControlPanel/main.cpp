#include <QApplication>
#include <QtCore>
#include <QtGui>
#include <QPushButton>
#include <QGridLayout>

#include "mainLayout.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    MainLayout *mainLayout = new MainLayout;

    // Create a widget
    QWidget *w = new QWidget();

    // Set the grid layout as a main layout
    w->setLayout(mainLayout);

    // Window title
    w->setWindowTitle("SUV_Wailmer Control Panel");
    w->resize(800, 400);
    // Display
    w->show();

    // Event loop
    return app.exec();
}