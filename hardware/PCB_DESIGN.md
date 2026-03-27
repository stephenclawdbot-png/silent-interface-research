# SILENT-001 PCB Design Document

**Version:** 0.1.0  
**Target Fabrication:** JLCPCB 2-Layer  
**Board Dimensions:** 50mm x 35mm (neckband form factor)  
**Target Cost:** $2-5/unit @ 10 qty

---

## Module Layout

The PCB is organized into functional zones for optimal signal integrity and manufacturability.

```
                    ┌─────────────────────────────────────────────────┐
                    │                 PCB Top View                     │
                    │                    50mm x 35mm                     │
                    └─────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────────────┐
│   [EMG ZONE - Left]            [MCU ZONE]           [AUDIO/NFMI ZONE - Right]│
│                                                                             │
│   ┌──────────────┐            ┌──────────┐           ┌──────────────────┐  │
│   │  Channel 0  │            │ STM32L4  │           │    NFMI TX/RX     │  │
│   │  (LLS)      │────────────│          │───────────│   (AS3932/3933)   │  │
│   └──────────────┘            │ L476     │           └──────────────────┘  │
│   ┌──────────────┐            │          │           ┌──────────────────┐  │
│   │  Channel 1  │            │  80MHz   │           │  Audio Amp       │  │
│   │  (LRS)      │────────────│  Ultra   │───────────│  (TPA2005D1)     │  │
│   └──────────────┘            │  Low     │           └─────────┬────────┘  │
│   ┌──────────────┐            │  Power   │                     │         │
│   │  Channel 2  │            │          │           ┌───────────────┐      │
│   │  (LLI)      │────────────│          │           │ Bone Conduct  │      │
│   └──────────────┘            └──────────┘           │ Connector     │      │
│   ┌──────────────┐                                    └───────────────┘      │
│   │  Channel 3  │            ┌──────────┐                                 │
│   │  (LRI)      │────────────│   Power  │                                 │
│   └──────────────┘            │  Mgmt    │                                 │
│                               │ (TPS7A20)│                                 │
│                               └──────────┘                                 │
│                                                                             │
│   [Guard Ring] ───────────────────────────────────────── [Guard Ring]       │
│                                                                             │
│   USB-C                                      Battery Connector              │
│   (Power +                              (JST-PH 2.0mm, 3.7V LiPo)         │
│   Programming)                                                              │
└─────────────────────────────────────────────────────────────────────────────┘
```

### Zone Specifications

| Zone | Components | Signal Concerns | Routing Priority |
|------|-----------|-----------------|------------------|
| EMG (Left) | INA333 x4, filters | Very high impedance, µV signals | Star ground, guard rings |
| MCU (Center) | STM32L476, decoupling | Clock noise, fast edges | Solid ground under MCU |
| NFMI (Right-Center) | AS3932/3933, coil | 13.56MHz RF, sensitive RX | Keep away from MCU clock |
| Audio (Right) | TPA2005D1, filters | Class-D switching noise | Separate return path |
| Power (Bottom) | TPS7A20, USB-C | Current transients | Short, wide traces |

---

## Pin Assignments - STM32L476RG

### GPIO Pin Map (LQFP64)

```
Pin    Function                    Direction   Notes
───────────────────────────────────────────────────────────────────────
PA0    ADC1_IN0 (EMG_CH0+)        Analog      Laryngeal Left Superior
PA1    ADC1_IN1 (EMG_CH1+)        Analog      Laryngeal Right Superior
PA2    USART2_TX                  AF_PP       Debug/UART output
PA3    ADC1_IN3 (EMG_CH2+)        Analog      Laryngeal Left Inferior
PA4    ADC1_IN4 (EMG_CH3+)        Analog      Laryngeal Right Inferior
PA5    GPIO_Output (STATUS_LED)    PP          User LED (on Nucleo)
PA6    GPIO_Output (EMG_ACTIVE)    PP          EMG activity indicator
PA7    SPI1_MOSI                  AF_PP       AS3933 data/config
PA9    USB_VBUS (VBUS_SENSE)      Input       USB power detect
PA10   USB_ID                     Input       USB OTG detect
PA11   USB_DM                     Analog      USB differential
PA12   USB_DP                     Analog      USB differential
PA13   SWDIO                      AF          Debug interface
PA14   SWCLK                      AF          Debug interface
PA15   SPI1_NSS                   AF_PP       AS3933 chip select

PB0    ADC1_IN15 (EMG_AGC_REF)    Analog      AGC reference monitor
PB3    SPI1_SCK                   AF_PP       AS3933 clock
PB4    SPI1_MISO                  AF_PP       AS3933 data out
PB6    I2C1_SCL                   AF_OD       Audio amp config (pull-up)
PB7    I2C1_SDA                   AF_OD       Audio amp config (pull-up)
PB8    NFMI_DATA                  Input       AS3932/3933 wake indicator
PB9    NFMI_IRQ                   Input       AS3932/3933 interrupt
PB12   NFMI_ANT_TX                Output      TX coil drive (complementary)
PB13   NFMI_ANT_TXN               Output      TX coil drive (complementary)
PB14   NFMI_ANT_RX                Analog      RX coil sense

PC0    GPIO_Output (PWR_EN)       PP          System power enable
PC1    GPIO_Output (CHG_EN)       PP          Battery charge enable
PC2    ADC1_IN12 (BAT_VDIV)       Analog      Battery voltage monitor
PC3    GPIO_Input (CHG_STATUS)    Input       Charge status from TP4054
PC13   GPIO_Input (USR_BTN)       Input       User button (wake from stop)
PC14   OSC32_IN                   Analog      LSE crystal (32.768kHz)
PC15   OSC32_OUT                  Analog      LSE crystal (32.768kHz)

PD0    GPIO_Output (AUD_SD)       PP          Audio shutdown (active low)
PD1    GPIO_Output (AUD_MUTE)     PP          Audio mute

PE0    TIM2_ETR (ADC_TRIG_EXT)    AF          Optional external ADC trigger
PE1    TIM2_CH1                   AF          PWM for class-D audio

PH0    OSC_IN                     Analog      HSE crystal (8MHz or 16MHz)
PH1    OSC_OUT                    Analog      HSE crystal

VREF+  VREF+                      Analog      Decoupled to GND
VBAT   VBAT                       Power       Battery backup domain
```

### Alternate Function Summary

| Peripheral | Pins | Purpose |
|------------|------|---------|
| ADC1 | PA0, PA1, PA3, PA4, PB0, PC2 | EMG sampling, battery monitor |
| USART2 | PA2, PA3 | Debug/Command output |
| SPI1 | PA7, PA15, PB3, PB4 | NFMI transceiver interface |
| I2C1 | PB6, PB7 | Audio amplifier config |
| TIM2 | PE0, PE1 | Audio PWM, timing |
| USB_FS | PA9-PA12 | USB device + charging |

---

## Component Placement Guidelines

### EMG Front-End (Channels 0-3)

```
Component Layout:

    EMG_CH+  ───┬───[100Ω]───┬───[INA333]──┬──┬──[RC Filter]──┬── ADC
                │            │             │  │               │
                │         ┌──┴──┐          │  │               │
               [1M]      │ 1pF │     ┌────┴──┴──┐           ┌──┴──┐
                │        │ Guard│     │   10k    │           │ 10pF│
                │        │ Ring │     └────┬─────┘           │ Shunt│
               GND       └─────┘          │                 └──┬──┘
                                          │                    │
    EMG_CH-  ─────────────────────────────┴──[10k bias]──────┴── GND

Placement Rules:
1. INA333 must be within 10mm of electrode connector
2. Guard ring surrounds high-impedance inputs (IN+/IN-)
3. Input trace width: 0.3mm max (reduce capacitance)
4. No copper pour under input traces
5. Differential pair routing for sense lines
```

### NFMI Transceiver

```
AS3932/3933 Placement:

                    ┌────────────┐
    SPI (MOSI/MISO) │            │ TX coil drive (complementary)
    SPI (SCK/CS)   │  AS3932/3  ├──────┬──────┬──┐
    IRQ/Data       │            │      │      │  │
                    └──────┬─────┘   [C1]   [L1]  │
                           │         100pF  10µH  │
                          [C2]                [C2]
                          100nF               100pF
                           │                  │
                          GND                GND

Coil Specifications:
- Inductance: 10µH ±10%
- Q factor: >50 @ 13.56MHz
- Package: 0805 or custom wound
- Placement: Keep 5mm from MCU
```

### Audio Output Stage

```
TPA2005D1 Class-D Audio:

    I2C (PB6/PB7) ──┬─────────┐
    SD (shutdown)   │ TPA2005 ├────┬────┬────┬── OUT+
    MUTE           │   D1    │    │    │   [C]
    PWM input ─────┤         ├────┴────┘   100nF
                   │         │         [L]
                   │         ├────┬────┬── OUT-
                   └────┬────┘    │    │  [C]
                       [C]      [L]   │   100nF
                      1µF       33µH  │
                       │              │   Bone Conductor
                      GND            ┌┴┐  Connector
                                   ═╡ ╞═
                                     └┬┘
                                      │
    Placement: Keep switching noise away from EMG front-end
```

---

## Design Rules - JLCPCB 2-Layer

### Stackup

| Layer | Type | Thickness | Notes |
|-------|------|-----------|-------|
| Top | Signal | 1oz Cu | Components, routing |
| Dielectric | FR4 | 1.6mm ±10% | Standard |
| Bottom | Signal/GND | 1oz Cu | Ground pours, minimal routing |

### Trace Specifications

| Signal Type | Width | Clearance | Notes |
|-------------|-------|-----------|-------|
| Standard digital | 0.2mm (8mil) | 0.2mm | 5V max, <100mA |
| Power (VCC 3.3V) | 0.5mm (20mil) | 0.2mm | Max 500mA |
| Power (VBAT) | 0.8mm (32mil) | 0.3mm | Peak 1A charging |
| USB D+/D- | 0.25mm (10mil) | 0.25mm | 90Ω differential |
| EMG sense | 0.3mm (12mil) max | 0.3mm | Microvolt signals |
| Guard ring | 0.4mm (16mil) | 0.15mm | Around EMG inputs |

### Via Specifications

| Type | Drill | Pad | Purpose |
|------|-------|-----|---------|
| Standard | 0.3mm (12mil) | 0.6mm | Signal, general |
| Power | 0.4mm (16mil) | 0.8mm | VCC, GND distribution |
| Thermal | 0.3mm | 0.6mm array | Thermal relief |

### Clearances (Minimum)

| Feature | Clearance | JLCPCB Constraint |
|---------|-----------|-------------------|
| Trace to trace | 0.15mm (6mil) | ✅ Standard |
| Trace to pad | 0.15mm (6mil) | ✅ Standard |
| Pad to pad (different nets) | 0.15mm (6mil) | ✅ Standard |
| Via to via | 0.2mm (8mil) | ✅ Standard |
| Component edge to trace | 0.25mm | Best practice |
| Board edge to copper | 0.3mm (12mil) | JLCPCB spec |

### Copper Pour Strategies

```
┌─────────────────────────────────────────────────────────────┐
│  TOP LAYER (Signal + Ground Islands)                       │
│                                                             │
│   ┌──────────┐                                              │
│   │  EMG     │    ┌────────────────────────┐                 │
│   │  Zone    │    │         GND Pour      │                 │
│   │ (No pour)│    │   (Avoids EMG area)    │                 │
│   └──────────┘    │                        │                 │
│                   │   ┌────────┐   ┌─────┐  │                 │
│                   │   │  MCU   │   │NFMI │  │                 │
│   ┌──────────┐    │   │        │   │     │  │                 │
│   │ Guard    │    │   └────────┘   └─────┘  │                 │
│   │ Ring     │    │                        │                 │
│   └──────────┘    └────────────────────────┘                 │
│                                                             │
└─────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────┐
│  BOTTOM LAYER (Ground Plane + Minimal Signals)              │
│                                                             │
│  ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░  │
│  ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░  │
│  ░░░┌────────┐░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░  │
│  ░░░│  EMG   │░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░  │
│  ░░░│ GND    │░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░  │
│  ░░░│ Return │░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░  │
│  ░░░└────────┘░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░  │
│  ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░  │
│  ░░░[USB-C]░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░  │
│  ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░  │
│                                                             │
│  ░ = GND pour (solid, no thermal spokes for GND pads)       │
└─────────────────────────────────────────────────────────────┘
```

### Critical Routing Rules

1. **EMG Signal Integrity**
   - No vias on EMG sense traces (keep on top layer)
   - Guard ring around high-impedance nodes (0.4mm trace, driven by opamp output)
   - Star ground: all EMG returns to single point near INA333
   - Differential routing for each channel pair (+/-)

2. **NFMI RF Routing**
   - TX coil traces: keep short (<10mm), equal length
   - No stubs on 13.56MHz lines
   - Ground stitching vias around coil area
   - Keep away from fast-switching digital signals (SPI >5MHz)

3. **Power Distribution**
   - VBAT rail: 0.8mm wide, bypass caps at each IC
   - 3.3V rail: star distribution from LDO, decoupling caps 100nF + 10µF
   - Separate analog 3.3V (AVDD) with LC filter (10µH + 10µF)

4. **Audio Class-D**
   - Output filter components close to IC
   - Separate ground return for switching currents
   - Keep away from EMG (minimum 15mm separation)

---

## Manufacturing Notes

### JLCPCB Standard 2-Layer Stackup
- Material: FR-4 TG130
- Surface finish: HASL (lead-free recommended)
- Silkscreen: White on both sides
- Solder mask: Green (default) or specify
- Min hole: 0.2mm (8mil)
- Min via: 0.3mm drill, 0.6mm pad

### Recommended Fabrication Specs
```
PCB Thickness:    1.6mm
Copper Weight:      1oz (standard)
Surface Finish:     Lead-free HASL
Silkscreen:         White
Solder Mask:        Green
Min Trace/Space:    6mil/6mil (0.15mm/0.15mm)
Via Tenting:        Yes (for BGA-like density)
Edge Plating:       No
Castellated Holes:  No
```

### Assembly Considerations
- All passives 0805 or larger (0603 acceptable for non-critical)
- No BGA packages (QFP-64 is largest)
- Single-sided component placement preferred
- Programming header: 0.1" pin header or Tag-Connect

---

## Gerber Outputs

| Layer | Filename Pattern | Purpose |
|-------|-----------------|---------|
| Top Copper | *GTL | Signal traces |
| Bottom Copper | *GBL | Ground plane |
| Top Solder Mask | *GTS | Solder mask openings |
| Bottom Solder Mask | *GBS | Solder mask openings |
| Top Silkscreen | *GTO | Component labels |
| Bottom Silkscreen | *GBO | Optional labels |
| Drill | *TXT, *DRL | Through-hole vias |
| Board Outline | *GKO, *GM1 | Edge cuts |

---

## Revision History

| Version | Date | Changes |
|---------|------|---------|
| 0.1.0 | 2025-03-28 | Initial PCB design document |

---

## Next Steps

1. [ ] Create KiCAD schematic from this document
2. [ ] Place components per zone layout
3. [ ] Route critical EMG traces first
4. [ ] Add test points for debugging
5. [ ] Generate Gerbers and submit to JLCPCB
6. [ ] Order 5-10 prototypes for validation