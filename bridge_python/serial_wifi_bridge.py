import serial
import requests
import time

ARDUINO_PORT = "COM6"           # Change to your Arduino port (e.g., /dev/ttyUSB0 or /dev/ttyACM0 on Linux)
BAUD         = 115200
ESP32_IP     = "192.168.29.31"  # Change to the IP printed by your ESP32 in Serial Monitor

arduino = serial.Serial(ARDUINO_PORT, BAUD, timeout=1)
time.sleep(2)

print("Running. Ctrl+C to stop.")

while True:
    line = arduino.readline().decode("utf-8", errors="ignore").strip()
    if not line:
        continue

    print("Sending:", line)
    try:
        requests.get(f"http://{ESP32_IP}/cmd?val={line}", timeout=0.5)
    except requests.RequestException:
        print("ESP32 not reachable")