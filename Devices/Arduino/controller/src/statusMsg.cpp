#include "statusMsg.h"


StatusMsg::StatusMsg() {
    validationByte = 1;
}

uint8_t StatusMsg::GetValidationByte() {
    return validationByte;
}

void StatusMsg::SetValidationByte(uint8_t setByte) {
    validationByte = setByte;
}

