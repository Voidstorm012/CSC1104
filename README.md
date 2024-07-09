
# Raspberry Pi LED Blinker

## Description
This project allows you to control the blinking of red and green LEDs on a Raspberry Pi. You can set the frequency and brightness of the LED blinks.

## Installation
To set up the project on your Raspberry Pi:

1. Clone the repository:
    ```bash
    git clone https://github.com/Voidstorm012/Raspberry-Pi-LED-Blinker.git
    ```
2. Navigate to the project directory:
    ```bash
    cd Raspberry-Pi-LED-Blinker
    ```
3. Install the wiringPi library (if not already installed):
    ```bash
    sudo apt-get install wiringpi
    ```
4. Compile the C program:
    ```bash
    gcc NewStudent.c -o NewStudent -lwiringPi
    ```

## Usage
1. Run the program with the following syntax:
    ```bash
    ./NewStudent <red/green> <frequency> <brightness>
    ```
    - `<red/green>`: Choose the color of the LED to blink.
    - `<frequency>`: Set the blink frequency (in Hz).
    - `<brightness>`: Set the brightness (0-100%).

### Example
To blink the red LED at 2 Hz with 50% brightness:
```bash
./NewStudent red 2 50
```

## Features
- Control the blinking of red and green LEDs on a Raspberry Pi.
- Set the blink frequency and brightness via command-line arguments.

## Contributing
To contribute to the project:
1. Fork the repository
2. Create a new branch (`git checkout -b feature/your-feature`)
3. Commit your changes (`git commit -m 'Add some feature'`)
4. Push to the branch (`git push origin feature/your-feature`)
5. Open a Pull Request

## License
Distributed under the MIT License. See `LICENSE` for more information.

## Authors
- **Voidstorm012** - *Initial work* - [Voidstorm012](https://github.com/Voidstorm012)

## Additional Information
For more details on how to use the waveform data or any additional features, refer to the `docs` directory.
