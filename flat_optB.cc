//生成手写 NEON 优化 B 的汇编代码
#include <arm_neon.h>
#include <cstddef>

float inner_product_neon_optB(const float* a, const float* b, size_t d) {
    float32x4_t sum1 = vdupq_n_f32(0.0f);
    float32x4_t sum2 = vdupq_n_f32(0.0f);
    float32x4_t sum3 = vdupq_n_f32(0.0f);
    float32x4_t sum4 = vdupq_n_f32(0.0f);
    size_t i = 0;
    for (; i + 16 <= d; i += 16) {
        float32x4x4_t va = vld1q_f32_x4(a + i);
        float32x4x4_t vb = vld1q_f32_x4(b + i);
        sum1 = vmlaq_f32(sum1, va.val[0], vb.val[0]);
        sum2 = vmlaq_f32(sum2, va.val[1], vb.val[1]);
        sum3 = vmlaq_f32(sum3, va.val[2], vb.val[2]);
        sum4 = vmlaq_f32(sum4, va.val[3], vb.val[3]);
    }
    float32x4_t sum12 = vaddq_f32(sum1, sum2);
    float32x4_t sum34 = vaddq_f32(sum3, sum4);
    float32x4_t sum = vaddq_f32(sum12, sum34);
    float total = vaddvq_f32(sum);
    for (; i < d; ++i) total += a[i] * b[i];
    return 1.0f - total;
}