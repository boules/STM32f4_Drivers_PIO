

#ifndef F_CPU
#define F_CPU 16000000
#endif

# define delay_ms(numof_milliseconds)   {	unsigned long cycles = ((F_CPU)/8000UL) * numof_milliseconds; \
											for (unsigned long i = 0; i < cycles; i++){\
												__asm volatile("nop");\
											}\
										}
