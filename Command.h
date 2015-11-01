#ifndef COMMAND_H_INCLUDED
#define COMMAND_H_INCLUDED

#define CMD_UP 1
#define CMD_DOWN 2
#define CMD_RIGHT 3
#define CMD_LEFT 4
#define CMD_USE 5
#define CMD_EQUIP 6

#define DIR_UP    0x01
#define DIR_UP    0x02
#define DIR_RIGHT 0x04
#define DIR_LEFT  0x08


struct Command{
    uint8_t protocolHead;
    uint8_t cmd;
    uint8_t cmdData;
    uint32_t extraData;

};

#endif // COMMAND_H_INCLUDED
