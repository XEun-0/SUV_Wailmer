#ifndef MAINLAYOUT_H
#define MAINLAYOUT_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QSizePolicy>
#include <QLabel>
#include <QTimer>
#include "../../../ROS_ws/src/Common/serialib.h"

#define SENSOR_BUFFER_SIZE 32

namespace Ui {
  class MainLayout;
}
 
class MainLayout : public QGridLayout
{
  Q_OBJECT
public:
  explicit MainLayout(QWidget *parent = nullptr);
private slots:
  void startTimer();
  void stopTimer();
  // Function called by the timer every second
  void txRxFromSerial();
private:
  // Label
  QLabel *baroPressure_Label;
  QLabel *baroTemp_Label;
  QLabel *baroDepth_Label;
  QLabel *baroAltitude_Label;

  QLabel *imuOrientX_Label;
  QLabel *imuOrientY_Label;
  QLabel *imuOrientZ_Label;
  QLabel *imuTemp_Label;
  
  // Buttons
  QPushButton *startTxRx_Button;
  QPushButton *stopTxRx_Button;
  QPushButton *b3;
  QPushButton *b4;
  QPushButton *b5;
  QPushButton *b6;

  QTimer *timer;

  // Member functions
  void initializeUI();
  void updateLabel();
  int16_t calculateChecksum(uint8_t *data, size_t length);
};
#endif // MAINLAYOUT_H