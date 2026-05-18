# 10 Codex Next Task：M1 项目启动包与文档骨架

## 本轮目标

建立或校验 M1 项目启动包，使仓库具备后续 M2-M8 开发所需的中文文档、协作规则、示例数据 fixture 和测试说明。

## 本轮非目标

- 不实现完整 PPG-IBI 算法。
- 不实现复杂滤波、SQI、主通道选择、峰值检测、IBI 输出或状态机逻辑。
- 不冻结最终 C API。
- 不引入第三方库。
- 不创建 PR、不 push、不 merge。

## 允许修改范围

仅允许创建或修改：`README.md`、`AGENTS.md`、`docs/*.md`、`tests/fixtures/sample_ppg.csv`、`tests/fixtures/README.md`。如仓库已有其他文件，不要无关修改。

## 不允许修改范围

不要新增 `src/`、`include/` 中的算法源码，除非 Owner 另行确认；不要新增 Makefile / CMakeLists 来暗示已进入编码阶段，除非仅为 fixture 文档校验且已说明；不要删除现有业务文件；不要执行任何远程 Git 操作。

## 必备文档

```text
docs/00_PROJECT_BRIEF.md
docs/01_DECISION_LOG.md
docs/02_MILESTONE_PLAN.md
docs/03_ALGORITHM_SCOPE.md
docs/04_IO_CONTRACT.md
docs/05_MCU_ALGORITHM_STRATEGY.md
docs/06_RESOURCE_BUDGET.md
docs/07_TEST_STRATEGY.md
docs/08_RISK_REVIEW.md
docs/09_CODEX_RUNBOOK.md
docs/10_CODEX_NEXT_TASK.md
docs/11_GIT_WORKFLOW.md
AGENTS.md
README.md
```

## 示例数据要求

固定路径：`tests/fixtures/sample_ppg.csv`

字段必须为：`timestamp_ms,PPG_G1,PPG_G2,PPG_G3,PPG_G4`

该文件应为仓库内 fixture，不得引用聊天附件、sources 或绝对路径。

## 测试命令

使用 Python 标准库校验 fixture：

```bash
python3 - <<'PY'
import csv
from pathlib import Path
p = Path('tests/fixtures/sample_ppg.csv')
assert p.exists(), 'missing tests/fixtures/sample_ppg.csv'
with p.open(newline='', encoding='utf-8-sig') as f:
    rows = list(csv.DictReader(f))
assert rows, 'empty csv'
assert list(rows[0].keys()) == ['timestamp_ms','PPG_G1','PPG_G2','PPG_G3','PPG_G4']
ts = [int(r['timestamp_ms']) for r in rows]
assert len(rows) == 1001, len(rows)
assert ts[0] == 0, ts[0]
assert ts[-1] == 20000, ts[-1]
assert all((b - a) == 20 for a, b in zip(ts, ts[1:])), 'timestamp step not 20ms'
print('fixture ok:', len(rows), 'rows', ts[0], ts[-1])
PY
```

## 通过标准

必备文档均存在；示例数据存在于 `tests/fixtures/sample_ppg.csv`；fixture 校验命令通过；未实现算法源码；未引入第三方依赖；未使用动态内存；未执行远程 Git 操作。

## 失败时报告

如失败，请报告：缺失文件、fixture 校验失败原因、是否误改了不允许修改的文件、是否存在需要 Owner 决策的 S0 问题。

## 交付材料

```text
Summary
Changed files
Test commands
Test results
Known limitations
是否执行 Git 远程操作：否
```
