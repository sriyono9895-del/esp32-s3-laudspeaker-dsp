# ESP32-S3 Laudspeaker Management System

**Sistem manajemen audio digital untuk laudspeaker dengan kontrol EQ 10-band, crossover 3-way, dan gain adjustable.**

## Fitur Utama

✅ 10-band Equalizer (31Hz - 16kHz)  
✅ 3-way Crossover (Low, Mid, High)  
✅ Gain Master + Per Channel  
✅ Menu TFT LCD 320x240  
✅ Kontrol Serial (115200 baud)  
✅ Test signal processing  
✅ Default profile management  

## Hardware Requirements

- **MCU**: ESP32-S3 DevKitC-1
- **Display**: ST7789 TFT 320x240
- **Connections**:
  - TFT MOSI (SPI) → GPIO 23
  - TFT SCLK (SPI) → GPIO 18
  - TFT CS → GPIO 5
  - TFT DC → GPIO 4
  - TFT RST → GPIO 2
  - TFT BL (Backlight) → GPIO 15
  - TFT GND → GND
  - TFT VCC → 3V3

*Note: Sesuaikan pin jika menggunakan board yang berbeda*

## Instalasi

### 1. Install PlatformIO
```bash
# Install di VS Code
# Atau gunakan command line: pip install platformio
```

### 2. Clone atau Download Project
```bash
git clone https://github.com/sriyono9895-del/esp32-s3-laudspeaker-dsp.git
cd esp32-s3-laudspeaker-dsp
```

### 3. Build & Upload
```bash
# Build
pio run -e esp32-s3-devkitc-1

# Upload
pio run -e esp32-s3-devkitc-1 -t upload

# Monitor Serial
pio device monitor -b 115200
```

## Menu Serial

| Perintah | Fungsi |
|----------|--------|
| `0` | Tampilkan menu |
| `1` | Lihat EQ settings |
| `2` | Set band EQ |
| `3` | Lihat crossover |
| `4` | Set crossover |
| `5` | Lihat gain |
| `6` | Set gain |
| `7` | Process test signal |
| `8` | Reset profile default |
| `9` | Show DSP summary |
| `u` | Menu UP |
| `d` | Menu DOWN |
| `e` | Menu SELECT |

## Struktur File

```
src/
├── main.cpp              # Main program & serial menu
├── audio/
│   ├── AudioDSP.h       # DSP header
│   └── AudioDSP.cpp     # DSP implementation
└── ui/
    ├── LcdMenu.h        # LCD menu header
    └── LcdMenu.cpp      # LCD menu implementation

User_Setup.h             # TFT_eSPI configuration
platformio.ini          # Build configuration
```

## EQ Bands (10-band)

| Band | Frequency |
|------|----------|
| 1 | 31 Hz |
| 2 | 63 Hz |
| 3 | 125 Hz |
| 4 | 250 Hz |
| 5 | 500 Hz |
| 6 | 1 kHz |
| 7 | 2 kHz |
| 8 | 4 kHz |
| 9 | 8 kHz |
| 10 | 16 kHz |

## Crossover Default Settings

- **Low cutoff**: 150 Hz
- **Mid-Low**: 900 Hz
- **Mid-High**: 3500 Hz
- **High cutoff**: 12000 Hz

## Contoh Penggunaan

### Via Serial Monitor

```
# Set Band 1 EQ ke +3 dB
2
1
3

# Set Gain Low ke +6 dB
6
0
6
0
0
```

### Via LCD Menu

1. Ketik `u` untuk navigasi UP
2. Ketik `d` untuk navigasi DOWN
3. Ketik `e` untuk SELECT

## DSP Processing

**Alur Sinyal:**

```
Input Sample
    ↓
[Master Gain]
    ↓
[10-Band EQ]
    ↓
[3-Way Crossover]
    ├→ Low Pass Filter  → Low Gain   → Output 1
    ├→ Band Pass Filter → Mid Gain   → Output 2
    └→ High Pass Filter → High Gain  → Output 3
```

## Default Profile

Default EQ setup yang sudah dikonfigurasi:
- Band 2: +1.5 dB
- Band 3: +2.5 dB
- Band 5: +3.0 dB
- Band 6: +1.0 dB
- Band 7: -1.5 dB
- Band 8: +1.5 dB
- Band 9: +2.0 dB

## Troubleshooting

### Display tidak muncul
1. Cek koneksi pin TFT ke ESP32-S3
2. Verifikasi User_Setup.h sesuai dengan board Anda
3. Pastikan TFT_eSPI library terinstall di PlatformIO

### Serial monitor error
1. Pastikan baud rate 115200
2. Gunakan USB cable yang benar (data + power)
3. Install CH340/CP210x driver jika diperlukan

### Build error
1. Run `pio lib update`
2. Clear build dengan `pio run -t clean`
3. Build ulang: `pio run -e esp32-s3-devkitc-1`

## License

MIT License

## Support

Untuk bantuan lebih lanjut, buka GitHub issues di repository ini.
