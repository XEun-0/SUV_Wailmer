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

struct SensorInfo {
  float baroPressure;  // Changed from float to float
  float baroTemp;      // Changed from float to float
  float baroDepth;     // Changed from float to float
  float baroAltitude;  // Changed from float to float

  float imuOrientX;    // Changed from float to float
  float imuOrientY;    // Changed from float to float
  float imuOrientZ;    // Changed from float to float
  uint8_t imuTemp;
  uint8_t padding1[1];    // Adjust padding for alignment (optional)
  uint16_t checksum;      // Changed checksum to be 2 bytes (uint16_t)
}; // 4 x 7 + 1 + 1 = 32 bytes
SensorInfo sensor_info;

uint8_t sensorBuffer[SENSOR_BUFFER_SIZE];
uint8_t outBuffer[SENSOR_BUFFER_SIZE];

MainLayout::MainLayout(QWidget *parent)
  : QGridLayout(parent), timer(new QTimer(this)) {
  
  char errorOpening = serial.openDevice(SERIAL_PORT, BAUD_RATE);
      
  // If connection fails, return the error code otherwise, display a success message
  if (errorOpening!=1) {
    printf ("ERROR CONNECTING TO %s\n",SERIAL_PORT);
  } else {
    printf ("Successful connection to %s\n",SERIAL_PORT);
    memset(outBuffer, 0, SENSOR_BUFFER_SIZE);
    this->initializeUI();
  }
}

void MainLayout::initializeUI() {
  // Initialize Labels
  baroPressure_Label =  new QLabel("baroPressure: ");
  baroTemp_Label =      new QLabel("baroTemp: ");
  baroDepth_Label =     new QLabel("baroDepth: ");
  baroAltitude_Label =  new QLabel("baroAltitude: ");
  
  imuOrientX_Label =    new QLabel("imuOrientX: ");
  imuOrientY_Label =    new QLabel("imuOrientY: ");
  imuOrientZ_Label =    new QLabel("imuOrientZ: ");
  imuTemp_Label =       new QLabel("imuTemp: ");

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

  // TxRx buttons and layout
  QGridLayout *TxRxButtonLayout = new QGridLayout();
  // Create the button, make "this" the parent
  startTxRx_Button = new QPushButton("Start Tx/Rx");
  // set size and location of the button
  startTxRx_Button->setGeometry(QRect(QPoint(100, 100), QSize(200, 50)));
  startTxRx_Button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
  
  stopTxRx_Button = new QPushButton("Stop Tx/Rx");
  stopTxRx_Button->setGeometry(QRect(QPoint(100, 100), QSize(200, 50)));
  stopTxRx_Button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
  
  TxRxButtonLayout->addWidget(startTxRx_Button, 0, 0, 1, 1);
  TxRxButtonLayout->addWidget(stopTxRx_Button, 1, 0, 1, 1);
  
  // Initialize Buttons
  b3 = new QPushButton("C");
  b4 = new QPushButton("D");
  b5 = new QPushButton("E");
  b6 = new QPushButton("F");
  
  // addWidget(*Widget, row, column, rowspan, colspan)
  // 0th row
  this->addLayout(displayLayout,0,0,1,1);
  this->addLayout(TxRxButtonLayout, 3, 0, 1, 1);
  // 1st row
  this->addWidget(b4,1,0,1,1);

  // 2nd row with 2-column span
  this->addWidget(b5,2,0,1,2);

  // Connect button signal to appropriate slot
  connect(startTxRx_Button, &QPushButton::released, this, &MainLayout::startTimer);
  connect(stopTxRx_Button, &QPushButton::released, this, &MainLayout::stopTimer);
  connect(timer, &QTimer::timeout, this, &MainLayout::txRxFromSerial);
  stopTxRx_Button->setEnabled(false);
}

void MainLayout::startTimer()
{
  timer->start(100); // Set interval to 100ms
  startTxRx_Button->setEnabled(false);
  stopTxRx_Button->setEnabled(true);
}

// Function to stop the timer
void MainLayout::stopTimer()
{
  timer->stop();
  startTxRx_Button->setEnabled(true);
  stopTxRx_Button->setEnabled(false);
}

// Function called by the timer every second
void MainLayout::txRxFromSerial()
{
  printf("%ld\n", sizeof(sensor_info));

#if DEBUG_TXRX
  // Barometer
  sensor_info.baroPressure = 11;
  sensor_info.baroTemp = 1;
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
#else
  //serial.readBytes(sensorBuffer, SENSOR_BUFFER_SIZE, 2000, 1000);
  
  uint8_t byteFloatBuffer[sizeof(sensor_info.imuOrientX)];
  float rxFloat = 0;
  serial.readBytes(byteFloatBuffer, sizeof(sensor_info.imuOrientX), 2000, 1000);

  memcpy(&rxFloat, byteFloatBuffer, sizeof(rxFloat));
  printf("sensorBuffer is %d bytes\n", (int)sizeof(sensorBuffer));
  printf("sensor_info is %d bytes\n", (int)sizeof(sensor_info));
  printf("value is %f bytes\n", rxFloat);
  
  memcpy(&sensor_info, sensorBuffer, SENSOR_BUFFER_SIZE);


  // Verify checksum
  uint16_t receivedChecksum = sensor_info.checksum;
  uint16_t calculatedChecksum = calculateChecksum((uint8_t*)&sensor_info, SENSOR_BUFFER_SIZE - 1); // Exclude checksum byte

  if (receivedChecksum == calculatedChecksum) {
    updateLabel();
  } else {
    // Debugging output for checksum mismatch
    printf("Checksum mismatch! Received: %d, Calculated: %d\n", receivedChecksum, calculatedChecksum);
  }
#endif 
}

int16_t MainLayout::calculateChecksum(uint8_t *data, size_t length) {
  int16_t sum = 0;
  for (size_t i = 0; i < length; i++) {
    sum += data[i];
  }
  return sum;
}

void MainLayout::updateLabel() {
  // Barometer
  baroPressure_Label->setText(QString("Barometer Pressure: %1").arg(sensor_info.baroPressure));
  baroTemp_Label->setText(QString("Barometer Temperature: %1").arg(sensor_info.baroTemp));
  baroDepth_Label->setText(QString("Barometer Depth: %1").arg(sensor_info.baroDepth));
  baroAltitude_Label->setText(QString("Barometer Altitude: %1").arg(sensor_info.baroAltitude));
  
  // IMU
  imuOrientX_Label->setText(QString("IMU Orientation X: %1").arg(sensor_info.imuOrientX));
  imuOrientY_Label->setText(QString("IMU Orientation Y: %1").arg(sensor_info.imuOrientY));
  imuOrientZ_Label->setText(QString("IMU Orientation Z: %1").arg(sensor_info.imuOrientZ));
  imuTemp_Label->setText(QString("IMU Temperature: %1").arg(sensor_info.imuTemp));
}
