# 08 Risk Review v0.2 (M2 对齐)

| ID | 风险 | 等级 | 当前处理 |
|---|---|---:|---|
| R-001 | 无 ECG / 人工标注 / 参考 IBI | S2 | 早期仅做 smoke test 与合理性检查 |
| R-002 | 50Hz 导致 IBI 分辨率有限 | S2 | 记录精度边界，后续里程碑评估 |
| R-003 | 外部 `allow_measure` 质量未知 | S2 | 当前只执行门控，不评价外部识别准确性 |
| R-004 | 佩戴不良 / 低灌注 / 强噪声 | S2 | 后续通过 SQI/reject reason 管理 |
| R-005 | 多通道质量差异 | S2 | 后续实现主通道选择策略 |
| R-006 | timestamp 与 sample counter 不一致 | S1 | M3 实现异常处理 |
| R-007 | 资源超预算风险（RAM/ROM/耗时） | S2 | M2 已冻结边界，M8 审查 |
| R-008 | 误把 smoke test 当准确性证明 | S2 | 文档中持续声明 fixture 仅用于 smoke test |
| R-009 | 擅自引入第三方库/动态内存/API 漂移 | S0/S1 | 由 AGENTS 与任务文档约束，变更需 Owner 决策 |

风险分级：S0（Owner 决策），S1（阻塞实现），S2（技术风险跟踪），S3（轻微问题）。
