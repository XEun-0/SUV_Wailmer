#ifndef ERRORCODES_H
#define ERRORCODES_H

enum ControllerErrorCodeType {
  ERROR_CODE_NO_SEMAPHORE,
  ERROR_CODE_SEMAPHORE_DOES_NOT_EXIST,
};

enum InitSerialErrorCodeType {
  ERROR_CODE_SERIAL_NOT_INITIALIZED,

};

enum SensorInitStatusCodeType {
  ERROR_NO_BARO,
  BARO_GOOD,
  ERROR_NO_IMU,
  IMU_GOOD,
  ERROR_SENSORS_BAD,
  SENSORS_GOOD
};

#endif //ERRORCODES_H