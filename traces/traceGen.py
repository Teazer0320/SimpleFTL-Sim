# 隨機生成 trace
# 若有指定檔名，可以使用 `python randomGenerateTrace.py custom.trace` 
# 來生成 `custom.trace`，否則預設為 `basic.trace`


import sys
import random

def generate_trace(filename):
  # 設定 SSD 規格（應與 C++ HardwareConfig 一致）
  MAX_LBA = 10000  # 假設 LBA 範圍
  MAX_SIZE = 32    # 單次最大寫入長度 (Sector/Page)
  
  try:
    with open(filename, "w") as f:
      f.write("# [OP] [LBA] [Size]\n")
      
      for i in range(100):
        #寫入 60%, 讀取 30%, Trim 10% 
        op = random.choices(["W", "R", "T"], weights=[60, 30, 10])[0]
        
        # 2. 50% 的機率存取前 100 個 LBA 
        if random.random() < 0.5:
          lba = random.randint(0, 100)
        else:
          lba = random.randint(0, MAX_LBA)
        
        # 3. Size 確保大於 0
        size = random.randint(1, MAX_SIZE)
        
        f.write(f"{op} {lba} {size}\n")
        
    print(f"Successfully generated: {filename}")
    
  except Exception as e:
    print(f"Error generating trace: {e}")

if __name__ == "__main__":
  if len(sys.argv) > 1:
    target_file = sys.argv[1] + ".trace"
  else:
    target_file = "basic.trace"
      
  generate_trace(target_file)