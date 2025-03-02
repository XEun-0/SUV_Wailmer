#ifndef MAINLAYOUT_H
#define MAINLAYOUT_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QSizePolicy>
#include <QLabel>
#include <QTimer>
#include <QSpacerItem>
#include <QTextStream>
#include "serialib.h"
#include "Common/util.h"
#include "../../../Vehicle/controller/include/Interface/ttcSohResp.h"
#include "pPrintf.h"

namespace Ui {
  class MainLayout;
}

enum Label_Type {
  SENSOR_FIELDS, 
  MOTOR_SPEED, 
  LABEL_GO, 
  LABEL_NOGO
};
 
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

  QLabel *currentMotorSpeed1_Label;
  QLabel *currentMotorSpeed2_Label;
  QLabel *currentMotorSpeed3_Label;
  QLabel *currentMotorSpeed4_Label;
  QLabel *currentMotorSpeed5_Label;
  QLabel *currentMotorSpeed6_Label;
  QLabel *sensorSerialConf_Label;
  QLabel *thrusterControllerState_Label;
  
  // Buttons
  QPushButton *startTxRx_Button;
  QPushButton *stopTxRx_Button;
  QPushButton *verticalUp_Button;
  QPushButton *verticalDown_Button;
  QPushButton *forward_Button;
  QPushButton *backward_Button;
  QPushButton *turnRight_Button;
  QPushButton *turnLeft_Button;

  QTimer *timer;

  // Member functions
  void initializeUI();
  void buttonSetup();
  void updateLabel(Label_Type label_type);
  void dummyDecoder();
  int16_t calculateChecksum(uint8_t *data, size_t length);

  uint8_t bytesAvailable;
};
#endif // MAINLAYOUT_H