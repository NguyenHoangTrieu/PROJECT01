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
def uart_com11():
    """Open COM11 at 115200 baud rate if available."""
    available_ports = get_available_ports()
    if "COM11" not in available_ports:
        pytest.skip("COM11 is not available!")

    ser = serial.Serial(port="COM11", baudrate=115200, timeout=1)
    print(f"Connected to: {ser.name}")  # Display device name
    yield ser
    ser.close()

@pytest.fixture(scope="module")
def uart_com6():
    """Open COM6 at 115200 baud rate if available."""
    available_ports = get_available_ports()
    if "COM6" not in available_ports:
        pytest.skip("COM6 is not available!")

    ser = serial.Serial(port="COM6", baudrate=115200, timeout=1)
    print(f"Connected to: {ser.name}")  # Display device name
    yield ser
    ser.close()

@pytest.fixture(scope="module")
def uart_com13():
    """Open COM13 at 115200 baud rate if available."""
    available_ports = get_available_ports()
    if "COM13" not in available_ports:
        pytest.skip("COM13 is not available!")

    ser = serial.Serial(port="COM13", baudrate=115200, timeout=1)
    print(f"Connected to: {ser.name}")  # Display device name
    yield ser
    ser.close()

@pytest.fixture(scope="module")
def uart_com12():
    """Open COM12 at 115200 baud rate if available."""
    available_ports = get_available_ports()
    if "COM12" not in available_ports:
        pytest.skip("COM12 is not available!")

    ser = serial.Serial(port="COM12", baudrate=115200, timeout=1)
    print(f"Connected to: {ser.name}")  # Display device name
    yield ser
    ser.close()

def test_uart_read(uart_com11, uart_com6, uart_com13, uart_com12):
    """Read data from COM11 and COM6 for 60 seconds, group bursts, and print at the end."""

    start_time = time.time()
    timeout = 7200  # Total reading duration
    burst_timeout_com11 = 0.01
    burst_timeout_com13 = 0.01
    burst_timeout_com12 = 0.01
    burst_timeout_com6 = 0.005  # 5 ms — if no data within this time, consider burst ended

    buffer_com11 = ""
    buffer_com6 = ""
    buffer_com13 = ""
    buffer_com12 = ""

    last_rx_com13 = time.time()
    last_rx_com12 = time.time()
    last_rx_com11 = time.time()
    last_rx_com6 = time.time()

    log_com11 = []
    log_com6 = []
    log_com13 = []
    log_com12 = []

    while time.time() - start_time < timeout:
        now = time.time()

        # COM11 burst logic
        if uart_com11.in_waiting:
            data = uart_com11.read(uart_com11.in_waiting).decode("utf-8", errors="ignore")
            buffer_com11 += data
            last_rx_com11 = now
        elif buffer_com11 and (now - last_rx_com11) > burst_timeout_com11:
            # Burst complete
            timestamp = datetime.now().strftime("%H:%M:%S:%f")[:-3]
            log_com11.append(f"[{timestamp}] {buffer_com11.strip()}")
            buffer_com11 = ""

        # COM6 burst logic
        if uart_com6.in_waiting:
            data = uart_com6.read(uart_com6.in_waiting).decode("utf-8", errors="ignore")
            buffer_com6 += data
            last_rx_com6 = now
        elif buffer_com6 and (now - last_rx_com6) > burst_timeout_com6:
            # Burst complete
            timestamp = datetime.now().strftime("%H:%M:%S:%f")[:-3]
            log_com6.append(f"[{timestamp}] {buffer_com6.strip()}")
            buffer_com6 = ""

        # COM13 burst logic
        if uart_com13.in_waiting:
            data = uart_com13.read(uart_com13.in_waiting).decode("utf-8", errors="ignore")
            buffer_com13 += data
            last_rx_com13 = now
        elif buffer_com13 and (now - last_rx_com13) > burst_timeout_com13:
            # Burst complete
            timestamp = datetime.now().strftime("%H:%M:%S:%f")[:-3]
            log_com13.append(f"[{timestamp}] {buffer_com13.strip()}")
            buffer_com13 = ""

        # COM12 burst logic
        if  uart_com12.in_waiting:
            data = uart_com12.read(uart_com12.in_waiting).decode("utf-8", errors="ignore")
            buffer_com12 += data
            last_rx_com12 = now
        elif buffer_com12 and (now - last_rx_com12) > burst_timeout_com12:
            # Burst complete
            timestamp = datetime.now().strftime("%H:%M:%S:%f")[:-3]
            log_com12.append(f"[{timestamp}] {buffer_com12.strip()}")
            buffer_com12 = ""

    # Print results once
    if log_com11:
        print("COM11 (Gateway):")
        for entry in log_com11:
            print(entry)

    if log_com6:
        print("COM6 (Test_Node):")
        for entry in log_com6:
            print(entry)

    if log_com12:
        print("COM12 (Node_01):")
        for entry in log_com12:
            print(entry)
    
    if log_com13:
        print("COM13 (Node_02):")
        for entry in log_com13:
            print(entry)

    print("Data collection complete.")
    assert True  # Ensure pytest does not fail due to missing assert
