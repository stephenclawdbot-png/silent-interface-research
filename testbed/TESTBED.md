# SILENT Interface - Testbed Requirements

## Project: SILENT-001
**Phase:** Cycle 1 - Prototype Validation
**Date:** 2026-03-28

---

## 1. Testbed Overview

The SILENT prototype testbed provides a controlled environment for validating:
- EMG signal acquisition quality
- NFMI transmission range and reliability
- Bone conduction audio output
- System power consumption
- Environmental robustness

---

## 2. Physical Test Environment

### 2.1 Test Chamber Requirements

| Requirement | Specification | Rationale |
|-------------|---------------|-----------|
| Shielding | >40dB attenuation @ 13.56MHz | Prevent external RF interference |
| Material | Copper mesh or aluminum foil lined | Cost-effective Faraday cage |
| Dimensions | 2m x 2m x 2m | Accommodate range tests |
| Access | Removable panel with RF gasket | Equipment entry/exit |
| Grounding | Single point to earth ground | Safety + noise reduction |
| Ventilation | Passive with EMI honeycomb | Heat dissipation |

**DIY Option:** Cardboard box lined with aluminum tape, grounded via foil strip to earth ground.

### 2.2 Range Markers

- **Distance markers:** 0.5m, 1.0m, 1.5m, 2.0m, 2.5m, 3.0m intervals
- **Material:** Non-conductive (wood, plastic)
- **Labeling:** Visible from test positions
- **Alignment:** Measured from center of transceiver coil

### 2.3 Test Positions

| Position | Purpose | Notes |
|----------|---------|-------|
| Static Standing | Baseline measurements | Feet on marked spots |
| Static Seated | Comfort/longevity tests | Office chair, no metal |
| Walking | Mobile operation | Figure-8 path within chamber |
| Orientation Test | Coil alignment effects | Rotate user 360° |

---

## 3. Measurement Equipment

### 3.1 Required Instruments

| Instrument | Minimum Specs | Purpose | Source |
|------------|---------------|---------|--------|
| Digital Oscilloscope | 100MHz BW, 2ch | EMG + NFMI signal viewing | ADALM2000 (recommended) |
| Spectrum Analyzer | 1kHz-100MHz span | NFMI carrier analysis | ADALM2000 or RTL-SDR |
| True RMS Multimeter | 4.5 digit | Power measurements | Available |
| Function Generator | 1Hz-1MHz, sine | Reference signal injection | ADALM2000 or AD9833 |
| Power Supply | 0-5V, 2A adjustable | Controlled voltage testing | Lab supply |
| DMM (2nd) | Current measurement | Battery drain monitoring | Available |

### 3.2 RF Equipment (Optional)

| Equipment | Purpose | Notes |
|-----------|---------|-------|
| Signal Generator (RF) | Calibrated 13.56MHz | Test receiver sensitivity |
| Attenuators | 10dB, 20dB | Range simulation |
| Near-field Probe | H-field measurement | Coil characterization |
| RF Power Meter | TX power verification | Regulatory compliance check |

### 3.3 Audio Equipment

| Equipment | Purpose | Notes |
|-----------|---------|-------|
| Reference Mic | Calibrated SPL | Sound level verification |
| Sound Level Meter | 30-130dB range | Bone conduction output |
| Recording Device | WAV/48kHz | Test subject playback |
| Headphones (isolated) | Test monitor | Feedback isolation |

---

## 4. Test Fixtures

### 4.1 Articulated Head Model

**Purpose:** Consistent electrode placement for repeatable EMG tests

**Specifications:**
- Adjustable neck angle: 0° to 45° flexion
- Standardized head circumference: 58cm (adult)
- Mounting for EMG electrodes: 20mm snap mounts
- Material: 3D printed TPU (rubber-like) or foam dummy

**Components to Build:**
1. Base stand with clamp
2. Articulated neck joint
3. Head form (3D printed or modified mannequin)
4. Electrode mounting pads

### 4.2 Transceiver Coil Mounts

**Purpose:** Position TX/RX coils at precise distances

**Specifications:**
- Adjustable height: 1.0m - 1.8m (standing positions)
- Horizontal adjustment: ±30cm
- Angle adjustment: 0°-30° tilt
- Non-metallic construction: Wood or plastic

**Components:**
1. Camera tripod (modified, non-metallic head)
2. Adjustable arm (gooseneck)
3. Coil mounting plates
4. Distance scale attached

### 4.3 Power Measurement Fixture

**Purpose:** In-circuit current monitoring

**Circuit:**
```
LiPo (+) → 1Ω Sense Resistor → PSU
                ↓
            DMM (mV = mA)
                ↓
            Load (DUT)
```

**Components:**
1. 1Ω 1% precision resistor (1W)
2. Test leads with Kelvin connection
3. Switch for current path (bypass/series)
4. Banana jack connectors

---

## 5. Test Procedures

### 5.1 EMG Signal Quality Test

**Objective:** Verify EMG front-end detects subvocal activity

**Setup:**
1. Mount electrodes on head model (or live subject)
2. Connect to Unit A (TX)
3. Oscilloscope on preamp output

**Procedure:**
1. Baseline measurement (no activity): <50μV p-p
2. Jaw clench test: >500μV p-p
3. Subvocal "test" word: >200μV envelope
4. Noise floor check: <2μV RMS after filtering

**Pass Criteria:**
- Signal amplitude sufficient for ADC input
- 20:1 SNR minimum at preamp output
- No 60Hz interference (notch filter working)

### 5.2 NFMI Range Test

**Objective:** Measure effective communication range

**Setup:**
1. Place TX at position 0 (reference)
2. Connect RX at 0.5m marker
3. Signal generator feeding TX standard tone

**Procedure:**
1. Transmit continuous carrier @ 13.56MHz
2. Measure received signal strength at each marker
3. Record packet loss at 2kbps data rate
4. Map signal vs distance curve

**Pass Criteria:**
- 100% packet reception at 2m
- >95% packet reception at 3m
- Fade margin >10dB at max range

### 5.3 Audio Quality Test

**Objective:** Verify bone conduction output intelligibility

**Setup:**
1. RX unit with bone conduction driver on test fixture
2. Reference microphone at 5mm from driver surface
3. Test subject (for subjective evaluation)

**Procedure:**
1. Send standard word list (modified rhyme test)
2. Record received audio level
3. Calculate frequency response
4. Conduct listening test with 3 subjects minimum

**Pass Criteria:**
- >80% word recognition
- MOS score >3.0
- Comfortable listening level (not too loud)

### 5.4 Battery Life Test

**Objective:** Verify 30-minute minimum operation

**Setup:**
1. Fully charged 18650 cell (4.2V)
2. Units in transmit/receive mode
3. Current measurement fixture in series

**Procedure:**
1. Continuous transmission of test signal
2. Log current draw each minute
3. Record time to low-battery threshold (3.3V)
4. Calculate total capacity consumed

**Pass Criteria:**
- >30 minutes minimum operation
- Average current <2A (within battery spec)
- Voltage stays above 3.3V for duration

### 5.5 End-to-End System Test

**Objective:** Validate complete communication chain

**Setup:**
1. Two live test subjects wearing units
2. Test within shielded chamber
3. Subjects at 2.5m distance

**Procedure:**
1. Subject A subvocalizes test phrases
2. Subject B reports what was heard
3. Record audio from both sides
4. Measure latency (clap test reference)

**Pass Criteria:**
- >80% phrase comprehension
- Latency <100ms measured
- No dropouts during 5-minute test

---

## 6. Data Collection

### 6.1 Measurement Log Template

```
Test ID: SIL-001-YYYYMMDD-XXX
Date: YYYY-MM-DD
Operator: [Name]
Conditions: [Temperature, humidity]

Parameters:
- TX Power: [dBm]
- RX Gain: [setting]
- Distance: [m]
- Audio Volume: [level]

Measurements:
| Time | Metric | Value | Unit |

Results:

Pass/Fail:
```

### 6.2 File Naming Convention

```
YYYY-MM-DD_TEST-TYPE_TEST-ID_CONDITION.ext

Examples:
2026-04-05_range-test_SIL-001-01_2m.wav
2026-04-05_emg-baseline_SIL-001-02_jaw-clench.csv
```

---

## 7. Safety Considerations

### 7.1 RF Exposure

- Maximum EIRP: 10mW (-20dBm to +10dBm)
- Duty cycle: Continuous in test (worst case)
- Safe distance: 10cm minimum to TX coil
- Compliance: FCC Part 2.1091 limits for general population

### 7.2 Electrical Safety

- Battery chemistry: Protected 18650 cells required
- Overcurrent fuse: 3A fast-blow in battery pack
- Charger safety: TP4056 module with thermal cutoff
- Ground fault: GFCI on test equipment

### 7.3 Biological Safety

- EMG electrodes: Disinfect between uses
- Bone conduction: Limit continuous exposure to 30min
- Current through body: <10μA (well below perception)

---

## 8. Test Schedule

| Day | Activity | Duration |
|-----|----------|----------|
| 1 | Setup test chamber, calibrate equipment | 4 hours |
| 2 | EMG validation, tuning | 3 hours |
| 3 | NFMI range characterization | 4 hours |
| 4 | Audio quality testing | 3 hours |
| 5 | Battery life profiling | 2 hours |
| 6 | End-to-end validation | 3 hours |
| 7 | Documentation, report writing | 4 hours |

**Total Test Time:** 23 hours

---

## 9. Reference Standards

- **IEEE C95.1** - RF exposure limits
- **ISO 2631** - Mechanical vibration (bone conduction)
- **IEC 60601-1** - Medical electrical equipment safety
- **ANSI S3.5** - Speech intelligibility testing

---

*Document: TESTBED.md*  
*Location: testbed/*  
*Project: SILENT-001*
