#include "ppg_ibi.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

int main(void)
{
    ppg_ibi_config_t config;
    ppg_ibi_context_t ctx;
    ppg_ibi_sample_t sample;
    ppg_ibi_event_t event;
    ppg_ibi_status_t status;
    bool has_event = true;

    ppg_ibi_config_default(&config);
    assert(config.sample_rate_hz == 50u);
    assert(config.sample_period_ms == 20u);
    assert(config.num_channels == 4u);
    assert(config.min_ibi_ms == 300u);
    assert(config.max_ibi_ms == 2000u);

    status = ppg_ibi_init(&ctx, &config);
    assert(status == PPG_IBI_STATUS_OK);

    sample.timestamp_ms = 0u;
    sample.ppg[0] = 10;
    sample.ppg[1] = 11;
    sample.ppg[2] = 12;
    sample.ppg[3] = 13;
    sample.allow_measure = true;

    status = ppg_ibi_process(&ctx, &sample, &event, &has_event);
    assert(status == PPG_IBI_STATUS_OK);
    assert(has_event == false);
    assert(event.reject_reason == PPG_IBI_REJECT_NOT_READY);

    sample.timestamp_ms = 20u;
    sample.allow_measure = false;
    status = ppg_ibi_process(&ctx, &sample, &event, &has_event);
    assert(status == PPG_IBI_STATUS_OK);
    assert(has_event == false);
    assert(event.reject_reason == PPG_IBI_REJECT_MEASURE_NOT_ALLOWED);
    assert(event.state == PPG_IBI_STATE_REACQUIRE);

    assert(ppg_ibi_version() != NULL);

    return 0;
}
