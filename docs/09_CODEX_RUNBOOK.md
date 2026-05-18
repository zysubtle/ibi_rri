# 09 Codex Runbook v0.1

## Codex 默认入口

每轮任务请先读取：`docs/10_CODEX_NEXT_TASK.md`

Owner 给 Codex 的默认一句话指令：

```text
请读取 docs/10_CODEX_NEXT_TASK.md，并严格执行。请不要创建 PR、不要 push、不要 merge；完成后报告修改文件和测试结果。
```

## Codex 禁止事项

不要创建 PR；不要 push；不要 merge；不要 rebase 或改写远程历史；不要引入第三方 PPG / IBI / HR / HRV 算法库；不要引入 CMSIS-DSP 或厂商 DSP 库，除非任务文件明确说明已由 Owner 决策；不要使用 `malloc/calloc/realloc/free`；不要用 Python 或 host 脚本替代 MCU C 算法；不要擅自改变输入 API、输出字段、采样率、资源预算、运动门控策略。

## Codex 输出格式建议

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

## 测试命令原则

必须使用仓库内相对路径；不得写“使用聊天附件 / sources 中的文件”；如涉及示例数据，固定使用 `tests/fixtures/sample_ppg.csv`。

## S0 处理

如任务执行过程中需要改变 API、算法策略、采样率、资源预算、第三方库、CMSIS-DSP、动态内存、Git 远程操作策略等，立即停止并报告需要 Owner 决策。
