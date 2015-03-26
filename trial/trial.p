.origin 0 
.entrypoint START

#include "trial.hp"

START:
    // Enable OCP master port
    LBCO r0, CONST_PRUCFG, 4, 4
    CLR r0, r0, 4
    SBCO r0, CONST_PRUCFG, 4, 4

    //C28 will point to 0x00012000 (PRU shared RAM)
    MOV r0, 0x00000120
    MOV r1, CTPPR_0
    ST32 r0, r1

    MOV r5, 8 //Current Offset
    MOV r6, 0
    LBCO r7, CONST_PRUSHAREDRAM, 0, 4	//number of locations
    LBCO r8, CONST_PRUSHAREDRAM, 4, 4 	//value to be added
    LBCO r3, CONST_PRUSHAREDRAM, 8, 4	//load shared memory location to write
	
READ:
	ADD r3, r3, r8
	ADD r5, r5, 4
        SBCO r3, CONST_PRUSHAREDRAM, r5, 4
  	      
	ADD r6, r6, 1
	QBEQ END, r6, r7
        QBA READ
        
    END:
    	//Send event to host program
    	MOV r31.b0, PRU0_ARM_INTERRUPT+16 
HALT
