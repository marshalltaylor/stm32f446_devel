#ifndef OS_H
#define OS_H

#ifdef __cplusplus
 extern "C" {
#endif

#define TEST_EVENT_0	( 1 << 0 )
#define TEST_EVENT_1	( 1 << 1 )

//Local functions
void osInit(void);
void vConfigureTimerForRunTimeStats( void );

//Ext Functions
extern void xPortSysTickHandler(void);

#ifdef __cplusplus
}

#endif
#endif
