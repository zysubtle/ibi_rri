#include "ppg_ibi.h"
#include "ppg_ibi_config.h"
#include "ppg_ibi_internal.h"

#include <string.h>

static bool ppg_ibi_config_is_valid(const ppg_ibi_config_t *config)
{
    if (config == NULL)
    {
        return false;
    }

    if ((config->sample_rate_hz != PPG_IBI_SAMPLE_RATE_HZ) ||
        (config->sample_period_ms != PPG_IBI_SAMPLE_PERIOD_MS) ||
        (config->num_channels != PPG_IBI_NUM_CHANNELS))
    {
        return false;
    }

    if ((config->min_ibi_ms == 0u) || (config->max_ibi_ms < config->min_ibi_ms))
    {
        return false;
    }

    return true;
}

const char *ppg_ibi_version(void)
{
    return PPG_IBI_VERSION_STRING;
}

void ppg_ibi_config_default(ppg_ibi_config_t *config)
{
    if (config == NULL)
    {
        return;
    }

    config->sample_rate_hz = PPG_IBI_SAMPLE_RATE_HZ;
    config->sample_period_ms = PPG_IBI_SAMPLE_PERIOD_MS;
    config->num_channels = PPG_IBI_NUM_CHANNELS;
    config->min_ibi_ms = PPG_IBI_DEFAULT_MIN_IBI_MS;
    config->max_ibi_ms = PPG_IBI_DEFAULT_MAX_IBI_MS;
    config->timestamp_tolerance_ms = PPG_IBI_DEFAULT_TIMESTAMP_TOLERANCE_MS;
    config->sample_count_tolerance = PPG_IBI_DEFAULT_SAMPLE_COUNT_TOLERANCE;
}

ppg_ibi_status_t ppg_ibi_init(ppg_ibi_context_t *ctx, const ppg_ibi_config_t *config)
{
    if ((ctx == NULL) || (config == NULL))
    {
        return PPG_IBI_STATUS_NULL_ARG;
    }

    if (!ppg_ibi_config_is_valid(config))
    {
        return PPG_IBI_STATUS_BAD_CONFIG;
    }

    (void)memset(ctx, 0, sizeof(*ctx));
    ctx->config = *config;
    ctx->state = PPG_IBI_STATE_INIT;
    ctx->last_reject_reason = PPG_IBI_REJECT_NOT_READY;
    ctx->initialized = true;

    return PPG_IBI_STATUS_OK;
}

void ppg_ibi_reset(ppg_ibi_context_t *ctx)
{
    if (ctx == NULL)
    {
        return;
    }

    ctx->state = PPG_IBI_STATE_INIT;
    ctx->last_reject_reason = PPG_IBI_REJECT_NOT_READY;
    ctx->sample_count = 0u;
    ctx->valid_sample_count = 0u;
    ctx->last_timestamp_ms = 0u;
    ctx->beat_count = 0u;
    ctx->have_last_timestamp = false;
}

ppg_ibi_status_t ppg_ibi_process(ppg_ibi_context_t *ctx,
                                 const ppg_ibi_sample_t *sample,
                                 ppg_ibi_event_t *event,
                                 bool *has_event)
{
    if ((ctx == NULL) || (sample == NULL) || (event == NULL) || (has_event == NULL))
    {
        return PPG_IBI_STATUS_NULL_ARG;
    }

    if (!ctx->initialized)
    {
        return PPG_IBI_STATUS_NOT_INITIALIZED;
    }

    *has_event = false;
    event->timestamp_ms = sample->timestamp_ms;
    event->ibi_ms = 0u;
    event->beat_count = ctx->beat_count;
    event->confidence = 0.0f;
    event->signal_quality = 0.0f;
    event->selected_channel = PPG_IBI_INVALID_CHANNEL;

    ctx->sample_count += 1u;
    ctx->last_timestamp_ms = sample->timestamp_ms;
    ctx->have_last_timestamp = true;

    if (!sample->allow_measure)
    {
        ctx->state = PPG_IBI_STATE_REACQUIRE;
        ctx->last_reject_reason = PPG_IBI_REJECT_MEASURE_NOT_ALLOWED;
    }
    else
    {
        ctx->state = PPG_IBI_STATE_ACQUIRE;
        ctx->valid_sample_count += 1u;
        ctx->last_reject_reason = PPG_IBI_REJECT_NOT_READY;
    }

    event->state = ctx->state;
    event->reject_reason = ctx->last_reject_reason;

    return PPG_IBI_STATUS_OK;
}
