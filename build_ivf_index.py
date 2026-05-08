import numpy as np
import struct
from sklearn.cluster import KMeans
import os

def load_fbin(filename):
    with open(filename, 'rb') as f:
        n = struct.unpack('I', f.read(4))[0]
        d = struct.unpack('I', f.read(4))[0]
        data = np.fromfile(f, dtype=np.float32).reshape(n, d)
    return data, n, d

# 加载 base
base, base_num, dim = load_fbin('DEEP100K.base.100k.fbin')
print(f"Loaded base: {base_num} vectors, dim={dim}")

# IVF 参数
n_centroids = 256   # 聚类数量（可调，比如 256）
n_iter = 20

# K-means 聚类
print("Running k-means...")
kmeans = KMeans(n_clusters=n_centroids, random_state=0, n_init=10, verbose=1)
kmeans.fit(base)
centroids = kmeans.cluster_centers_.astype(np.float32)
labels = kmeans.labels_

# 构建倒排列表：每个中心对应的 base 向量索引
inverted_lists = [[] for _ in range(n_centroids)]
for idx, label in enumerate(labels):
    inverted_lists[label].append(idx)

# 保存中心点
with open('ivf_centroids.bin', 'wb') as f:
    f.write(centroids.tobytes())
print("Saved ivf_centroids.bin")

# 保存倒排列表（存储为偏移量 + 连续数组）
# 格式：先写每个列表的长度（4字节整数），再写所有索引（4字节整数）
with open('ivf_inverted.bin', 'wb') as f:
    for lst in inverted_lists:
        length = len(lst)
        f.write(struct.pack('I', length))
        for idx in lst:
            f.write(struct.pack('I', idx))
print("Saved ivf_inverted.bin")

# 同时保存一份参数文件
with open('ivf_params.txt', 'w') as f:
    f.write(f"n_centroids={n_centroids}\n")
    f.write(f"dim={dim}\n")
    f.write(f"base_num={base_num}\n")
print("Saved ivf_params.txt")