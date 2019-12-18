#ifndef OSTASKS_H
#define OSTASKS_H

#ifdef __cplusplus
extern "C" {
#endif

extern bool genTestLog;

void taskConsoleStart(void * argument);
void taskTestStart(void * argument);

#ifdef __cplusplus
}
#endif

#endif
