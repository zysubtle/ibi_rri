# 04 IO Contract v0.2 (M2 冻结)

## 输入（冻结）

| 字段 | 类型 | 约束 |
|---|---|---|
| `timestamp_ms` | `uint32_t` | 单位 ms，标称每 20ms 递增 |
| `ppg[4]` | `int32_t[4]` | 4 路同步绿光 PPG，24bit signed 载于 `int32_t` |
| `allow_measure` | `bool` | `false` 时立即停止输出 IBI |

固定输入条件：4 通道、50Hz、20ms。

## 输出 event（冻结）

仅包含以下字段：

- `timestamp_ms`
- `ibi_ms`
- `beat_count`
- `confidence`
- `signal_quality`
- `selected_channel`
- `state`
- `reject_reason`

类型与范围：

- `ibi_ms: uint16_t`，M2 有效范围定义为 300–2000ms（M2 骨架默认不产生有效 IBI）。
- `confidence: float`，0.0f–1.0f。
- `signal_quality: float`，0.0f–1.0f。
- `selected_channel: uint8_t`，有效通道 0–3；无有效通道为 `PPG_IBI_INVALID_CHANNEL`。

## 状态（冻结）

`INIT / ACQUIRE / TRACK / HOLD / REACQUIRE / INVALID`

## reject reason（冻结）

`NONE`、`NOT_READY`、`MEASURE_NOT_ALLOWED`、`TIMESTAMP_INVALID`、`SAMPLE_COUNT_MISMATCH`、`SIGNAL_LOW_QUALITY`、`NO_VALID_CHANNEL`、`IBI_TOO_SHORT`、`IBI_TOO_LONG`、`PEAK_UNSTABLE`、`INTERNAL_ERROR`。

## 配置默认值（冻结）

- `timestamp_tolerance_ms = 20`
- `sample_count_tolerance = 1`

## M2 与 M3 边界

M2 仅要求维护基础计数器与占位行为（包括 `allow_measure=false` 时立即停止输出）。
完整的 timestamp/sample counter 异常识别与处理在 M3 实现。

## Fixture Contract

路径：`tests/fixtures/sample_ppg.csv`

字段固定：`timestamp_ms,PPG_G1,PPG_G2,PPG_G3,PPG_G4`
