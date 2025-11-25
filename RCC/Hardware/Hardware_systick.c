#include "Hardware_systick.h"


static __IO uint32_t TimingDelay;

void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}
