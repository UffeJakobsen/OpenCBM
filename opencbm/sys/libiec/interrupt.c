/*
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version
 *  2 of the License, or (at your option) any later version.
 *
 *  Copyright 1999-2004 Michael Klein <michael.klein@puffin.lb.shuttle.de>
 *  Copyright 2001-2004 Spiro Trikaliotis <cbm4win@trikaliotis.net>
 *
 */

/*! ************************************************************** 
** \file sys/libiec/interrupt.c \n
** \author Spiro Trikaliotis \n
** \version $Id: interrupt.c,v 1.2 2005-03-02 18:17:22 strik Exp $ \n
** \authors Based on code from
**    Michael Klein <michael.klein@puffin.lb.shuttle.de>
** \n
** \brief Interrupt handler for the IEC bus
**
****************************************************************/

#include <wdm.h>
#include "cbm_driver.h"
#include "i_iec.h"

/*! \brief Interrupt handler for the IEC bus

 This function is the interrupt handler for the IEC bus.
 Its only purpose is to count the number of interrupts
 for cbmiec_wait_for_listener().

 \param Pdx
   Pointer to the device extension.

 \return
   TRUE if this interrupt was generated by the parallel port,
\n FALSE else
*/
BOOLEAN
cbmiec_interrupt(IN PDEVICE_EXTENSION Pdx)
{
    BOOLEAN isMyInt;
    UCHAR irqResult;

    FUNC_ENTER();

    /* acknowledge the interrupt */
    irqResult = READ_PORT_UCHAR(IN_PORT); 
    DBG_PPORT((DBG_PREFIX "irqResult before:     %02x", irqResult));

    DBG_IRQ(("Interrupt occurred"));

/*! \bug this does not work. At least my parallel port card does not
    change bit 2 in any way! */

/*   
    if ((irqResult & 0x04) == 0)

    Thus, we check DATA_IN directly:

/*! \bug Anyway, there is still a chance we get the wrong IRQ. What to do about this?
*/
    if (!CBMIEC_GET(PP_DATA_IN))
    {
        isMyInt = TRUE;

        DBG_IRQ(("It's a PP interrupt!"));

        if (Pdx->IrqCount != 0)
        {
            if (InterlockedDecrement(&Pdx->IrqCount) == 0)
            {
                CBMIEC_SET(PP_CLK_OUT);
                DBG_SUCCESS((DBG_PREFIX "continue to send"));

#ifdef USE_DPC
                // Request a DPC which will wake up wait_for_listener()

                DBG_DPC((DBG_PREFIX "IoRequestDpc() "));

                DBG_IRQL( >= DISPATCH_LEVEL); // in fact, at DIRQL
                IoRequestDpc(Pdx->Fdo, Pdx->IrpQueue.CurrentIrp, NULL);

#endif // #ifdef USE_DPC

            }
        }
        else
        {
            DBG_ERROR((DBG_PREFIX "****************************************************"));
            DBG_ERROR((DBG_PREFIX "Interrupt occurred, BUT ALREADY IRQCOUNT=0! ********"));
            DBG_ERROR((DBG_PREFIX "****************************************************"));
        }
    }
    else
    {
        isMyInt = FALSE;
        DBG_WARN((DBG_PREFIX "********************************************"));
        DBG_WARN((DBG_PREFIX "Interrupt occurred, but not for me! ********"));
        DBG_WARN((DBG_PREFIX "********************************************"));
    }

    irqResult = READ_PORT_UCHAR(IN_PORT); 
    DBG_PPORT((DBG_PREFIX "irqResult afterwards: %02x", irqResult));

    FUNC_LEAVE_BOOL(isMyInt);
}
