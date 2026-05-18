# 02 Milestone Plan v0.1

## 当前状态

- M0：已完成，Project Brief v0.1 已确认。
- M1：当前阶段，生成项目启动包与文档骨架。

## 里程碑计划

| Milestone | 名称 | 目的 | 主要产出 | 状态 |
|---|---|---|---|---|
| M0 | 项目启动问诊 | 确认 Project Brief v0.1 所需事实 | Project Brief v0.1、决策卡 | 已完成 |
| M1 | 项目启动包与文档骨架 | 把已确认内容沉淀为仓库文档和 Codex 规则 | docs、AGENTS、fixture、Codex runbook | 当前 |
| M2 | IO Contract、MCU API 与资源约束冻结 | 冻结公开 API、输出枚举、资源预算和验收边界 | API 草案、资源预算、M3 Codex 任务 | 待启动 |
| M3 | 输入缓冲、采样计数与基础预处理 | 实现逐点输入、timestamp/sample counter 校验和基础预处理 | C API 骨架、输入校验、host 测试 | 待启动 |
| M4 | SQI 与主通道选择 | 实现多通道质量评估与主通道选择 | SQI、selected_channel、测试 | 待启动 |
| M5 | 脉搏候选检测与 IBI 输出 | 实现候选检测、IBI 合法性和 event 输出 | IBI events、合理性测试 | 待启动 |
| M6 | 状态机、运动门控与异常处理 | 完成门控、状态转换、reject reason | 状态机、门控测试 | 待启动 |
| M7 | host 端评估工具与示例数据 smoke test | 用固定 fixture 做端到端 smoke test | 工具、输出 CSV、指标 | 待启动 |
| M8 | 资源预算审查、MCU 移植准备与风险复盘 | 审查资源、移植风险和文档收敛 | 验收报告、风险复盘 | 待启动 |

## 里程碑推进规则

- 每个 Milestone 启动前必须先输出里程碑启动卡。
- 每个 Milestone 最多两轮 Codex 修复；两轮仍未通过则重新拆分任务。
- 出现 S0 Owner 决策时暂停当前修复轮。
- Codex 默认不执行远程 Git 操作。
- 每个 Milestone 结束后只判断是否可以进入下一阶段。
