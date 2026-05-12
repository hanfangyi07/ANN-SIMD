//生成编译器自动向量化版本的汇编代码
#include <cstddef>

float naive_dot(const float* a, const float* b, size_t n) {
    float sum = 0.0f;
    for (size_t i = 0; i < n; ++i) {
        sum += a[i] * b[i];
    }
    return sum;
}