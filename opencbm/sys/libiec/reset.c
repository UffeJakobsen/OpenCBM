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
** \file sys/libiec/reset.c \n
** \author Spiro Trikaliotis \n
** \version $Id: reset.c,v 1.2 2005-03-02 18:17:22 strik Exp $ \n
** \authors Based on code from
**    Michael Klein <michael.klein@puffin.lb.shuttle.de>
** \n
** \brief Send a RESET on the IEC bus
**
****************************************************************/

#include <wdm.h>
#include "cbm_driver.h"
#include "i_iec.h"

/*! \brief Send a RESET to the IEC bus

 This functions sends a RESET on the IEC bus.

 \param Pdx
   Pointer to the device extension.

 \return 
   If the routine succeeds, it returns STATUS_SUCCESS. Otherwise, it
   returns one of the error status values.
*/
NTSTATUS
cbmiec_reset(IN PDEVICE_EXTENSION Pdx)
{
    FUNC_ENTER();

    CBMIEC_RELEASE(PP_DATA_OUT | PP_ATN_OUT | PP_CLK_OUT | PP_LP_BIDIR | PP_LP_IRQ);

    CBMIEC_SET(PP_RESET_OUT);
    cbmiec_schedule_timeout(libiec_global_timeouts.T_holdreset);
    CBMIEC_RELEASE(PP_RESET_OUT);

    DBG_SUCCESS((DBG_PREFIX "sleeping after RESET..."));
    cbmiec_schedule_timeout(libiec_global_timeouts.T_afterreset);
    CBMIEC_SET(PP_CLK_OUT);

    FUNC_LEAVE_NTSTATUS_CONST(STATUS_SUCCESS);
}
