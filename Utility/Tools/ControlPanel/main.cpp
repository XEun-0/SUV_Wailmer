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
    w->setWindowTitle("Grid Layouts (3x4)");
    w->resize(800, 400);
    // Display
    w->show();

    // Event loop
    return app.exec();
}

// 18:19:44.228 -> Barometer Pressure: 1014.00
// 18:19:44.228 -> Barometer Temperature: 25.72
// 18:19:44.228 -> Barometer Depth: 8.29
// 18:19:44.228 -> Barometer Altitude: -6.24
// 18:19:44.228 -> IMU Orientation X: 105.44
// 18:19:44.273 -> IMU Orientation Y: 0.81
// 18:19:44.273 -> IMU Orientation Z: 1.37
// 18:19:44.273 -> IMU Temperature: 32
// 18:19:44.408 -> Barometer Pressure: 1015.00
// 18:19:44.408 -> Barometer Temperature: 25.72
// 18:19:44.408 -> Barometer Depth: 16.58
// 18:19:44.408 -> Barometer Altitude: -14.55
// 18:19:44.408 -> IMU Orientation X: 105.44
// 18:19:44.408 -> IMU Orientation Y: 0.81
// 18:19:44.408 -> IMU Orientation Z: 1.37
// 18:19:44.408 -> IMU Temperature: 32
// 18:19:44.543 -> Barometer Pressure: 1015.00
// 18:19:44.543 -> Barometer Temperature: 25.72
// 18:19:44.543 -> Barometer Depth: 16.58
// 18:19:44.543 -> Barometer Altitude: -14.55
// 18:19:44.543 -> IMU Orientation X: 105.44
// 18:19:44.543 -> IMU Orientation Y: 0.81
// 18:19:44.589 -> IMU Orientation Z: 1.37
// 18:19:44.589 -> IMU Temperature: 32
// 18:19:44.724 -> Barometer Pressure: 1015.00
// 18:19:44.724 -> Barometer Temperature: 25.72
// 18:19:44.724 -> Barometer Depth: 16.58
// 18:19:44.724 -> Barometer Altitude: -14.55
// 18:19:44.724 -> IMU Orientation X: 105.44
// 18:19:44.724 -> IMU Orientation Y: 0.81
// 18:19:44.724 -> IMU Orientation Z: 1.37
// 18:19:44.724 -> IMU Temperature: 32
// 18:19:44.861 -> Barometer Pressure: 1015.00
// 18:19:44.861 -> Barometer Temperature: 25.72
// 18:19:44.861 -> Barometer Depth: 16.58
// 18:19:44.861 -> Barometer Altitude: -14.55
// 18:19:44.861 -> IMU Orientation X: 105.44
// 18:19:44.861 -> IMU Orientation Y: 0.81
// 18:19:44.861 -> IMU Orientation Z: 1.37
// 18:19:44.861 -> IMU Temperature: 32
// 18:19:45.042 -> Barometer Pressure: 1015.00
// 18:19:45.042 -> Barometer Temperature: 25.73
// 18:19:45.042 -> Barometer Depth: 16.58
// 18:19:45.042 -> Barometer Altitude: -14.55
// 18:19:45.042 -> IMU Orientation X: 105.44
// 18:19:45.042 -> IMU Orientation Y: 0.81
// 18:19:45.042 -> IMU Orientation Z: 1.37
// 18:19:45.042 -> IMU Temperature: 32
// 18:19:45.179 -> Barometer Pressure: 1015.00
// 18:19:45.179 -> Barometer Temperature: 25.73
// 18:19:45.179 -> Barometer Depth: 16.58
// 18:19:45.179 -> Barometer Altitude: -14.55
// 18:19:45.179 -> IMU Orientation X: 105.44
// 18:19:45.179 -> IMU Orientation Y: 0.81
// 18:19:45.179 -> IMU Orientation Z: 1.37
// 18:19:45.179 -> IMU Temperature: 32
// 18:19:45.360 -> Barometer Pressure: 1015.00
// 18:19:45.360 -> Barometer Temperature: 25.72
// 18:19:45.360 -> Barometer Depth: 16.58
// 18:19:45.360 -> Barometer Altitude: -14.55
// 18:19:45.360 -> IMU Orientation X: 105.44
// 18:19:45.360 -> IMU Orientation Y: 0.81
// 18:19:45.360 -> IMU Orientation Z: 1.37
// 18:19:45.360 -> IMU Temperature: 32

// 18:19:45.496 -> Barometer Pressure: 1015.00
// 18:19:45.496 -> Barometer Temperature: 25.72
// 18:19:45.496 -> Barometer Depth: 16.58
// 18:19:45.496 -> Barometer Altitude: -14.55
// 18:19:45.496 -> IMU Orientation X: 105.44
// 18:19:45.496 -> IMU Orientation Y: 0.81
// 18:19:45.496 -> IMU Orientation Z: 1.37
// 18:19:45.496 -> IMU Temperature: 32

// 20:30:00.439 -> Baro Pressure: 1017.00
// 20:30:00.439 -> Baro Temp: 25.23
// 20:30:00.439 -> Baro Depth: 0.04
// 20:30:00.439 -> Baro Altitude: -31.16
// 20:30:00.439 -> IMU Orientation X: 358.63
// 20:30:00.439 -> IMU Orientation Y: 2.56
// 20:30:00.439 -> IMU Orientation Z: 2.56
// 20:30:00.484 -> IMU Temp: 31
// 20:30:00.484 -> Checksum: 2560

