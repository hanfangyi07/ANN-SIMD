import numpy as np
import struct
from sklearn.cluster import KMeans
import os

# ========== 1. 加载 base 向量 ==========
def load_fbin(filename):
    """读取 DEEP100K 的 fbin 格式文件，返回 (数据数组, 向量个数, 维度)"""
    with open(filename, 'rb') as f:
        n = struct.unpack('I', f.read(4))[0]   # 读取 4 字节，表示向量个数
        d = struct.unpack('I', f.read(4))[0]   # 读取 4 字节，表示维度
        data = np.fromfile(f, dtype=np.float32).reshape(n, d)
    return data, n, d

print("Loading base vectors...")
base, base_num, vecdim = load_fbin('DEEP100K.base.100k.fbin')
print(f"Loaded {base_num} vectors of dimension {vecdim}")

# ========== 2. 设置 PQ 参数 ==========
m = 8                     # 子空间个数（96 / 8 = 12 维/子空间）
sub_dim = vecdim // m     # 每个子空间的维度 = 12
k_sub = 256               # 每个子空间的聚类数（2^8，用 1 个字节表示）

print(f"Subspace dimension: {sub_dim}, number of clusters per subspace: {k_sub}")

# 存储结果
codebook = []   # 每个子空间的码本，形状 (256, sub_dim)
codes = []      # 每个子空间的编码，形状 (base_num,)

# ========== 3. 对每个子空间进行 k-means 聚类 ==========
for s in range(m):
    print(f"Processing subspace {s+1}/{m} ...")
    # 提取该子空间的所有子向量 (base_num, sub_dim)
    start = s * sub_dim
    end = (s+1) * sub_dim
    sub_vectors = base[:, start:end]
    
    # 执行 k-means 聚类，生成 256 个中心
    kmeans = KMeans(n_clusters=k_sub, random_state=0, n_init=10, verbose=0)
    kmeans.fit(sub_vectors)
    
    # 保存中心点（浮点数）
    codebook.append(kmeans.cluster_centers_.astype(np.float32))
    # 保存每个向量的编码（0~255 的整数，用 uint8 存储）
    codes.append(kmeans.labels_.astype(np.uint8))
    
    print(f"  Subspace {s+1} done.")

# ========== 4. 保存为二进制文件，供 C++ 程序读取 ==========
# 保存原始 AoS 码本（基础版使用）
print("Saving codebook.bin ...")
with open('codebook.bin', 'wb') as f:
    for s in range(m):
        f.write(codebook[s].tobytes())
print("Saved codebook.bin")

# 新增：保存 SoA 布局码本（优化版使用）
print("Saving codebook_soa.bin ...")
with open('codebook_soa.bin', 'wb') as f:
    for s in range(m):
        # 转置并保存
        codebook_soa = codebook[s].T.astype(np.float32)
        f.write(codebook_soa.tobytes())
print("Saved codebook_soa.bin")

# 保存编码（基础版和优化版共用）
print("Saving codes.bin ...")
with open('codes.bin', 'wb') as f:
    for s in range(m):
        f.write(codes[s].tobytes())
print("Saved codes.bin")

print("All done! Now upload codebook.bin, codebook_soa.bin and codes.bin to the server's files/ directory.")