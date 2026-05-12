//用于生成优化版 PQ 查表累加（批量+预取）的汇编代码的测试源文件
#include <cstdint>
#include <cstddef>

// 模拟 LUT 和 codes 的指针访问（避免 STL 依赖，使汇编更干净）
void adc_scan_opt(const uint8_t* codes[8],  // 每个子空间的编码指针数组
                  const float* luts[8],     // 每个子空间的 LUT 指针数组
                  size_t base_num,          // base 向量数量
                  int m,                    // 子空间数（实际固定 8）
                  float* approx_dist) {
    const size_t BLOCK = 8;
    for (size_t i = 0; i < base_num; i += BLOCK) {
        float dist[BLOCK] = {0.0f};
        size_t valid = (base_num - i < BLOCK) ? base_num - i : BLOCK;

        for (int s = 0; s < m; ++s) {
            const uint8_t* code_ptr = codes[s] + i;
            const float* lut = luts[s];

            // 预取下一次迭代的编码（距离当前 16 个字节）
            __builtin_prefetch(code_ptr + 16, 0, 1);

            if (valid == BLOCK) {
                // 手工展开 8 个累加
                dist[0] += lut[code_ptr[0]];
                dist[1] += lut[code_ptr[1]];
                dist[2] += lut[code_ptr[2]];
                dist[3] += lut[code_ptr[3]];
                dist[4] += lut[code_ptr[4]];
                dist[5] += lut[code_ptr[5]];
                dist[6] += lut[code_ptr[6]];
                dist[7] += lut[code_ptr[7]];
            } else {
                for (size_t j = 0; j < valid; ++j)
                    dist[j] += lut[code_ptr[j]];
            }
        }

        for (size_t j = 0; j < valid; ++j)
            approx_dist[i + j] = dist[j];
    }
}