#include <stdint.h>
// chibios
#include <ch.h>
#include <hal.h>
// drive
#include "console.h"

/**
 * This is the configuration for the Serial Port
 * 1rst Parameter is the BaudRate
 */
static SerialConfig serial1_config = {
    115200,
    0,
    0,
    0,
};


/**
 * This function initialise the console
 * @param c The address of the console that needs to be initialised
 */
void console_init(struct console *c) {
    c->sd = &SD1;
    c->sc = &serial1_config;

    sdStart(c->sd, c->sc);
    palSetPadMode(GPIOA, 9, PAL_MODE_ALTERNATE(7));
    palSetPadMode(GPIOA, 10, PAL_MODE_ALTERNATE(7));
}

#ifdef TODO
int16_t console_readline(struct console *c, uint8_t *buffer, uint16_t count) {
    (void) c;
    (void) buffer;
    (void) count;
    return 0;
}

int16_t console_read(struct console *c, uint8_t *buffer, uint16_t count) {
    (void) c;
    (void) buffer;
    (void) count;
    return 0;
}
#endif

int16_t console_writeline(console_t *c, const char *buffer) {
    int16_t bytes_sent = 0;

    bytes_sent = console_write(c, buffer);
    bytes_sent += console_write(c, "\r\n");

    return bytes_sent;
}

int16_t console_write(struct console *c, const char *buffer) {
    uint16_t i = 0;

    while (buffer[i] != '\0') {
        sdPut(c->sd, buffer[i++]);
    }

    return i;
}

char* itoa(int i, char b[]) {
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i = -1;
    }
    int shifter = i;
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
}

void consolePrintStatus(cell_t cells[], console_t *console, acs_t *acs,
                           battery_t *batt) {

  char buffer[20];

  // Screen Clear
  console_write(console, "\033[2J\033[1;1H");

  // Cell 1
  console_write(console, "cell1: ");
  console_write(console, itoa(cells[0].voltage, buffer));

  console_write(console, " bal: ");
  console_write(console, itoa(cells[0].is_balancing, buffer));

  console_write(console, " HEALTH: ");
  switch(cells[0].health) {
  case CELL_HEALTH_UNKNOWN:
    console_write(console, " UNKNOWN ");
    break;
  case CELL_HEALTH_GOOD:
    console_write(console, " GOOD ");
    break;
  case CELL_HEALTH_OVERHEAT:
    console_write(console, " OVERHEAT ");
    break;
  case CELL_HEALTH_OVER_VOLTAGE:
    console_write(console, " OVERVOLTAGE ");
    break;
  case CELL_HEALTH_UNDER_VOLTAGE:
    console_write(console, " UNDERVOLTAGE ");
    break;
  case CELL_HEALTH_UNSPECIFIED_FAILURE:
    console_write(console, " UNSPECIFIED_FAILURE ");
    break;
  case CELL_HEALTH_DEAD:
    console_write(console, " DEAD ");
  }
  console_writeline(console, "");


  // Cell 2
  console_write(console, "cell2: ");
  console_write(console, itoa(cells[1].voltage, buffer));

  console_write(console, " bal: ");
  console_write(console, itoa(cells[1].is_balancing, buffer));

  console_write(console, " HEALTH: ");
  switch(cells[1].health) {
  case CELL_HEALTH_UNKNOWN:
    console_write(console, " UNKNOWN ");
    break;
  case CELL_HEALTH_GOOD:
    console_write(console, " GOOD ");
    break;
  case CELL_HEALTH_OVERHEAT:
    console_write(console, " OVERHEAT ");
    break;
  case CELL_HEALTH_OVER_VOLTAGE:
    console_write(console, " OVERVOLTAGE ");
    break;
  case CELL_HEALTH_UNDER_VOLTAGE:
    console_write(console, " UNDERVOLTAGE ");
    break;
  case CELL_HEALTH_UNSPECIFIED_FAILURE:
    console_write(console, " UNSPECIFIED_FAILURE ");
    break;
  case CELL_HEALTH_DEAD:
    console_write(console, " DEAD ");
  }
  console_writeline(console, "");

  // Cell 3
  console_write(console, "cell3: ");
  console_write(console, itoa(cells[2].voltage, buffer));

  console_write(console, " bal: ");
  console_write(console, itoa(cells[2].is_balancing, buffer));

  console_write(console, " HEALTH: ");
  switch(cells[2].health) {
  case CELL_HEALTH_UNKNOWN:
    console_write(console, " UNKNOWN ");
    break;
  case CELL_HEALTH_GOOD:
    console_write(console, " GOOD ");
    break;
  case CELL_HEALTH_OVERHEAT:
    console_write(console, " OVERHEAT ");
    break;
  case CELL_HEALTH_OVER_VOLTAGE:
    console_write(console, " OVERVOLTAGE ");
    break;
  case CELL_HEALTH_UNDER_VOLTAGE:
    console_write(console, " UNDERVOLTAGE ");
    break;
  case CELL_HEALTH_UNSPECIFIED_FAILURE:
    console_write(console, " UNSPECIFIED_FAILURE ");
    break;
  case CELL_HEALTH_DEAD:
    console_write(console, " DEAD ");
  }
  console_writeline(console, "");

  // Cell 4
  console_write(console, "cell4: ");
  console_write(console, itoa(cells[3].voltage, buffer));

  console_write(console, " bal: ");
  console_write(console, itoa(cells[3].is_balancing, buffer));

  console_write(console, " HEALTH: ");
  switch(cells[3].health) {
  case CELL_HEALTH_UNKNOWN:
    console_write(console, " UNKNOWN ");
    break;
  case CELL_HEALTH_GOOD:
    console_write(console, " GOOD ");
    break;
  case CELL_HEALTH_OVERHEAT:
    console_write(console, " OVERHEAT ");
    break;
  case CELL_HEALTH_OVER_VOLTAGE:
    console_write(console, " OVERVOLTAGE ");
    break;
  case CELL_HEALTH_UNDER_VOLTAGE:
    console_write(console, " UNDERVOLTAGE ");
    break;
  case CELL_HEALTH_UNSPECIFIED_FAILURE:
    console_write(console, " UNSPECIFIED_FAILURE ");
    break;
  case CELL_HEALTH_DEAD:
    console_write(console, " DEAD ");
  }
  console_writeline(console, "");

  // Cell 5
  console_write(console, "cell5: ");
  console_write(console, itoa(cells[4].voltage, buffer));

  console_write(console, " bal: ");
  console_write(console, itoa(cells[4].is_balancing, buffer));

  console_write(console, " HEALTH: ");
  switch(cells[4].health) {
  case CELL_HEALTH_UNKNOWN:
    console_write(console, " UNKNOWN ");
    break;
  case CELL_HEALTH_GOOD:
    console_write(console, " GOOD ");
    break;
  case CELL_HEALTH_OVERHEAT:
    console_write(console, " OVERHEAT ");
    break;
  case CELL_HEALTH_OVER_VOLTAGE:
    console_write(console, " OVERVOLTAGE ");
    break;
  case CELL_HEALTH_UNDER_VOLTAGE:
    console_write(console, " UNDERVOLTAGE ");
    break;
  case CELL_HEALTH_UNSPECIFIED_FAILURE:
    console_write(console, " UNSPECIFIED_FAILURE ");
    break;
  case CELL_HEALTH_DEAD:
    console_write(console, " DEAD ");
  }
  console_writeline(console, "");

  // Cell 6
  console_write(console, "cell6: ");
  console_write(console, itoa(cells[5].voltage, buffer));

  console_write(console, " bal: ");
  console_write(console, itoa(cells[5].is_balancing, buffer));

  console_write(console, " HEALTH: ");
  switch(cells[5].health) {
  case CELL_HEALTH_UNKNOWN:
    console_write(console, " UNKNOWN ");
    break;
  case CELL_HEALTH_GOOD:
    console_write(console, " GOOD ");
    break;
  case CELL_HEALTH_OVERHEAT:
    console_write(console, " OVERHEAT ");
    break;
  case CELL_HEALTH_OVER_VOLTAGE:
    console_write(console, " OVERVOLTAGE ");
    break;
  case CELL_HEALTH_UNDER_VOLTAGE:
    console_write(console, " UNDERVOLTAGE ");
    break;
  case CELL_HEALTH_UNSPECIFIED_FAILURE:
    console_write(console, " UNSPECIFIED_FAILURE ");
    break;
  case CELL_HEALTH_DEAD:
    console_write(console, " DEAD ");
  }
  console_writeline(console, "");

  // Total Voltage
  console_write(console, "Total: ");
  console_writeline(console, itoa(batt->voltage, buffer));

  // Output Current
  console_write(console, "OutCur: ");
  if (acs->current_direction == CURRENT_IS_NEGATIVE)
    console_write(console, "-");
  itoa(acs_read_currsens(acs), buffer);
  console_writeline(console, buffer);
}
