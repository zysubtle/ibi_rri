# PPG-IBI MCU 算法项目

当前状态：M1 项目启动包与文档骨架。

本项目目标是在智能手环 / 智能戒指 / 可穿戴设备 MCU 上，从 4 路同步绿光 PPG 信号中计算逐搏 IBI，用于后续 HRV / RMSSD / 心率变异性分析。

## 当前边界

- 算法核心默认使用 C 语言实现。
- 当前不使用第三方 PPG / IBI / HR / HRV 算法库。
- 当前不引入 CMSIS-DSP、厂商 DSP 库或外部算法依赖；如需引入，必须作为 S0 事项由 Owner 决策。
- 允许 `float`，禁止 `malloc/calloc/realloc/free`。
- 当前只输出 IBI event，不在算法内部计算 RMSSD。
- 当前无 ACC 输入；运动门控由外部 `allow_measure` 输入控制。
- 本项目为研究 / 工程验证算法原型，不作为医疗诊断软件。

## M1 产物

- `docs/00_PROJECT_BRIEF.md`：Project Brief v0.1
- `docs/01_DECISION_LOG.md`：Decision Log v0.1
- `docs/02_MILESTONE_PLAN.md`：Milestone Plan v0.1
- `docs/03_ALGORITHM_SCOPE.md`：Algorithm Scope v0.1
- `docs/04_IO_CONTRACT.md`：IO Contract v0.1 草案
- `docs/05_MCU_ALGORITHM_STRATEGY.md`：自研 MCU PPG-IBI 算法策略
- `docs/06_RESOURCE_BUDGET.md`：资源预算草案
- `docs/07_TEST_STRATEGY.md`：测试策略
- `docs/08_RISK_REVIEW.md`：风险记录
- `docs/09_CODEX_RUNBOOK.md`：Codex 运行说明
- `docs/10_CODEX_NEXT_TASK.md`：当前 Codex 任务文件
- `docs/11_GIT_WORKFLOW.md`：Owner 管理 Git / PR 的流程说明
- `AGENTS.md`：Agent / Codex 协作规则
- `tests/fixtures/sample_ppg.csv`：示例 PPG fixture

## 示例数据

路径：`tests/fixtures/sample_ppg.csv`

字段：`timestamp_ms,PPG_G1,PPG_G2,PPG_G3,PPG_G4`

已知特征：

- 行数：1001 行数据，不含表头。
- 时间戳：0ms 到 20000ms。
- 步进：20ms。
- 采样率：50Hz。
- SHA256：`5ed94bc62222cf52a264d44d260efdd5dbb72976e7489574c2aeb9ae95b758f8`

该数据只能作为 smoke test / 回归样例，不能证明算法总体准确性。

## 给 Codex 的一句话指令

请读取 `docs/10_CODEX_NEXT_TASK.md` 并严格执行；不要创建 PR、不要 push、不要 merge；完成后报告修改文件和测试结果。

## M1 启动包校验记录

- 最近一次按 `docs/10_CODEX_NEXT_TASK.md` 完成校验日期：2026-05-18（UTC）。
- 校验项：必备文档存在、fixture 路径与字段格式正确、时间戳范围与步进正确。
