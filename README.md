# SimpleFTL-Sim

[![C++ Standard](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)]()

一個現代化的 C++ FTL 模擬器，旨在模擬 SSD 核心邏輯。支援頁面級映射 (Page-level Mapping)、貪婪垃圾回收演算法 (Greedy GC) 以及寫入放大 (WAF) 分析。

## Project Highlights
- **位址轉換**: 實作精確的 L2P (Logical to Physical) 映射表。
- **空間管理**: 動態維護空閒區塊 (Free Blocks) 與有效資料統計。
- **垃圾回收**: 實作 Greedy GC，優先回收無效頁面最多的區塊。
- **效能分析**: 自動統計寫入放大比率 (Write Amplification Factor)。
- **自動化測試**: 整合 Pytest 與 Python Trace 生成器，確保邏輯正確性。

---

## File Architecture
```
SimpleFTL-Sim/
├── CMakeLists.txt             # 專案建置腳本
├── README.md                  # 專案說明
├── include/                   # 標頭檔 (.hpp)
│   ├── Logger.hpp             # 終端機彩色 Log 輸出工具
│   ├── FTL_Controller.hpp     # FTL 核心邏輯
│   ├── AddressMapper.hpp      # LBA/PBA 映射管理
│   ├── BlockManager.hpp       # 區塊配置與 GC 觸發邏輯
│   ├── NandDevice.hpp         # NAND Flash 物理層模擬
│   ├── Trace_Player.hpp       # 解析與執行 .trace 腳本
│   └── Common.hpp             # 系統常數與硬體配置 (OP, Page Size)
├── src/                       # 實作檔 (.cpp)
│   ├── main.cpp               # 程式入口與參數處理
│   ├── Trace_Player.cpp       # Trace 讀取器實作
│   └── ...                    # 其他組件實作
├── tests/                     # 自動化測試
│   ├── test_basic_io.py       # 基礎 Read/Write/Trim 測試
│   └── test_gc_logic.py       # GC 觸發與資料正確性壓力測試
├── traces/                    # 測試負載
│   ├── traceGen.py            # 隨機生成 trace 測試檔
│   ├── test.trace             # 範例測試
│   └── basic.trace            # 範例測試
└── scripts/
```

---

## 🏗 建置與執行 (Build & Execution)

### 1. 使用 CMake 建置 (推薦)
```bash
mkdir build && cd build
cmake ..
make
# 執行指定 trace
./FTL_Sim basic.trace
```

### 2. 使用 G++ 直接編譯
```
g++ -std=c++17 -I./include src/*.cpp -o FTL_Sim
./FTL_Sim basic.trace
```
---
## Trace 格式說明
Trace 檔案採純文字格式，每行代表一個 Host 請求：
`[操作] [起始 LBA] [長度/Size]`

- W: Write (寫入資料)
- R: Read (讀取資料)
- T: Trim (標記資料失效)

### 範例
```
W 10 1   # 寫入 LBA 10
W 10 1   # 覆蓋寫入 LBA 10 (觸發 Invalidation)
T 10 1   # 標記 LBA 10 為空
```