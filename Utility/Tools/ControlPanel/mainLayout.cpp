#include "mainLayout.h"

serialib serial;

char buffer[15] = "hello\n";

#if defined (_WIN32) || defined(_WIN64)
    #define SERIAL_PORT "\\\\.\\COM5"
#endif
#if defined (__linux__) || defined(__APPLE__)
  #define SERIAL_PORT "/dev/ttyACM0"
#endif

#define BAUD_RATE 115200
#define DEBUG_TXRX 0
#define DEBUG_SINGLE_TXRX 1

#define SENSOR_BUFFER_SIZE 52
#define CHECKSUM_SIZE 2
#define STX_AND_ETX_BYTES 4
#define OUT_BUTTER_SIZE  SENSOR_BUFFER_SIZE + CHECKSUM_SIZE + STX_AND_ETX_BYTES

#define STX 0x02
#define ETX 0x03

#define SERIAL_RECEIVE_MS 50

enum Progress_State {
  INITIALIZED_SENSORS,
  GO,
  RUNNING,
  NOGO
};

Progress_State recvProg = NOGO;
Progress_State currProg = NOGO;
Label_Type curr_label_type = SENSOR_FIELDS;

struct SensorInfo {
  float baroPressure;     // Changed from float to float
  float baroTemp;         // Changed from float to float
  float baroDepth;        // Changed from float to float
  float baroAltitude;     // Changed from float to float

  float imuOrientX;       // Changed from float to float
  float imuOrientY;       // Changed from float to float
  float imuOrientZ;       // Changed from float to float
  uint8_t imuTemp;
  uint8_t padding1[1];    // Adjust padding for alignment (optional)
  uint16_t checksum;      // Changed checksum to be 2 bytes (uint16_t)
}; // 4 x 7 + 1 + 1 = 32 bytes

SensorInfo sensor_info;
TTCSohRespType ttcSohResp;
uint8_t ttcSohRespBuffer[sizeof(TTCSohRespType)];
uint8_t sensorBuffer[SENSOR_BUFFER_SIZE];
uint8_t outBuffer[SENSOR_BUFFER_SIZE];

MainLayout::MainLayout(QWidget *parent)
  : QGridLayout(parent), timer(new QTimer(this)) {
  this->initializeUI();
  memset(outBuffer, 0, SENSOR_BUFFER_SIZE);
  memset(sensorBuffer, 0, SENSOR_BUFFER_SIZE);
  memset(ttcSohRespBuffer, 0, SENSOR_BUFFER_SIZE);
}

void MainLayout::initializeUI() {
  
  /////displayLayout//////
  // Initialize Labels
  baroPressure_Label =  new QLabel("baroPressure: ");
  baroTemp_Label =      new QLabel("baroTemp: ");
  baroDepth_Label =     new QLabel("baroDepth: ");
  baroAltitude_Label =  new QLabel("baroAltitude: ");
  
  imuOrientX_Label =    new QLabel("imuOrientX: ");
  imuOrientY_Label =    new QLabel("imuOrientY: ");
  imuOrientZ_Label =    new QLabel("imuOrientZ: ");
  imuTemp_Label =       new QLabel("imuTemp: ");

  sensorSerialConf_Label = new QLabel("Sensor Serial Status: NOGO");

  QGridLayout *displayLayout = new QGridLayout();
  // Barometer Sensor Labels
  displayLayout->addWidget(baroPressure_Label, 0, 0, 1, 1);
  displayLayout->addWidget(baroTemp_Label, 1, 0, 1, 1);
  displayLayout->addWidget(baroDepth_Label, 2, 0, 1, 1);
  displayLayout->addWidget(baroAltitude_Label, 3, 0, 1, 1);
  
  // IMU sensor labels
  displayLayout->addWidget(imuOrientX_Label, 4, 0, 1, 1);
  displayLayout->addWidget(imuOrientY_Label, 5, 0, 1, 1);
  displayLayout->addWidget(imuOrientZ_Label, 6, 0, 1, 1);
  displayLayout->addWidget(imuTemp_Label, 7, 0, 1, 1);

  displayLayout->addWidget(sensorSerialConf_Label, 8, 0, 1, 1);
  /////displayLayout//////

  /////statusDisplayLayout//////
  currentMotorSpeed1_Label = new QLabel("Thruster 1 Speed: ");
  currentMotorSpeed2_Label = new QLabel("Thruster 2 Speed: ");
  currentMotorSpeed3_Label = new QLabel("Thruster 3 Speed: ");
  currentMotorSpeed4_Label = new QLabel("Thruster 4 Speed: ");
  currentMotorSpeed5_Label = new QLabel("Thruster 5 Speed: ");
  currentMotorSpeed6_Label = new QLabel("Thruster 6 Speed: ");

  thrusterControllerState_Label = new QLabel("Thruster Controller State: ");

  QGridLayout *statusDisplayLayout = new QGridLayout();
  statusDisplayLayout->addWidget(currentMotorSpeed1_Label, 0, 0, 1, 1);
  statusDisplayLayout->addWidget(currentMotorSpeed2_Label, 1, 0, 1, 1);
  statusDisplayLayout->addWidget(currentMotorSpeed3_Label, 2, 0, 1, 1);
  statusDisplayLayout->addWidget(currentMotorSpeed4_Label, 3, 0, 1, 1);
  statusDisplayLayout->addWidget(currentMotorSpeed5_Label, 4, 0, 1, 1);
  statusDisplayLayout->addWidget(currentMotorSpeed6_Label, 5, 0, 1, 1);
  statusDisplayLayout->addWidget(thrusterControllerState_Label, 6, 0, 1, 1);
  /////statusDisplayLayout//////

  // TxRx buttons and layout
  QGridLayout *TxRxButtonLayout = new QGridLayout();
  // Create the button, make "this" the parent
  startTxRx_Button = new QPushButton("Start Tx/Rx");
  // set size and location of the button
  startTxRx_Button->setFixedSize(QSize(100, 30));
  //startTxRx_Button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
  
  stopTxRx_Button = new QPushButton("Stop Tx/Rx");
  stopTxRx_Button->setFixedSize(QSize(100, 30));
  //stopTxRx_Button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
  
  TxRxButtonLayout->addWidget(startTxRx_Button, 0, 0, 1, 1);
  TxRxButtonLayout->addWidget(stopTxRx_Button, 1, 0, 1, 1);
  
  // Initialize Buttons
  verticalUp_Button = new QPushButton("UP");
  verticalUp_Button->setFixedSize(QSize(75, 40));
  //verticalUp_Button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

  verticalDown_Button = new QPushButton("DOWN");
  verticalDown_Button->setFixedSize(QSize(75, 40));
  
  forward_Button = new QPushButton("Fwd");
  forward_Button->setFixedSize(QSize(75, 40));
  
  backward_Button = new QPushButton("Bwd");
  backward_Button->setFixedSize(QSize(75, 40));
  
  turnRight_Button = new QPushButton("Trn ->");
  turnRight_Button->setFixedSize(QSize(75, 40));
  
  turnLeft_Button = new QPushButton("<- Trn");
  turnLeft_Button->setFixedSize(QSize(75, 40));

  QGridLayout *MovementControls = new QGridLayout();
  // 00 01 02
  // 10 11 12
  // 20 21 22
  MovementControls->addWidget(verticalUp_Button, 0, 2, 1, 1);
  MovementControls->addWidget(verticalDown_Button, 2, 2, 1, 1);
  
  MovementControls->addWidget(forward_Button, 0, 1, 1, 1);
  MovementControls->addWidget(backward_Button, 2, 1, 1, 1);
  MovementControls->addWidget(turnRight_Button, 1, 2, 1, 1);
  MovementControls->addWidget(turnLeft_Button, 1, 0, 1, 1);
  QSpacerItem *verticalSpacer = new QSpacerItem(200, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
  this->addItem(verticalSpacer, 3, 2, 1, 1);
  
  this->addLayout(displayLayout,0,0,1,1);
  this->addLayout(TxRxButtonLayout, 3, 0, 1, 1);
  this->addLayout(MovementControls, 3, 3, 1, 1);
  //this->addWidget(currentMotorSpeed_Label, 0, 3, 1, 1);
  this->addLayout(statusDisplayLayout, 0, 3, 1, 1);

  // Connect button signal to appropriate slot
  connect(startTxRx_Button, &QPushButton::released, this, &MainLayout::startTimer);
  connect(stopTxRx_Button, &QPushButton::released, this, &MainLayout::stopTimer);
  connect(timer, &QTimer::timeout, this, &MainLayout::txRxFromSerial);
  stopTxRx_Button->setEnabled(false);

  updateLabel(SENSOR_FIELDS);
}

void MainLayout::startTimer()
{
  char errorOpening = serial.openDevice(SERIAL_PORT, BAUD_RATE);
      
  // If connection fails, return the error code otherwise, display a success message
  if (errorOpening!=1) {
    printf ("ERROR CONNECTING TO %s\n",SERIAL_PORT);
  } else {
    printf ("Successful connection to %s\n",SERIAL_PORT);
  }
  serial.flushReceiver();

  timer->start(SERIAL_RECEIVE_MS); // Set interval to 100ms
  startTxRx_Button->setEnabled(false);
  stopTxRx_Button->setEnabled(true);
}

// Function to stop the timer
void MainLayout::stopTimer()
{
  timer->stop();
  startTxRx_Button->setEnabled(true);
  stopTxRx_Button->setEnabled(false);
  serial.closeDevice();
  currProg = NOGO;
  updateLabel(LABEL_NOGO);
}

// Function called by the timer every second
void MainLayout::txRxFromSerial()
{
  //printf("%ld\n", sizeof(sensor_info));

#if DEBUG_TXRX
  // Barometer
  sensor_info.baroPressure = 11.12;
  sensor_info.baroTemp = 1.34;
  sensor_info.baroDepth = 2;
  sensor_info.baroAltitude = 3;
  
  // IMU
  sensor_info.imuOrientX = 32;
  sensor_info.imuOrientY = 6;
  sensor_info.imuOrientZ = 7;
  sensor_info.imuTemp = 8;

  memcpy(outBuffer, &sensor_info, SENSOR_BUFFER_SIZE);
  serial.writeBytes(outBuffer, SENSOR_BUFFER_SIZE);
  serial.readBytes(sensorBuffer, SENSOR_BUFFER_SIZE, 2000, 1000);
  memcpy(&sensor_info, sensorBuffer, SENSOR_BUFFER_SIZE);
  updateLabel();
#elif DEBUG_SINGLE_TXRX
  // switch (recvProg) {
  //   case INITIALIZED_SENSORS:
  //     currProg = RUNNING;
  //     serial.writeBytes((uint8_t*)&currProg, 1);
  //     recvProg = currProg;
  //     break;
  //   case GO:
  //     break;
  //   case RUNNING: {
  //     uint8_t byteFloatBuffer[sizeof(sensor_info.imuOrientX)];
  //     float rxFloat = 0;
      
  //     serial.readBytes(byteFloatBuffer, sizeof(sensor_info.imuOrientX), 2000, 1000);
  //     //if (bytesRead == sizeof(sensor_info.imuOrientX)) {
  //       memcpy(&rxFloat, byteFloatBuffer, sizeof(rxFloat));
      
  //       printf("value is %f\n", rxFloat);
  //     //}
  //     break;
  //   }
  //   case NOGO:
  //   default: {
  //     uint8_t msgBuffer[1];
  //     memset(msgBuffer, 0, 1);

  //     serial.readBytes(msgBuffer, 1, 2000, 1000);
      
  //     recvProg = (Progress_State)msgBuffer[0];
  //     currProg = recvProg;
  //     printf("value is %d\n", recvProg);
  //   }
  // }
  
  // uint8_t byteFloatBuffer[sizeof(sensor_info.imuOrientX)];

  // serial.readBytes(byteFloatBuffer, sizeof(sensor_info.imuOrientX), 2000, 1000);
  //if (bytesRead == sizeof(sensor_info.imuOrientX)) {
  // memcpy(&rxFloat, byteFloatBuffer, sizeof(rxFloat));

  // printf("value is %f\n", rxFloat);

  bytesAvailable = serial.available();
  printf("bytes available: %d\n", bytesAvailable);
  
  // checkStructSizes();

  if (bytesAvailable != SENSOR_BUFFER_SIZE && currProg != GO) {
    serial.flushReceiver();
    return;
  } else {
    currProg = GO;
    updateLabel(LABEL_GO);
  }

  serial.readBytes(ttcSohRespBuffer, sizeof(TTCSohRespType), 2000, 1000);
  //hexDump((uint8_t *)ttcSohRespBuffer, SENSOR_BUFFER_SIZE);
  memcpy(&ttcSohResp, ttcSohRespBuffer, SENSOR_BUFFER_SIZE);
  //hexDump((uint8_t *)&ttcSohResp, SENSOR_BUFFER_SIZE);

  //printf("orientationx: %f\n", ttcSohResp.sensorInfo.imuOrientX);
  // memcpy(&sensor_info.baroPressure, &sensorBuffer[0], sizeof(sensor_info.baroPressure));
  // memcpy(&sensor_info.baroTemp, &sensorBuffer[4], sizeof(sensor_info.baroTemp));
  // memcpy(&sensor_info.baroDepth, &sensorBuffer[8], sizeof(sensor_info.baroDepth));
  // memcpy(&sensor_info.baroAltitude, &sensorBuffer[12], sizeof(sensor_info.baroAltitude));

  // memcpy(&sensor_info.imuOrientX, &sensorBuffer[16], sizeof(sensor_info.imuOrientX));
  // memcpy(&sensor_info.imuOrientY, &sensorBuffer[20], sizeof(sensor_info.imuOrientY));
  // memcpy(&sensor_info.imuOrientZ, &sensorBuffer[24], sizeof(sensor_info.imuOrientZ));

  // // bytes 28-29 = 1 byte
  // memcpy(&sensor_info.imuTemp, &sensorBuffer[28], sizeof(sensor_info.imuTemp));

  // memcpy(&sensor_info, sensorBuffer, SENSOR_BUFFER_SIZE);
  
  // Testing pPrint statement
  // pPrintf("testing pPrintf %d\n", 5);
  
  updateLabel(SENSOR_FIELDS);
  //}
#else
  //serial.readBytes(sensorBuffer, SENSOR_BUFFER_SIZE, 2000, 1000);
  
  
  int bytesRead = serial.readBytes(sensorBuffer, sizeof(sensor_info), 2000, 1000);
  if (bytesRead == SENSOR_BUFFER_SIZE) {
    memcpy(&sensor_info, sensorBuffer, SENSOR_BUFFER_SIZE);
    printf("sensor_info.imuOrientX is %f\n", sensor_info.imuOrientX);
  }
  printf("sensorBuffer is %d bytes\n", (int)sizeof(sensorBuffer));
  printf("sensor_info is %d bytes\n", (int)sizeof(sensor_info));
  
  

  // // Verify checksum
  // uint16_t receivedChecksum = sensor_info.checksum;
  // uint16_t calculatedChecksum = calculateChecksum((uint8_t*)&sensor_info, SENSOR_BUFFER_SIZE - 1); // Exclude checksum byte

  // if (receivedChecksum == calculatedChecksum) {
  //   updateLabel();
  // } else {
  //   // Debugging output for checksum mismatch
  //   printf("Checksum mismatch! Received: %d, Calculated: %d\n", receivedChecksum, calculatedChecksum);
  // }
#endif 
}

int16_t MainLayout::calculateChecksum(uint8_t *data, size_t length) {
  int16_t sum = 0;
  for (size_t i = 0; i < length; i++) {
    sum += data[i];
  }
  return sum;
}

void MainLayout::updateLabel(Label_Type label_type) {
  switch(label_type) {
    case SENSOR_FIELDS:
      // Barometer
      baroPressure_Label->setText(QString("Barometer Pressure: %1").arg(ttcSohResp.sensorInfo.baroPressure, 0, 'f', 2));
      baroTemp_Label->setText(QString("Barometer Temperature: %1").arg(ttcSohResp.sensorInfo.baroTemp, 0, 'f', 2));
      baroDepth_Label->setText(QString("Barometer Depth: %1").arg(ttcSohResp.sensorInfo.baroDepth, 0, 'f', 2));
      baroAltitude_Label->setText(QString("Barometer Altitude: %1").arg(ttcSohResp.sensorInfo.baroAltitude, 0, 'f', 2));
      
      // IMU
      imuOrientX_Label->setText(QString("IMU Orientation X: %1").arg(ttcSohResp.sensorInfo.imuOrientX, 0, 'f', 2));
      imuOrientY_Label->setText(QString("IMU Orientation Y: %1").arg(ttcSohResp.sensorInfo.imuOrientY, 0, 'f', 2));
      imuOrientZ_Label->setText(QString("IMU Orientation Z: %1").arg(ttcSohResp.sensorInfo.imuOrientZ, 0, 'f', 2));
      imuTemp_Label->setText(QString("IMU Temperature: %1").arg(ttcSohResp.sensorInfo.imuTemp));

      // Motors
      currentMotorSpeed1_Label->setText(QString("Thruster 1 Speed: %1").arg(ttcSohResp.thrusterInfo.thrusterSpeeds.left_thruster_speed));
      currentMotorSpeed2_Label->setText(QString("Thruster 2 Speed: %1").arg(ttcSohResp.thrusterInfo.thrusterSpeeds.right_thruster_speed));
      currentMotorSpeed3_Label->setText(QString("Thruster 3 Speed: %1").arg(ttcSohResp.thrusterInfo.thrusterSpeeds.front_right_thruster_speed));
      currentMotorSpeed4_Label->setText(QString("Thruster 4 Speed: %1").arg(ttcSohResp.thrusterInfo.thrusterSpeeds.front_left_thruster_speed));
      currentMotorSpeed5_Label->setText(QString("Thruster 5 Speed: %1").arg(ttcSohResp.thrusterInfo.thrusterSpeeds.back_left_thruster_speed));
      currentMotorSpeed6_Label->setText(QString("Thruster 6 Speed: %1").arg(ttcSohResp.thrusterInfo.thrusterSpeeds.back_right_thruster_speed));

      thrusterControllerState_Label->setText(QString("Thruster Controller State: %1").arg(ttcSohResp.thrusterInfo.thrusterControllerState));

      break;
    case MOTOR_SPEED:
      break;
    case LABEL_GO:
      //"Sensor Serial Status: "
      sensorSerialConf_Label->setText(QString("Sensor Serial Status: GO"));
      break;
    case LABEL_NOGO:
      sensorSerialConf_Label->setText(QString("Sensor Serial Status: NOGO"));
      break;
  }
  
}
