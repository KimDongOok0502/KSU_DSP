#include <stdio.h>

#include "Configuration.h"
#include "BasicUltraSonic.h"

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/

Basic_UltraSonic IR_US; /**< \brief Stm global data */
/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/
/** \addtogroup IfxLld_Demo_Stm_SrcDoc_Main_Interrupt
 * \{ */

/** \name Interrupts for SystemTimer(STM) driver.
 * \{ */
IFX_INTERRUPT(US_Int0Handler, 0, ISR_PRIORITY_STM_INT1);
//IFX_INTERRUPT(ECHO_Int0Handler, 0, ISR_PRIORITY_STM_INT2);
/** \} */

/** \} */
/** \brief Handle SystemTimer(STM) interrupt
 *
 * \isrProvider \ref ISR_PROVIDER_SystemTimer(STM)
 * \isrPriority \ref ISR_PRIORITY_SystemTimer(STM)
 *
 */
void US_Int0Handler(void)
{
    IfxStm_clearCompareFlag(IR_US.stmSfr, IR_US.stmConfig.comparator);
#ifdef SIMULATION
	IfxStm_increaseCompare(IR_US.stmSfr, IR_US.stmConfig.comparator, 1000);
#else
	IfxStm_increaseCompare(IR_US.stmSfr, IR_US.stmConfig.comparator, TimeConst_100us);
#endif
    IfxCpu_enableInterrupts();

    if(IfxPort_getPinState(ECHOleft.port, ECHOleft.pinIndex)==TRUE)
    {
    	 IR_US.counterleft++;
    }
    if(IfxPort_getPinState(ECHOleft.port, ECHOleft.pinIndex)==FALSE)
    {
    	IR_US.EchoTickleft = IR_US.counterleft;
    }

    if(IfxPort_getPinState(ECHOright.port, ECHOright.pinIndex)==TRUE)
    {
	  IR_US.counterright++;
    }
    if(IfxPort_getPinState(ECHOright.port, ECHOright.pinIndex)==FALSE)
    {
	  IR_US.EchoTickright = IR_US.counterright;
    }



}

/*void ECHO_Int0Handler(void)
{
	if(IfxPort_getPinState(ECHO.port, ECHO.pinIndex))
	{
		if(IfxPort_getPinState(ECHO.port, ECHO.pinIndex))
		{
			IR_US.counter = 0;
		}

		if(IfxPort_getPinState(ECHO.port, ECHO.pinIndex))
		{
			IR_US.EchoTick = IR_US.counter;
		}
	}

    IfxCpu_enableInterrupts();

    }
}*/
/** \brief Demo init API
 *
 * This function is called from main during initialization phase
 */
void BasicUltraSonic_init(void)
{
    printf("BasicUltraSonic_init() called\n");

    /* disable interrupts */
    boolean interruptState = IfxCpu_disableInterrupts();

    //IR_US.counter  = 0;

    initTime();

    IfxPort_setPinMode(TRIG.port, TRIG.pinIndex, IfxPort_Mode_outputPushPullGeneral);
	IfxPort_setPinPadDriver(TRIG.port, TRIG.pinIndex, IfxPort_PadDriver_cmosAutomotiveSpeed1);
	IfxPort_setPinState(TRIG.port, TRIG.pinIndex, IfxPort_State_low);

	IfxPort_setPinModeInput(ECHOleft.port, ECHOleft.pinIndex, IfxPort_InputMode_noPullDevice);
	IfxPort_setPinModeInput(ECHOright.port, ECHOright.pinIndex, IfxPort_InputMode_noPullDevice);

	// suspend by debugger enabled
    IfxStm_enableOcdsSuspend (&MODULE_STM1);

    IR_US.stmSfr = &MODULE_STM1;
    IfxStm_initCompareConfig(&IR_US.stmConfig);

    IR_US.stmConfig.triggerPriority = ISR_PRIORITY_STM_INT1;
    IR_US.stmConfig.typeOfService   = IfxSrc_Tos_cpu0;
#ifdef SIMULATION
    g_SrcSwInt.stmConfig.ticks      = 1000;
#else
    IR_US.stmConfig.ticks           = TimeConst_100us;
#endif
    IfxStm_initCompare(IR_US.stmSfr, &IR_US.stmConfig);

    /* enable interrupts again */
    IfxCpu_restoreInterrupts(interruptState);
}


/** \brief Demo run API
 *
 * This function is called from main, background loop
 */
void BasicUltraSonic_run(void)
{
    IR_US.distanceleft = (IR_US.EchoTickleft * 340/2)/100;
	IR_US.counterleft = 0;
	IR_US.distanceright = (IR_US.EchoTickright * 340/2)/100;
    IR_US.counterright = 0;

	IfxPort_setPinState(TRIG.port, TRIG.pinIndex, IfxPort_State_low);
	waitTime(TimeConst_100us);

	IfxPort_setPinState(TRIG.port, TRIG.pinIndex, IfxPort_State_toggled);
	waitTime(TimeConst_10us);

	IfxPort_setPinState(TRIG.port, TRIG.pinIndex, IfxPort_State_toggled);
	waitTime(TimeConst_100us);


}
