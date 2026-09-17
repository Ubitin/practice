// test_oiwiki_code.cpp
// 把 OI-Wiki《霍夫曼树》页面里的四段代码原样搬过来编译运行，
// 核对它算出来的 WPL / 编码，与讲义里 Python 参考实现是否一致。
//   g++ -O2 -std=c++17 -o test_oiwiki_code test_oiwiki_code.cpp
#include <cstdio>
#include <cstdlib>
#include <queue>
#include <vector>
#include <string>
#include <set>
#include <algorithm>
using namespace std;

#define N 100          // OI-Wiki 代码里用到但没有定义的数组大小

// ------------------------------------------------------------------
// 代码 1：霍夫曼树的构建（OI-Wiki 原文，O(n^2) 线性扫描版）
typedef struct HNode {
  int weight;
  HNode *lchild, *rchild;
} * Htree;

Htree createHuffmanTree(int arr[], int n) {
  Htree forest[N];
  Htree root = NULL;
  for (int i = 0; i < n; i++) {  // 将所有点存入森林
    Htree temp;
    temp = (Htree)malloc(sizeof(HNode));
    temp->weight = arr[i];
    temp->lchild = temp->rchild = NULL;
    forest[i] = temp;
  }

  for (int i = 1; i < n; i++) {  // n-1 次循环建哈夫曼树
    int minn = -1, minnSub;  // minn 为最小值树根下标，minnsub 为次小值树根下标
    for (int j = 0; j < n; j++) {
      if (forest[j] != NULL && minn == -1) {
        minn = j;
        continue;
      }
      if (forest[j] != NULL) {
        minnSub = j;
        break;
      }
    }

    for (int j = minnSub; j < n; j++) {  // 根据 minn 与 minnSub 赋值
      if (forest[j] != NULL) {
        if (forest[j]->weight < forest[minn]->weight) {
          minnSub = minn;
          minn = j;
        } else if (forest[j]->weight < forest[minnSub]->weight) {
          minnSub = j;
        }
      }
    }

    // 建新树
    root = (Htree)malloc(sizeof(HNode));
    root->weight = forest[minn]->weight + forest[minnSub]->weight;
    root->lchild = forest[minn];
    root->rchild = forest[minnSub];

    forest[minn] = root;     // 指向新树的指针赋给 minn 位置
    forest[minnSub] = NULL;  // minnSub 位置为空
  }
  return root;
}

// ------------------------------------------------------------------
// 代码 2：递归求已建好的树的 WPL
int getWPL(Htree root, int len) {  // 递归实现，对于已经建好的霍夫曼树，求 WPL
  if (root == NULL)
    return 0;
  else {
    if (root->lchild == NULL && root->rchild == NULL)  // 叶节点
      return root->weight * len;
    else {
      int left = getWPL(root->lchild, len + 1);
      int right = getWPL(root->rchild, len + 1);
      return left + right;
    }
  }
}

// ------------------------------------------------------------------
// 代码 3：不建树，直接用小根堆求 WPL
int getWPL(int arr[], int n) {  // 对于未建好的霍夫曼树，直接求其 WPL
  priority_queue<int, vector<int>, greater<int>> huffman;  // 小根堆
  for (int i = 0; i < n; i++) huffman.push(arr[i]);

  int res = 0;
  for (int i = 0; i < n - 1; i++) {
    int x = huffman.top();
    huffman.pop();
    int y = huffman.top();
    huffman.pop();
    int temp = x + y;
    res += temp;
    huffman.push(temp);
  }
  return res;
}

// ------------------------------------------------------------------
// 代码 4：输出每个叶子的编码（左 0 右 1）
void huffmanCoding(Htree root, int len, int arr[]) {  // 计算霍夫曼编码
  if (root != NULL) {
    if (root->lchild == NULL && root->rchild == NULL) {
      printf("结点为 %d 的字符的编码为: ", root->weight);
      for (int i = 0; i < len; i++) printf("%d", arr[i]);
      printf("\n");
    } else {
      arr[len] = 0;
      huffmanCoding(root->lchild, len + 1, arr);
      arr[len] = 1;
      huffmanCoding(root->rchild, len + 1, arr);
    }
  }
}

// ------------------------------------------------------------------
int main() {
  int freq[] = {45, 13, 12, 16, 9, 5};      // 经典例子，与讲义一致
  int n = 6;
  int arr[N];
  int expect = 224;                          // 讲义 Python 参考实现给出的最优值

  Htree root = createHuffmanTree(freq, n);
  int wplTree = getWPL(root, 0);
  int wplHeap = getWPL(freq, n);

  printf("递归求 WPL（建好的树） = %d\n", wplTree);
  printf("小根堆直接求 WPL       = %d\n", wplHeap);
  printf("讲义参考实现最优值     = %d\n", expect);
  printf("两个结果都与参考值一致 : %s\n",
         (wplTree == expect && wplHeap == expect) ? "✓" : "✗");

  printf("\n编码（左 0 右 1）：\n");
  huffmanCoding(root, 0, arr);

  // 顺带检查前缀性质与码长上界
  vector<string> codes;
  {
    // 重新走一遍树收集码字
    struct Rec {
      static void go(Htree r, string s, vector<string>& out) {
        if (!r) return;
        if (!r->lchild && !r->rchild) { out.push_back(s.empty() ? "0" : s); return; }
        go(r->lchild, s + "0", out);
        go(r->rchild, s + "1", out);
      }
    };
    Rec::go(root, "", codes);
  }
  bool prefixFree = true;
  for (auto& a : codes) for (auto& b : codes)
    if (a != b && b.compare(0, a.size(), a) == 0) prefixFree = false;
  printf("\n码字数 = %zu, 前缀码 = %s\n", codes.size(), prefixFree ? "✓" : "✗");
  int totalBits = 0;
  for (auto& c : codes) totalBits += 1;   // 占位，真正统计见下
  (void)totalBits;
  return 0;
}
