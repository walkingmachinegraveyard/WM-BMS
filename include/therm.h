#ifndef THERM_H
#define THERM_H

#include <stdint.h>
#include "ad72.h"
#include "Board.h"


// ================================================================
// The therm_t structure delaration is defined in bms.h to prevent
// circular inclusion
// ================================================================


extern void therm_read_temp(therm_t therm[], ad7280a_t *ad72);
extern void therm_init(therm_t therm[]);

#endif // THERM_H
