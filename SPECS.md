# SILENT Interface - Technical Specifications

## Project: SILENT-001
**Version:** 1.0  
**Date:** 2026-03-28  
**Phase:** Cycle 1 - Prototype

---

## 1. System Overview

### 1.1 Architecture
Two-unit wearable system employing:
- **Input:** Electromyography (EMG) subvocalization detection
- **Processing:** ARM Cortex-M4 microcontroller
- **Transmission:** Near-field magnetic induction (NFMI) at 13.56 MHz
- **Output:** Bone conduction audio transducers

### 1.2 Operating Principle
Signal chain: 
```
Laryngeal EM Activity → EMG Amplifier → MCU ADC → 
Digital Processing → NFMI Modulation → TX Coil → 
Magnetic Field → RX Coil → NFMI Demodulation → 
MCU DAC → Audio Amplifier → Bone Conduction Driver
```

---

## 2. Electrical Specifications

### 2.1 Power Supply

| Parameter | Specification | Notes |
|-----------|--------------|-------|
| Input Voltage | 3.7V nominal | LiPo battery |
| Voltage Range | 3.0V - 4.2V | Operating range |
| Current (TX Mode) | ~45mA | Peak during transmission |
| Current (RX Mode) | ~35mA | Receive + decoding |
| Current (Sleep) | <500μA | Standby with EMG monitoring |
| Battery Capacity | 1000mAh minimum | 18650 or pouch cell |
| Battery Life | >30 minutes | Continuous use |
| Charging | USB-C, 500mA | TP4056 or equivalent |

### 2.2 EMG Front-End (Input Stage)

| Parameter | Specification | Notes |
|-----------|--------------|-------|
| Sensor Type | Surface EMG electrodes | Ag/AgCl, 20mm diameter |
| Electrode Count | 2 per unit (differential) | Neck placement |
| Input Impedance | >10 MΩ | For good skin contact |
| CMRR | >80 dB @ 60Hz | Power line rejection |
| Frequency Response | 0.5 Hz - 500 Hz | EMG spectrum |
| Gain | 1000x - 10000x adjustable | PGA controlled |
| Noise | <2 μV p-p | Input referred |
| ADC Resolution | 12-bit, 1 Msps | STM32L4 built-in |
| Sampling Rate | 1000 Hz | Nyquist for EMG |

### 2.3 NFMI Transceiver

| Parameter | Specification | Notes |
|-----------|--------------|-------|
| Carrier Frequency | 13.56 MHz | ISM band (worldwide license-free) |
| Modulation | FSK, 2-FSK | ±10kHz deviation |
| Data Rate | 2 kbps | Sufficient for voice codec |
| TX Power | -20 dBm to +10 dBm | Adjustable, regulatory compliant |
| RX Sensitivity | -80 dBm | For extended range |
| Channel Bandwidth | 50 kHz | Narrowband operation |
| Protocol | Custom TDMA | Half-duplex ping-pong |

### 2.4 Audio Output Stage

| Parameter | Specification | Notes |
|-----------|--------------|-------|
| Driver Type | Bone conduction transducer | Radioear B81 or BH01B |
| Frequency Response | 100 Hz - 8 kHz | Bone conduction range |
| Impedance | 8Ω - 32Ω | Depending on driver |
| Amplifier Power | 250mW @ 8Ω | TPA2005D1 Class-D |
| THD | <1% @ 1kHz | Total harmonic distortion |
| SNR | >85 dB | Audio quality |
| Volume Control | Digital, 32-step | Software controlled |

---

## 3. Mechanical Specifications

### 3.1 Form Factor

| Parameter | Specification |
|-----------|--------------|
| Enclosure Type | Clamp-style neckband or wrist strap |
| Dimensions (neck) | 180mm x 25mm x 15mm (curved) |
| Dimensions (wrist) | 50mm x 40mm x 15mm |
| Weight (neck unit) | <150g |
| Weight (wrist unit) | <80g |
| Material | ABS/PC blend or PETG (3D printed) |
| Finish | Matte black, skin-friendly coating |

### 3.2 Environmental

| Parameter | Specification |
|-----------|--------------|
| Operating Temperature | 5°C to 40°C |
| Storage Temperature | -10°C to 60°C |
| Humidity | 5% to 95% RH (non-condensing) |
| Ingress Protection | IP54 (sweat/splash resistant) |

### 3.3 User Interface

| Component | Specification |
|-----------|--------------|
| Power Button | Tactile, 3mm travel |
| Status LED | RGB, configurable patterns |
| Volume Control | Capacitive slider or 2x buttons |
| Charging Port | USB-C, waterproof gasket |

---

## 4. Signal Processing Specifications

### 4.1 EMG Signal Chain

```
Raw EMG (0.5-500Hz)
    ↓
High-pass Filter (0.5Hz, remove baseline drift)
    ↓
Notch Filter (60Hz, power line interference)
    ↓
Low-pass Filter (500Hz, anti-aliasing)
    ↓
Amplification (60dB gain)
    ↓
Envelope Detection (RMS, 50ms window)
    ↓
Threshold Detection (adaptive)
    ↓
Activity Classification (onset/offset)
    ↓
Voice Activity Detection (VAD)
    ↓
Codec Encoding (ADPCM, 8kbps)
    ↓
Packet Formation + FEC
```

### 4.2 Transmission Protocol

| Layer | Specification |
|-------|--------------|
| Physical | 13.56 MHz FSK |
| Data Link | Custom TDMA frame |
| Frame Size | 20ms per slot |
| Slots per frame | 2 (A→B, B→A) |
| Payload per frame | 160 bits (20 bytes) |
| FEC | Hamming (8,4) code |
| Interleaving | 4-frame depth |
| Sync | 16-bit Barker code |

---

## 5. Performance Specifications

### 5.1 Communication Performance

| Parameter | Target | Test Method |
|-----------|--------|-------------|
| Range | 2-3 meters | Line-of-sight, indoor |
| Latency | <100ms | End-to-end measurement |
| Packet Loss | <5% | BER test over 1000 packets |
| Link Reliability | >95% | Connection uptime metric |
| Throughput | 2 kbps | Effective user data |
| Interference Rejection | >40dB | Adjacent channel |

### 5.2 Audio Performance

| Parameter | Target | Test Method |
|-----------|--------|-------------|
| Intelligibility | >80% | Diagnostic Rhyme Test (DRT) |
| MOS Score | >3.0 | Mean Opinion Score |
| Frequency Response | 100Hz-4kHz | ±3dB tolerance |
| Output Level | 70-85 dB | Bone conduction (re: 1μN) |
| Crosstalk | <-40dB | Between units |

### 5.3 EMG Performance

| Parameter | Target | Test Method |
|-----------|--------|-------------|
| Detection Rate | >90% | Subvocal word recognition |
| False Positive | <5% | Silence period measurement |
| Onset Detection | <50ms | Time to trigger after EMG |
| Signal Quality | >20dB SNR | Measured at ADC input |

---

## 6. Testbed Requirements

### 6.1 Test Environment

| Requirement | Specification |
|-------------|--------------|
| EM Shielding | Faraday cage or shielded room |
| Ambient Noise | <1μV/m @ 13.56MHz |
| Reference Source | Calibrated signal generator |
| Measurement Tools | Oscilloscope, spectrum analyzer |
| Recording | Audio capture system |

### 6.2 Test Fixtures

| Fixture | Purpose |
|---------|---------|
| Articulated Head Model | Consistent electrode placement |
| Range Marker Grid | 0.5m, 1m, 2m, 3m positions |
| RF Enclosure | Isolated test chamber |
| Battery Simulator | Constant voltage supply |

### 6.3 Test Protocols

1. **Range Test:** Fixed phrase transmission at marked distances
2. **Audio Quality:** Standard word lists (PB-50), subjective MOS
3. **Battery Duration:** Continuous operation until low-voltage cutoff
4. **Interference:** WiFi, Bluetooth, NFC coexistence tests
5. **Environmental:** Temperature chamber cycles (if available)

---

## 7. Regulatory Compliance

### 7.1 RF Emissions

| Standard | Requirement |
|----------|-------------|
| FCC Part 15.225 | 13.553-13.567 MHz ISM band |
| ETSI EN 300 330 | Short range devices, Europe |
| IC RSS-210 | Canada license-free operation |

### 7.2 Safety

| Standard | Requirement |
|----------|-------------|
| IEC 62368-1 | Audio/Video safety |
| IEC 60601-1 | Medical device (if applicable) |
| UN 38.3 | Lithium battery transport |

### 7.3 Prototype Exemptions
- Lab/testbed use only (not field deployment)
- Limited power (<10mW ERP planned)
- No medical diagnostic claims

---

## 8. Revision History

| Version | Date | Changes |
|---------|------|---------|
| 1.0 | 2026-03-28 | Initial Cycle 1 specifications |

---

*Document: SPECS.md*  
*Project: SILENT-001*  
*Part of META-SYSTEM autonomous research consortium*
