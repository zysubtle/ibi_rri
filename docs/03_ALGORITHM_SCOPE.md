# 03 Algorithm Scope v0.1

## 算法目标

从 4 路同步绿光 PPG 信号中输出逐搏 IBI event，供外部 HRV / RMSSD 计算使用。

## 当前输入

- 4 路 PPG：`PPG_G1`、`PPG_G2`、`PPG_G3`、`PPG_G4`
- 采样率：50Hz
- 时间戳：`uint32_t timestamp_ms`
- 运动门控：`allow_measure`

## 当前输出

逐搏 IBI event：

- `timestamp_ms`
- `ibi_ms`
- `beat_count`
- `confidence`
- `signal_quality`
- `selected_channel`
- `state`
- `reject_reason`

## 模块范围

当前计划模块：输入接口与采样计数、timestamp 与 sample counter 一致性检查、PPG 多通道基础预处理、SQI / 信号质量评估、主通道选择、脉搏峰 / 脉搏周期候选检测、IBI 合法性检查、运动 / 低质量时不输出、状态机、输出 event、host 端测试和仿真、资源预算审查、MCU 移植准备。

## 当前非范围

不计算 RMSSD；不输出 HR；不做 ECG 对照评估；不做医疗诊断；不做 ACC 内部运动识别；不使用第三方 PPG / IBI / HRV 算法库；不引入 CMSIS-DSP 或厂商 DSP 库，除非 S0 决策确认；不做复杂 GUI 或上位机可视化。

## 准确性边界

当前无 ECG / 人工标注 / 参考 IBI，早期只能通过 smoke test、工程合理性、人工可视化检查推进。单个 fixture 不能作为总体准确性的证明。
