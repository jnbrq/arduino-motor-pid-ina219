# arduino-motor-pid-controller-ina219
A simple GUI and Arduino firmware. GUI can be used for tuning PID.

## Using the wxPython3 and Python 3 Based GUI to communicate with Arduino

### Prerequisites

To execute the GUI, you must have following packages installed:

  1. wxPython
  2. pySerial
  3. matplotlib
  4. numpy
  
You can install each by typing:

```
python3 -m pip install --user <package_name>
```

This command will install it without any administrator privileges.

### Usage

You can start the GUI with following command (of course, after changing the
working directory to `pid_gui`):

```
env SERIAL_PATH=<serial_port_path> SERIAL_BAUD=<serial_baud_rate> python3 motorCurrentPID.py
```

If you need more options to configure serial, you can alter the code under
`main()` function (for example, Parity, etc.).

## Flashing the Arduino Firmware

It's done as usual, open the `*.ino` file with Arduino IDE, configure it for
your board, then compile and flash!

You can configure the Arduino firmware to satisfy your needs.

