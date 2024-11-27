#ifndef STATUSMSG_H
#define STATUSMSG_H

#include "Common/commonTypes.h"
class StatusMsg {
public:
    StatusMsg();
    uint8_t GetValidationByte();
    void SetValidationByte(uint8_t setByte);
    
private:
    uint8_t validationByte;

};

#endif //STATUSMSG_H