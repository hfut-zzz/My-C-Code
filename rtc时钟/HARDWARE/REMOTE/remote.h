#ifndef _REMOTE_H
#define _REMOTE_H

#include "sys.h"

//ң��ģ�飬����x��ģ��-PA5��y��ģ��-PA6��΢����ť-PA4(������

#define SW_PEWSS 0    //���º�Ϊ0

void AdcInit(void);
void DMAInit(void);
u16* PS2_Scan(void);

#endif
