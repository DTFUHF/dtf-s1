
#ifndef _RC_OUTPUT_H_
#define _RC_OUTPUT_H_

#include "board.h"

#define MAX_CHANNEL_OUTPUTS 32

enum {
  OUTPUT_AUTO = 0,
  OUTPUT_PPM,
  OUTPUT_SBUS,
  OUTPUT_SUMD,
  NUM_OUTPUT_MODES,
};

// TODO: per channel (minor) failsafe types
enum {
  FAILSAFE_AUTODETECT = 0, // if no failsafe values are set, stop output. if failsafe values are set, use them
  FAILSAFE_STOPOUTPUT,
  FAILSAFE_SAVEDVALUES,
  FAILSAFE_HOLDLAST,
  NUM_FAILSAFE_MODES,
};

void init_output(void);

typedef void (*outputStopFunc)(void);
typedef void (*outputUpdateFunc)(void);

typedef struct output_handle_functions_typedef {
  outputStopFunc outputStop;
  outputUpdateFunc outputUpdate;
} output_handle_functions_t;

#endif
