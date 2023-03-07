#pragma once

typedef unsigned cmd_type;
static const cmd_type CALL = 0;
static const cmd_type SEND = 1;

const uint32_t recv_buff_size = 1024;

struct MsgHead {
    char flag[8];
    uint32_t cmd;
    uint32_t len;
};