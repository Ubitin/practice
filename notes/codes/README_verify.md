# CF377A（`test_\01.cpp`）本地自检步骤

> 每次改完代码照着跑一遍，10 秒出结论。工具都在本文件夹里。
> 记录时间：2026-09-25（最近一次实测见文末）

---

## 一、编译（必须带警告，`-Wall` 里就有 `=`/`==` 那条）

```powershell
g++ -O2 -std=c++17 -Wall -Wextra -Wshadow -o .\test_\01_verify.exe .\test_\01.cpp
```

⚠️ 警告一条都不该有（当前只剩 `tx/ty may be used uninitialized`，是本题数据碰不到的那条）。

## 二、跑用例（用 `cmd /c` 重定向，**不要用 PowerShell 的 `>`**）

PowerShell 的 `>` 会写成 UTF-16，C++ 读进去全是乱码（这个坑已经踩过一次，三个程序齐刷刷 `0xC0000005`）。

```powershell
cmd /c ".\test_\01_verify.exe < .\test_\param_maze.txt > .\test_\v_out_param_maze.txt"
python .\test_\cf377a_check.py .\test_\param_maze.txt .\test_\v_out_param_maze.txt
```

期望输出：`OK: exactly N walls added, walls intact, M empty cells connected`

## 三、现成的用例

| 用例 | 内容 | 期望 |
|---|---|---|
| `param_maze.txt` | 3×4 小迷宫，k=2 | OK（9 格连通） |
| `e_1x1.txt` | 1×1，k=0 | OK（1 格连通） |
| `e_1x5.txt` | 1×5，k=2 | OK（3 格连通） |
| `snake500.txt` | **500×500 蛇形走廊**（12.5 万格深） | ⚠️ **默认栈会崩**，见下 |

## 四、⚠️ 蛇形走廊的爆栈（本题唯一真实风险）

```powershell
# 默认栈：退出码 -1073741571 = 0xC00000FD，输出 0 字节
cmd /c ".\test_\01_verify.exe < .\test_\snake500.txt > .\test_\v_out_snake500.txt"

# 加大栈：正常通过
g++ -O2 -std=c++17 '-Wl,--stack,268435456' -o .\test_\01_verify_stack.exe .\test_\01.cpp
cmd /c ".\test_\01_verify_stack.exe < .\test_\snake500.txt > .\test_\v_out_snake_stack.txt"
python .\test_\cf377a_check.py .\test_\snake500.txt .\test_\v_out_snake_stack.txt
```

> PowerShell 里 `-Wl,--stack,268435456` **必须加引号**，否则逗号被当成参数分隔符，报 `Missing argument in parameter list`。

**更稳的做法**：把这题的 `dfs` 换成显式栈的迭代版（讲义 `CF377A迷宫-反着删格子的连通构造` §四 / 手册 §4.8 有现成骨架）。

## 五、参考解（迭代 DFS，默认栈就能过）

```powershell
cmd /c ".\test_\cf377a_ref.exe < .\test_\snake500.txt > .\test_\v_ref_snake.txt"
```

---

## 最近一次实测记录（2026-09-25，针对 `01.cpp` 的 1608 字节版本）

| 用例 | 退出码 | checker |
|---|---|---|
| 编译 `-Wall -Wextra -Wshadow` | 0 | 只剩 `tx/ty` 两条警告 |
| `param_maze.txt` | 0 | `OK: exactly 2 walls added, walls intact, 9 empty cells connected` ✅ |
| `e_1x1.txt` | 0 | `OK: exactly 0 walls added, walls intact, 1 empty cells connected` ✅ |
| `e_1x5.txt` | 0 | `OK: exactly 2 walls added, walls intact, 3 empty cells connected` ✅ |
| `snake500.txt`（默认栈） | **-1073741571** | ❌ 输出 0 行 |
| `snake500.txt`（`-Wl,--stack,268435456`） | 0 | `OK: exactly 5 walls added, walls intact, 125245 empty cells connected` ✅ |
| `cf377a_ref.exe`（参考解） | 0 | 同上 ✅ |

**结论**：`=` → `==` 那处致命 bug 已经改对，前三个用例全过；**唯一剩下的风险是递归深度**。
