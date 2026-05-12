//生成 int8 量化点积的汇编代码
#include <arm_neon.h>
#include <cstddef>

int32_t dot_int8_neon(const int8_t* a, const int8_t* b, size_t d) {
    int32x4_t sum = vdupq_n_s32(0);
    size_t i = 0;
    for (; i + 16 <= d; i += 16) {
        int8x16_t va = vld1q_s8(a + i);
        int8x16_t vb = vld1q_s8(b + i);
        int16x8_t prod_low = vmull_s8(vget_low_s8(va), vget_low_s8(vb));
        int16x8_t prod_high = vmull_s8(vget_high_s8(va), vget_high_s8(vb));
        sum = vpadalq_s16(sum, prod_low);
        sum = vpadalq_s16(sum, prod_high);
    }
    int32_t total = vaddvq_s32(sum);
    for (; i < d; ++i) total += a[i] * b[i];
    return total;
}