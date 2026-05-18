# PPG-IBI MCU 算法项目

当前状态：M2（IO Contract、MCU C API 骨架、资源预算冻结）。

本项目目标是在智能手环 / 智能戒指 / 可穿戴设备 MCU 上，从 4 路同步绿光 PPG 信号中计算逐搏 IBI，用于后续 HRV / RMSSD 分析。

## 当前边界（M2）

- 算法核心语言：C。
- 不使用第三方 PPG / IBI / HR / HRV 算法库。
- 不引入 CMSIS-DSP、厂商 DSP 库或外部算法依赖。
- 允许 `float`，禁止 `malloc/calloc/realloc/free`。
- 仅输出 IBI event；不输出 `hr_bpm`，不在算法内部计算 RMSSD。
- 无 ACC 输入；运动门控由外部 `allow_measure` 控制。
- M2 仅冻结 API 与工程边界，不实现真实滤波/SQI/峰值检测/IBI 计算。
- 当前项目不作为医疗诊断软件。

## 历史说明

M1 的文档启动包已完成，M2 在其基础上冻结 IO Contract、资源预算与测试骨架要求。

## 示例数据（smoke test）

路径：`tests/fixtures/sample_ppg.csv`

字段：`timestamp_ms,PPG_G1,PPG_G2,PPG_G3,PPG_G4`

已知特征：
- 行数：1001（不含表头）
- 时间戳：0ms 到 20000ms
- 步进：20ms
- 采样率：50Hz
- SHA256：`5ed94bc62222cf52a264d44d260efdd5dbb72976e7489574c2aeb9ae95b758f8`

该数据仅用于 smoke test / 回归样例，不能证明算法总体准确性。
