# 10 Codex Next Task：M2 IO Contract、MCU C API 与资源约束冻结

## 当前里程碑

M2：IO Contract、MCU API 与资源约束冻结。

本轮任务的目标是把 M0/M1 已确认的需求固化为**可编译的 MCU C API 骨架、IO Contract、资源预算与测试骨架**。本轮仍然不是算法实现阶段。

## 已冻结背景

- 项目目标：在 MCU 上从 4 路同步绿光 PPG 计算逐搏 IBI，用于 HRV / RMSSD 外部分析。
- 输出：只输出 IBI event；算法内部不计算 RMSSD，不输出 HR。
- 输入：4 路绿光 PPG，50Hz，每 20ms 同时输入 4 路样本。
- PPG raw：`int32_t`，24bit，signed。
- 时间戳：输入包含 `uint32_t timestamp_ms`。
- 内部计数：算法内部必须保留 sample counter，用于后续校验 timestamp 与样本数是否一致。
- 运动门控：无 ACC；由外部传入 `allow_measure`。
- 门控行为：`allow_measure=false` 时立即停止输出 IBI，恢复后进入 `REACQUIRE`。
- MCU：Apollo3.5 / Cortex-M4F 级别。
- 实现约束：允许 `float`；禁止 `malloc/calloc/realloc/free`；要求 MISRA 风格限制；禁止大数组上栈；禁止递归。
- 示例数据固定路径：`tests/fixtures/sample_ppg.csv`。
- 当前无 ECG / 人工标注 IBI / 参考 IBI；早期只能做 smoke test 与合理性检查。
- Codex 不创建 PR、不 push、不 merge、不 rebase 远程分支。

## 本轮目标

完成以下事项：

1. 冻结 `docs/04_IO_CONTRACT.md` 为 M2 版本，明确输入、输出、状态、reject reason、字段类型和数值范围。
2. 冻结 `docs/06_RESOURCE_BUDGET.md` 为 M2 版本，明确 RAM、ROM、单次 `process()` 时间和栈使用的工程预算。
3. 创建 MCU C API 骨架，要求 caller 分配 context，不使用动态内存。
4. 创建最小可编译实现，保证 API 可被 host 测试调用。
5. 创建最小测试，验证 API 编译、默认配置、初始化、逐点 `process()` 调用、`allow_measure=false` 不输出 IBI。
6. 更新 README 或测试文档，记录 M2 的构建和测试命令。
7. 保持 `tests/fixtures/sample_ppg.csv` 作为固定示例数据，不引用聊天附件、sources 或绝对路径。

## 本轮非目标

本轮不要实现以下内容：

- 不实现完整 PPG-IBI 算法。
- 不实现滤波器、SQI、主通道选择、峰值检测、IBI 计算、HR 计算或 RMSSD 计算。
- 不输出真实 IBI event；M2 中 `ppg_ibi_process()` 默认不产生有效 IBI。
- 不实现复杂状态机，只允许最小状态字段初始化和 `allow_measure=false` 的门控占位行为。
- 不引入第三方 PPG / IBI / HR / HRV 算法库。
- 不引入 CMSIS-DSP 或厂商 DSP 库。
- 不创建 PR、不 push、不 merge。

## 允许修改范围

允许创建或修改：

```text
README.md
AGENTS.md
docs/*.md
include/ppg_ibi.h
include/ppg_ibi_config.h
src/ppg_ibi.c
src/ppg_ibi_internal.h
tests/test_api_compile.c
tests/fixtures/README.md
Makefile
```

如果仓库已有同名文件，应尽量最小修改，不要无关重构。

## 不允许修改范围

- 不删除 `tests/fixtures/sample_ppg.csv`。
- 不改变 fixture 字段名：`timestamp_ms,PPG_G1,PPG_G2,PPG_G3,PPG_G4`。
- 不新增 Python / C++ / Rust / Go 等替代核心算法实现。
- 不新增外部依赖管理文件来引入第三方算法库。
- 不修改远程 Git 历史，不执行 push / merge / rebase。
- 不把 `hr_bpm` 加入 API、event 或测试输出字段。

## M2 推荐公开 API

请在 `include/ppg_ibi.h` 中冻结或实现等价的公开 API。若必须偏离以下 API，停止并报告需要 Owner 决策。

```c
#ifndef PPG_IBI_H
#define PPG_IBI_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define PPG_IBI_NUM_CHANNELS (4u)
#define PPG_IBI_SAMPLE_RATE_HZ (50u)
#define PPG_IBI_SAMPLE_PERIOD_MS (20u)
#define PPG_IBI_INVALID_CHANNEL (255u)

typedef enum
{
    PPG_IBI_STATUS_OK = 0,
    PPG_IBI_STATUS_NULL_ARG,
    PPG_IBI_STATUS_BAD_CONFIG,
    PPG_IBI_STATUS_NOT_INITIALIZED
} ppg_ibi_status_t;

typedef enum
{
    PPG_IBI_STATE_INIT = 0,
    PPG_IBI_STATE_ACQUIRE,
    PPG_IBI_STATE_TRACK,
    PPG_IBI_STATE_HOLD,
    PPG_IBI_STATE_REACQUIRE,
    PPG_IBI_STATE_INVALID
} ppg_ibi_state_t;

typedef enum
{
    PPG_IBI_REJECT_NONE = 0,
    PPG_IBI_REJECT_NOT_READY,
    PPG_IBI_REJECT_MEASURE_NOT_ALLOWED,
    PPG_IBI_REJECT_TIMESTAMP_INVALID,
    PPG_IBI_REJECT_SAMPLE_COUNT_MISMATCH,
    PPG_IBI_REJECT_SIGNAL_LOW_QUALITY,
    PPG_IBI_REJECT_NO_VALID_CHANNEL,
    PPG_IBI_REJECT_IBI_TOO_SHORT,
    PPG_IBI_REJECT_IBI_TOO_LONG,
    PPG_IBI_REJECT_PEAK_UNSTABLE,
    PPG_IBI_REJECT_INTERNAL_ERROR
} ppg_ibi_reject_reason_t;

typedef struct
{
    uint32_t timestamp_ms;
    int32_t ppg[PPG_IBI_NUM_CHANNELS];
    bool allow_measure;
} ppg_ibi_sample_t;

typedef struct
{
    uint32_t timestamp_ms;
    uint16_t ibi_ms;
    uint32_t beat_count;
    float confidence;
    float signal_quality;
    uint8_t selected_channel;
    ppg_ibi_state_t state;
    ppg_ibi_reject_reason_t reject_reason;
} ppg_ibi_event_t;

typedef struct
{
    uint16_t sample_rate_hz;
    uint16_t sample_period_ms;
    uint8_t num_channels;
    uint16_t min_ibi_ms;
    uint16_t max_ibi_ms;
    uint16_t timestamp_tolerance_ms;
    uint16_t sample_count_tolerance;
} ppg_ibi_config_t;

typedef struct
{
    ppg_ibi_config_t config;
    ppg_ibi_state_t state;
    ppg_ibi_reject_reason_t last_reject_reason;
    uint32_t sample_count;
    uint32_t valid_sample_count;
    uint32_t last_timestamp_ms;
    uint32_t beat_count;
    bool initialized;
    bool have_last_timestamp;
} ppg_ibi_context_t;

const char *ppg_ibi_version(void);
void ppg_ibi_config_default(ppg_ibi_config_t *config);
ppg_ibi_status_t ppg_ibi_init(ppg_ibi_context_t *ctx, const ppg_ibi_config_t *config);
void ppg_ibi_reset(ppg_ibi_context_t *ctx);
ppg_ibi_status_t ppg_ibi_process(ppg_ibi_context_t *ctx,
                                 const ppg_ibi_sample_t *sample,
                                 ppg_ibi_event_t *event,
                                 bool *has_event);

#endif /* PPG_IBI_H */
```

## M2 默认数值范围

请在 `docs/04_IO_CONTRACT.md` 和头文件中保持一致：

| 项目 | M2 默认值 / 范围 |
|---|---|
| 通道数 | 4 |
| 采样率 | 50Hz |
| 采样周期 | 20ms |
| `selected_channel` | 0–3；无有效通道时为 `PPG_IBI_INVALID_CHANNEL` |
| `confidence` | `float`，0.0f–1.0f |
| `signal_quality` | `float`，0.0f–1.0f |
| `ibi_ms` | `uint16_t`，M2 默认有效范围 300–2000ms |
| `timestamp_tolerance_ms` | 20ms |
| `sample_count_tolerance` | 1 sample |
| `has_event` | M2 默认永远为 `false`，因为尚未实现 IBI 检测 |

## M2 最小实现要求

`src/ppg_ibi.c` 只做 API 骨架和安全占位：

1. `ppg_ibi_version()` 返回非空版本字符串，例如 `0.2.0-m2`。
2. `ppg_ibi_config_default()` 写入默认配置：50Hz、20ms、4 通道、IBI 300–2000ms、timestamp tolerance 20ms、sample count tolerance 1。
3. `ppg_ibi_init()` 校验指针和配置；仅接受 50Hz、20ms、4 通道；清零 context；设置 `initialized=true`、`state=PPG_IBI_STATE_INIT`、`last_reject_reason=PPG_IBI_REJECT_NOT_READY`。
4. `ppg_ibi_reset()` 不使用动态内存，恢复 context 到初始化后的安全状态。
5. `ppg_ibi_process()`：
   - 校验 `ctx/sample/event/has_event` 非空。
   - 未初始化时返回 `PPG_IBI_STATUS_NOT_INITIALIZED`。
   - 默认 `*has_event=false`。
   - 填充 event 的安全默认值：`ibi_ms=0`、`confidence=0.0f`、`signal_quality=0.0f`、`selected_channel=PPG_IBI_INVALID_CHANNEL`。
   - 可以维护 `sample_count`、`valid_sample_count`、`last_timestamp_ms` 等计数器，但不要实现完整 timestamp 异常处理；完整校验留到 M3。
   - 当 `allow_measure=false` 时，立即不输出 IBI，状态可置为 `PPG_IBI_STATE_REACQUIRE`，`reject_reason=PPG_IBI_REJECT_MEASURE_NOT_ALLOWED`。
   - 当 `allow_measure=true` 时，M2 仍不输出 IBI，`reject_reason=PPG_IBI_REJECT_NOT_READY`。
6. 不得在任何路径调用 `malloc/calloc/realloc/free`。
7. 不得加入 `hr_bpm` 字段。

## 文档更新要求

至少更新：

```text
docs/04_IO_CONTRACT.md
docs/06_RESOURCE_BUDGET.md
docs/07_TEST_STRATEGY.md
docs/08_RISK_REVIEW.md
docs/10_CODEX_NEXT_TASK.md
README.md
```

文档应明确：M2 已冻结 C API 骨架与字段类型，但尚未实现真实 IBI 算法；fixture 仍只用于 smoke test。

## 资源预算 M2 默认冻结值

请在 `docs/06_RESOURCE_BUDGET.md` 中冻结以下 M2 工程预算：

| 项目 | M2 冻结值 |
|---|---:|
| RAM 总预算 | 10–15KB |
| 单个 `ppg_ibi_context_t` 目标 | M2 骨架阶段应远小于 1KB；后续算法阶段总 context 不超过 RAM 预算 |
| 局部栈使用 | 单次 `process()` 不得定义大数组；目标小于 256B |
| ROM | M2 暂不设硬上限，后续 M8 审查 |
| 单次 `process()` 时间 | M2 暂设工程目标：远低于 20ms 采样周期；后续 M8 审查 |
| 动态内存 | 0B，禁止 |

## 测试要求

创建 `Makefile` 和 `tests/test_api_compile.c`。推荐 `make test` 至少执行：

1. 编译 `src/ppg_ibi.c` 与 `tests/test_api_compile.c`。
2. 运行测试程序。
3. 验证 fixture 仍然存在且字段、行数、时间戳步进正确。
4. 扫描动态内存禁用项。
5. 扫描 API / 源码 / 测试中没有 `hr_bpm`。

推荐测试命令：

```bash
make test

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

if grep -R -n -E '\b(malloc|calloc|realloc|free)[[:space:]]*\(' include src tests; then
  echo 'dynamic memory call found'
  exit 1
fi

if grep -R -n 'hr_bpm' include src tests; then
  echo 'forbidden hr_bpm field found'
  exit 1
fi
```

`tests/test_api_compile.c` 至少验证：

- `ppg_ibi_config_default()` 输出 50Hz / 20ms / 4 通道。
- `ppg_ibi_init()` 成功。
- `ppg_ibi_process()` 可以逐点调用。
- M2 默认不产生 IBI event，即 `has_event=false`。
- `allow_measure=false` 时 `has_event=false`，且 reject reason 为 `PPG_IBI_REJECT_MEASURE_NOT_ALLOWED` 或 context 中对应状态可追踪。
- 不访问越界通道。

## 通过标准

本轮通过必须满足：

1. `make test` 通过。
2. fixture 校验通过。
3. `include/ppg_ibi.h` 中公开 API 字段完整。
4. 输出 event 字段包含且仅包含已冻结核心字段：`timestamp_ms / ibi_ms / beat_count / confidence / signal_quality / selected_channel / state / reject_reason`。
5. 没有 `hr_bpm` 字段。
6. 没有动态内存调用。
7. 没有外部算法依赖。
8. 没有实现真实滤波、SQI、峰值检测或 IBI 计算。
9. 没有执行 Git 远程操作。
10. 文档明确 M2 只是 API / Contract / 资源预算冻结，不是准确性验证。

## 失败时报告

如失败，请报告：

```text
失败项：
涉及文件：
失败命令：
失败原因：
是否需要 Owner 决策：是/否
建议修复：
```

## S0 停止条件

遇到以下情况必须停止，不要自行决定：

- 需要改变输入字段、输出字段或公开 API。
- 需要输出 HR / `hr_bpm`。
- 需要使用动态内存。
- 需要引入第三方算法库、CMSIS-DSP 或厂商 DSP 库。
- 需要改变采样率、通道数、raw 数据类型或时间戳类型。
- 需要改变 `allow_measure=false` 的立即停止输出策略。
- 需要把项目声明为医疗诊断用途。
- 需要执行 push / PR / merge / rebase 等远程 Git 操作。

## 交付材料

Codex 完成后请按以下格式报告：

```text
Summary
Changed files
Test commands
Test results
Known limitations
是否修改 API：是/否；如是，说明原因
是否引入动态内存：否
是否引入外部依赖：否
是否实现滤波/SQI/峰值检测/IBI 算法：否
是否执行 Git 远程操作：否
```


## M2 执行记录

- 已由 Codex 落地 M2 API 骨架、资源预算冻结与最小测试骨架。
