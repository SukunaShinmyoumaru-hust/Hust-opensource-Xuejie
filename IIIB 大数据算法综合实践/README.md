### 提示
- 可以参考VOLT（Vertex Orderings to List Triangles faster），https://github.com/lecfab/volt
- 是一种比较新的SOTA方法
- 优化思路：加并行处理、改变读写逻辑（例如只读一次再写一次，或者不写，避免中间冗余结果）等
