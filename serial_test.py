import serial

# Configure the serial port
PORT = "/dev/ttyACM0"
BAUD_RATE = 115200  # change this to match your device

try:
    ser = serial.Serial(PORT, BAUD_RATE, timeout=1)
    print(f"Connected to {PORT} at {BAUD_RATE} baud")

    while True:
        if ser.in_waiting > 0:
            data = ser.readline()  # read until newline
            try:
                print(data.decode('utf-8').strip())
            except UnicodeDecodeError:
                print(data)  # fallback if not valid UTF-8

except serial.SerialException as e:
    print(f"Error: {e}")
except KeyboardInterrupt:
    print("\nExiting...")

finally:
    if 'ser' in locals() and ser.is_open:
        ser.close()
