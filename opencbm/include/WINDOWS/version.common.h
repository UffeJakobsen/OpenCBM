/* $Id: version.common.h,v 1.2 2005-04-17 15:32:17 strik Exp $ */

#include "version.h"

#undef VER_PRODUCTNAME_STR
#undef VER_PRODUCTVERSION
#undef VER_PRODUCTVERSION_STR
#undef VER_COMPANYNAME_STR

#define VER_LEGALCOPYRIGHT_STR      "(c) 2001-2005 Spiro Trikaliotis, based on cbm4linux, (c) 1999-2005 by Michael Klein"
#define VER_COMPANYNAME_STR         "(Spiro Trikaliotis, private)"

#define VER_PRODUCTVERSION          CBM4WIN_VERSION_MAJOR,CBM4WIN_VERSION_MINOR,CBM4WIN_VERSION_SUBMINOR,CBM4WIN_VERSION_DEVEL
#define VER_FILEVERSION             VER_PRODUCTVERSION
#define VER_PRODUCTVERSION_STR      CBM4WIN_VERSION_STRING
#define VER_FILEVERSION_STR         VER_PRODUCTVERSION_STR
#define VER_LANGNEUTRAL
#define VER_PRODUCTNAME_STR         "CBM4WIN - Accessing CBM drives from Windows"