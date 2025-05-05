import pytest
import serial
import serial.tools.list_ports
import time
import sys
from datetime import datetime

# Ensure stdout uses UTF-8 encoding:
sys.stdout.reconfigure(encoding="utf-8")

def get_available_ports():
    """Retrieve a list of available COM ports."""
    return [port.device for port in serial.tools.list_ports.comports()]

@pytest.fixture(scope="module")
def uart_com3():
    """Open COM3 at 115200 baud rate if available."""
    available_ports = get_available_ports()
    if "COM3" not in available_ports:
        pytest.skip("COM3 is not available!")

    ser = serial.Serial(port="COM3", baudrate=115200, timeout=1)
    print(f"Connected to: {ser.name}")  # Display device name
    yield ser
    ser.close()

@pytest.fixture(scope="module")
def uart_com7():
    """Open COM7 at 115200 baud rate if available."""
    available_ports = get_available_ports()
    if "COM7" not in available_ports:
        pytest.skip("COM7 is not available!")

    ser = serial.Serial(port="COM7", baudrate=115200, timeout=1)
    print(f"Connected to: {ser.name}")  # Display device name
    yield ser
    ser.close()

@pytest.fixture(scope="module")
def uart_com5():
    """Open COM5 at 115200 baud rate if available."""
    available_ports = get_available_ports()
    if "COM5" not in available_ports:
        pytest.skip("COM5 is not available!")

    ser = serial.Serial(port="COM5", baudrate=115200, timeout=1)
    print(f"Connected to: {ser.name}")  # Display device name
    yield ser
    ser.close()

@pytest.fixture(scope="module")
def uart_com8():
    """Open COM8 at 115200 baud rate if available."""
    available_ports = get_available_ports()
    if "COM8" not in available_ports:
        pytest.skip("COM8 is not available!")

    ser = serial.Serial(port="COM8", baudrate=115200, timeout=1)
    print(f"Connected to: {ser.name}")  # Display device name
    yield ser
    ser.close()

def test_uart_read(uart_com3, uart_com7, uart_com5, uart_com8):
    """Read data from COM3 and COM7 for 60 seconds, group bursts, and print at the end."""

    start_time = time.time()
    timeout = 7200  # Total reading duration
    burst_timeout_com3 = 0.01
    burst_timeout_com5 = 0.01
    burst_timeout_com8 = 0.01
    burst_timeout_com7 = 0.005  # 5 ms — if no data within this time, consider burst ended

    buffer_com3 = ""
    buffer_com7 = ""
    buffer_com5 = ""
    buffer_com8 = ""

    last_rx_com5 = time.time()
    last_rx_com8 = time.time()
    last_rx_com3 = time.time()
    last_rx_com7 = time.time()

    log_com3 = []
    log_com7 = []
    log_com5 = []
    log_com8 = []

    while time.time() - start_time < timeout:
        now = time.time()

        # COM3 burst logic
        if uart_com3.in_waiting:
            data = uart_com3.read(uart_com3.in_waiting).decode("utf-8", errors="ignore")
            buffer_com3 += data
            last_rx_com3 = now
        elif buffer_com3 and (now - last_rx_com3) > burst_timeout_com3:
            # Burst complete
            timestamp = datetime.now().strftime("%H:%M:%S:%f")[:-3]
            log_com3.append(f"[{timestamp}] {buffer_com3.strip()}")
            buffer_com3 = ""

        # COM7 burst logic
        if uart_com7.in_waiting:
            data = uart_com7.read(uart_com7.in_waiting).decode("utf-8", errors="ignore")
            buffer_com7 += data
            last_rx_com7 = now
        elif buffer_com7 and (now - last_rx_com7) > burst_timeout_com7:
            # Burst complete
            timestamp = datetime.now().strftime("%H:%M:%S:%f")[:-3]
            log_com7.append(f"[{timestamp}] {buffer_com7.strip()}")
            buffer_com7 = ""

        # COM5 burst logic
        if uart_com5.in_waiting:
            data = uart_com5.read(uart_com5.in_waiting).decode("utf-8", errors="ignore")
            buffer_com5 += data
            last_rx_com5 = now
        elif buffer_com5 and (now - last_rx_com5) > burst_timeout_com5:
            # Burst complete
            timestamp = datetime.now().strftime("%H:%M:%S:%f")[:-3]
            log_com5.append(f"[{timestamp}] {buffer_com5.strip()}")
            buffer_com5 = ""

        # COM8 burst logic
        if  uart_com8.in_waiting:
            data = uart_com8.read(uart_com8.in_waiting).decode("utf-8", errors="ignore")
            buffer_com8 += data
            last_rx_com8 = now
        elif buffer_com8 and (now - last_rx_com8) > burst_timeout_com8:
            # Burst complete
            timestamp = datetime.now().strftime("%H:%M:%S:%f")[:-3]
            log_com8.append(f"[{timestamp}] {buffer_com8.strip()}")
            buffer_com8 = ""

    # Print results once
    if log_com3:
        print("COM3 (Gateway):")
        for entry in log_com3:
            print(entry)

    if log_com7:
        print("COM7 (Test_Node):")
        for entry in log_com7:
            print(entry)

    if log_com8:
        print("COM8 (Node_01):")
        for entry in log_com8:
            print(entry)
    
    if log_com5:
        print("COM5 (Node_02):")
        for entry in log_com5:
            print(entry)

    print("Data collection complete.")
    assert True  # Ensure pytest does not fail due to missing assert
