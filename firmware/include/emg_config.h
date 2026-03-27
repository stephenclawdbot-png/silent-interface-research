/**
 * SILENT-001 Firmware - EMG Configuration Header
 */

#ifndef EMG_CONFIG_H
#define EMG_CONFIG_H

#include <stdint.h>

/* EMG Channel Mapping */
#define EMG_CH_LLS  0   /* Laryngeal Left Superior */
#define EMG_CH_LRS  1   /* Laryngeal Right Superior */
#define EMG_CH_LLI  2   /* Laryngeal Left Inferior */
#define EMG_CH_LRI  3   /* Laryngeal Right Inferior */

/* Filter Configuration */
#define EMG_BANDPASS_LOW_HZ    10.0f   /* Lower cutoff frequency */
#define EMG_BANDPASS_HIGH_HZ    500.0f  /* Upper cutoff frequency */
#define EMG_NOTCH_FREQ_HZ       50.0f   /* Mains hum frequency */

/* ADC Scaling */
#define ADC_VREF_MV             3300
#define ADC_RESOLUTION_BITS     12
#define ADC_MAX_VALUE           (1 << ADC_RESOLUTION_BITS)

/* Command vocabulary */
typedef enum {
    CMD_NULL = 0,
    CMD_VOWEL_A,
    CMD_VOWEL_E,
    CMD_VOWEL_I,
    CMD_VOWEL_O,
    CMD_VOWEL_U,
    CMD_CONSONANT_B,
    CMD_CONSONANT_P,
    CMD_CONSONANT_M,
    CMD_WORD_YES,
    CMD_WORD_NO,
    CMD_WORD_HELLO,
    CMD_WORD_STOP,
    CMD_WORD_GO,
    CMD_WORD_OK,
    CMD_EMERGENCY,
    CMD_COUNT
} Command_t;

/* Function prototypes */
const char* CommandToString(Command_t cmd);
Command_t PatternToCommand(uint8_t pattern);

#endif /* EMG_CONFIG_H */