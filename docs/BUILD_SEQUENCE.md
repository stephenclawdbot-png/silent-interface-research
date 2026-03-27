# SILENT Interface - Build Sequence

## Project: SILENT-001
**Phase:** Cycle 1 - Prototype Assembly
**Estimated Duration:** 14 days

---

## Pre-Assembly Checklist

- [ ] All components received from BOM
- [ ] PCBs fabricated and inspected
- [ ] 3D printed enclosures ready
- [ ] Test equipment available (oscilloscope, multimeter)
- [ ] Workspace prepared with ESD protection
- [ ] Soldering station, hot air rework station ready
- [ ] Firmware toolchain installed (STM32CubeIDE)

---

## Week 1: Circuit Assembly

### Day 1-2: EMG Front-End (Unit A & B)

**Components:**
- INA333 instrumentation amplifiers (2 per unit)
- OPA2333 op-amps (2 per unit)
- Passive components (resistors, capacitors)
- EMG electrode connectors

**Steps:**
1. Clean PCB with isopropyl alcohol
2. Apply solder paste (if using hot air) or flux
3. Install INA333 chips (QFN/MSOP) - reflow or hand-solder
4. Install filter capacitors and resistors
5. Add header pins for electrode connection
6. Test continuity with DMM

**Verification:**
- Power on with 3.3V
- Test with function generator input
- Verify gain at scope: 1000x expected
- Check for stable reference voltage

**Checkpoint:** Signal passes through both units

---

### Day 3: NFMI Transceivers

**Components:**
- AS3932 or AS3933 receivers
- 10µH ferrite core inductors (TX/RX coils)
- RF capacitors (C0G/NP0, critical for tuning)
- Matching network components

**Steps:**
1. Install AS3933 with hot air (TSSOP package)
2. Solder 13.56MHz crystal
3. Wind/mount TX coil (10 turns, 30mm diameter)
4. Mount RX coil (10 turns, 25mm diameter)
5. Adjust tuning capacitors per datasheet

**Verification:**
- SPI communication with MCU (use logic analyzer or scope on CLK/MOSI)
- Carrier frequency: 13.560 MHz ± 5kHz
- Link budget test: units 1m apart, verify signal detected

**Checkpoint:** Units communicate at 1m

---

### Day 4: Audio Output Stage

**Components:**
- TPA2005D1 Class-D amplifier
- Bone conduction connector/wiring
- Output capacitors
- Volume control potentiometer (optional)

**Steps:**
1. Solder TPA2005D1 (MSOP package)
2. Install output coupling capacitors
3. Connect bone conduction transducer socket
4. Add low-pass filter (LC for EMI)

**Verification:**
- Feed 1kHz sine wave from MCU DAC
- Measure output at transducer terminals
- Check for expected current draw (~50mA)
- Listen for clean tone (no buzz/distortion)

**Checkpoint:** Audio output working on both units

---

### Day 5: Power Management

**Components:**
- TP4056 charge controller
- Buck-boost converter (TPS63001)
- Protection IC (DW01A or integrated)
- 18650 holder or JST connector
- USB-C port

**Steps:**
1. Install USB-C connector (may need hot plate for through-hole lugs)
2. Solder TP4056 and supporting passives
3. Install buck-boost converter
4. Add battery protection (if not using protected cell)
5. Connect battery holder

**Verification:**
- Charge cycle: plug in USB, LED indicates charging
- Fully charged: LED switches to ready
- Unplug, power from battery: system runs
- Check 3.3V rail stable under load

**Checkpoint:** Battery power working, charging functional

---

### Day 6: MCU Integration

**Components:**
- STM32L476 or dev board (Nucleo for breadboard option)
- Programming header (SWD: SWDIO, SWCLK, GND, 3.3V)
- Decoupling capacitors
- 32.768kHz RTC crystal (optional)

**Steps:**
1. Install STM32L476 (LQFP64) - use drag solder or hot air
2. Add bulk decoupling (10µF + 100nF)
3. Connect SWD header for programming
4. Route all signals:
   - EMG front-end → ADC1_IN (PA0, PA1)
   - SPI → AS3933 (PA5=SCK, PA6=MISO, PA7=MOSI, PA4=nCS)
   - I2S → Audio (PB10=CK, PB15=SD)
   - GPIO → Status LEDs, buttons

**Verification:**
- Connect ST-Link debugger
- Flash test firmware (blinky)
- Verify clock at 80MHz (HSE)

**Checkpoint:** MCU running custom firmware

---

## Week 2: Firmware & Integration

### Day 7-8: Signal Processing Firmware

**Development Tasks:**
1. ADC initialization (dual-channel, 1kHz)
2. Digital filtering:
   - High-pass: 0.5Hz (remove DC drift)
   - Notch: 60Hz (power line)
   - Low-pass: 500Hz (anti-alias)
3. Envelope detection (RMS calculation)
4. Threshold detection (adaptive)
5. Activity classification (state machine)

**Code Structure:**
```
main.c
├── emg.c/h          - EMG signal processing
├── audio.c/h        - Codec + DAC output
├── nfmi.c/h         - AS3933 driver
├── protocol.c/h     - TDMA frame handling
└── system.c/h       - GPIO, timers, power
```

**Verification:**
- EMG signal visible on debug UART
- Envelope follows muscle activity
- Trigger events on subvocalization

---

### Day 9: Communication Firmware

**Development Tasks:**
1. AS3933 driver (SPI interface)
2. FSK modulation control
3. TDMA slot timing
4. Packet formation + FEC
5. Sync word detection
6. Link establishment protocol

**Protocol Details:**
- Frame period: 40ms (25Hz)
- Slot A (TX→RX): 0-20ms
- Slot B (RX→TX): 20-40ms
- Packet: 16-bit sync + 160-bit payload + 16-bit CRC

**Verification:**
- Loopback test: units side by side
- Range test: 2-3m packet success
- Error injection: verify FEC works

---

### Day 10: Audio Codec Integration

**Development Tasks:**
1. ADPCM encoder/decoder (4-bit)
2. Audio DMA buffer management
3. I2S DAC output driver
4. Volume control
5. Bone conduction frequency shaping (optional EQ)

**Parameters:**
- Sample rate: 8kHz (sufficient for speech)
- Compression: 4:1 ADPCM
- Bitrate: 32kbps raw → 8kbps compressed
- Latency: <50ms processing

**Verification:**
- Feed test audio via lookup table
- Verify output on bone conduction driver
- Check for artifacts/distortion

---

### Day 11: System Integration

**Tasks:**
1. Connect all modules together
2. Power-up sequence testing
3. Fault handling (watchdog, brownout)
4. Sleep mode entry/exit
5. Complete message flow test

**Power Modes:**
- Active TX/RX: 80MHz, all peripherals on
- Listen mode: 80MHz, ADC polling
- Sleep: 32kHz, wake on EMG threshold

---

## Week 3: Testbed & Validation

### Day 12: Testbed Setup

**Physical Setup:**
1. Construct shielded test area
2. Mark range positions (0.5m-3m)
3. Assemble articulated head model for EMG tests
4. Install current measurement fixture
5. Calibrate oscilloscope

**Tools:**
- Faraday cage operational
- Range markers placed
- Reference tone generator ready
- Data logging sheets printed

---

### Day 13-14: Test Execution

**Test Sequence:**

| Time | Test | Target | Duration |
|------|------|--------|----------|
| 0:00 | EMG Signal Quality | >20dB SNR | 30min |
| 0:30 | NFMI Range Sweep | 3m range | 1hr |
| 1:30 | Audio Quality | MOS >3.0 | 1hr |
| 2:30 | Battery Life | 30+ min | 1hr |
| 3:30 | End-to-End | 2-person comm | 2hr |

**Documentation:**
- Photo/video of setup
- Measurement logs
- Test subject surveys (subjective audio)
- Bug/issue tracking

---

## Assembly Diagram

```
Unit A - Transmitter
┌─────────────────────────────────────────────────┐
│  ┌──────────┐    ┌──────────┐    ┌──────────┐ │
│  │  EMG     │───→│  Pre-    │───→│   MCU    │ │
│  │ Sensors  │    │ amp      │    │ STM32L4  │ │
│  └──────────┘    └──────────┘    └────┬─────┘ │
│                                         │       │
│                              ┌──────────┴──┐  │
│                              │    NFMI     │  │
│                              │  (AS3933)   │  │
│                              └──────┬──────┘  │
│                                     │         │
│                              ┌──────┴──────┐  │
│                              │   TX Coil   │  │
│                              └─────────────┘  │
└─────────────────────────────────────────────────┘
                              │
                         Magnetic
                           Field
                              │
Unit B - Receiver
┌─────────────────────────────────────────────────┐
│                              ┌─────────────┐   │
│                              │   RX Coil   │   │
│                              └──────┬──────┘   │
│                                     │          │
│                              ┌──────┴──────┐  │
│                              │    NFMI     │  │
│                              │  (AS3933)   │  │
│                              └──────┬──────┘  │
│                                     │         │
│                              ┌──────┴──────┐ │
│  ┌──────────┐    ┌───────────┐←──│   MCU     │ │
│  │  Bone    │←───│   Audio   │   │  STM32L4  │ │
│  │Conduct.  │    │    Amp    │   └───────────┘ │
│  └──────────┘    └───────────┘                 │
└─────────────────────────────────────────────────┘
```

---

## Troubleshooting Guide

| Symptom | Likely Cause | Fix |
|---------|--------------|-----|
| No EMG signal | Poor skin contact | Adjust electrode gel, check electrode snaps |
| 60Hz hum | Ground loop | Single-point ground, check shield |
| No NFMI link | Wrong frequency | Calibrate crystal, adjust capacitors |
| Short range | Low power | Increase TX power, check coil orientation |
| Muffled audio | Codec issue | Verify ADPCM tables, check sample rate |
| Unit resets | Power sag | Add bulk capacitance, check battery |
| False triggers | Threshold too low | Adjust VAD threshold in firmware |

---

## Sign-Off Checkpoints

| Checkpoint | Sign-Off | Date |
|------------|----------|------|
| EMG circuit working | ______ | ______ |
| NFMI link established | ______ | ______ |
| Audio output clear | ______ | ______ |
| Battery >30min runtime | ______ | ______ |
| 2m range demonstrated | ______ | ______ |
| 3m range demonstrated | ______ | ______ |
| End-to-end voice test | ______ | ______ |

---

## Post-Build Actions

- [ ] Update CYCLE_LOG with completion
- [ ] Tag git repo with "cycle-1-complete"
- [ ] Generate test report
- [ ] Prepare for Cycle 2 planning

---

*Document: BUILD_SEQUENCE.md*  
*Project: SILENT-001*  
*Part of META-SYSTEM autonomous research consortium*
