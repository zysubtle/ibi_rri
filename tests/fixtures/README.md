# tests/fixtures

## sample_ppg.csv

用途：M1 之后的固定 PPG smoke test fixture。

字段：`timestamp_ms,PPG_G1,PPG_G2,PPG_G3,PPG_G4`

已知特征：数据行数 1001 行，不含表头；时间戳范围 0ms 到 20000ms；时间戳步进 20ms；采样率 50Hz；SHA256：`5ed94bc62222cf52a264d44d260efdd5dbb72976e7489574c2aeb9ae95b758f8`。

说明：该 fixture 不包含 ACC、ECG、参考 IBI 或 `allow_measure` 字段。后续 host 测试应默认注入 `allow_measure=true`，并单独构造门控测试场景。
