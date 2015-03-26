#include <stdio.h>
#include <prussdrv.h>
#include <pruss_intc_mapping.h>
#include <string.h>
#include <sys/time.h>

#include "trial_bin.h"

#define PRU_NUM 					0
#define OFFSET_SHAREDRAM 			2048
#define PRUSS0_SHARED_DATARAM   	4

int main(int argc, char* argv[]) {
  
	int num_locns = 10, addVal = 25, value, ret, i;
	static void *sharedMem;
	static unsigned int *sharedMem_int;
	struct timeval start_time, end_time;
	
	tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;

  	prussdrv_init();
	ret = prussdrv_open(PRU_EVTOUT_0);
	if (ret){
		printf("\tERROR: prussdrv_open open failed\n");		
	}

	if(argc > 1){
		num_locns = atoi(argv[1]);
	}
	
	if(argc > 2){
		addVal = atoi(argv[2]);
	}

	printf("\n\t\tPRU Example program using shared memory access\n");
	printf("\t\t----------------------------------------------\n");
	printf("\tProgram takes number of locations to write to in the shared memory \n\tand value to add repetitively\n");
	printf("\n\tUsage \n");
	printf("\t\t using default values : sudo ./run \n");
	printf("\t\t specifying arguments : sudo ./run <numOfLocns> <incrementValue>\n\n\n");
	prussdrv_pruintc_init(&pruss_intc_initdata);
	prussdrv_map_prumem(PRUSS0_SHARED_DATARAM, &sharedMem);
	sharedMem_int = (unsigned int*) sharedMem;
	
	sharedMem_int[OFFSET_SHAREDRAM] = num_locns;
	sharedMem_int[OFFSET_SHAREDRAM + 1] = addVal;

	prussdrv_pru_write_memory(PRUSS0_PRU0_IRAM, 0, PRUcode, sizeof(PRUcode));
	prussdrv_pru_enable(0);

	printf("After execution of PRU program....\n");
	for( i = 0; i < num_locns + 2 ; i++){
		value = (sharedMem_int[OFFSET_SHAREDRAM +  i ] ) & 0x00000FFF;
		printf("\tshared_mem[%d] : %d  \n", i + 1, value);
	}

	prussdrv_pru_clear_event (PRU_EVTOUT_0, PRU0_ARM_INTERRUPT);

	/* Disable PRU*/
	prussdrv_pru_disable(PRU_NUM);
	prussdrv_exit();
    
	/* Done */
	return 0;
}
