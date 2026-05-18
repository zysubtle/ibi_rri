# 06 Resource Budget v0.2 (M2 冻结)

## 平台与约束

- 目标平台：Apollo3.5 / Cortex-M4F 级别
- 语言：C
- 允许 `float`
- 禁止动态内存（`malloc/calloc/realloc/free`）
- 禁止递归
- 避免大数组上栈

## M2 冻结预算

| 项目 | M2 冻结值 |
|---|---:|
| RAM 总预算 | 10–15KB |
| 单个 `ppg_ibi_context_t` 目标 | M2 骨架远小于 1KB；后续算法总 context 不超过 RAM 预算 |
| 局部栈使用 | 单次 `process()` 不得定义大数组；目标 <256B |
| ROM | M2 不设硬上限，M8 审查 |
| 单次 `process()` 时间 | 工程目标远低于 20ms 采样周期，M8 审查 |
| 动态内存 | 0B（禁止） |

## M2 说明

M2 仅冻结预算边界与 API 骨架，不做性能/精度结论。
