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
** \file sys/libiec/util.c \n
** \author Spiro Trikaliotis \n
** \version $Id: util.c,v 1.2 2005-03-02 18:17:22 strik Exp $ \n
** \n
** \brief Some utility functions for the IEC library
**
****************************************************************/

#include <wdm.h>
#include "cbm_driver.h"
#include "i_iec.h"

/*! \brief Schedule a timeout

 This functions schedules a timeout.
 Scheduling means that other threads have the opportunity to
 use the processor while we're waiting.

 \param Howlong
   How long to wait (in us)
*/
VOID
cbmiec_schedule_timeout(IN ULONG Howlong)
{
    LARGE_INTEGER li;
    NTSTATUS ntStatus;
    KTIMER cbmTimer;

    FUNC_ENTER();

    // Note: The following timer is NEVER fired! We just use it
    //       to allow a KeWaitForSingleObject()-determined timeout!

    KeInitializeTimer (&cbmTimer);

    li.QuadPart  = - (LONG) Howlong;
    li.QuadPart *= 10i64;

    ntStatus = KeWaitForSingleObject(&cbmTimer, UserRequest,
                  KernelMode, FALSE, &li);

    FUNC_LEAVE();
}

/*! \brief Wait for a timeout

 This functions waits for a timeout.
 Waiting means that we want to have an exact timing, so
 don't give away the processor.

 \param Howlong
   How long to wait (in us)
*/
VOID
cbmiec_udelay(IN ULONG Howlong)
{
    LARGE_INTEGER li;
    NTSTATUS ntStatus;

    FUNC_ENTER();

#if 1
    KeStallExecutionProcessor(Howlong);
#else
    li.QuadPart  = - (LONG) Howlong;
    li.QuadPart *= 10i64;

    ntStatus = KeDelayExecutionThread(KernelMode, FALSE, &li);
#endif

    FUNC_LEAVE();
}


/*! Helper for cbmiec_block_irq */
static KIRQL i_blockirq_previous_irql;

/*! Helper for cbmiec_block_irq */
DBGDO(static LONG i_blockirq_usage_count = 0;)

/*! \brief Block all interrupts

 This functions blocks all interrupt, thus that we cannot
 be interrupted while executing some critical things.

 This should not be used for big time periods.
*/
VOID
cbmiec_block_irq(VOID)
{
    FUNC_ENTER();

    DBGDO(DBG_ASSERT(InterlockedIncrement(&i_blockirq_usage_count)==1));

    KeRaiseIrql(HIGH_LEVEL, &i_blockirq_previous_irql);
    __asm cli

    FUNC_LEAVE();
}

/*! \brief Release the interrupts

 This functions releases all interrupt, undoing a
 previous cbmiec_block_irq() call.
*/
VOID
cbmiec_release_irq(VOID)
{
    FUNC_ENTER();

    __asm sti
    KeLowerIrql(i_blockirq_previous_irql);

    DBGDO(DBG_ASSERT(InterlockedDecrement(&i_blockirq_usage_count)==0));

    FUNC_LEAVE();
}
