# 07 Test Strategy v0.1

## 总体策略

当前无 ECG / 人工标注 IBI / 参考 IBI，因此早期测试以 smoke test、接口完整性、数据一致性、门控行为和工程合理性为主。单个示例数据不能证明算法总体准确性。

## 固定 fixture

路径：`tests/fixtures/sample_ppg.csv`

字段：`timestamp_ms,PPG_G1,PPG_G2,PPG_G3,PPG_G4`

已校验：1001 行，timestamp 从 0 到 20000ms，步进 20ms，采样率 50Hz。SHA256：`5ed94bc62222cf52a264d44d260efdd5dbb72976e7489574c2aeb9ae95b758f8`。

## M1 fixture 校验命令

```bash
python3 - <<'PY'
import csv
from pathlib import Path
p = Path('tests/fixtures/sample_ppg.csv')
with p.open(newline='', encoding='utf-8-sig') as f:
    rows = list(csv.DictReader(f))
assert rows, 'empty csv'
assert list(rows[0].keys()) == ['timestamp_ms','PPG_G1','PPG_G2','PPG_G3','PPG_G4']
ts = [int(r['timestamp_ms']) for r in rows]
assert len(rows) == 1001, len(rows)
assert ts[0] == 0 and ts[-1] == 20000
assert all((b - a) == 20 for a, b in zip(ts, ts[1:]))
print('fixture ok:', len(rows), 'rows')
PY
```

## 后续测试分层

| 阶段 | 测试重点 |
|---|---|
| M2 | API 字段、枚举、资源预算、编译骨架 |
| M3 | timestamp/sample counter、一致性校验、基础预处理 |
| M4 | SQI、主通道选择、低质量拒绝 |
| M5 | 脉搏候选、IBI 输出、IBI 合理范围 |
| M6 | 状态机、allow_measure 门控、reject reason |
| M7 | host 端评估工具、fixture smoke test、人工可视化支持 |
| M8 | 资源审查、移植准备、风险复盘 |

## 后续合理性指标草案

M5 后至少检查：IBI 数量是否合理；IBI 是否在工程合理范围内；由 IBI 换算出的 HR 是否在合理范围内，仅用于测试判定，不作为输出字段；`allow_measure=false` 时不输出有效 IBI；输出字段完整；低质量信号时不输出或降低 confidence。
