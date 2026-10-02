# ESP32-S3 Laudspeaker Management System

Digital audio management system untuk laudspeaker dengan fitur:

- 10-band equalizer
- 3-way crossover
- control gain master dan channel
- menu konfigurasi via serial terminal
- struktur siap diperluas ke display TFT/GUI

## Fitur utama

- 10 band EQ dengan level dB per band
- crossover 3-way: low, mid, high
- pengaturan gain master dan per channel
- profil bawaan yang siap dipakai
- menu serial untuk pengujian dan tuning

## Hardware target

- ESP32-S3
- DAC/I2S output (misal: MAX98357A, PCM5102, atau DAC eksternal)
- power amplifier 3 output channel
- optional TFT display 320x240 untuk UI visual

## Struktur proyek

- `src/main.cpp` : menu control dan runtime aplikasi
- `src/audio/AudioDSP.h` : model DSP untuk EQ, crossover, gain
- `src/audio/AudioDSP.cpp` : implementasi DSP

## Cara build

Menggunakan PlatformIO:

1. Install PlatformIO di VS Code
2. Buka folder repo ini
3. Jalankan build
4. Upload ke ESP32-S3
5. Buka Serial Monitor 115200 baud

## Menu serial

Setelah booting, menu akan tampil:

- `1` = Tampilkan EQ
- `2` = Set EQ band
- `3` = Tampilkan crossover
- `4` = Set crossover
- `5` = Tampilkan gain
- `6` = Set gain
- `7` = Prosess test tone
- `8` = Reset ke profil default
- `0` = Tampilkan menu

## Contoh penggunaan

- `2` -> pilih band 1..10 dan masukkan nilai dB
- `4` -> pilih frekuensi crossover 3-way
- `6` -> atur master gain dan gain low/mid/high

## Catatan

Project ini adalah starter firmware yang fokus pada logika kontrol DSP dan menu manajemen. Untuk produksi audio nyata, Anda dapat menambahkan:

- IIR filter yang lebih presisi
- display TFT/LVGL
- penyimpanan ke flash/NVS
- GUI touch
- logging tuning

## Lisensi

MIT
