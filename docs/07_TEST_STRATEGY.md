# 07 Test Strategy v0.2 (M2)

## 当前策略

当前无 ECG / 参考 IBI。M2 以 API 编译与行为 smoke test 为主，不做准确性验证。

## 固定 fixture

- 路径：`tests/fixtures/sample_ppg.csv`
- 字段：`timestamp_ms,PPG_G1,PPG_G2,PPG_G3,PPG_G4`
- 行数：1001
- timestamp：0 -> 20000ms，步进 20ms

## M2 最小测试

1. 编译 `src/ppg_ibi.c` + `tests/test_api_compile.c`
2. 运行测试程序并验证：
   - default config 为 50Hz/20ms/4ch
   - init 成功
   - process 可逐点调用
   - `has_event=false`
   - `allow_measure=false` 时 reject reason 为 `MEASURE_NOT_ALLOWED`
3. fixture 一致性校验
4. 禁用动态内存调用扫描
5. `hr_bpm` 禁止字段扫描

## 命令

- `make test`
