# priority_queue 允许元素重复吗？—— 允许

> 触发点：想知道 `std::priority_queue` 能不能存重复元素（会不会像 `set` 那样自动去重）。
> 一句话：**完全允许。** `priority_queue` 是**序列容器适配器**（默认底层 `vector`），`push` 就是 `push_back` + 上浮，**没有任何查重步骤**；只有**关联容器**（`set` / `map` / `unordered_*`）才有"键唯一"的要求。
> 实测程序：`test_\35.cpp`。
> 相关：`priority_queue无count-按值计数怎么做`（那篇讲"堆里某个值有几个怎么数"，正是因为有重复才需要数）。

---

## 一、最直接的证据

```cpp
priority_queue<int> pq;
pq.push(5); pq.push(5); pq.push(5);
```

**实测**：

```
push(5) ×3 之后 size() = 3
依次 pop：5 5 5
```

三个 5 全部保留、逐个弹出。

再加一个极端场景——**push 10 万个 42**：

```
push(42) × 100000 → size = 100000，top = 42
全部弹出后数到 100000 个 42
```

**重复元素不会被合并、不会被丢弃。** 堆里就是老老实实放着 10 万个。

## 二、和 `set` / `multiset` 对照

同一个"插入 3 个 5"的操作，结果完全不同：

```
set::insert(5) 第 1 次：插入成功，size=1
set::insert(5) 第 2 次：插入【失败，已存在】，size=1
set::insert(5) 第 3 次：插入【失败，已存在】，size=1
multiset 插入 3 次 5：size = 3，count(5) = 3
```

| 容器 | 结果 |
|---|---|
| `set` | **去重**，size = 1 |
| `multiset` | **保留**，size = 3 |
| `priority_queue` | **保留**，size = 3 |

## 三、为什么要看 `push` 的返回值 —— 这是最本质的判别法

```cpp
set<int> s;
auto r = s.insert(5);     // 返回 pair<iterator, bool>
                          // bool = 是否真的插进去了（重复时为 false）

priority_queue<int> pq;
pq.push(5);               // 返回 void —— 根本没有"插入失败"这个概念
```

**编译期核实**（`static_assert` 通过）：

```cpp
static_assert(is_same<decltype(declval<priority_queue<int>&>().push(1)), void>::value);
// ✔ push 的返回类型就是 void
```

> **`set` 的 `insert` 需要返回"成功/失败"，因为它可能失败；`priority_queue` 的 `push` 永远成功，所以无需返回值。** 这个接口设计本身就说明了它不去重。

## 四、判别标准：序列容器 vs 关联容器

| 容器 | 允许重复？ | 插入 3 个 5 后的 `size()` |
|---|---|---|
| **`priority_queue<int>`** | **✔ 允许** | **3** |
| `vector` / `deque` / `list` | ✔ 允许 | 3 |
| `stack` / `queue` | ✔ 允许 | 3 |
| `multiset` / `multimap` | ✔ 允许 | 3 |
| `set` / `map` | ✘ 键唯一 | 1 |
| `unordered_set` / `unordered_map` | ✘ 键唯一 | 1 |
| `unordered_map` 当计数器用 | ✘ 键唯一 | 1（但 `m[5] == 3`） |

### 一句话判别法

> - **序列容器 / 适配器**（`vector` / `deque` / `list` / `stack` / `queue` / **`priority_queue`**）→ **不管值，来就收**，允许重复
> - **关联容器**（`set` / `map` / `unordered_*`）→ **按键去重**；**名字带 `multi` 的才保留重复**

`priority_queue` 属于前者——它只是"一个序列容器 + 堆算法"的包装。

## 五、"相等"在这里是什么意思

堆**只通过 `operator<`（或自定义比较器）判断元素大小关系**，它不关心"这个值是不是已经存在"。所以：

```cpp
struct Node { int v; string name; };
bool operator<(const Node& a, const Node& b) { return a.v < b.v; }

priority_queue<Node> qn;
qn.push({5,"a"}); qn.push({5,"b"}); qn.push({5,"c"});   // v 都相同
// 实测：size = 3，三个都留着，弹出顺序 {5,a} {5,b} {5,c}
```

**只比 `v`，三个元素"等价"，但依然全部保留。** 对 `pair` 也一样：

```
push({5,7}) ×3 → size = 3
(5,7) (5,7) (5,7)
```

> ⚠️ 两个"等价"（谁也不小于谁）的元素，在堆里的**相对顺序不保证**——堆不是稳定结构。如果你的比较器只用 `v`，就别指望同 `v` 的元素按插入顺序出来。
> 想让同 `v` 的元素也有确定次序，**把 `name` 或插入序号加进比较器**（比如存 `pair<pair<int,int>, string>`，或给每个元素一个递增 `id`）。

## 六、允许重复这件事的实际意义

重复是**常态而不是例外**，很多模板都依赖它：

| 场景 | 为什么需要重复 |
|---|---|
| **Top-k**（小根堆只留 k 个） | 数据里本来就有重复值，堆里保留重复才能正确统计 |
| **合并果子 / 哈夫曼** | 合并出的新果子可能和已有的同重量 |
| **Dijkstra** | 同一节点可能被多次入堆（不同距离），靠"重复入堆 + 弹出时判过期"来工作 |
| **滑动窗口最值** | 窗口内可能有多个相同值 |
| **优先队列 BFS / A\*** | 同一状态可能以不同代价多次入队 |

**反过来说**：正因为 `priority_queue` 保留重复，它才需要你另想办法数个数——这就是 `priority_queue无count-按值计数怎么做` 那篇要解决的问题（堆**没有** `count` / `find` / `erase`，想要"5 有几个"得另开 `unordered_map` 计数表）。

## 七、常见误解与坑

| # | 误解 / 坑 | 事实 |
|---|---|---|
| 1 | "堆会自动去重" | ❌ 它保留全部元素；`size()` 就是总个数 |
| 2 | "`pq.size()` 是不同值的个数" | ❌ 是**元素总个数**（含重复）。要知道"5 有几个"得另开计数表 |
| 3 | 用 `multiset` 代替堆时忘了 `multi` | `set` 会去重，必须写 `multiset` |
| 4 | 以为同值元素的出堆顺序稳定 | ❌ 堆不稳定。要定序就把次序信息放进比较器 |
| 5 | 以为 `push` 会返回"是否插入成功" | ❌ 返回 `void`（永远成功） |
| 6 | 拿 `set` 当"堆 + 去重"用，结果丢数据 | 若题目要保留重复，`set` 会静默丢掉元素，`size()` 变小 → WA |
| 7 | `unordered_map` 计数时忘同步 `pop` | 计数与堆必须成对维护，否则不一致 |

> ⚠️ **第 6 条最危险**：把 `priority_queue` 换成 `set` 想"顺便去重"，如果题目本来需要重复，会**静默少元素**——样例可能恰好没有重复值，于是本地过、评测 WA。

## 八、自查

1. `priority_queue` 允许重复元素吗？为什么？
2. 怎么用一个接口特征快速判断"这个容器会不会去重"？
3. `set`、`multiset`、`priority_queue` 各插入 3 个 5，`size()` 分别是多少？
4. `pq.size() == 3` 能说明堆里有 3 种不同的值吗？
5. 堆里两个"等价"（比较器认为相等）的元素，弹出顺序有保证吗？想要保证怎么办？

**答案要点**
1. 允许。它是序列容器适配器（底层默认 `vector`），`push` 只做 `push_back` + 上浮，不做任何查重；只有关联容器才有"键唯一"要求。
2. 看插入接口的返回值：`set::insert` 返回 `pair<iterator,bool>`（可能失败 ⇒ 会去重）；`priority_queue::push` 返回 `void`（永远成功 ⇒ 不去重）。
3. 分别是 1、3、3。
4. 不能。`size()` 是**元素总个数**（含重复），不是不同值的个数。要数某个值几次得用 `unordered_map` 计数表（或换 `multiset`）。
5. 没有保证，堆不是稳定结构。若需要确定顺序，把次序信息（如插入序号）加进比较器。

---

## 附录：`test_\35.cpp` 实测输出

```
========== [1] priority_queue：push 三个 5，看是否都保留 ==========
  push(5) ×3 之后 size() = 3
  依次 pop：5 5 5
  → 三个 5 全部保留、逐个弹出 ✔ 允许重复

========== [2] 和 set / multiset 对照：能不能存下 3 个 5 ==========
  set::insert(5) 第 1 次：插入成功，size=1
  set::insert(5) 第 2 次：插入【失败，已存在】，size=1
  set::insert(5) 第 3 次：插入【失败，已存在】，size=1
  multiset 插入 3 次 5：size = 3，count(5) = 3

========== [3] priority_queue 的 push 有没有返回值？ ==========
  set::insert 返回 pair<iterator,bool>，能告诉你插进去了没有
  priority_queue::push 返回 void —— 没有任何「插入失败」的概念
  编译期核实：decltype(pq.push(1)) 就是 void ✔（static_assert 通过）

========== [4] pair / 自定义结构体 也允许完全重复 ==========
  push({5,7}) ×3 → size = 3
  (5,7)   (5,7)   (5,7)
  push(Node{5,a/b/c}) ×3 → size = 3
  {5,a}   {5,b}   {5,c}

========== [5] 含大量重复值的极端场景 ==========
  push(42) × 100000 → size = 100000，top = 42
  全部弹出后数到 100000 个 42

========== [6] 汇总：各容器/适配器 允许重复吗 ==========
  容器                             允许重复? 插入 3 个 5 后的 size
  priority_queue<int>                ✔ 允许   3
  vector / deque / list              ✔ 允许   3
  multiset / multimap                ✔ 允许   3
  set / map                          ✘ 键唯一 1
  unordered_set / unordered_map      ✘ 键唯一 1
  unordered_map 当计数器用           ✘ 键唯一 1（但 m[5]=3）

========== [7] 常见误解：以为 priority_queue 会去重 ==========
  数据 {5,1,9,5,5,3,7,5,2} 共 9 个
  priority_queue 的 size() = 9（没有变成 6）
```
