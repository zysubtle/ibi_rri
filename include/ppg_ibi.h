#ifndef PPG_IBI_H
#define PPG_IBI_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define PPG_IBI_NUM_CHANNELS (4u)
#define PPG_IBI_SAMPLE_RATE_HZ (50u)
#define PPG_IBI_SAMPLE_PERIOD_MS (20u)
#define PPG_IBI_INVALID_CHANNEL (255u)

typedef enum
{
    PPG_IBI_STATUS_OK = 0,
    PPG_IBI_STATUS_NULL_ARG,
    PPG_IBI_STATUS_BAD_CONFIG,
    PPG_IBI_STATUS_NOT_INITIALIZED
} ppg_ibi_status_t;

typedef enum
{
    PPG_IBI_STATE_INIT = 0,
    PPG_IBI_STATE_ACQUIRE,
    PPG_IBI_STATE_TRACK,
    PPG_IBI_STATE_HOLD,
    PPG_IBI_STATE_REACQUIRE,
    PPG_IBI_STATE_INVALID
} ppg_ibi_state_t;

typedef enum
{
    PPG_IBI_REJECT_NONE = 0,
    PPG_IBI_REJECT_NOT_READY,
    PPG_IBI_REJECT_MEASURE_NOT_ALLOWED,
    PPG_IBI_REJECT_TIMESTAMP_INVALID,
    PPG_IBI_REJECT_SAMPLE_COUNT_MISMATCH,
    PPG_IBI_REJECT_SIGNAL_LOW_QUALITY,
    PPG_IBI_REJECT_NO_VALID_CHANNEL,
    PPG_IBI_REJECT_IBI_TOO_SHORT,
    PPG_IBI_REJECT_IBI_TOO_LONG,
    PPG_IBI_REJECT_PEAK_UNSTABLE,
    PPG_IBI_REJECT_INTERNAL_ERROR
} ppg_ibi_reject_reason_t;

typedef struct
{
    uint32_t timestamp_ms;
    int32_t ppg[PPG_IBI_NUM_CHANNELS];
    bool allow_measure;
} ppg_ibi_sample_t;

typedef struct
{
    uint32_t timestamp_ms;
    uint16_t ibi_ms;
    uint32_t beat_count;
    float confidence;
    float signal_quality;
    uint8_t selected_channel;
    ppg_ibi_state_t state;
    ppg_ibi_reject_reason_t reject_reason;
} ppg_ibi_event_t;

typedef struct
{
    uint16_t sample_rate_hz;
    uint16_t sample_period_ms;
    uint8_t num_channels;
    uint16_t min_ibi_ms;
    uint16_t max_ibi_ms;
    uint16_t timestamp_tolerance_ms;
    uint16_t sample_count_tolerance;
} ppg_ibi_config_t;

typedef struct
{
    ppg_ibi_config_t config;
    ppg_ibi_state_t state;
    ppg_ibi_reject_reason_t last_reject_reason;
    uint32_t sample_count;
    uint32_t valid_sample_count;
    uint32_t last_timestamp_ms;
    uint32_t beat_count;
    bool initialized;
    bool have_last_timestamp;
} ppg_ibi_context_t;

const char *ppg_ibi_version(void);
void ppg_ibi_config_default(ppg_ibi_config_t *config);
ppg_ibi_status_t ppg_ibi_init(ppg_ibi_context_t *ctx, const ppg_ibi_config_t *config);
void ppg_ibi_reset(ppg_ibi_context_t *ctx);
ppg_ibi_status_t ppg_ibi_process(ppg_ibi_context_t *ctx,
                                 const ppg_ibi_sample_t *sample,
                                 ppg_ibi_event_t *event,
                                 bool *has_event);

#endif /* PPG_IBI_H */
