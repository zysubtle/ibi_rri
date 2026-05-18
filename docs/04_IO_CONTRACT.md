# 04 IO Contract v0.1 草案

> 本文档在 M1 阶段为 IO Contract 草案。公开 API、枚举值、字段类型和数值范围需在 M2 冻结后才能作为最终接口实现依据。

## 输入约束

| 字段 | 类型 | 说明 |
|---|---|---|
| `timestamp_ms` | `uint32_t` | 当前样本时间戳，单位 ms |
| `ppg[4]` | `int32_t[4]` | 4 路同步绿光 PPG raw |
| `allow_measure` | `bool` 或等价整型 | 外部门控，false 时立即停止输出 IBI |

固定输入条件：50Hz；每 20ms 输入 1 点；每点同时包含 4 路 PPG；PPG raw 为 signed 24bit，有效载体为 `int32_t`；当前无 ACC 输入。

## sample counter 校验

算法内部必须维护 sample counter，并与 `timestamp_ms` 进行一致性检查。标称采样周期为 20ms。如果 timestamp 前进 1000ms，则期望样本数约为 50。如果样本数与 timestamp 明显不一致，应产生 reject reason 或内部异常标志。具体容忍范围在 M2/M3 冻结。

## 输出事件字段

| 字段 | 建议类型 | 说明 | M2 待冻结项 |
|---|---|---|---|
| `timestamp_ms` | `uint32_t` | 当前 IBI 对应脉搏事件时间戳 | 对应峰值时间还是输出时间 |
| `ibi_ms` | `uint16_t` 或 `uint32_t` | 相邻有效脉搏间隔 | 类型与有效范围 |
| `beat_count` | `uint32_t` | 有效 beat 累计数 | 初始值规则 |
| `confidence` | `float` 或整数百分比 | IBI event 置信度 | 0–1 还是 0–100 |
| `signal_quality` | `float` 或整数百分比 | 当前选择通道信号质量 | 0–1 还是 0–100 |
| `selected_channel` | `uint8_t` | 主通道编号 | 0–3 还是 1–4 |
| `state` | enum | 当前状态机状态 | 枚举值 |
| `reject_reason` | enum | 拒绝 / 异常原因 | 枚举值 |

## 状态机草案

`INIT / ACQUIRE / TRACK / HOLD / REACQUIRE / INVALID`

## reject reason 草案

M2 需要冻结枚举。候选项：`NONE`、`NOT_READY`、`MEASURE_NOT_ALLOWED`、`TIMESTAMP_INVALID`、`SAMPLE_COUNT_MISMATCH`、`SIGNAL_LOW_QUALITY`、`NO_VALID_CHANNEL`、`IBI_TOO_SHORT`、`IBI_TOO_LONG`、`PEAK_UNSTABLE`。

## 示例 CSV Contract

路径：`tests/fixtures/sample_ppg.csv`

字段：`timestamp_ms,PPG_G1,PPG_G2,PPG_G3,PPG_G4`

该 fixture 不包含 `allow_measure`，host 测试工具需要默认注入 `allow_measure=true`，并单独构造门控测试场景。
