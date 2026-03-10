# SimpleFTL-Sim

Modern C++ FTL simulator featuring page-level mapping, Greedy GC, and WAF analysis. Validated with Pytest.

# File Architecture

```
SimpleFTL-Sim/
├── CMakeLists.txt             # 專案建置腳本
├── README.md                  # 專案說明
├── include/                   # 標頭檔 (.hpp)
│   ├── FTL_Controller.hpp     # 頂層邏輯控制
│   ├── AddressMapper.hpp      # LBA/PBA 映射表
│   ├── BlockManager.hpp       # 空間管理與 GC 邏輯
│   ├── NandDevice.hpp         # 物理層模擬
│   └── Common.hpp             # 常數定義 (Page Size, Blocks Count)
├── src/                       # 實作檔 (.cpp)
│   ├── main.cpp               # 程式入口，負責讀取 Trace
│   ├── FTL_Controller.cpp
│   ├── AddressMapper.cpp
│   ├── BlockManager.cpp
│   └── NandDevice.cpp
├── tests/                     # 測試資料夾
│   ├── conftest.py            # Pytest 設定
│   ├── test_basic_io.py       # 基礎讀寫測試
│   └── test_gc_logic.py       # 壓力測試，觸發 GC 並驗證資料
├── traces/                    # 存放測試用的 .txt 腳本
│   └── basic_workload.txt
└── docs/                      # 架構圖或開發手冊 (選配)
```

# Include Heirarchical Graph

```
[ HardwareConfig.hpp ]  <-- 最底層：定義物理常數 (128 pages, 1024 blocks)
          ↑
[ Common.hpp ]          <-- 邏輯層：定義 LBA/PBA 與系統容量 (90% OP)
          ↑
----------------------------------------------------------
          ↑                        ↑
[ AddressMapper.hpp ]    [ BlockManager.hpp ]
          ↑                        ↑
[           FTL_Controller.hpp              ]
          ↑
[               main.cpp                    ]
```

# Build & Execution
- use Cmake
```bash
# build folder
mkdir build && cd build

# Generate build script and compile
cmake ..
make

# execution
./FTL_Sim
```

- Use G++ compiler
```
g++ -std=c++17 -I./include src/*.cpp -o FTL_Sim

# 執行模擬器
./FTL_Sim
```