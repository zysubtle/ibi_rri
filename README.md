# PPG-IBI MCU 算法项目

当前状态：M2（IO Contract、MCU C API 骨架、资源预算冻结）。

## M2 交付范围

- 冻结 `docs/04_IO_CONTRACT.md` 与 `docs/06_RESOURCE_BUDGET.md`。
- 提供可编译 MCU C API 骨架：`include/ppg_ibi.h`、`src/ppg_ibi.c`。
- 不实现真实 IBI 算法；`ppg_ibi_process()` 默认 `has_event=false`。
- 保持固定 fixture：`tests/fixtures/sample_ppg.csv`。

## 构建与测试

```bash
make test
```

测试覆盖：API 编译、默认配置、初始化、逐点 `process()`、`allow_measure=false` 门控占位行为、fixture 一致性、动态内存禁用、`hr_bpm` 禁用字段。

## 边界

- 允许 `float`。
- 禁止 `malloc/calloc/realloc/free`。
- 不引入第三方 PPG/IBI/HR/HRV 算法库、CMSIS-DSP 或厂商 DSP。
- 当前项目不作为医疗诊断软件。
