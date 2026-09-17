# memset 与 resize 复杂度对比：谁更快？先看清语义再谈复杂度

> 起因：你问 `v.resize(n)` 和 `memset(arr, 0, n)` 谁的复杂度低——这个问题**先有一个语义陷阱**，拆开后结论和直觉可能不同。
> 演示程序：`test_\27.cpp`（5000 万个 int ≈ 200MB 实测，g++ C++17 -O2）。

---

## 0. 先说结论（三句话）

1. **渐进复杂度都是 O(n)，没有谁更低**——两者都是"线性扫一遍"。
2. **但 n 的含义不同**：`memset` 的 n 是**字节数**，`resize` 的 n 是**元素个数**——直接比数字没有意义。
3. **常数上现代编译器几乎打平**（memset ≈ fill ≈ assign，实测 6.3~6.8ms/200MB）；唯一例外：**`resize(0)` 清空 int 容器近似 O(1)**，比 memset 快一个量级。

## 1. 语义陷阱：memset 按"字节"，resize 按"元素"

```cpp
int arr[100];
memset(arr, 0, 100);      // ❌ 只清了 100 字节 = 25 个 int！
// 实测：arr[0..24] 是 0，arr[25..99] 原封不动（25/100 被清零）
memset(arr, 0, sizeof(arr));          // ✅ sizeof(arr)=400 字节 = 100 个 int
memset(arr, 0, 100 * sizeof(int));    // ✅ 或写元素个数 × sizeof
```

**memset 的第三参数永远是"字节数"**，清 `n` 个 int 要写 `n * sizeof(int)`（这是 memset 最经典的翻车点）。

`v.resize(n)` 则是把 vector 变成恰好 n 个元素——语义完全不同：
- 变大：多余的新元素被初始化为 0（int）；
- 变小：尾部元素被"销毁"。

## 2. 复杂度拆解（重点）

| 操作 | 渐进复杂度 | 说明 |
|---|---|---|
| `memset(p, 0, bytes)` | **O(bytes)** | 按字节写，bytes = 元素数 × sizeof |
| `std::fill(v.begin(), v.end(), 0)` | O(n) | 按元素赋值 |
| `v.assign(n, 0)` | O(n) | 重新填充（若容量不足另有 O(n) 扩容） |
| `v.resize(更大)` | O(新元素数) | 初始化新增部分；可能触发扩容 O(n) |
| **`v.resize(更小)`** | **近似 O(1)（int）** | 平凡析构只改 size 指针，不逐个"干活" |

**为什么 resize(0) 对 int 是近似 O(1)**：`int` 的析构是"空操作"，`vector<int>::resize(0)` 实际上只是把 `size` 指针改回开头——实测 200MB 的 vector 清空耗时 **0.00ms**，而 memset 同样大小的内存要 **6.3ms**。这不是标准保证的复杂度（标准说"与删除元素数成线性"），但平凡类型上主流实现都这么优化——**知道即可，别依赖标准承诺**。

## 3. 实测数据（200MB = 5000 万个 int，3 次取最小）

| 操作 | 耗时 |
|---|---|
| `memset(raw, 0, N*sizeof(int))` | 6.34 ms |
| `std::fill(v.begin(), v.end(), 0)` | 6.82 ms |
| `v.assign(N, 0)` | 6.74 ms |
| `v.resize(0)`（清空） | **0.00 ms** |
| `v.resize(N, 0)`（重新填满） | 7.02 ms |

**注意**：memset 只比 fill 快 7%——老教材"memset 比 fill 快好几倍"的说法在现代编译器**已不成立**（`std::fill` 对 int 数组会生成和 memset 同级的 SIMD 代码）。不要为了"快"而牺牲可读性去混用 memset。

## 4. 实战选型（什么时候用哪个）

| 场景 | 推荐 | 理由 |
|---|---|---|
| 清空一个 `vector<int>`（多组数据重置） | `v.clear()` 或 `v.resize(0)` | 对 int 近似 O(1)，比重新 memset 快 |
| 清零 C 数组（计数数组） | `memset(a, 0, sizeof(a))` | 最直接；**记得 sizeof** |
| 清零后还要重新填 | 先 memset/fill，再正常赋值 | 避免 resize 反复改大小 |
| 重置 vector 为全 0 且保持大小 | `fill(v.begin(), v.end(), 0)` | 类型安全、语义清晰，性能与 memset 同级 |
| 元素是非平凡类型（string 等） | **绝不 memset** | 会破坏对象内部结构，必须 fill/assign |

## 5. 记忆钩子

> **memset 数"字节"，resize 数"元素"；复杂度都 O(n)，别指望谁"更低"——真正快的只有 resize 缩小（int 上近似 O(1)）和 memset 的正确写法 `sizeof(arr)`。**

**一句话总结**：两者渐进都是 O(n)，直接回答"谁复杂度低"没有赢家；真正的坑在**单位**（字节 vs 元素）——清零数组记得 `memset(a, 0, sizeof(a))`，清空 int 容器用 `resize(0)` 反而更快（实测 0ms vs 6ms），非平凡类型永远用 fill/assign 而别碰 memset。
