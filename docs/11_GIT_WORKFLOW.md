# 11 Git Workflow v0.1

## 职责边界

Owner 负责创建分支、将文件加入本地仓库、push 到远程仓库、创建 PR、合并 PR。

Codex 不负责创建 PR、push、merge、rebase、删除远程分支、改写 main / master 历史。

Architect / Reviewer 负责基于 PR 链接、项目 zip、关键 diff / 修改文件、测试日志进行审查，判断是否满足当前 Milestone，并给出决策卡。

## 推荐流程

1. Owner 创建本地分支，例如：`git checkout -b feature/m1-startup-pack`
2. Owner 将 M1 package 解压到仓库根目录。
3. Owner 将仓库交给 Codex，并给出一句话指令：

```text
请读取 docs/10_CODEX_NEXT_TASK.md，并严格执行。请不要创建 PR、不要 push、不要 merge；完成后报告修改文件和测试结果。
```

4. Owner 本地检查后自行 commit / push。
5. Owner 自行创建 PR，或直接提供 zip / diff / 测试日志给 Architect 审查。

## 审查材料

正式审查至少提供以下之一：PR 链接、项目 zip、关键 diff / 修改文件 + 测试日志、Codex 输出摘要 + 修改文件列表 + 测试命令 + 测试结果。

如果只有文字摘要、没有可审查的文件或测试证据，不应判定通过。
