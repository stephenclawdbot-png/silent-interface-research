# SILENT Interface - Bill of Materials

## Project: SILENT-001
**Phase:** Cycle 1 - Prototype
**Last Updated:** 2026-03-28

---

## Component Summary

| Category | Item Count | Est. Cost (USD) | Lead Time |
|----------|------------|-----------------|-----------|
| Microcontrollers | 2 | $40 | In stock |
| NFMI Transceivers | 2 | $30 | 7 days |
| EMG Front-End | 2 sets | $50 | In stock |
| Audio Output | 2 | $60 | In stock |
| Power Management | 2 sets | $25 | In stock |
| Passive Components | ~100 | $30 | In stock |
| PCB/Connectors | 2 sets | $40 | 5 days |
| Enclosures | 2 | $30 | 3D printed |
| Misc/Hardware | - | $25 | In stock |
| **TOTAL** | - | **~$330** | **~7 days** |

---

## Detailed BOM

### 1. Microcontrollers (2 units)

| Qty | Part Number | Description | Supplier | Est. Cost | Notes |
|-----|-------------|-------------|----------|-----------|-------|
| 2 | STM32L476RG | ARM Cortex-M4, 1MB Flash, 128KB SRAM, ultra-low power | Digi-Key | $15.23 | Core processor. Alternative: STM32L4A6 (more RAM) |
| 2 | - | STM32 Nucleo-L476RG dev board (optional, for prototyping) | Digi-Key | $14.46 | For initial breadboard testing |

**Alt Sources:** Mouser, Arrow, ST Direct

---

### 2. NFMI Transceivers (2 units)

| Qty | Part Number | Description | Supplier | Est. Cost | Notes |
|-----|-------------|-------------|----------|-----------|-------|
| 2 | AS3932 | 3D Low Frequency Receiver & Wake-up IC | Digi-Key | $8.45 | RX side, also has transmitter capability |
| 2 | AS3933 | 3D Low Frequency Receiver IC (simpler) | Digi-Key | $6.89 | Alternative to AS3932 |
| 2 | 10µH Ferrite Core Inductor | TX/RX coil, 10µH, Q>50 @ 13.56MHz | Coilcraft/Digi-Key | $1.50 | Custom winding possible |

**Alt Sources:** ams-OSRAM direct, Future Electronics

**Note:** AS3932/AS3933 eval boards recommended for initial prototyping (order separately).

---

### 3. EMG Front-End (2 sets)

| Qty | Part Number | Description | Supplier | Est. Cost | Notes |
|-----|-------------|-------------|----------|-----------|-------|
| 4 | INA333 | Precision instrumentation amplifier, low noise | Digi-Key | $2.85 | Input stage, rail-to-rail |
| 4 | OPA2333 | Zero-drift dual op-amp | Digi-Key | $2.10 | Active filter stages |
| 2 | AD8429 | Ultra-low noise instrumentation amp (alternative) | Digi-Key | $12.50 | Higher performance option |
| 8 | 20mm Ag/AgCl | Medical-grade EMG electrodes | Amazon/AliExpress | $0.50 | Buy in 20-packs |
| 2 | 2.54mm Header | Electrode connection headers | Digi-Key | $0.25 | Snap connectors |
| - | Conductive Gel | Aquasonic or equivalent | Amazon | $5.00 | Small tube |

---

### 4. Audio Output Stage (2 units)

| Qty | Part Number | Description | Supplier | Est. Cost | Notes |
|-----|-------------|-------------|----------|-----------|-------|
| 2 | TPA2005D1 | 1.4W mono Class-D audio amp | Digi-Key | $1.25 | Efficient, low heat |
| 2 | BH01B | Bone conduction transducer | AliExpress/Amazon | $18.00 | Search "bone conduction module" |
| 2 | Radioear B81 | Professional bone vibrator (optional upgrade) | Frye Electronics | ~$300 | Clinical grade (too expensive for prototype) |
| 4 | 10µF/25V | Output coupling capacitors | Digi-Key | $0.15 | Tantalum or ceramic |
| 4 | 100µF/16V | Power supply decoupling | Digi-Key | $0.25 | Low ESR |

**Bone Conductor Alt:**
- Generic 8mm bone conduction vibrator: ~$8 on AliExpress
- AfterShokz replacement drivers: ~$15 (salvage from broken units)

---

### 5. Power Management (2 sets)

| Qty | Part Number | Description | Supplier | Est. Cost | Notes |
|-----|-------------|-------------|----------|-----------|-------|
| 2 | TP4056 | LiPo charge controller with protection | Digi-Key | $1.10 | USB charging module |
| 2 | MCP73831 | Single-cell LiPo charger IC | Digi-Key | $0.85 | Alternative to TP4056 |
| 2 | TPS63001 | Buck-boost converter, 3.3V out | Digi-Key | $3.45 | Stable 3.3V from LiPo |
| 2 | 18650 Holder | Battery holder with protection | Amazon | $1.50 | Or pouch cell |
| 2 | 18650 Cell | LG MJ1 3500mAh or Samsung 30Q | Li-Ion Wholesale | $6.50 | Protected cells recommended |
| 4 | 10kΩ 1% | Pull-up resistors | Digi-Key | $0.05 | Standard |
| 4 | 4.7µH 2A | Buck-boost inductor | Digi-Key | $0.75 | Power inductor |

---

### 6. Passive Components (kit)

| Qty | Part Number | Description | Supplier | Est. Cost | Notes |
|-----|-------------|-------------|----------|-----------|-------|
| 100 | 0603/0805 Kit | Resistor assortment (1Ω - 1MΩ) | Amazon | $8.00 | SMD resistor kit |
| 100 | 0603/0805 Kit | Capacitor assortment (10pF - 100µF) | Amazon | $10.00 | Ceramic + electrolytic |
| 20 | 10nF/100nF | Decoupling capacitors | Digi-Key | $0.10 | X7R ceramic |
| 20 | 1µF/10V | Signal capacitors | Digi-Key | $0.15 | X5R/X7R |

---

### 7. PCB and Connectors (2 sets)

| Qty | Part Number | Description | Supplier | Est. Cost | Notes |
|-----|-------------|-------------|----------|-----------|-------|
| 2 | Custom PCB | 2-layer prototype PCB, 100mm x 50mm | JLCPCB/PCBWay | $15.00 | 2-day fast turn |
| 4 | USB-C Connector | Power/charging port, SMD | Digi-Key | $0.85 | Molex or Amphenol |
| 10 | JST-PH 2.0 | Battery and wire connectors | Digi-Key | $0.25 | Common standard |
| 4 | 20-pin Header | Programming/debug headers | Digi-Key | $0.60 | 2.54mm pitch |
| 2 | 8-pin Header | SPI/I2C expansion | Digi-Key | $0.30 | 2.54mm pitch |

---

### 8. Mechanical Enclosure (2 units)

| Qty | Part Number | Description | Supplier | Est. Cost | Notes |
|-----|-------------|-------------|----------|-----------|-------|
| 200g | PETG Filament | Prusament or Hatchbox | Amazon | $25 | For 3D printing |
| 4 | M2.5x8mm | Stainless screws | Amazon | $2.00 | Case assembly |
| 4 | M2.5 Heat Set | Threaded inserts (brass) | Amazon | $3.00 | For plastic housing |
| 2 | Silicone Sheet | 1mm thick, for gaskets | Amazon | $3.00 | Water resistance |
| - | Foam Padding | Double-sided adhesive | Amazon | $5.00 | Comfort layer |

**Enclosure Design:** Custom files to be created in Fusion360 / STEP format.

---

### 9. Test Equipment & Tools

| Qty | Part Number | Description | Supplier | Est. Cost | Notes |
|-----|-------------|-------------|----------|-----------|-------|
| 1 | ADALM2000 | Oscilloscope/Spectrum analyzer | Analog Devices | $279 | Recommended for signal analysis |
| 1 | - | Function generator (if not in ADALM2000) | - | $50 | AD9833 module as alt |
| - | Shielded Box | DIY Faraday cage | - | $20 | Aluminum foil + cardboard |
| - | Multimeter | Basic DMM | Amazon | $15 | Voltage/current checks |

---

## Sourcing Strategy

### Priority 1 (Order Immediately)
1. AS3932/AS3933 ICs - longest lead time (~1 week)
2. Bone conduction transducers - ship from China (~2 weeks)
3. PCB fabrication - allow 3-5 days

### Priority 2 (Breadboard Prototype)
1. STM32 Nucleo dev boards
2. INA333, OPA2333 op-amps
3. Standard passives, breadboards

### Priority 3 (Final Assembly)
1. Custom 3D printed enclosures
2. 18650 batteries
3. Final connectors, wiring

---

## Alternative/Vendor Sources

| Component | Primary | Backup 1 | Backup 2 |
|-----------|---------|----------|----------|
| AS3932 | Digi-Key | Mouser | ams-OSRAM Direct |
| STM32L476 | Digi-Key | Mouser | Arrow |
| Bone Conduction | Amazon | AliExpress | Aftershokz spare parts |
| PCB | JLCPCB | PCBWay | OSHPark |
| Passives | Digi-Key | Mouser | Amazon (Kit) |
| 3D Print | Local | JLCPCB | Shapeways |

---

## Budget Estimate by Phase

| Phase | Cost | Notes |
|-------|------|-------|
| Breadboard Prototype | $150 | Dev boards, DIP components, eval modules |
| PCB Assembly (2 units) | $120 | Custom PCBs, SMD soldering |
| Enclosure & Final | $60 | 3D printing, finishing, wires |
| Test Equipment | $300 | Oscilloscope/purchased as needed |
| **Total Project** | **~$630** | Includes single test instrument |

---

## Procurement Checklist

- [ ] AS3932/AS3933 transceivers ordered
- [ ] STM32L4 dev boards in stock
- [ ] Bone conduction modules ordered
- [ ] PCB design sent to fab
- [ ] Passive component kit ordered
- [ ] Batteries sourced (if not on hand)
- [ ] 3D printer access confirmed
- [ ] Test equipment available

---

*Document: BOM.md*  
*Project: SILENT-001*  
*Part of META-SYSTEM autonomous research consortium*
