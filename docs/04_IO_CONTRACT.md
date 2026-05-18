# 04 IO Contract v0.2 (M2 冻结)

M2 冻结范围：输入、输出、状态、reject reason、字段类型、默认范围。M2 仅冻结 API Contract，不包含真实 IBI 算法输出。

## 输入 Contract

| 字段 | 类型 | 约束 |
|---|---|---|
| `timestamp_ms` | `uint32_t` | 单位 ms，50Hz 输入下 nominal 步进 20ms |
| `ppg[4]` | `int32_t[4]` | 4 路同步绿光 PPG raw，24bit signed 载体 |
| `allow_measure` | `bool` | `false` 时立即停止输出 IBI，并进入 `REACQUIRE` |

固定参数：4 通道，50Hz，20ms。

## 输出 Event Contract

| 字段 | 类型 | M2 说明 |
|---|---|---|
| `timestamp_ms` | `uint32_t` | 当前处理样本时间戳 |
| `ibi_ms` | `uint16_t` | M2 默认输出 0；有效范围冻结为 300–2000ms |
| `beat_count` | `uint32_t` | 当前累计 beat 计数（M2 骨架默认不增长） |
| `confidence` | `float` | 0.0f–1.0f，M2 默认 0.0f |
| `signal_quality` | `float` | 0.0f–1.0f，M2 默认 0.0f |
| `selected_channel` | `uint8_t` | 0–3；无有效通道时 `PPG_IBI_INVALID_CHANNEL` |
| `state` | `ppg_ibi_state_t` | 状态机占位状态 |
| `reject_reason` | `ppg_ibi_reject_reason_t` | 当前拒绝原因 |

`has_event`：M2 默认恒为 `false`。

## 状态与 Reject Reason（M2 冻结）

- 状态：`INIT / ACQUIRE / TRACK / HOLD / REACQUIRE / INVALID`
- reject reason：
  - `NONE`
  - `NOT_READY`
  - `MEASURE_NOT_ALLOWED`
  - `TIMESTAMP_INVALID`
  - `SAMPLE_COUNT_MISMATCH`
  - `SIGNAL_LOW_QUALITY`
  - `NO_VALID_CHANNEL`
  - `IBI_TOO_SHORT`
  - `IBI_TOO_LONG`
  - `PEAK_UNSTABLE`
  - `INTERNAL_ERROR`

## 配置默认值（M2）

- `sample_rate_hz=50`
- `sample_period_ms=20`
- `num_channels=4`
- `min_ibi_ms=300`
- `max_ibi_ms=2000`
- `timestamp_tolerance_ms=20`
- `sample_count_tolerance=1`

## 说明

M2 不实现滤波、SQI、峰值检测或真实 IBI 检测；仅提供可编译、可测试、可追踪的 C API 骨架。
