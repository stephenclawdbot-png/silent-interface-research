# SILENT-001: 14-Day Build Sequence

**Project:** SILENT Interface (SILENT-001)  
**Phase:** Cycle 1 - Prototype  
**Version:** 1.0  
**Created:** 2026-03-28  
**Target Completion:** 2026-04-11

---

## Overview

This document details the 14-day hardware build sequence for the SILENT prototype. Two identical units (Unit A and Unit B) will be constructed to demonstrate two-way silent communication.

**Critical Path:** Component Procurement → Circuit Assembly → Firmware → Testbed → Validation

---

## Day 1-3: Component Procurement (Days 1-3, Complete by Mar 31)

### Day 1: Priority 1 Orders (Mar 28)
**Goal:** Place orders for longest lead-time items

| Component | Qty | Supplier | Est. Cost | Ordered |
|-----------|-----|----------|-----------|---------|
| AS3932 Transceivers | 2 | Digi-Key | $16.90 | ☐ |
| AS3933 Alternative | 2 | Digi-Key | $13.78 | ☐ |
| Bone Conduction Modules (BH01B) | 2 | AliExpress | $36.00 | ☐ |
| 10µH RF Inductors (Q>50) | 4 | Coilcraft | $6.00 | ☐ |

**Verification Tasks:**
- [ ] Confirm AS3932 pin compatibility with AS3933
- [ ] Verify bone conducer impedance (8-32Ω expected)
- [ ] Check inductor SRF >20MHz

### Day 2: Priority 2 - Dev Boards & Semiconductors (Mar 29)
**Goal:** Order breadboard prototyping components

| Component | Qty | Supplier | Est. Cost | Ordered |
|-----------|-----|----------|-----------|---------|
| STM32 Nucleo-L476RG | 2 | Digi-Key | $28.92 | ☐ |
| INA333 Instrumentation Amp | 4 | Digi-Key | $11.40 | ☐ |
| OPA2333 Op-Amp | 4 | Digi-Key | $8.40 | ☐ |
| TPA2005D1 Audio Amp | 2 | Digi-Key | $2.50 | ☐ |
| TP4056 Charge Controller | 2 | Digi-Key | $2.20 | ☐ |
| TPS63001 Buck-Boost | 2 | Digi-Key | $6.90 | ☐ |

**Verification Tasks:**
- [ ] Confirm Nucleo board Arduino headers
- [ ] Verify INA333 supply voltage range (1.8-5.5V)
- [ ] Check TPA2005D1 output power @ 8Ω

### Day 3: Priority 3 - Passives, Connectors, Mechanical (Mar 30)
**Goal:** Complete BOM with supporting components

| Component | Qty | Supplier | Est. Cost | Ordered |
|-----------|-----|----------|-----------|---------|
| 0603/0805 Resistor Kit | 1 | Amazon | $8.00 | ☐ |
| 0603/0805 Capacitor Kit | 1 | Amazon | $10.00 | ☐ |
| Ag/AgCl Electrodes (20-pack) | 1 | Amazon | $10.00 | ☐ |
| USB-C Connectors | 4 | Digi-Key | $3.40 | ☐ |
| JST-PH 2.0 Connectors | 10 | Digi-Key | $2.50 | ☐ |
| 18650 Battery Holders | 2 | Amazon | $3.00 | ☐ |
| Samsung 30Q 18650 Cells | 2 | Li-Ion Wholesale | $13.00 | ☐ |
| PETG Filament (1kg) | 1 | Amazon | $25.00 | ☐ |

**Verification Tasks:**
- [ ] Confirm electrode gel compatibility
- [ ] Verify 18650 cell protection circuit
- [ ] Check PETG color availability (black/grey)

**Day 3 Deliverable:** Complete order list, track shipment confirmations

---

## Day 4-6: Circuit Assembly (Days 4-6, Apr 1-3)

### Day 4: EMG Sensor Front-End Assembly (Apr 1)
**Goal:** Build functional EMG amplification circuit

**Morning: Breadboard Setup**
- Set up 2x breadboards (one per unit)
- Power rails: 3.3V, GND
- STM32 Nucleo boards configured

**Afternoon: Sensor Circuit Build**
```
Electrode → INA333 → OPA2333 (filter) → STM32 ADC
Steps:
1. Wire INA333 with gain set to 1000x (Rg = 50Ω)
2. Add 0.5Hz high-pass filter (C=330nF, R=1MΩ)
3. Add 500Hz low-pass filter (C=330pF, R=1kΩ)
4. Add 60Hz notch filter (twin-T configuration)
5. Connect to STM32 ADC1_IN0
```

**Testing Checklist:**
- [ ] DC offset < 10mV at amplifier output
- [ ] Noise floor < 5mV p-p (scope measurement)
- [ ] 60Hz rejection > 40dB
- [ ] Step response test with function generator

**Day 4 Deliverable:** Verified EMG front-end on breadboard

---

### Day 5: NFMI Transceiver Integration (Apr 2)
**Goal:** Establish 13.56MHz magnetic link

**Morning: Transmitter Assembly**
```
AS3932/3933 Wiring:
- VDD: 3.3V
- GND: GND
- SDI: SPI MOSI (PA7)
- SDO: SPI MISO (PA6)
- CLK: SPI SCK (PA5)
- CS: PA4
- IRQ: PA3 (interrupt for received data)
- LC Tank: 10µH inductor + 150pF cap (13.56MHz)
```

**Afternoon: Link Verification**
- [ ] Configure AS3932 as transmitter
- [ ] Measure TX coil current (expect ~10mA)
- [ ] Verify 13.56MHz with oscilloscope
- [ ] Test basic data transmission between two units

**Debug Points:**
- Check LC tank resonance with scope
- Verify SPI communication (scope/logic analyzer)
- Confirm carrier frequency accuracy

**Day 5 Deliverable:** Working NFMI link, verified data transmission

---

### Day 6: Audio Output Stage Assembly (Apr 3)
**Goal:** Complete signal chain to bone conduction output

**Morning: Audio Amplifier Integration**
```
STM32 DAC → TPA2005D1 → Bone Conduction Driver
Steps:
1. Configure STM32 DAC (PA4/PA5)
2. Wire TPA2005D1 (shutdown, bypass, output)
3. Connect bone conduction transducer
4. Add output coupling capacitors (100µF)
```

**Afternoon: Power Management Integration**
- Wire TP4056 charge controller
- Add TPS63001 buck-boost for stable 3.3V
- Battery holder wiring with protection

**Testing Checklist:**
- [ ] Audio output with test tone (1kHz)
- [ ] Volume range check
- [ ] Charging circuit functional
- [ ] Battery-powered operation
- [ ] Current consumption measurement

**Day 6 Deliverable:** Complete signal chain from EMG to audio out

---

## Day 7-9: Firmware Development (Days 7-9, Apr 4-6)

### Day 7: Core Signal Acquisition Firmware (Apr 4)
**Goal:** Real-time EMG sampling and processing

**Tasks:**
```c
// ADC Configuration
- Configure ADC1: 12-bit, 1ksps, DMA
- Timer-triggered sampling
- Circular buffer for continuous acquisition

// Signal Processing
- Implement bandpass filter (0.5-500Hz, IIR)
- RMS envelope calculation
- Adaptive threshold detection
- Voice activity detection (VAD)
```

**Verification:**
- [ ] ADC sampling at 1kHz verified
- [ ] Filter response matches simulation
- [ ] Envelope tracks EMG activity
- [ ] VAD triggers correctly on test signals

---

### Day 8: NFMI Communication Stack (Apr 5)
**Goal:** Packetized data transmission protocol

**Tasks:**
```c
// AS3932 Driver
- SPI initialization and register access
- Carrier frequency calibration
- FSK modulation setup

// Protocol Layer
- TDMA frame structure (20ms slots)
- Packet formation with headers
- Hamming(8,4) FEC encoding
- Sync word detection

// Data Flow
- EMG → Codec → Packet → NFMI TX
- NFMI RX → Packet → Decode → Audio DAC
```

**Verification:**
- [ ] Bidirectional data exchange
- [ ] Packet error rate < 5%
- [ ] Latency < 50ms (RF link only)

---

### Day 9: Audio Codec & Integration (Apr 6)
**Goal:** Complete end-to-end audio path

**Tasks:**
```c
// Audio Codec
- ADPCM encoder/decoder (8kbps target)
- Adaptive delta modulation
- Decompression to DAC

// System Integration
- Main loop: Sample → Encode → TX → RX → Decode → Output
- Power management (sleep modes)
- LED status indicators
- Volume control
```

**Verification:**
- [ ] Test tone round-trip (scope measurement)
- [ ] Audio quality assessment
- [ ] Full duplex operation
- [ ] Battery life measurement

**Day 9 Deliverable:** Functional firmware with end-to-end audio

---

## Day 10-11: Testbed Setup (Days 10-11, Apr 7-8)

### Day 10: Physical Test Environment (Apr 7)
**Goal:** Create controlled testing space

**Tasks:**
- [ ] Construct Faraday cage (aluminum mesh + wood frame)
- [ ] Setup RF-quiet test area (2m x 2m minimum)
- [ ] Install range markers (0.5m, 1m, 2m, 3m)
- [ ] Position reference mannequin head for consistent electrode placement
- [ ] Prepare shielded power supply for test instruments

**Equipment Setup:**
- Oscilloscope positioned outside test area
- Signal generator for reference signals
- Audio recording equipment for MOS testing
- Multimeters for current monitoring

---

### Day 11: Test Protocols & Calibration (Apr 8)
**Goal:** Establish repeatable test procedures

**Calibration Tasks:**
- [ ] Calibrate EMG gain settings with known signal
- [ ] Verify NFMI TX power level
- [ ] Measure baseline noise floor
- [ ] Establish audio output reference level

**Test Protocols Created:**
1. **Range Test Protocol** - Standard phrase transmission at distance
2. **Audio Quality Protocol** - PB-50 word list methodology
3. **Battery Test Protocol** - Continuous operation measurement
4. **Interference Test Protocol** - WiFi/BT coexistence

**Day 11 Deliverable:** Calibrated testbed with documented procedures

---

## Day 12-14: Validation Testing (Days 12-14, Apr 9-11)

### Day 12: Functional Testing (Apr 9)
**Goal:** Verify core functionality

**Tests:**
| Test | Target | Method |
|------|--------|--------|
| EMG Signal Detection | >90% | Subvocal word test |
| NFMI Link at 1m | 100% packet rx | Fixed distance |
| NFMI Link at 2m | >95% packet rx | Fixed distance |
| Audio Output | Intelligible | Subjective test |

**Pass Criteria:**
- Clear message transmission at 2m
- Intelligible speech via bone conduction
- <100ms end-to-end latency

---

### Day 13: Performance Characterization (Apr 10)
**Goal:** Measure performance metrics

**Tests:**
| Metric | Target | Result |
|--------|--------|--------|
| Maximum Range | 3m | TBD |
| Battery Life | 30 min | TBD |
| Latency | <100ms | TBD |
| Audio MOS | >3.0 | TBD |
| False Trigger | <5% | TBD |

**Documentation:**
- Record all measurements
- Photograph setup
- Video demonstration clips

---

### Day 14: Final Validation & Documentation (Apr 11)
**Goal:** Cycle 1 completion

**Morning: Final Tests**
- [ ] Complete range sweep (0.5m - 3m)
- [ ] Battery life confirmation
- [ ] Demonstration for documentation

**Afternoon: Documentation**
- [ ] Update SPECS.md with measured values
- [ ] Create test report
- [ ] Photo/video documentation
- [ ] Git commit all changes
- [ ] Prepare Cycle 2 plan

**Cycle 1 Completion Criteria:**
- [ ] Working dual-unit prototype
- [ ] Validated 2m range communication
- [ ] 30+ minute battery life demonstrated
- [ ] Test report complete
- [ ] GitHub repo updated

---

## Risk Mitigation

| Risk | Probability | Impact | Mitigation |
|------|-------------|--------|------------|
| AS3932 delivery delay | Medium | High | Order AS3933 backup; have eval boards |
| EMG noise issues | Medium | Medium | Shielded cables; software filtering |
| Bone conducer fit | Low | Medium | Multiple mounting options |
| Range <2m | Low | High | Retransmission protocol; tune TX power |
| Battery life <30m | Low | Medium | Larger battery pack option |

---

## Daily Standup Checklist

Each day, verify:
- [ ] Blockers identified
- [ ] Progress against plan
- [ ] Component deliveries on track
- [ ] Next day's tasks clear

---

## Emergency Contacts & Resources

| Resource | Link/Contact |
|----------|--------------|
| AS3932 Datasheet | docs/datasheets/AS3932.pdf |
| STM32L4 Reference | ST website RM0394 |
| Supplier Support | Digi-Key tech support |
| OpenClaw Workspace | /workspace/silent-001_research/ |

---

*Document: BUILD_PLAN_14DAY.md*  
*Project: SILENT-001*  
*Part of META-SYSTEM autonomous research consortium*
