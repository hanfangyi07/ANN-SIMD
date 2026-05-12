//用于生成基础版 PQ 查表累加（标量）汇编代码的测试源文件
#include <cstdint>
#include <vector>

void adc_scan(const std::vector<std::vector<uint8_t>>& codes,
              const std::vector<std::vector<float>>& lut,
              size_t base_num, int m, float* approx_dist) {
    for (size_t i = 0; i < base_num; ++i) {
        float dist = 0.0f;
        for (int s = 0; s < m; ++s) {
            uint8_t id = codes[s][i];
            dist += lut[s][id];
        }
        approx_dist[i] = dist;
    }
}