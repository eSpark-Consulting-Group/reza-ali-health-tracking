
# OTA DFU Setup Guide (nRF52832)

This guide enables BLE-based firmware updates via nRF Connect.

---

## 🔧 Flash Partition Layout (example: 512KB Flash)

- MBR          : 0x00000000 - 0x00000FFF
- SoftDevice   : 0x00001000 - 0x00025FFF (S132 v6.1.1)
- Application  : 0x00026000 - 0x0007BFFF
- Bootloader   : 0x0007C000 - 0x0007FFFF
- Bootloader Settings + MBR Params = 0x0007F000

---

## 📦 Build Bootloader

1. Clone Nordic SDK (v16 recommended)
2. Use secure_bootloader example:
   `examples/dfu/secure_bootloader/pca10040_s132`

3. Set linker script for above regions

---

## ⚙️ sdk_config.h Flags

- `NRF_DFU_BLE_REQUIRES_BONDS = 0`
- `NRF_DFU_BLE_BUTTONLESS_SUPPORT = 1`
- `NRF_DFU_TRANSPORT_BLE = 1`
- `NRF_DFU_SETTINGS_VERSION = 2`
- `NRF_CRYPTO_ENABLED = 1`

---

## 🚀 Generate and Flash Firmware

```bash
nrfutil pkg generate --hw-version 52 --sd-req 0x91 --application app.hex --application-version 1 --key-file private.key dfu_package.zip

nrfutil dfu ble -ic NRF52 -pkg dfu_package.zip -n "NESTOR_DFU"
```

--- 
✅ Ready for OTA via nRF Connect app or production updater.
