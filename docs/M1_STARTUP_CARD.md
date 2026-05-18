# M1 Startup Card

## 当前里程碑

M1：项目启动包与文档骨架

## 本阶段目的

把已确认的 Project Brief v0.1 固化为仓库文档、Codex 协作规则、当前任务文件和示例数据 fixture。

## 为什么现在做

M0 已确认项目目标、输入输出、运动门控、资源约束和 Git / PR 边界。进入编码前，需要先把这些约束沉淀到仓库，避免 Codex 后续误改 API、误用第三方库或无法访问示例数据。

## 本阶段产出

中文项目文档骨架、`AGENTS.md` 与 Codex runbook、`docs/10_CODEX_NEXT_TASK.md`、`tests/fixtures/sample_ppg.csv`、M1 fixture 校验命令。

## 本阶段不做

不实现完整 PPG-IBI 算法；不冻结最终 C API；不做复杂滤波 / SQI / 峰值检测 / 状态机实现；不做 ECG 对照准确性评估；不让 Codex 创建 PR、push 或 merge。

## 验收标准

必备文档存在；示例数据进入仓库固定相对路径；fixture 校验命令通过；未实现算法源码；未引入第三方依赖或动态内存。

## 是否需要我决策

无。

## 我下一步要做什么

将 M1 package 解压到仓库根目录，然后交给 Codex 执行 `docs/10_CODEX_NEXT_TASK.md`。
