# Nestor Firmware

This project is a compact firmware for a wearable health tracker using Nordic nRF52 SoC.

## ✅ Features

- Heart Rate (HR) and SpO₂ via MAX86178
- Heart Rate Variability (HRV), Respiratory Rate (RR)
- Skin Temperature Deviation Monitoring
- 3D Accelerometer for Movement/Activity
- Caloric Burn Estimation (Keytel formula)
- Readiness Score, Stress Estimation, Fever Alerts
- BLE Peripheral Notifications for All Metrics
- OTA DFU Firmware Update Support
- Flash-based Historical Logging
- Python BLE Client for Desktop Testing

---

## 📂 Project Structure

```
nestor_firmware/
├── src/            # All C files
├── include/        # All header files
├── linker/         # Linker scripts (e.g. flash_placement_dfu.ld)
├── config/         # sdk_config.h, sdk_config_dfu.h
├── dfu/            # Private keys, README, OTA build artifacts
├── build/          # Output folder (hex, elf, zip)
├── mobile_client/  # Python BLE test app
├── Makefile        # Build entry point
└── README.md       # This file
```

---

## 🔧 Build Instructions

1. Install **nRF5 SDK v17.0.2**
2. Update `SDK_ROOT` path in the `Makefile`
3. Run in terminal:

```bash
make
```

Output: `build/nestor_firmware.hex`

---

## 📲 Flashing Firmware

Use Nordic tools:

```bash
nrfjprog --eraseall
nrfjprog --program build/nestor_firmware.hex --reset
```

---

## 🔁 OTA DFU (Optional)

1. Build secure bootloader using Nordic SDK
2. Use `nrfutil` to generate DFU package
3. Flash via `nrfutil dfu ble`

---

## 📡 BLE Client (Python)

```bash
pip install bleak
python mobile_client/ble_central_nestor_example.py
```

---

## 🧩 BLE Characteristics

| UUID (0xFFFx) | Purpose           |
|--------------|-------------------|
| 0xFFF0        | Readiness Score   |
| 0xFFF1        | Fever Alert       |
| 0xFFF2        | Stress Score      |
| 0xFFF3        | Calorie Total     |

---

Made with ❤️ for wearable health innovation.
