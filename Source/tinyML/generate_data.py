import csv
import random

filename = "sensor_data.csv"

NUM_NORMAL = 300
NUM_ABNORMAL = 300

data = []

# ==================== NORMAL (Label 0) ====================
for _ in range(NUM_NORMAL):
    temp = random.uniform(22, 32) + random.gauss(0, 0.8)      # Noise ±0.8°C
    humidity = random.uniform(45, 75) + random.gauss(0, 2.5)  # Noise ±2.5%
    data.append([round(temp, 1), round(humidity, 1), 0])

# ==================== ABNORMAL (Label 1) ====================
for _ in range(NUM_ABNORMAL):
    scenario = random.choice(["HOT_DRY", "COLD_WET", "EXTREME_HOT", "EXTREME_WET"])
    
    if scenario == "HOT_DRY":
        temp = random.uniform(36, 55)      # Too hot
        humidity = random.uniform(10, 35)  # Too dry
        
    elif scenario == "COLD_WET":
        temp = random.uniform(5, 18)       # Too cold
        humidity = random.uniform(80, 100) # Too wet
        
    elif scenario == "EXTREME_HOT":
        temp = random.uniform(56, 80)      # Extremely hot + dry
        humidity = random.uniform(5, 25)
        
    else:  # EXTREME_WET
        temp = random.uniform(0, 10)       # Extremely cold + wet
        humidity = random.uniform(85, 100)
    
    data.append([round(temp, 1), round(humidity, 1), 1])

# Shuffle data
random.shuffle(data)

# Write into CSV file
with open(filename, mode='w', newline='') as file:
    writer = csv.writer(file)
    writer.writerows(data)

print(f"Created {NUM_NORMAL + NUM_ABNORMAL} lines of new data into {filename}!")