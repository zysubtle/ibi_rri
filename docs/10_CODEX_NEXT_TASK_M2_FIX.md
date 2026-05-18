# 10 Codex Next Task：M2 修复补丁（PR #2 同分支更新）

## 当前结论
本任务不是 M3。请在当前 M2 PR 分支上做最小修复，使 PR #2 可以被合并。

## 修复目标
M2 的代码骨架基本符合要求，但文档和 Makefile 需要收敛：

1. 清理文档中 M1/v0.1 草案与 M2 冻结内容混杂的问题。
2. 清理重复标题、重复表格、重复风险行。
3. 确保 `docs/04_IO_CONTRACT.md`、`docs/06_RESOURCE_BUDGET.md`、`docs/07_TEST_STRATEGY.md`、`docs/08_RISK_REVIEW.md` 表述一致：M2 已冻结 API/字段/默认值/资源边界；真实 timestamp 异常处理在 M3 实现。
4. 将 `Makefile` 中禁止项扫描从 `rg` 改为 POSIX/常见环境更容易满足的 `grep -R -n -E`，不要依赖 ripgrep。
5. 不进入 M3，不实现滤波、SQI、主通道选择、峰值检测或真实 IBI。
6. 不改变已经冻结的公开 API，除非发现明显编译错误；如需改变 API，停止并报告 Owner 决策。

## 必须修复的文件
允许修改：

```text
README.md
docs/04_IO_CONTRACT.md
docs/06_RESOURCE_BUDGET.md
docs/07_TEST_STRATEGY.md
docs/08_RISK_REVIEW.md
Makefile
```

如确有必要，也可更新：

```text
docs/10_CODEX_NEXT_TASK.md
```

## 具体要求

### README.md
- 只保留一个当前状态：`M2（IO Contract、MCU C API 骨架、资源预算冻结）`。
- 不要同时出现“当前状态：M1”。
- 不要出现连续重复标题，例如 `## M2 交付范围` 后立刻 `## 当前边界` 且内容混杂。
- 将 M1 产物说明移动到“历史/已完成”或删除冗余内容。

### docs/04_IO_CONTRACT.md
- 标题应为 `# 04 IO Contract v0.2 (M2 冻结)`。
- 删除或改写 `v0.1 草案`、`M2 待冻结项`、`M2 需要冻结` 等旧表述。
- 保留清晰的 M2 冻结值：
  - 4 通道；50Hz；20ms；`timestamp_ms:uint32_t`；`ppg[4]:int32_t`；`allow_measure:bool`
  - event 字段：`timestamp_ms / ibi_ms / beat_count / confidence / signal_quality / selected_channel / state / reject_reason`
  - `ibi_ms:uint16_t`，有效范围 300–2000ms
  - `confidence` 与 `signal_quality` 为 `float`，0.0f–1.0f
  - `selected_channel` 为 0–3，无有效通道为 `PPG_IBI_INVALID_CHANNEL`
  - reject reason 枚举与头文件一致
  - `timestamp_tolerance_ms=20`，`sample_count_tolerance=1`
- 明确：M2 只维护计数器与占位行为，完整 timestamp/sample counter 异常处理留到 M3。

### docs/06_RESOURCE_BUDGET.md
- 标题应为 `# 06 Resource Budget v0.2 (M2 冻结)`。
- 删除“待 M2 冻结”“当前默认假设”等与 M2 冻结矛盾的旧内容。
- 明确：RAM 总预算 10–15KB；单个 context 骨架远小于 1KB；process 局部栈目标 <256B；动态内存 0B；ROM 无硬上限，M8 审查；单次 process 工程目标远低于 20ms，M8 审查。

### docs/07_TEST_STRATEGY.md
- 删除重复的 M1 校验段落和重复 fixture 描述。
- 保留 M2 最小测试、后续测试分层、fixture smoke test 边界。

### docs/08_RISK_REVIEW.md
- 只保留一张风险表。
- 不要同时保留旧 R-001..R-011 与新 R-001..R-009 两套编号。
- 风险 `RAM / ROM / 耗时未冻结` 应改为“资源超预算风险”，状态为“M2 已冻结边界，M8 审查”。

### Makefile
- 将 `rg` 替换为 `grep -R -n -E`。
- 保持动态内存扫描和 `hr_bpm` 扫描。
- `make test` 必须仍然通过。

## 测试命令
必须运行并报告：

```bash
make test
```

并报告：

```text
Summary
Changed files
Test commands
Test results
Known limitations
是否修改 API：否
是否引入动态内存：否
是否引入外部依赖：否
是否实现滤波/SQI/峰值检测/IBI 算法：否
是否执行 Git 远程操作：否
```

## 通过标准
- 文档不再存在 M1/v0.1 草案与 M2 冻结互相矛盾的表述。
- `Makefile` 不再依赖 `rg`。
- `make test` 通过。
- 公开 API 未改变。
- 没有新增算法实现、动态内存、外部算法依赖或 `hr_bpm` 字段。
