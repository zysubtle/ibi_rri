# 05 MCU Algorithm Strategy v0.1

## 总策略

自研 C 语言 MCU 算法，从 4 路同步绿光 PPG 中流式逐点输出 IBI event。

## 禁止事项

禁止引入第三方 PPG / IBI / HR / HRV 算法库；禁止使用 Python 第三方库替代 C 算法；禁止引入 NeuroKit2、HeartPy、BioSPPy、WFDB、SciPy、NumPy 作为核心实现；禁止擅自引入 CMSIS-DSP、厂商 DSP 库或芯片 SDK 特殊数学函数；禁止 `malloc/calloc/realloc/free`；禁止递归；禁止大数组上栈。

## 允许事项

算法核心使用 C；允许 `float`；允许必要 C 标准库基础能力；允许 host 端编译和 smoke test；测试脚本可使用 Python 标准库，但不得替代 MCU C 算法。

## 流式处理原则

`process()` 逐点调用；每次输入 4 路 PPG 和 timestamp；内部保存必要状态，不依赖动态内存；参数集中管理，避免魔法数散落；输出 event 必须可追踪 timestamp、state、reject reason。

## 初始算法模块建议

输入校验、预处理、SQI、通道选择、候选检测、IBI 合法性、状态机、输出事件、测试工具。

## S0 决策边界

以下变化必须由 Owner 决策：核心算法策略、API、输出字段、MCU 平台、资源预算、是否允许第三方库、是否允许 CMSIS-DSP、是否允许 malloc、是否允许 float、采样率、运动门控策略、IBI 输出策略。
