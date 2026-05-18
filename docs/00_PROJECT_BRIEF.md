# 00 Project Brief v0.1

## 1. 项目目标

在嵌入式 MCU 上，从 4 路同步绿光 PPG 中计算逐搏 IBI，用于后续 HRV / RMSSD / 心率变异性分析。

本项目只输出 IBI，不在算法内部计算 RMSSD。

## 2. 项目非目标

当前不做：医疗诊断用途、RMSSD 内部计算、ECG 对照误差评估、第三方 PPG / IBI / HRV 库集成、ACC 内部运动识别、完整 GUI 或上位机可视化工具、运动状态下 IBI 输出。

## 3. 输入数据与字段

| 项目 | 已确认规格 |
|---|---|
| PPG 通道 | 4 路 |
| 光源 | 绿光 |
| 采样率 | 50Hz |
| 输入方式 | 逐点调用 `process()` |
| 每点输入 | 每 20ms 同时输入 4 路 PPG |
| 数据类型 | `int32_t` |
| ADC 位宽 | 24bit |
| raw 类型 | signed |
| 时间戳 | `uint32_t timestamp_ms` |
| 运动门控 | 外部传入 `allow_measure` |
| ACC | 当前无 ACC 输入 |

算法内部必须维护 sample counter，用于校验 timestamp 与样本数是否一致。例如 timestamp 已跨过 1000ms，但累计样本不足 50 个，应记录为采样异常或拒绝原因。

## 4. 输出结果与字段

输出为逐搏 IBI event。字段冻结为：

- `timestamp_ms`
- `ibi_ms`
- `beat_count`
- `confidence`
- `signal_quality`
- `selected_channel`
- `state`
- `reject_reason`

当前不要求输出 `hr_bpm`，也不要求内部计算 RMSSD。

## 5. PPG-IBI 算法范围

当前算法范围包括输入接口与采样计数、timestamp 与 sample counter 一致性检查、4 路 PPG 基础预处理、SQI / 信号质量评估、主通道选择、脉搏候选检测、IBI 合法性检查、状态机、外部 `allow_measure` 门控、IBI event 输出、host 端 smoke test、MCU 资源约束审查。

## 6. 自研算法策略

- 算法核心使用 C 语言。
- 不使用第三方 PPG / IBI / HRV 算法库。
- 不引入 NeuroKit2、HeartPy、BioSPPy、WFDB、SciPy、NumPy 等作为核心算法。
- 不引入 CMSIS-DSP 或厂商 DSP 库，除非后续作为 S0 决策由 Owner 明确确认。
- 允许使用必要的 C 标准库基础能力，但不得破坏 MCU 可移植性。
- 算法采用流式逐点处理。
- 初始模块拆分建议：输入校验、预处理、SQI、通道选择、候选检测、IBI 合法性、状态机、输出事件、测试工具。

## 7. MCU 平台与工程约束

| 项目 | 约束 |
|---|---|
| MCU | Apollo3.5 / Cortex-M4F 级别，M2 最终冻结 |
| 语言 | C |
| 允许 float | 是 |
| 动态内存 | 禁止 |
| MISRA 风格 | 要求 |
| 处理方式 | 逐点实时处理 |
| 大数组上栈 | 禁止 |
| 递归 | 禁止 |
| 参数 | 集中管理，避免魔法数 |
| host 测试 | 允许 |

## 8. RAM / ROM / 运行时间预算

当前待 M2 冻结：

- RAM：暂按 10–15KB 级别作为待确认默认假设。
- ROM：待确认。
- 单次 `process()` 运行时间：待确认。
- 功耗预算：待确认。

## 9. 是否允许 float

确认：允许 `float`。

## 10. 是否禁止 malloc

确认：严格禁止 `malloc/calloc/realloc/free`。也不允许通过动态容器或变相动态分配规避。

## 11. 运动门控策略

- 当前无 ACC。
- 运动判断由外部完成。
- 外部输入 `allow_measure`。
- `allow_measure=false` 时立即停止输出 IBI。
- 恢复为 `allow_measure=true` 后进入 `REACQUIRE`。
- 运动期间不输出有效 IBI。
- 输出或内部状态中应保留 `reject_reason`。

## 12. 状态机默认假设

接受默认状态机：`INIT / ACQUIRE / TRACK / HOLD / REACQUIRE / INVALID`。

当前策略：初始进入 `INIT / ACQUIRE`；捕获稳定脉搏后进入 `TRACK`；`allow_measure=false` 时停止输出；恢复后进入 `REACQUIRE`；严重输入异常进入 `INVALID` 或输出 reject reason；`HOLD` 的具体条件在 M6 冻结。

## 13. 测试与验收标准

当前无 ECG、人工标注 IBI 或参考 IBI，因此先按 smoke test 与合理性检查推进。

M1-M5 阶段默认验收：C 代码可在 host 上编译、禁止动态内存、公开 API 字段完整、示例 CSV 可读取、时间戳 20ms 间隔可校验、sample counter 与 timestamp 可做一致性检查、`allow_measure=false` 时不输出 IBI、输出 event 字段完整、IBI 落在合理工程范围。

单个示例数据只作为 smoke test，不作为准确性证明。

## 14. 示例数据安排

示例 CSV 已纳入仓库固定路径：

```text
tests/fixtures/sample_ppg.csv
```

字段：

```text
timestamp_ms,PPG_G1,PPG_G2,PPG_G3,PPG_G4
```

该文件用于 CSV 读取 smoke test、50Hz 时间戳一致性测试、4 通道逐点输入测试、`process()` 逐点调用测试和后续 IBI 输出合理性测试。

## 15. Git / PR 职责边界

- Owner 负责创建分支。
- Owner 负责 push。
- Owner 负责创建 PR。
- Owner 负责 merge。
- Codex 不负责 push / 创建 PR / 更新 PR / merge。
- Architect 可基于 PR 链接、zip、diff、修改文件列表、测试日志进行审查。
- 若未来希望 Codex 执行远程 Git 操作，必须单独作为 S0 事项确认。

## 16. 当前待确认默认假设

- RAM 暂按 10–15KB 级别。
- ROM 预算暂未定。
- 单次 `process()` 最大运行时间暂未定。
- 是否需要详细 debug trace 结构体暂未定。
- `confidence` 与 `signal_quality` 数值范围待 M2 冻结。
- `reject_reason` 枚举值待 M2 冻结。
- `selected_channel` 编号规则待 M2 冻结。
- `HOLD` 状态是否实际启用及进入条件待 M6 冻结。

## 17. 后置问题

RAM 是否严格限制为 10–15KB；ROM 预算是否有硬限制；单次 `process()` 是否有最大耗时要求；是否需要输出 debug frame；是否需要未来支持 ACC 内部运动识别；是否需要后续做固定点版本；示例数据是否需要再截取更小版本。

## 18. 风险清单

| 风险 | 等级 | 说明 |
|---|---:|---|
| 无 ECG / 人工 IBI 参考 | S2 | 早期只能做 smoke test，无法证明准确率 |
| 50Hz 分辨率有限 | S2 | IBI 时间分辨率天然为 20ms 级 |
| 外部 `allow_measure` 质量未知 | S2 | 运动门控效果依赖外部算法 |
| PPG 低灌注 / 佩戴不良 | S2 | 需要 SQI 和 reject reason |
| 多通道信号质量差异 | S2 | 需要主通道选择 |
| timestamp 与 sample counter 不一致 | S1 | 必须纳入输入异常处理 |
| RAM / ROM 未冻结 | S2 | M2 需要冻结资源预算 |
| 误把 smoke test 当准确性证明 | S2 | 必须明确避免 |

## 19. 里程碑草案

```text
M0：项目启动问诊
M1：项目启动包与文档骨架
M2：IO Contract、MCU API 与资源约束冻结
M3：输入缓冲、采样计数与基础预处理
M4：SQI 与主通道选择
M5：脉搏候选检测与 IBI 输出
M6：状态机、运动门控与异常处理
M7：host 端评估工具与示例数据 smoke test
M8：资源预算审查、MCU 移植准备与风险复盘
```

## 20. 当前不处理的问题

完整 PPG 峰值检测算法实现、RMSSD 计算、ECG 对照验证、医疗级准确性声明、ACC 运动识别、固定点优化、GUI 可视化、CMSIS-DSP / 厂商 DSP 集成、远程 Git 操作自动化。
