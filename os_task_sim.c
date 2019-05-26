#include "Windows.h"

/* Mocked headers */
#include "MockMod1.h"	
/* Production code headers */
#include "func1.h"

void Simulate_OS_Task_Mocks_In(void);
void Simulate_OS_Task_Mocks_Out(void);
void Simulate_OS_Task_100ms_Windows(void);

void Simulate_OS_Task_Mocks_In(void){
	Mod1_read_signal_a_ExpectAnyArgs();
	Mod1_read_signal_b_ExpectAnyArgsAndReturn(0);
	Mod1_read_signal_c_ExpectAnyArgsAndReturn(0);
}

void Simulate_OS_Task_Mocks_Out(void){
	Mod1_write_signal_error_ExpectAnyArgs();
}

void Simulate_OS_Task_100ms_Windows(void){
	#define CYCLES 10u
	#define PERIOD 100u

	unsigned char cycles = 0;
	SYSTEMTIME st_in,st_out;
	int diff=0;
	unsigned int retVal = 0;

	while(cycles <= CYCLES){
		GetSystemTime(&st_in);
		#ifdef TASK_DEBUG
			printf("t1=%d\n",st_in.wMilliseconds);
		#endif

		/* Mocks */
			Simulate_OS_Task_Mocks_In();
			Simulate_OS_Task_Mocks_Out();

		/* Task to test */
			retVal = receiveInputAndReturnBoolean();

		/* Simulation cfg */
		GetSystemTime(&st_out);
		diff = st_out.wMilliseconds - st_in.wMilliseconds;
		#ifdef TASK_DEBUG
			printf("t2=%d\n",st_out.wMilliseconds);
		#endif
		if(diff<PERIOD){
			Sleep(PERIOD-diff);
		}
		else{
			printf("Time elapsed too long! (%d)\n",diff);
		}
		cycles++;
	}
	#undef CYCLES
	#undef PERIOD
}

int main(void){
	Simulate_OS_Task_100ms_Windows();
	return 0;
}