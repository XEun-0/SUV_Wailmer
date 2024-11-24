#ifndef COMMONSTRUCTS_H
#define COMMONSTRUCTS_H

#include <semphr.h>

typedef struct {
    SemaphoreHandle_t semaphore;

} TaskParams;

// Enums
typedef enum {
  INITIALIZED_SENSORS,
  GO,
  RUNNING,
  NOGO
} ProgressState;
// ProgressState progressState;

#endif //COMMONSTRUCTS_H