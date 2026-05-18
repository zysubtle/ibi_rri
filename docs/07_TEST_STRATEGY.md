# 07 Test Strategy v0.2 (M2 对齐)

## 总体策略

当前无 ECG / 人工标注 IBI / 参考 IBI。M2 以 API 可用性、契约一致性、fixture smoke test 和工程约束检查为主。

## M2 最小测试

- `make test`：执行 fixture 校验、禁用动态内存调用扫描、`hr_bpm` 禁止项扫描。
- API 相关编译/运行测试在代码骨架文件存在时纳入 `make test`。

## Fixture smoke test 边界

固定路径：`tests/fixtures/sample_ppg.csv`。

- 字段必须为：`timestamp_ms,PPG_G1,PPG_G2,PPG_G3,PPG_G4`
- 行数：1001
- 时间戳范围：0 到 20000
- 步进：20ms

说明：fixture 仅用于 smoke test，不构成算法准确性证明。

## 后续测试分层

- M3：timestamp/sample counter 异常处理。
- M4-M6：SQI、主通道、候选与状态机行为。
- M7-M8：host 评估、资源审查与移植准备。
