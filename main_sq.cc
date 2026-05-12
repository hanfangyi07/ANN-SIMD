//专门测试 SQ-SIMD 的源文件
#include <vector>
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include <set>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <sys/time.h>
#include <omp.h>
#include "hnswlib/hnswlib/hnswlib.h"
#include "flat_scan.h"
#include "my_simd.h"
#include <limits>
#include <algorithm>
#include <cfloat>  
#include <cstdint>
#include <cstdlib>
// 可以自行添加需要的头文件

using namespace hnswlib;

template<typename T>
T *LoadData(std::string data_path, size_t& n, size_t& d)
{
    std::ifstream fin;
    fin.open(data_path, std::ios::in | std::ios::binary);
    fin.read((char*)&n,4);
    fin.read((char*)&d,4);
    T* data = new T[n*d];
    int sz = sizeof(T);
    for(int i = 0; i < n; ++i){
        fin.read(((char*)data + i*d*sz), d*sz);
    }
    fin.close();

    std::cerr<<"load data "<<data_path<<"\n";
    std::cerr<<"dimension: "<<d<<"  number:"<<n<<"  size_per_element:"<<sizeof(T)<<"\n";

    return data;
}

struct SearchResult
{
    float recall;
    int64_t latency; // 单位us
};

void build_index(float* base, size_t base_number, size_t vecdim)
{
    const int efConstruction = 150; // 为防止索引构建时间过长，efc建议设置200以下
    const int M = 16; // M建议设置为16以下

    HierarchicalNSW<float> *appr_alg;
    InnerProductSpace ipspace(vecdim);
    appr_alg = new HierarchicalNSW<float>(&ipspace, base_number, M, efConstruction);

    appr_alg->addPoint(base, 0);
    #pragma omp parallel for
    for(int i = 1; i < base_number; ++i) {
        appr_alg->addPoint(base + 1ll*vecdim*i, i);
    }

    char path_index[1024] = "files/hnsw.index";
    appr_alg->saveIndex(path_index);
}
// 加载码本：m 个子空间，每个子空间 256 个中心，每个中心 sub_dim 维 float
std::vector<std::vector<float>> load_codebook(const std::string& path, int m, int sub_dim) {
    std::ifstream fin(path, std::ios::binary);
    if (!fin) {
        std::cerr << "Failed to open codebook file: " << path << std::endl;
        exit(1);
    }
    std::vector<std::vector<float>> codebook(m);
    for (int s = 0; s < m; ++s) {
        codebook[s].resize(256 * sub_dim);
        fin.read((char*)codebook[s].data(), 256 * sub_dim * sizeof(float));
        if (!fin) {
            std::cerr << "Error reading codebook for subspace " << s << std::endl;
            exit(1);
        }
    }
    fin.close();
    return codebook;
}

// 加载编码：m 个子空间，每个子空间 base_num 个 uint8
std::vector<std::vector<uint8_t>> load_codes(const std::string& path, int m, size_t base_num) {
    std::ifstream fin(path, std::ios::binary);
    if (!fin) {
        std::cerr << "Failed to open codes file: " << path << std::endl;
        exit(1);
    }
    std::vector<std::vector<uint8_t>> codes(m);
    for (int s = 0; s < m; ++s) {
        codes[s].resize(base_num);
        fin.read((char*)codes[s].data(), base_num);
        if (!fin) {
            std::cerr << "Error reading codes for subspace " << s << std::endl;
            exit(1);
        }
    }
    fin.close();
    return codes;
}

int main(int argc, char *argv[])
{
    size_t test_number = 0, base_number = 0;
    size_t test_gt_d = 0, vecdim = 0;
    size_t p = 2000;  // 粗排候选数，可以手动修改测试不同值

    std::string data_path = "/anndata/"; 
    auto test_query = LoadData<float>(data_path + "DEEP100K.query.fbin", test_number, vecdim);
    auto test_gt = LoadData<int>(data_path + "DEEP100K.gt.query.100k.top100.bin", test_number, test_gt_d);
    auto base = LoadData<float>(data_path + "DEEP100K.base.100k.fbin", base_number, vecdim);
    //SQ 离线预处理
  // 计算每维的最小值和最大值
std::vector<float> dim_min(vecdim, std::numeric_limits<float>::max());
std::vector<float> dim_max(vecdim, std::numeric_limits<float>::lowest());
for (size_t i = 0; i < base_number; ++i) {
    const float* vec = base + i * vecdim;
    for (size_t d = 0; d < vecdim; ++d) {
        if (vec[d] < dim_min[d]) dim_min[d] = vec[d];
        if (vec[d] > dim_max[d]) dim_max[d] = vec[d];
    }
}

// 量化 base 为 int8_t
std::vector<int8_t> base_quant(base_number * vecdim);
for (size_t i = 0; i < base_number; ++i) {
    const float* vec = base + i * vecdim;
    int8_t* qvec = base_quant.data() + i * vecdim;
    for (size_t d = 0; d < vecdim; ++d) {
        float t = (vec[d] - dim_min[d]) / (dim_max[d] - dim_min[d]);
        if (t < 0.0f) t = 0.0f;
        if (t > 1.0f) t = 1.0f;
        // 映射到 [-128, 127]
        qvec[d] = static_cast<int8_t>(t * 255.0f - 128.0f);
    }
}
    // 只测试前2000条查询
    test_number = 2000;

    const size_t k = 10;

    std::vector<SearchResult> results;
    results.resize(test_number);

    // 如果你需要保存索引，可以在这里添加你需要的函数，你可以将下面的注释删除来查看pbs是否将build.index返回到你的files目录中
    // 要保存的目录必须是files/*
    // 每个人的目录空间有限，不需要的索引请及时删除，避免占空间太大
    // 不建议在正式测试查询时同时构建索引，否则性能波动会较大
    // 下面是一个构建hnsw索引的示例
    // build_index(base, base_number, vecdim);
  
    
    // 查询测试代码
    for(int i = 0; i < test_number; ++i) {
        const unsigned long Converter = 1000 * 1000;
        struct timeval val;
        int ret = gettimeofday(&val, NULL);
     


        // 该文件已有代码中你只能修改该函数的调用方式
        // 可以任意修改函数名，函数参数或者改为调用成员函数，但是不能修改函数返回值。
        auto res = sq_search(base, base_quant.data(), test_query + i * vecdim,
                     dim_min.data(), dim_max.data(),
                     base_number, vecdim, k, 200);   // p=200
        struct timeval newVal;
        ret = gettimeofday(&newVal, NULL);
        int64_t diff = (newVal.tv_sec * Converter + newVal.tv_usec) - (val.tv_sec * Converter + val.tv_usec);

        std::set<uint32_t> gtset;
        for(int j = 0; j < k; ++j){
            int t = test_gt[j + i*test_gt_d];
            gtset.insert(t);
        }

        size_t acc = 0;
        while (res.size()) {   
            int x = res.top().second;
            if(gtset.find(x) != gtset.end()){
                ++acc;
            }
            res.pop();
        }
        float recall = (float)acc/k;

        results[i] = {recall, diff};
    }

    float avg_recall = 0, avg_latency = 0;
    for(int i = 0; i < test_number; ++i) {
        avg_recall += results[i].recall;
        avg_latency += results[i].latency;
    }

    // 浮点误差可能导致一些精确算法平均recall不是1
    std::cout << "average recall: "<<avg_recall / test_number<<"\n";
    std::cout << "average latency (us): "<<avg_latency / test_number<<"\n";
    return 0;
}
