
/**************************************************************************************************
 *																								  *
 *							            SSSSS    PPPPPP    SSSSS								  * 
 *							          SS   SS   PP   PP  SS   SS								  * 
 *							         SS        PP   PP  SS										  *
 *							         SSSSS    PPPPPP    SSSSS									  *
 *							            SS   PP            SS									  *
 *							      SS   SS   PP       SS   SS									  *
 *							      SSSSS    PP        SSSSS										  *
 *						 																		  *
 *							     SIGNAL  PROCESSING  SOLUTIONS									  *
 *																								  *
 **************************************************************************************************
 *																								  *
 *					Project:							  *
 *																								  *
 *					Author:		Michael E. Spencer, Ph.D.										  *
 *																								  *
 *					Copyright(c) Signal Processing Solutions, 2007								  *
 *																								  *
 **************************************************************************************************
 *
 * File: c672x_dmax_mcasp.c
 *
 * This is the multi-channel audio serial port driver from the "echo" demo and TI.
 * More specifically it is SIO driver implementation for McASP using dMAX.
 *
 * Modified by: MSpencer
 *
 */

/*
    This is an implementation of SIO driver for McASP peripheral on the 
    672x DSP. This implementation uses the dMAX module for performing 
    data I/O to and from McASP. The driver is thus 
    named DMD (dMAX McASP driver)
*/


/* 
    The c672x_dmax_mcasp.h file defines the structures and includes other
    definitions that are required by the DMD driver.
*/
#include <c672x_dmax_mcasp.h>

/*
    The drvObj is the driver's internal structure. This structure is used by
    the DMD driver to understand the status of the driver, information about 
    the various instances of the driver that are activated and any other
    global information that is needed to be maintained by the driver.
    The structure is defined in c672x_dmax_mcasp.h
*/
static DMD_DriverObject drvObj;

/*
    The DMD_dMAX_ISR function is hooked to the interrupt 8 (as part of 
    DMD_init function) The ISR is called every time any instance of 
    the DMD driver completes receiving or transmiting one buffer.
*/


Void DMD_dMAX_ISR ()
{
    DEV_Handle            pDevice;    /* Device handle */
    DMD_DeviceExtension   *pDevExt;   /* Device Extension */
    DEV_Frame             *pFrame;    /* Device Frame */
    CSL_DmaxHandle        hDmax;      /* dMAX handle */
    CSL_DmaxRegsOvly      DTCR;       /* Local registe to check DTCR0 */
    Int32                 i;

/*
    dMAX will set the TCC bit in the DTCR register only if TCINT is enabled.
    Thus, the interrupt will be called even for dMAX transfers not initiated
    by the DMD driver.
    For such interrupts, return as soon as possible to avoid MIPS overhead.
    The code below checks if any of the TCC bits reserved for the DMD driver
    is set. If so, do processing in ISR. Else, return.
*/    
    DTCR = CSL_DMAX_0_REGS;
    if (DTCR->DTCR1 || (DTCR->DTCR0 & ~DMD_TCC_MASK)) {
     /* If the TCC bit not used by driver is set, call */
	 /* a hook function. The user can add processing here */
     /* for dMAX transfers not controlled by the DMD */
        DMD_ISR_hook ();
    }
    if (!(DTCR->DTCR0 & DMD_TCC_MASK)) {
        return;
    }
/*
    Loop over all the open driver instances to find out which instance has
    generated the interrupt. Once interrupted, handle all the instances that 
    have the tcc bit set.
    Loop for CSL_MCASP_CNT*2. This is because for each McASP we can have
    atmost 2 instances of DMD opened. One for input(RX) and one for output(TX).
*/
    for (i = 0; i < (CSL_MCASP_CNT*2); i++) {
        pDevice = drvObj.pDevice[i];     /* Get the device pointer */
        pDevExt = pDevice->object;       /* Get the device extension Str */
        hDmax = pDevExt->hDmax;          /* Get the dMAX handle */
    
        /* If device in not NULL, proceed */
        if (pDevice) {
            /* If the TCC for the DMD instance is set, proceed */
            if (hDmax->regs->DTCR0 & (1 << pDevExt->tcc)) {
                /* Clear the TCC bit */
                CSL_dmaxHwControl(hDmax, CSL_DMAX_CMD_CLEARTCC, NULL);
                
                /* 
                    If numQueuedRcv is 1, it indicates that this is the final 
                    buffer that we are returning with. This is an error state.
                    Report the error and also disable any further dMAX events.
                    McASP events from this point on will be ignored. 
                    If in this state, the driver requires to be 
                    Idled and fresh set of buffers to be issued
                */
                if (pDevExt->numQueuedRcv == 1) {
                    pDevExt->errorState = DMD_ERR_NOBUF;
                    CSL_dmaxHwControl(hDmax, CSL_DMAX_CMD_EVENTDISABLE, NULL);
                }

                /* 
                    The driver object has Rx and Tx queues for receiving and 
                    transmiting buffers b/w driver and framework. The driver
                    also maintains Rx and Tx queues in the pDevExt. 
                    These are the internal queues.
                    The driver receives the buffers from the framework and 
                    enqueues in the driver internal queue. Here the driver 
                    moves the buffer from internal Rx Queue to internal Tx 
                    queue. Later (DMD_reclaim) the driver will dequeue buffer 
                    from internal Tx queue and enqueue in the driver Tx queue
                    so that F/W can receive it.
                */
                pFrame = (DEV_Frame *) QUE_get (&pDevExt->rcvQue);

                /* 
                    Decrement the numQueuedRcv variable. This variable holds 
                    the count of buffers in the driver internal Rx queue.
                */
                pDevExt->numQueuedRcv--;

                /* Enqueue the buffer in the driver internal Tx queue. */
                QUE_put (&pDevExt->xmtQue, pFrame);

                /* 
                    Increment the numQueuedXmt variable. This variable holds
                    the count of buffers in the driver internal Tx queue.
                */
                pDevExt->numQueuedXmt++;

                /* 
                    Post semaphore so that DMD_reclaim knows that there is 
                    buffer in the driver internal Tx queue that can be sent 
                    back to F/W
                */
                SEM_post (pDevExt->sync);
            }
        }
    }
}

/*
    The DMD_getTcc function is used by the driver to understand which TCC can
    be used for the dMAX for any given driver instance. When the driver
    instance is being created, this function is called to understand which 
    TCC can be used.
    Please note, at max DMD_MAX_TCC (6) TCC can be issued by this function. 
    This is because there can be max 6 instances of the driver simultaneously 
    operating.
*/


Int DMD_getTCC( Int32 * status)
{
    Int32 i;

    /* Status is passed by the caller to understand if any error happened. */
    *status = SYS_OK;

    /*
        Loop DMD_MAX_TCC times. Issue one of the available TCC. 
        Use drvObj.tcc variable to hold information about how many TCC 
        are already in use.
    */
    for (i = 0; i < DMD_MAX_TCC; i++) {
        /* If TCC is available, Issue it. Update information in drvObj.tcc */
        if (!(drvObj.tcc & (1 << i))) {
            drvObj.tcc = drvObj.tcc | (1 << i);
            return i;
        }
    }
    /* If no TCC is available, return error */
    *status = DMD_ERR_NOTCC;
    return (DMD_ERR_NOTCC);
}

/*
    The DMD_startMcASP function is called by the driver to activate
    the TX/RX serializers, release the TX/RX state machine and to release
    the frame sync generator out of reset.
    This function implements the steps: 6, 7, 8 and 9 as described in the 
    '3.1.1 Transmit/Receive Section Initialization' Section of the McASP
    Peripheral User's Guide (SPRU878a).
    Please note, the dMAX instance should have been created and configured
    by now to service McASP events. 
    The DMD_startMcASP function is called by the driver after the very first
    buffer has been issued by the F/W. Before the first buffer is issued, the
    McASP is not *completely active*. Only the McASP clocks are 
    running (DMD_startClock) The McASP clocks are intentionally started 
    earlier. Some ComboCodecs require the presence of clocks.
*/


Int DMD_startMcASP (DEV_Handle device)
{
    DMD_DeviceExtension *pDevExt;    /* Device extension pointer */
    const DMD_Params *pParams;       /* User provided driver parameters */
    CSL_McaspHandle hMcasp;          /* McASP handle */

    Int mcaspNum;        /* Variable holds the McASP number */
    Uint32 mask;
    Uint32 bitValue;

    pDevExt = (DMD_DeviceExtension *)device->object;
    pParams = pDevExt->pParams;
    mcaspNum = pParams->moduleNum;
    hMcasp = drvObj.hMcasp[mcaspNum];

    /* 
        The processing is different based on whether the device is 
        servicing the Rx or Tx section of McASP.
    */
    if (device->mode == DEV_INPUT) {
        /* Clear the receiver status registers */
        hMcasp->regs->RSTAT = 0xFFFF;
        
        /* Take the receive serializer out of reset */
        bitValue = 0;
        CSL_FINST (hMcasp->regs->RGBLCTL, MCASP_RGBLCTL_RSRCLR, ACTIVE);

        /* Verify that the bits are correctly latched in GBLCTL register */
        while (bitValue != CSL_MCASP_GBLCTL_RSRCLR_ACTIVE) {
            bitValue = CSL_FEXT (hMcasp->regs->GBLCTL, MCASP_GBLCTL_RSRCLR);
        }

        /* 
            Calculate 'mask' to release the receive state machine and 
            receive frame sync generator out of reset
        */
        mask = CSL_MCASP_GBLCTL_RSMRST_MASK   |
               CSL_MCASP_GBLCTL_RFRST_MASK;
    }
    else {
        /* Clear the transmitter status registers */
        hMcasp->regs->XSTAT = 0xFFFF;    

        /* Take the trasmit serializer out of reset */
        bitValue = 0;
        CSL_FINST (hMcasp->regs->XGBLCTL, MCASP_XGBLCTL_XSRCLR, ACTIVE);
        
        /* Verify that the bits are correctly latched in CBLCTL register */
        while (bitValue != CSL_MCASP_GBLCTL_XSRCLR_ACTIVE) {
            bitValue = CSL_FEXT (hMcasp->regs->GBLCTL, MCASP_GBLCTL_XSRCLR);
        }

        /* Verify that all transmit buffers are serviced. */
        while ( (hMcasp->regs->XSTAT & 0x0020) == 0x0020 );

        /* 
            Calculate 'mask' to release the transmit state machine and 
            transmit frame sync generator out of reset
        */
           mask =  CSL_MCASP_GBLCTL_XSMRST_MASK   |
                  CSL_MCASP_GBLCTL_XFRST_MASK;
    }

    /* Release the state machine and frame sync generator out of reset */    
    CSL_mcaspResetCtrl( hMcasp, mask );

    pDevExt->runState = 1;
    return SYS_OK;
}

/*
    The DMD_configureDMAX function is called by the driver to configure
    the dMAX instance with the params provided by the user (pDevExt->pParams)
    The DMD_configureDMAX function is called by the driver after the very first
    buffer has been issued by the F/W. Before the first buffer is issued, the
    dMAX is not active. Please note this function must be called before calling 
    the DMD_startMcASP function
*/


Int DMD_configureDMAX (DEV_Handle device, DEV_Frame *pFrame)
{
    DMD_DeviceExtension *pDevExt;    /* Device extension str pointer */
    const DMD_Params    *pParams;    /* User passed parameters */
    CSL_DmaxHandle      hDmax;       /* dMAX instance handle */

    /* 
        The below structures could as well have been allocated and freed
        as part of the function. That may lead to memory holes. Also,
        the structures are small in size, thus allocating them from scratch
    */
    CSL_DmaxHwSetup              DmaxHwSetup, *pDmaxHwSetup;
    CSL_DmaxGPTransferEventSetup *pGpxfrEventSetup, GpxfrEventSetup;
    CSL_DmaxGPXFRParameterSetup  *pGpxfrParameterSetup, GpxfrParameterSetup;

    Int32 status;

    pDevExt = (DMD_DeviceExtension *)device->object;
    pParams = pDevExt->pParams;

    /* Assumption here is that the dMAX handle is already available. */
    hDmax = pDevExt->hDmax;

    pDmaxHwSetup = &DmaxHwSetup;
    pGpxfrEventSetup = &GpxfrEventSetup;
    pGpxfrParameterSetup = &GpxfrParameterSetup;

    /* Do necessary assignments. Needed for the CSL_dmaxHwSetup */
    pGpxfrEventSetup->paramSetup = pGpxfrParameterSetup;
    pGpxfrEventSetup->pte = hDmax->paramPtr;
    pDmaxHwSetup->eventSetup = pGpxfrEventSetup;

    /* 
        Perform the pDMAX event and params setup. Please note CSL definitions
        are used to do the necessary initialization. Now, the definitions are
        provided for each event type (Even though definititons for all event
        types are the same). As we are only interested here in the value and 
        not the event type, using event 0 definitions for initialization.
        This is better than using hardocded values [but/also] [worse/better] 
        then using different definitions for different events.
    */
      
    /* General transfer */
    pGpxfrEventSetup->etype = CSL_DMAX_EVENT0_ETYPE_GENERALXFR;
    /* Quantum transfer size limit */
    pGpxfrEventSetup->qtsl = pParams->qtsl; 
    /* Transfer Count0 elements for each sync */
    pGpxfrEventSetup->sync = CSL_DMAX_EVENT0_SYNC_TRAN_COUNT0_ELE;
    /* Get the TCC value to use for the transfer */
    pGpxfrEventSetup->tcc = DMD_getTCC(&status);
    /* If error in allocating TCC, return with the error */
    if (status) {
        return status;
    }
    pDevExt->tcc = pGpxfrEventSetup->tcc;
    /* ATCINT disable */
    pGpxfrEventSetup->atcint = CSL_DMAX_EVENT0_ATCINT_DISABLE;
    /* Enable interrupt generation after completion of transfer */
    pGpxfrEventSetup->tcint = CSL_DMAX_EVENT0_TCINT_ENABLE;
    /* 
        This function is called after the very first buffer issue.
        Enable the reload. As part of dMAX setup below, the first buffer is
        attached to both RLD1 and RDL2. The RLD parameters are modified
        later when new buffers are issued. If no new buffer is issued,
        as part of the dMAX ISR, the dMAX event is disabled after the
        valid buffer are completely used so no incorrect transfer will happen.
    */
    pGpxfrEventSetup->rload = CSL_DMAX_EVENT0_RLOAD_ENABLE;
    /* COUNT0 = Max 8 bit Number of serializer */
    /* COUT1 = Max 8 bit TDM */
    /* COUNT2: Max 16bits Number of samples */
    pGpxfrEventSetup->cc = CSL_DMAX_EVENT0_CC_COUNT2_15_COUNT1_8_COUNT0_8;
    /* 32 bit element size. */
    pGpxfrEventSetup->esize = CSL_DMAX_EVENT0_ESIZE_32BIT;
    /* Set the priority as requested by the user */
    pDmaxHwSetup->priority = pParams->priority;
    /* Polarity Rising Edge */
    pDmaxHwSetup->polarity = pParams->polarity;

    /* 
        Different setups are performed based on whether the driver 
        is handling RX or TX section of the McASP
    */
    if (device->mode == DEV_INPUT) {
        /* The McASP RXBUF for McASP0/1 and 2 are each offset by 0x1000000 */
        pGpxfrParameterSetup->srcReloadAddr0 = (Uint32)(MCASP0_RXBUF_ADDR + 
                                                pParams->moduleNum*0x1000000);
        pGpxfrParameterSetup->srcReloadAddr1 = (Uint32)(MCASP0_RXBUF_ADDR + 
                                                pParams->moduleNum*0x1000000);    

        /* The destination address is derived from frame passed by the F/W */
        pGpxfrParameterSetup->dstReloadAddr0 = (Uint32) pFrame->addr;   

        /* The dstReload doesn't matter here. It will be re-initialized */
        /* when a new buffer is provided by the F/W. */
        pGpxfrParameterSetup->dstReloadAddr1 = (Uint32) pFrame->addr;
        
        /* 
            The SRCIdx0 used is 1 so that the dMAX performance is maximized
            The SRCIdx1 and SRCIdx2 are set to -(pDevExt->numSerializers-1)
            to reset the source pointer
        */ 
        pGpxfrParameterSetup->srcIndex0 = (Int16)1;
        pGpxfrParameterSetup->srcIndex1 = (Int16)-(pDevExt->numSerializers-1);
        pGpxfrParameterSetup->srcIndex2 = (Int16)-(pDevExt->numSerializers-1);

        /* 
            The below index will arrange the buffers as descibed below 
            with example: 
            Pin connections: AXR0[0] = L/R; AXR0[1] = LS/RS; AXR0[2] = LB/RB
            Date arrangment for i/p date:

-> Memory -> Memory -> Memory -> Memory ->....
|--L Buffer---|--LS Buffer---|--LB Buffer---|--R Buffer----|--RS Buffer---|--RB Buffer--|
<-NUM SAMPLES-><-NUM SAMPLES-><-NUM SAMPLES-><-NUM SAMPLES-><-NUM SAMPLES-><-NUM SAMPLES-> 

        */
        pGpxfrParameterSetup->dstIndex0 = (Int16)pParams->numSamples;
        pGpxfrParameterSetup->dstIndex1 = (Int16)pParams->numSamples;
        pGpxfrParameterSetup->dstIndex2 = (Int16)( 
                                                   -(
                                                     (pDevExt->numSerializers * 
                                                     ((pParams->rxFSCtl & 0xff80) >> 7) - 1) * 
                                                     (pParams->numSamples) - 1
                                                    )
                                                 );
    }
    else
    {
        /* The McASP TXBUF for McASP0/1 and 2 are each offset by 0x1000000 */
        pGpxfrParameterSetup->dstReloadAddr0 = (Uint32)(MCASP0_TXBUF_ADDR + 
                                                pParams->moduleNum*0x1000000);   
        pGpxfrParameterSetup->dstReloadAddr1 = (Uint32)(MCASP0_TXBUF_ADDR + 
                                                pParams->moduleNum*0x1000000);

        /* The SRC address is derived from frame passed by the F/W */
        pGpxfrParameterSetup->srcReloadAddr0 = (Uint32)pFrame->addr; 
        /* The srcReload doesn't matter here. It will be re-initialized */
        /* when a new buffer is provided by the F/W */
        pGpxfrParameterSetup->srcReloadAddr1 = (Uint32)pFrame->addr;    

        /* 
            The DSTIdx0 used is 1 so that the dMAX performance is maximized
            The DSTIdx1 and DSTIdx2 are set to -(pDevExt->numSerializers-1)
            to reset the destination pointer
        */ 
        pGpxfrParameterSetup->dstIndex0 = (Int16)1;
        pGpxfrParameterSetup->dstIndex1 = (Int16)-(pDevExt->numSerializers- 1);
        pGpxfrParameterSetup->dstIndex2 = (Int16)-(pDevExt->numSerializers- 1);

        /* 
            The below index will transmit the buffers as descibed below 
            with example:
            Pin connections: AXR0[0] = L/R; AXR0[1] = LS/RS; AXR0[2] = LB/RB
            Date arrangment for o/p data:

-> Memory -> Memory -> Memory -> Memory ->....
|--L Buffer---|--LS Buffer---|--LB Buffer---|--R Buffer----|--RS Buffer---|--RB Buffer--|
<-NUM SAMPLES-><-NUM SAMPLES-><-NUM SAMPLES-><-NUM SAMPLES-><-NUM SAMPLES-><-NUM SAMPLES-> 

        */
        pGpxfrParameterSetup->srcIndex0 = (Int16)pParams->numSamples;
        pGpxfrParameterSetup->srcIndex1 = (Int16)pParams->numSamples;
        pGpxfrParameterSetup->srcIndex2 = (Int16)( 
                                                   -(
                                                     (pDevExt->numSerializers * 
                                                     ((pParams->rxFSCtl & 0xff80) >> 7) - 1) * 
                                                     (pParams->numSamples) - 1
                                                    )
                                                 );
    }
    /* Handle all the serializers for one sync */
    pGpxfrParameterSetup->count0 = (Uint16)pDevExt->numSerializers;

    /* Each serializer can handle 1 - 32 channels based on the TDM setting */ 
    pGpxfrParameterSetup->count1 = (Uint16)((pParams->rxFSCtl & 0xff80) >> 7);

    /* Num of samples per channel is handled here */
    pGpxfrParameterSetup->count2 = (Uint16)pParams->numSamples;    

    /* Perform the H/W setup for the dMAX instance */
    status = CSL_dmaxHwSetup( hDmax, pDmaxHwSetup); 

    /* Clear any pending TCC */
    CSL_dmaxHwControl( hDmax, CSL_DMAX_CMD_CLEARTCC, NULL );

    /* Enable the dMAX event. dMAX is ready to service McASP. */
    CSL_dmaxHwControl( hDmax, CSL_DMAX_CMD_EVENTENABLE, NULL );

    return SYS_OK;
}

/*
    After opening the driver using SIO_create call, it is required that the 
    user provides the device parameters. The user params can be provided as 
    part of the SIO_create call but that method required appending information
    to the 'String name' parameter. Considring this method is not so user 
    friendly, alterate method is used by the DMD driver. The driver provides 
    the feature of providing driver parameters using the DMD_ctrl function. 
    Please see the DMD_ctrl function for more details.
    Once the driver parameters are provided, the DMD_ctrl function calls the
    DMD_openPeripheral function to create the McASP object. The McASP 
    configuration happens as part of the DMD_startClock and DMD_startMcASP 
    functions. This module additionaly creates the dMAX object to be used. 
    The dMAX configuration happens as part of the DMD_configureDMAX function
*/


Int DMD_openPeripheral( DEV_Handle device, const DMD_Params *pParams )
{
    DMD_DeviceExtension *pDevExt;    /* Device extension str ptr */
    CSL_McaspHandle     hMcasp;      /* McASP handle */
    CSL_McaspObj        *pMcaspObj;

    Int mcaspNum;

    CSL_DmaxHandle hDmax;            /* dMAX handle */
    CSL_DmaxObj    *pDmaxObj;

    CSL_Status status;

    pDevExt = (DMD_DeviceExtension *)device->object;
    mcaspNum = pParams->moduleNum;
    hMcasp = drvObj.hMcasp[mcaspNum];
    hDmax = pDevExt->hDmax;

    /* 
        If the particular McASP is already in use for the particular mode of
        operation (RX or TX), return error
    */
    if (drvObj.pDevice[mcaspNum*2 + device->mode]) {
        return DMD_ERR_BUSY;
    }

    /* Update the drvObj str with the device handle */
    drvObj.pDevice[mcaspNum*2 + device->mode] = device;

    /* Store the user passed device parameters in the device extension str. */
    /* The params are referred by later functions as well. */
    pDevExt->pParams = pParams;

    /* If McASP handle not created, create the handle now */
    if (!hMcasp) {
        /* 
            The handle memory is alloated from the segid provided as 
            part of the params
        */
        if (!(pMcaspObj = 
              MEM_alloc (device->segid, (sizeof(CSL_McaspObj)+3)/4*4, 4))
           ) {
            return DMD_ERR_ALLOC;
        }
    }
    else {
        pMcaspObj = hMcasp;
    }

    /* If error in creating handle, return */
    hMcasp = CSL_mcaspOpen( pMcaspObj, mcaspNum, NULL, &status);
    if (status != CSL_SOK) {
        return (DMD_ERR_SPOPEN);
    }

    /* 
        Update the created handle in the drvObj structure, the internal 
        driver structure
    */
    drvObj.hMcasp[mcaspNum] = hMcasp;
    
    /* 
        Set the power down and emulation managment register as OFF.
        Peripheral ignores the emulation suspend signal and continues to 
        function. Events to dMAX continue happening.
    */
    CSL_FINST (hMcasp->regs->PWRDEMU, MCASP_PWRDEMU_FREE, OFF);
    
    /* If the dMAX handle doesn't exist, create an instance of dMAX. */
    if (!hDmax) {
        /* Allocate the dMAX object */
        pDmaxObj = MEM_alloc (device->segid, (sizeof(CSL_DmaxObj)+3)/4*4, 4);
    }
    else {
        /* 
            If the handle alreday exists, assign to the dMAX object. This is 
            done to avoid MEM_free in the DMD_Idle call.
        */
        pDmaxObj = hDmax;
    }

    /* if input device, use the event ID for the McASP RX. */
    /* The priority and the McASP number information is derived from params.*/
    if (device->mode == DEV_INPUT) {
        if (pParams->priority == CSL_DMAX_HI_PRIORITY) {
            if (pParams->moduleNum == CSL_MCASP_0) {
                pDmaxObj->eventUid = CSL_DMAX_HIPRIORITY_MCASP0RX_UID;
            }  
            else if (pParams->moduleNum == CSL_MCASP_1) {
                pDmaxObj->eventUid = CSL_DMAX_HIPRIORITY_MCASP1RX_UID;
            }
            else { 
                pDmaxObj->eventUid = CSL_DMAX_HIPRIORITY_MCASP2RX_UID;
            }
        }
        else {
            if (pParams->moduleNum == CSL_MCASP_0) {
                pDmaxObj->eventUid = CSL_DMAX_LOPRIORITY_MCASP0RX_UID;
            }
            else if (pParams->moduleNum == CSL_MCASP_1) {
                pDmaxObj->eventUid = CSL_DMAX_LOPRIORITY_MCASP1RX_UID;
            }
            else {
                pDmaxObj->eventUid = CSL_DMAX_LOPRIORITY_MCASP2RX_UID;
            } 
        }
    }
    /* if output device, use the event ID for the McASP TX. */
    /* The priority and the McASP number information is derived from params. */
    else {
        if (pParams->priority == CSL_DMAX_HI_PRIORITY) {
            if (pParams->moduleNum == CSL_MCASP_0) {
                pDmaxObj->eventUid = CSL_DMAX_HIPRIORITY_MCASP0TX_UID;
            }
            else if (pParams->moduleNum == CSL_MCASP_1) {
                pDmaxObj->eventUid = CSL_DMAX_HIPRIORITY_MCASP1TX_UID;
            }
            else {
                pDmaxObj->eventUid = CSL_DMAX_HIPRIORITY_MCASP2TX_UID;
            }
        }
        else {
            if (pParams->moduleNum == CSL_MCASP_0) {
                pDmaxObj->eventUid = CSL_DMAX_LOPRIORITY_MCASP0TX_UID;
            }
            else if (pParams->moduleNum == CSL_MCASP_1) {
                pDmaxObj->eventUid = CSL_DMAX_LOPRIORITY_MCASP1TX_UID;
            }
            else {
                pDmaxObj->eventUid = CSL_DMAX_LOPRIORITY_MCASP2TX_UID;
            }
        }
    }
    /* Request for parameter RAM based on the priority requested */
    if (pParams->priority == CSL_DMAX_HI_PRIORITY) {
        pDmaxObj->paramUid = CSL_DMAX_HIPRIORITY_PARAMETERENTRY_ANY;    
    }
    else {
        pDmaxObj->paramUid = CSL_DMAX_LOPRIORITY_PARAMETERENTRY_ANY;    
    }

    /* Create the dMAX instance    */
    hDmax = CSL_dmaxOpen(pDmaxObj, CSL_DMAX, NULL, &status);                       
    /* If error in creating handle, return */
    if (status != CSL_SOK) {
        return (DMD_ERR_SPOPEN);
    }

    /* Store the dMAX object in the device extension str for later reference */
    pDevExt->hDmax = hDmax;

    /* 
        Disable the events for now. Events will be enabled after the dMAX 
        instance is configured in the function DMD_configureDMAX
    */
    CSL_dmaxHwControl(hDmax, CSL_DMAX_CMD_EVENTDISABLE, NULL);
    
    return SYS_OK;
}

/*
    DMD_startClock function starts the McASP clocking circuitry. 
    This function implements the steps: 1, 2, 3 and 4 as described in the 
    '3.1.1 Transmit/Receive Section Initialization' Section of the McASP
    Peripheral User's Guide (SPRU878a)
    Starting McASP clocks even before any buffer has been issued is desirable 
    for ceratin comboCodes.
*/


Int DMD_startClock( DEV_Handle device)
{
    DMD_DeviceExtension *pDevExt;    /* Device extension str ptr */
    CSL_McaspHandle     hMcasp;      /* McASP handle */

    /* 
        The below structures could as well have been allocated and freed
        as part of the function. That may lead to memory holes. Also
        as the structure is small in size, allocating it from scratch
    */    
    CSL_McaspHwSetup HwSetup, * pHwSetup;    
    Int mcaspNum;

    const DMD_Params *pParams;

    Uint32 mask;
    Uint32 serializer;
    Int32 i;
    Uint32 gbl;


    pDevExt = (DMD_DeviceExtension *)device->object;
    pParams = pDevExt->pParams;
    mcaspNum = pParams->moduleNum;
    hMcasp = drvObj.hMcasp[mcaspNum];

    pHwSetup = &HwSetup;
    /* Perform different setup for RX or TX device */
    if (device->mode == DEV_INPUT) {
        gbl = 0x00000000;  
        /* Reset Receive Section */
        CSL_mcaspHwControl(hMcasp, CSL_MCASP_CMD_SET_RCV, &gbl);
        
        /* 
            The receive format unit bit mask register (RMASK) determines 
            which bits of the received data are masked off and padded with 
            a known value before being read by the CPU or DMA.
        */
        pHwSetup->rx.mask = pParams->rxMask;
        
        /* 
            The receive bit stream format register configures the 
            receive data format
        */
        pHwSetup->rx.fmt = pParams->rxFmt;

        /* 
            The receive frame sync control register configures 
            the receive frame sync
        */
        pHwSetup->rx.frSyncCtl = pParams->rxFSCtl;

        /* 
            The receive TDM time slot register specifies which 
            TDM time slot the receiver is active
        */
        pHwSetup->rx.tdm = pParams->rxTDM;

        /* 
            The receiver interrupt control register controls generation of the 
            McASP receive interrupt. When the register bit(s) is set to 1, 
            the occurrence of the enabled McASP condition(s) generates RINT.
        */
        pHwSetup->rx.intCtl = pParams->rxIntCtl; 

        /* Clear the Receiver Status Register */
        pHwSetup->rx.stat = 0;

        /* 
            The receiver DMA event control register contains a disable bit for
            the receiver DMA event. Keep the DMA events enabled
        */
        pHwSetup->rx.evtCtl = 0;

        /* 
            The receive clock control register configures the receive
            bit clock (ACLKR) and the receive clock generator
        */
        pHwSetup->rx.clk.clkSetupClk = pParams->rxClkSetup;

        /* 
            The receive high-frequency clock control register configures 
            the receive high-frequency master clock and the receive clock 
            generator.
        */
        pHwSetup->rx.clk.clkSetupHiClk = pParams->rxHIClkSetup;

        /* 
            The receive clock check control register configures the 
            receive clock failure detection circuit.
        */
        pHwSetup->rx.clk.clkChk = pParams->rxClkChk;

        /* Configure the receiver section for the McASP */
        CSL_mcaspHwControl(hMcasp, CSL_MCASP_CMD_CONFIG_RCV_SECTION, 
                           (void *)&pHwSetup->rx);

        /* 
            The pinMask variable in the params decide which serializers are to 
            be enabled for data reception. Calculate total number of 
            serializers that are active and also configure the serializer
            control register for them. Also set the PFUNC register to set 
            the serializers as McASP pins.
        */
        for (i = 0; i < 16; i++) {
            serializer = pParams->pinMask & (1 << i);
            mask = ~serializer;
            if (serializer) {
                /* Setting the bit to 0 sets it as McASP. */
                hMcasp->regs->PFUNC = hMcasp->regs->PFUNC & mask;

                /* 
                    The below control function sets the PDIR so not 
                    configured seperately
                */
                CSL_mcaspHwControl(hMcasp, CSL_MCASP_CMD_SET_SER_RCV, (void *)&i);

                /* 
                    Calculate the total number of serializers. 
                    This is needed for dMAX configuretaoin
                */
                pDevExt->numSerializers++;
            }
        }

        /* Set the clock pins as McASP pins */
        CSL_FINST (hMcasp->regs->PFUNC, MCASP_PFUNC_AFSR, MCASP);
        CSL_FINST (hMcasp->regs->PFUNC, MCASP_PFUNC_ACLKR, MCASP);
        CSL_FINST (hMcasp->regs->PFUNC, MCASP_PFUNC_AHCLKR, MCASP);

        /* Set the clock pin direction based on information in the params */
        if (pParams->rxHIClkSetup & 0x8000) {
            CSL_FINST (hMcasp->regs->PDIR, MCASP_PDIR_AHCLKR, OUTPUT);
        }
        else {
            CSL_FINST (hMcasp->regs->PDIR, MCASP_PDIR_AHCLKR, INPUT);     
        }

        if (pParams->rxClkSetup & 0x0020) {
            CSL_FINST (hMcasp->regs->PDIR, MCASP_PDIR_ACLKR, OUTPUT);
        }
        else {
            CSL_FINST (hMcasp->regs->PDIR, MCASP_PDIR_ACLKR, INPUT);     
        }

        if (pParams->rxFSCtl & 0x0002) {
            CSL_FINST (hMcasp->regs->PDIR, MCASP_PDIR_AFSR, OUTPUT);
        }
        else {
            CSL_FINST (hMcasp->regs->PDIR, MCASP_PDIR_AFSR, INPUT);     
        }

        /* 
            Calculate the mask required to bring High Clock and Bit Clock
            out of reset
        */
        mask =  CSL_MCASP_RGBLCTL_RHCLKRST_MASK   |
                CSL_MCASP_RGBLCTL_RCLKRST_MASK;
    }
    else
    {
        /* REFER THE COMMENTS IN THE DEV_INPUT CASE ABOVE */
        /* THE DESCRIPTION IS SAME. REPLACE RECEIVE WITH TRANSMIT */

        gbl = 0x00000000;  
        CSL_mcaspHwControl(hMcasp, CSL_MCASP_CMD_SET_XMT, &gbl);
        
        pHwSetup->tx.mask = pParams->rxMask;
        pHwSetup->tx.fmt = pParams->rxFmt;
        pHwSetup->tx.frSyncCtl = pParams->rxFSCtl;
        pHwSetup->tx.tdm = pParams->rxTDM;
        pHwSetup->tx.intCtl = pParams->rxIntCtl; 
        pHwSetup->tx.stat = NULL;
        pHwSetup->tx.evtCtl = NULL;
        pHwSetup->tx.clk.clkSetupClk = pParams->rxClkSetup;
        pHwSetup->tx.clk.clkSetupHiClk = pParams->rxHIClkSetup;
        pHwSetup->tx.clk.clkChk = pParams->rxClkChk;

        CSL_mcaspHwControl(hMcasp, CSL_MCASP_CMD_CONFIG_XMT_SECTION, 
                           (void *)&pHwSetup->tx);

        for (i = 0; i < 16; i++) {
            serializer = pParams->pinMask & (1 << i);
            mask = ~serializer;
            if (serializer) {
                hMcasp->regs->PFUNC = hMcasp->regs->PFUNC & mask;
                CSL_mcaspHwControl(hMcasp, CSL_MCASP_CMD_SET_SER_XMT, (void *)&i);
                pDevExt->numSerializers++;
            }
        }

        CSL_FINST (hMcasp->regs->PFUNC, MCASP_PFUNC_AFSR, MCASP);
        CSL_FINST (hMcasp->regs->PFUNC, MCASP_PFUNC_ACLKR, MCASP);
        CSL_FINST (hMcasp->regs->PFUNC, MCASP_PFUNC_AHCLKR, MCASP);

        if (pParams->rxHIClkSetup & 0x8000) {
            CSL_FINST (hMcasp->regs->PDIR, MCASP_PDIR_AHCLKX, OUTPUT);
        }
        else {
            CSL_FINST (hMcasp->regs->PDIR, MCASP_PDIR_AHCLKX, INPUT);     
        } 

        if (pParams->rxClkSetup & 0x0020) {
            CSL_FINST (hMcasp->regs->PDIR, MCASP_PDIR_ACLKX, OUTPUT);
        }
        else {
            CSL_FINST (hMcasp->regs->PDIR, MCASP_PDIR_ACLKX, INPUT);     
        }

        if (pParams->rxFSCtl & 0x0002) {
            CSL_FINST (hMcasp->regs->PDIR, MCASP_PDIR_AFSX, OUTPUT);
        } 
        else {
            CSL_FINST (hMcasp->regs->PDIR, MCASP_PDIR_AFSX, INPUT);     
        }

        /* 
            Calculate the mask required to bring High Clock and Bit Clock
            out of reset
        */
        mask =  CSL_MCASP_XGBLCTL_XHCLKRST_MASK   |
                CSL_MCASP_XGBLCTL_XCLKRST_MASK;
    }

    /* Bring the  High Clock and Bit Clock out of reset */    
    CSL_mcaspResetCtrl(hMcasp, mask );

    return SYS_OK;
}

/*
    DMD_init is called as part of BIOS startup.
    This function does one time setup for the driver. It hooks up the dMAX ISR
    and also clears the initial state variables.
    Please note HWI_dispatchPlug doesn't clear the cache. Thus, as a work 
    around the cache is cleaned after the function call.
*/

extern cregister volatile Uint32 ISTP;
/*
    The DMDInitialized variable is used by the watchDog function to understand
    if the driver init has been called or not.
*/

static Int DMDInitialized = NULL;


Void DMD_init()
{
    Int32 i;
    /* Instead of creating ICACHE obj, the code here uses */
    /* direct register programming */
    volatile Uint32 *L1PISAR = (Uint32 *)0x20000000;
    volatile Uint32 *L1PICR  = (Uint32 *)0x20000004;

    /* Call the CSL Init functions */
    CSL_chipInit(NULL);
    CSL_dmaxInit(NULL);
    CSL_mcaspInit(NULL);

    /* Clear the fields for the drvObj structure */
    for (i = 0; i < CSL_MCASP_CNT; i++) {
        drvObj.hMcasp[i] = (CSL_McaspHandle) NULL;
        drvObj.pDevice[2*i] = (DEV_Handle)NULL;
        drvObj.pDevice[2*i + 1] = (DEV_Handle)NULL;
    }
    drvObj.tcc = NULL;

    /* Hook up the dMAX ISR */
    HWI_dispatchPlug(CSL_INTC_EVENTID_DMAXEVTOUT1, (Fxn)DMD_dMAX_ISR, -1, NULL);

    /* Clear the IST from cache */
    *L1PISAR = (int)(ISTP);
    *L1PICR = 128;
    while((*L1PICR) & 0xFFFF);

    /* Enable the dMAX ISR */
    C62_enableIER(0x100);
    DMDInitialized = 1;
}

/*
    DMD_open is called by SIO_create. This function does the initialization for
    the device instance.
*/


Int DMD_open(DEV_Handle device, String name)
{
    DMD_DeviceExtension *pDevExt;

    /* 
        Allocate memory for device extension structure.
        This structure holds various information related to the instance
        of the driver
    */
    device->object = NULL;
    if (
        !(pDevExt = 
          MEM_alloc (device->segid, (sizeof(DMD_DeviceExtension)+3)/4*4, 4)
         )
       ) {
        return DMD_ERR_ALLOC;
    }

    device->object = (Ptr)pDevExt;

    /* Initialize the fields in the device extension structure */
    pDevExt->pParams = NULL;
    pDevExt->pTraceLog = NULL;
    pDevExt->runState = NULL; 
    pDevExt->errorState = NULL;
    pDevExt->tcc = NULL;
    pDevExt->hDmax = NULL;
    pDevExt->numSerializers = NULL;
    pDevExt->numQueuedRcv = NULL;
    pDevExt->numQueuedXmt = NULL;
    pDevExt->pp = NULL;

    /* 
        Create semaphore for device. This semaphore is used by the DMD_reclaim
        function to understand if a completed buffer is availabe to be sent to 
        the F/W. The semaphore is posted by the dMAX ISR function
    */
    if (!(pDevExt->sync = SEM_create (0, NULL))) {
        return DMD_ERR_ALLOC;    
    }

    /* 
        Initialize the driver internal transmit and receive Queues
        The driver object has Rx and Tx queues for receiving and transmiting
        buffers b/w driver and framework. The driver also maintains Rx and Tx 
        queues in the pDevExt. These are the internal queues.
        The driver receives the buffers from the framework and enqueues in the
        driver internal queue. Once the buffer is completed, the buffer is
        de-queued from the driver internal Rx queue and moved to driver
        internal Tx queue. The DMD_reclaim function de-queues the buffer from
        internal Tx queue and moves it to driver object's Tx queue. This is
        done so that the FW can receive the buffer. 
    */
    QUE_new (&pDevExt->rcvQue);
    QUE_new (&pDevExt->xmtQue);

    return (SYS_OK);
}

/*
    DMD_issue function is called when the F/W issues a buffer to the driver 
    using the SIO_issue function. Among other things, this function is 
    responsible for attaching the new buffer to the dMAX. Also, it puts 
    the received device frame to the input queues
*/


Int DMD_issue(DEV_Handle device)
{
    DMD_DeviceExtension    *pDevExt;        /* Device extension str ptr */
    DEV_Frame              *pFrame;         /* Pointer to device frame */
    CSL_DmaxHandle         hDmax;           /* dMAX handle */
    CSL_DmaxParameterEntry *pDmaxParam;     /* Pointer to dMAX param entry */
    
    Uint32 oldCSR;
    Int32 errorState;

    errorState = SYS_OK;
    /* Disable interrupts to maintain context */
    oldCSR = HWI_disable();

    pDevExt = (DMD_DeviceExtension *)device->object;
    hDmax = pDevExt->hDmax;
    
    /* 
        Load pointer to dMAX parameter entry
        Check if the dMAX is configured using high priority group or not.
        Based on that load the appropriate parameter entry
    */
    if (*(hDmax->hiTableEventEntryPtr) != 0) {
        pDmaxParam = (CSL_DmaxParameterEntry*)(&(hDmax->regs->HiMaxParam));
    }
    else {
        pDmaxParam = (CSL_DmaxParameterEntry*)(&(hDmax->regs->LoMaxParam));
    }

    /*
        Adjust the obtained parameter entry to point to the parameter entry
        used by this instance of dMAX
    */
    pDmaxParam = (CSL_DmaxParameterEntry *)((int ) pDmaxParam + hDmax->paramPtr*4);

    /* Dequeue the device frame from the device Rx queue */
    pFrame = (DEV_Frame *) QUE_get (device->todevice);

    /* Enqueue the frame in the device internal Rx Queue */
    QUE_put (&pDevExt->rcvQue, pFrame);

    /* 
        Increment the numQueuedRcv information. This variable indicates how
        many buffers are enqueued in the driver's internal Rx Queue
    */
    pDevExt->numQueuedRcv += 1;
    
    /* 
        If numQueuedRcv == 1, this is the first buffer that has been issued.
        Configure the dMAX and start the McASP. It is required that the McASP
        clocks be started before this step. The McASP clocks are started by 
        calling the DMD_startClock function. This function can be called  
        by using the DMD_START_CLK command for the SIO_ctrl function.
    */
    if (pDevExt->numQueuedRcv == 1) {
        if (pDevExt->runState == 1) {
            errorState = DMD_ERR_ENQUE;
        }
        else {
            /* Configure dMAX with the new params */
            DMD_configureDMAX (device, pFrame);
            DMD_startMcASP (device);
        }
    }
    /* More than 3 enQueued not supported */
    else if (pDevExt->numQueuedRcv == 4) {
        errorState = DMD_ERR_ENQUE;
    }

    /* 
        If numQueuedRcv is 2, it means we have one buffer already in use by the
        dMAX. Thus, here add the new buffer in the reload fields of the dMAX 
        params.
    */              
    else if (pDevExt->numQueuedRcv == 2) {
        /* 
            Update the buffer pointer in the SRC or DST reload fields based
            on whether the device is Rx or Tx.
            We check here the PP bit to determine which reload entry to update.
        */
        if (device->mode == DEV_INPUT) {
            /* If PP is not set, load the address in the DST RELOAD ADDRESS0 */
            if (!pDevExt->pp) {
                pDmaxParam->word10 = (Uint32) pFrame->addr;
                pDevExt->pp = 1;
            }
            else {
                pDmaxParam->word8 = (Uint32) pFrame->addr;
                pDevExt->pp = 0;
            }
        }
        else {
            /* Do similar processing if the device in TX */
            if (!pDevExt->pp) {
                pDmaxParam->word9 = (Uint32) pFrame->addr;
                pDevExt->pp = 1;
            }
            else {
                pDmaxParam->word7 = (Uint32) pFrame->addr;
                pDevExt->pp = 0;
            }
        }
    }

    /* 
        If numQueuedRcv is 3, it means we have two buffer already in use by 
        the dMAX. Thus, here add the new buffer in the reload fields of the 
        dMAX params
    */
    else if (pDevExt->numQueuedRcv == 3) {
        /* 
            If PP indicates SRC/DST Reload Address as 0, add the new buffer
            to the SRC/DST Reload Address 1. Please note at Reload Address 0 
            we have the 2nd buffer already hooked
        */
        if (device->mode == DEV_INPUT) {
            if (!pDevExt->pp) {
                pDmaxParam->word8 = (Uint32) pFrame->addr;
                pDevExt->pp = 1;
            }
            else {
                pDmaxParam->word10 = (Uint32) pFrame->addr;
                pDevExt->pp = 0;
            }
        }
        else {
            if (!pDevExt->pp) {
                pDmaxParam->word7 = (Uint32) pFrame->addr;
                pDevExt->pp = 1;
            }
            else {
                pDmaxParam->word9 = (Uint32) pFrame->addr;
                pDevExt->pp = 0;
            }
        }
    }

    /* Restore the global interrupts */
    HWI_restore (oldCSR);
    
    return (errorState);
}

/*
    DMD_reclaim function is called when the F/W reclaims a buffer from the 
    driver using the SIO_reclaim function. This function dequeues a buffer 
    from the driver's internal TX queue and enqueues in the Drivers's TX queue 
    so that the F/W can get it.
*/


Int DMD_reclaim(DEV_Handle device)
{
    DMD_DeviceExtension *pDevExt;
    DEV_Frame           *pFrame;

    pDevExt = (DMD_DeviceExtension *)(device->object);
        
    /* Wait on the semaphore. The semaphore is posted by the ISR */
    if (!SEM_pend(pDevExt->sync, device->timeout)) {
        return SYS_ETIMEOUT;
    }

    /* 
        Dequeue the buffer from the driver's internal RX queue and add it
        to driver's RX queue
    */
    pFrame = (DEV_Frame *) QUE_get (&pDevExt->xmtQue);
    QUE_put (device->fromdevice, pFrame);
    pDevExt->numQueuedXmt -= 1;

    /* Check if any error happened */
    if (pDevExt->errorState) {
        return (pDevExt->errorState);
    }

    return (SYS_OK);
}

/*
    DMD_close function is called by the SIO_delete function. This function 
    releases all the memory that was allocated for the instance of the DMD 
    driver.
*/


Int DMD_close(DEV_Handle device)
{
    DMD_DeviceExtension *pDevExt;
    const DMD_Params    *pParams;
    CSL_McaspHandle     hMcasp;            
    Int                 mcaspNum;
    CSL_DmaxHandle      hDmax;            

    pDevExt = (DMD_DeviceExtension *)device->object;
    pParams = pDevExt->pParams;
    hDmax = pDevExt->hDmax;
    mcaspNum = pParams->moduleNum;
    hMcasp = drvObj.hMcasp[mcaspNum];

    /* 
        One McASP handle is used for both RX and TX. Thus, free the 
        McASP object only if it not being used for bith RX and TX
    */
    if (
           !(drvObj.pDevice[mcaspNum*2 + DEV_INPUT] || 
             drvObj.pDevice[mcaspNum*2 + DEV_OUTPUT])
       ) {
        CSL_mcaspClose (hMcasp);
        MEM_free (device->segid, (void *)hMcasp, (sizeof(CSL_McaspObj)+3)/4*4);
        drvObj.hMcasp[mcaspNum] = NULL;
    }

    /* Free the dMAX object */
    if (hDmax) {
        MEM_free (device->segid, (void *)hDmax, (sizeof(CSL_DmaxObj)+3)/4*4);
    }
    
    /* Delete the semaphore used by pDevExt */
    SEM_delete(pDevExt->sync);

    /* Free the memory allocated for the device extension strcuture. */
    if (pDevExt) {
        MEM_free (
                     device->segid, (void *)pDevExt, 
                     (sizeof(DMD_DeviceExtension)+3)/4*4
                 );
    }

    return (SYS_OK);
}

/*
    DMD_idle function is called by the SIO_idle call. This function brings the
    driver to it's initial state. This allows the data I/O to be restarted
*/


Int DMD_idle(DEV_Handle device, Bool flush)
{
    DMD_DeviceExtension   *pDevExt;
    const DMD_Params      *pParams;
    Int mcaspNum;
    CSL_DmaxHandle        hDmax;            
    DEV_Frame             *pFrame;
    CSL_McaspHandle       hMcasp;            
    
    Int32 gbl;
    Int32 i;

    pDevExt = (DMD_DeviceExtension *)device->object;

    /* If the device not running, return */
    if (!pDevExt->runState) {
        return (SYS_OK);
    }

    hDmax = pDevExt->hDmax;
    pParams = pDevExt->pParams;
    mcaspNum = pParams->moduleNum;
    hMcasp = drvObj.hMcasp[mcaspNum];

    /* 
        Return any buffers that may be held by the device.
        If output mode and flush not requested, wait for all the
        buffers to be transmitted.
    */
    if ((device->mode == DEV_OUTPUT) && !flush) {
        /* Wait till the time all the buffers are not consumed */
        while (pDevExt->numQueuedRcv) {
            if (!SEM_pend(pDevExt->sync, device->timeout)) {
                return SYS_ETIMEOUT;
            }

            /* 
                Dequeue the buffer from the driver's internal RX queue and add 
                it to driver's internal TX queue
            */
            pFrame = (DEV_Frame *) QUE_get (&pDevExt->rcvQue);
            QUE_put (&pDevExt->xmtQue, pFrame);
            pDevExt->numQueuedXmt++;
        }
        gbl = 0x00000000;  
        if (device->mode == DEV_INPUT) {
            /* Reset Receive Section */
            CSL_mcaspHwControl(hMcasp, CSL_MCASP_CMD_SET_RCV, &gbl);
        }
        else {
            /* Reset Transmit Section */
            CSL_mcaspHwControl(hMcasp, CSL_MCASP_CMD_SET_XMT, &gbl);
        }

        CSL_dmaxHwControl( hDmax, CSL_DMAX_CMD_EVENTDISABLE, NULL );
    }
    else if (
                ((device->mode == DEV_OUTPUT) && flush) || 
                (device->mode == DEV_INPUT)
            ) {
        gbl = 0x00000000;  
        if (device->mode == DEV_INPUT) {
            /* Reset Receive Section */
            CSL_mcaspHwControl(hMcasp, CSL_MCASP_CMD_SET_RCV, &gbl);
        }
        else {
            /* Reset Transmit Section */
            CSL_mcaspHwControl(hMcasp, CSL_MCASP_CMD_SET_XMT, &gbl);
        }

        CSL_dmaxHwControl( hDmax, CSL_DMAX_CMD_EVENTDISABLE, NULL );

        /* Move all the buffers from driver's internal RX queue to TX queue */
        while (pDevExt->numQueuedRcv) {
            pFrame = (DEV_Frame *) QUE_get (&pDevExt->rcvQue);
            QUE_put (&pDevExt->xmtQue, pFrame);
            pDevExt->numQueuedRcv -= 1;
            pDevExt->numQueuedXmt += 1;
        }
    }

    /* Post the semaphores equal to number of buffers moved to the xmt queue */
    for (i = 0 ; i < pDevExt->numQueuedXmt; i++) {
        SEM_post (pDevExt->sync);
    }    

    pDevExt->numSerializers = NULL;
    pDevExt->pTraceLog = NULL;
    pDevExt->runState = NULL; 
    pDevExt->errorState = NULL;
    
    /* Free the TCC */
    drvObj.tcc = drvObj.tcc & ~(1 << pDevExt->tcc);

    pDevExt->tcc = NULL;

    /* 
        The next time the device may be passed different set of user 
        parameters. This could be a different McASP number may be.
        Thus, remove the driver instance from the drvObj structure.
    */
    drvObj.pDevice[mcaspNum*2 + device->mode] = NULL;

    CSL_dmaxClose (hDmax);
            
    return (SYS_OK);
}

/*
    DMD_ctrl function provides an interface by which the F/W can talk to the
    driver. Various commands that can be passed to the DMD_ctrl are listed in 
    the driver header file
*/


Int DMD_ctrl(DEV_Handle device, Uns cmd, Arg arg)
{
    DMD_DeviceExtension *pDevExt;
    const DMD_Params    *pParams;
    LOG_Obj             *trace;
    Int32               result;

    pDevExt = (DMD_DeviceExtension *)device->object;
    result = SYS_OK;

    switch (cmd) {
        case DMD_USR_PARAM:
            if (!( pParams = (const DMD_Params *) arg )) {
                return SYS_EBADIO;
            }

            if (result = DMD_openPeripheral (device, pParams)) {
                return result;
            }
            break;
        case DMD_START_CLK:
            if (result = DMD_startClock (device)) {
                return result;
            }
            break;
        case DMD_HOOK_LOG:
            if (!( trace = (LOG_Obj *) arg )) {
                return SYS_EBADIO;
            }
            else {
                pDevExt->pTraceLog = trace;
            }
            break;        
    }
    return (result);
}

/* 
    DMD_watchDog is provided so that the driver can recover from error.
    Specifically the error DMD_watchDog checks against are errors that
    may cause SIO_reclaim to hang. These errors can be the McASP errors
    that prevent it from providing events to the dMAX.

    This function may be added as an IDL function.
    The DMD_watchDog checks RSTAT/XSTAT for the various driver instances.

    Another option to recover from error is to provide the 'timeout'
    as part of the SIO_attrs. Thus, the driver will recover from SEM_pend
    in the DMD_reclaim function and will return back with error.
    If error happens, the McASP can be closed and restarted.
*/

Void DMD_watchDog( Void )
{
    DEV_Handle pDevice;
    DMD_DeviceExtension *pDevExt;
    CSL_McaspHandle hMcasp;            

    Uint32 i;
    Bool response;
    
    if (!DMDInitialized) {
        return;
    }

    /* Disable the TSK manager to prevent context switch */
    TSK_disable();
    for (i = 0; i < (CSL_MCASP_CNT*2); i++) {
        pDevice = drvObj.pDevice[i];     /* Get the device pointer */

        /* If device in not NULL, proceed */
        if (pDevice) {
            pDevExt = pDevice->object;       /* Get the device extension Str */
            hMcasp = drvObj.hMcasp[i>>1];    /* Get the McASP handle */

            /* 
                Check if the instance of driver running. If not running, 
                do nothing
            */
            if (pDevExt->runState) {
                if(pDevice->mode == DEV_INPUT) {
                    CSL_mcaspGetHwStatus (hMcasp, CSL_MCASP_QUERY_RSTAT_RERR, &response);
                    if (response) {
                        pDevExt->errorState = DMD_ERR_SPERR;
                    }
                }
                else {
                    CSL_mcaspGetHwStatus (hMcasp, CSL_MCASP_QUERY_XSTAT_XERR, &response);
                    if (response) {
                        pDevExt->errorState = DMD_ERR_SPERR;
                    }
                }
            }

            /* If error handle the error */
            if (pDevExt->errorState & DMD_ERR_SPERR) {
                /* 
                    If outstanding semaphore, post semaphore so that the 
                    task wakes up
                */
                if (!SEM_pend(pDevExt->sync, 0)) {
                    SEM_post (pDevExt->sync);
                }
            }
        }
    }

    /* Enable the TSK manager */
    TSK_enable ();
}

/* Device function table */
DEV_Fxns DMD_FXNS = {
    DMD_close,
    DMD_ctrl,
    DMD_idle,
    DMD_issue,
    DMD_open,
    NULL,
    DMD_reclaim,
};

