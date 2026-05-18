# 01 Decision Log v0.1

| ID | 决策 | 等级 | 状态 | 说明 |
|---|---|---:|---|---|
| D-001 | IBI 用途为 HRV / RMSSD 后续分析 | S0 | 已确认 | 算法内部不计算 RMSSD |
| D-002 | 输出逐搏 IBI event | S0 | 已确认 | 每个有效脉搏间隔输出一次 event |
| D-003 | 算法内部只输出 IBI，不输出 HR | S0 | 已确认 | `hr_bpm` 不进入输出字段 |
| D-004 | 输入为 4 路绿光 PPG，50Hz，int32_t，24bit，signed | S0 | 已确认 | 4 路严格同步 |
| D-005 | 每 20ms 逐点调用 `process()` | S0 | 已确认 | 每点同时输入 4 路样本 |
| D-006 | 输入包含 `uint32_t timestamp_ms` | S0 | 已确认 | 内部还要用 sample counter 校验丢样 |
| D-007 | 当前无 ACC，外部传入 `allow_measure` | S0 | 已确认 | 运动判断不在当前算法内部完成 |
| D-008 | `allow_measure=false` 时立即停止输出，恢复后进 `REACQUIRE` | S0 | 已确认 | 门控期间不输出有效 IBI |
| D-009 | 允许 float | S0 | 已确认 | 后续不得擅自改为禁止 float |
| D-010 | 禁止 `malloc/calloc/realloc/free` | S0 | 已确认 | 不允许动态内存或变相动态容器 |
| D-011 | 要求 MISRA 风格限制 | S0 | 已确认 | 作为编码风格与审查约束 |
| D-012 | 输出字段为 `timestamp_ms/ibi_ms/beat_count/confidence/signal_quality/selected_channel/state/reject_reason` | S0 | 已确认 | M2 冻结具体类型与枚举 |
| D-013 | 状态机接受 `INIT/ACQUIRE/TRACK/HOLD/REACQUIRE/INVALID` | S0 | 已确认 | `HOLD` 具体逻辑 M6 冻结 |
| D-014 | 示例 CSV 允许纳入仓库 fixture | S0 | 已确认 | 路径为 `tests/fixtures/sample_ppg.csv` |
| D-015 | 当前无 ECG / 人工标注 IBI / 参考 IBI | S2 | 已确认 | 先按 smoke test 与合理性检查推进 |
| D-016 | Codex 不负责 push / 创建 PR / merge | S0 | 已确认 | Owner 管理远程 Git 操作 |

## 待 M2 决策 / 冻结

- MCU 目标按 Apollo3.5 / Cortex-M4F 级别处理，M2 进一步冻结。
- RAM 默认按 10–15KB 级别处理，M2 进一步冻结。
- ROM 预算、单次 `process()` 最大耗时、debug frame 是否输出，待后续确认。
