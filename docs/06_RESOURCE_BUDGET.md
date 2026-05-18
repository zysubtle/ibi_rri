# 06 Resource Budget v0.2 (M2 冻结)

## 冻结边界

目标平台：Apollo3.5 / Cortex-M4F 级别。允许 `float`。禁止动态内存。要求 MISRA 风格限制。禁止大数组上栈。

| 项目 | M2 冻结值 |
|---|---:|
| RAM 总预算 | 10–15KB |
| 单个 `ppg_ibi_context_t` 目标 | M2 骨架阶段远小于 1KB |
| 单次 `process()` 局部栈 | 目标 <256B |
| ROM | M2 不设硬上限，M8 审查 |
| 单次 `process()` 时间 | 工程目标远低于 20ms，M8 审查 |
| 动态内存 | 0B（禁止） |

## 工程约束说明

- `process()` 中不定义大数组。
- 不允许 `malloc/calloc/realloc/free`。
- 若后续算法引入更长窗口或复杂模块，必须重新进行 RAM/ROM/耗时评估并在里程碑评审记录。
