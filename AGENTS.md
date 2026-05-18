# AGENTS.md：Codex / Agent 协作规则

## 项目角色

- Owner：负责目标、约束、关键决策、阶段确认、分支、push、PR 和 merge。
- Architect / Reviewer：负责规格整理、任务拆分、审查、风险归类和里程碑推进建议。
- Runner / Codex：负责按 `docs/10_CODEX_NEXT_TASK.md` 修改文件、运行测试并报告结果。

## 当前任务入口

Codex 必须先读取：`docs/10_CODEX_NEXT_TASK.md`

## 强制约束

默认不使用第三方 PPG / IBI / HR / HRV 算法库；默认不引入 CMSIS-DSP、厂商 DSP 库或外部算法依赖；核心算法语言为 C；允许 `float`；禁止 `malloc/calloc/realloc/free`；要求 MISRA 风格限制；避免大数组上栈；禁止递归；参数集中管理，避免魔法数散落；输入、输出、状态、错误码必须可追踪；当前项目不作为医疗诊断软件。

## Git 规则

Codex 不得执行 push、创建 PR、更新 PR、merge、rebase 远程分支、删除远程分支、改写 main / master 历史。

## S0 决策

以下事项必须停止并请求 Owner 决策：改变核心算法策略；改变输入 API 或输出字段；改变采样率；改变 MCU 平台或资源预算；引入第三方库、CMSIS-DSP 或厂商 DSP 库；改变是否允许 float；改变是否允许动态内存；改变运动门控策略；将项目扩展为医疗诊断用途。

## 输出要求

```text
Summary
Changed files
Test commands
Test results
Known limitations
是否修改 API
是否引入动态内存
是否引入外部依赖
是否执行 Git 远程操作
```
