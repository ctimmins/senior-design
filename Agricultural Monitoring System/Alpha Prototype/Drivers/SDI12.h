#ifndef SDI12_H
#define SDI12_H

#include <project.h>

int sdi_send_cmd(char *cmd);
int sdi_send_break(void);

#endif