#ifndef COMMONSTRUCTS_H
#define COMMONSTRUCTS_H

#include <stdint.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

// Forward declaration for StatusMsg class
class StatusMsg;

typedef struct {
  SemaphoreHandle_t   msgSemaphore;
  StatusMsg*          statusMsg;
} TaskParams_t;

// Enums
typedef enum {
  INITIALIZED_SENSORS,
  GO,
  RUNNING,
  NOGO
} ProgressState_t;
// ProgressState progressState;

#endif //COMMONSTRUCTS_H