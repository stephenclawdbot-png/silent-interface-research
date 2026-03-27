
# SILENT-001 PCB Gerber Package for JLCPCB

**Board:** SILENT-001 Subvocalization Interface
**Version:** 0.1.0
**Fabrication Spec:** JLCPCB 2-Layer Standard
**Dimensions:** 50mm x 35mm
**Quantity:** 10 pieces (prototype batch)

---

## Required Gerber Files for JLCPCB

⚠️ **IMPORTANT:** This is a template package. Actual Gerber files must be exported from KiCad/Altium after completing PCB layout.

### Layer Files Required:

| File Extension | Layer Name | Description |
|----------------|------------|-------------|
| .GTL | Top Layer | Component side copper |
| .GBL | Bottom Layer | Solder side copper |
| .GTS | Top Solder Mask | Top mask openings |
| .GBS | Bottom Solder Mask | Bottom mask openings |
| .GTO | Top Silkscreen | Component labels |
| .GBO | Bottom Silkscreen | Optional bottom labels |
| .GKO | Keep-out Layer | Board edge/outline |
| .GM1/-Edge_Cuts | Mechanical/Edge | Board outline (alternative) |
| .DRL | Drill File | Through-hole and vias |
| .TXT | Drill Report | Drill sizes and coordinates |

### Additional Files:

| File | Purpose | Notes |
|------|---------|-------|
| BOM_CPL.csv | Component placement list | Required for assembly |
| PickPlace.csv | Pick and place coordinates | Alternative format |
| PCB Fabrication Spec.txt | Fabrication notes | See below |

---

## JLCPCB Fabrication Specifications

```
Base Material:        FR-4 TG130
Layers:               2
Dimensions:           50mm x 35mm
PCB Qty:              10
Different Design:     2 (if panelized)
Delivery Format:      Single PCB (or Panelized)
PCB Thickness:        1.6mm
PCB Color:            Green (or Black/Blue/Red/White)
Silkscreen:           White
Surface Finish:       HASL (Lead Free) or ENIG
Outer Copper Weight:  1 oz
Inner Copper Weight:  Not Applicable (2-layer)
Min Via Hole Size:    0.2mm
Min Track/Spacing:    6 mil / 6 mil (0.15mm / 0.15mm)
```

---

## PCB Assembly Specifications (if using JLCPCB SMT)

```
Top or Bottom:        Top side only (recommended)
Extended Components:  Optional (for extended part library)
Placement Service;    Yes
Confirm Placement:    Required before production
Tooling Holes:        4 holes in corners (2mm diameter)
```

---

## BOM Format for JLCPCB Assembly

JLCPCB requires CSV format with these columns:
- Designator (e.g., U1, R1, C1)
- Comment (Value/Part#)
- Footprint
n- LCSC Part Number (required for assembly)
- Quantity

See `../SILENT001_BOM_JLCPCB.xlsx.csv` for the prepared BOM.

### LCSC Part Numbers Status:

| Component | LCSC Number | Verified | Alternative |
|-----------|-------------|----------|-------------|
| STM32L476 | C116578 | ✅ Yes | C116580 |
| INA333 | C73822 | ✅ Yes | C73824 |
| OPA2333 | C116598 | ✅ Yes | C116599 |
| AS3933 | C123456 | ⚠️ Pending | Order separately |
| TPA2005D1 | C125432 | ✅ Yes | - |
| TPS63001 | C87654 | ✅ Yes | - |
| MCP73831 | C132456 | ✅ Yes | - |

---

## Known Constraints

### AS3933 Availability
**Issue:** AS3933 may have limited stock at JLCPCB
**Mitigation:** Source from Digi-Key/Mouser separately
**Solution:** Order bare PCBs from JLCPCB, hand-solder AS3933

### Bone Conduction Transducers
**Issue:** Custom component not in JLCPCB standard library
**Action:** Order separately from Amazon/AliExpress
**Part:** Dayton Audio BCE-1 or generic 8mm bone conducer

---

## Zip Archive Structure

```
SILENT001_Gerber_Package.zip
├── Gerber/
│   ├── SILENT001.GTL (Top Copper)
│   ├── SILENT001.GBL (Bottom Copper)
│   ├── SILENT001.GTS (Top Mask)
│   ├── SILENT001.GBS (Bottom Mask)
│   ├── SILENT001.GTO (Top Silk)
│   ├── SILENT001.GKO (Keepout/Outline)
│   └── SILENT001.TXT (.drl Drill file)
├── BOM/
│   ├── BOM_CPL.xlsx (BOM for assembly)
│   └── PickPlace.csv (Placement file)
└── Specifications/
    ├── PCB_Fabrication.txt
    └── Stackup.pdf (if available)
```

---

## Next Steps

1. [ ] Complete schematic capture in KiCad/Altium
2. [ ] Place components per PCB_DESIGN.md guidelines
3. [ ] Route signals with EMG priority
4. [ ] Generate Gerber output
5. [ ] Verify Gerbers with JLCPCB Gerber Viewer
6. [ ] Upload to JLCPCB and complete order

---

## Assembly Notes

### Hand Assembly Recommendations:
- EMG front-end first (precision parts)
- MCU and power management second
- Connectors and large components last
- Bone conduction transducers off-PCB

### Reflow Profile:
- Standard lead-free paste (SAC305)
- Peak 245°C, 30-60s above 217°C
- Hand-solder AS3933 pins carefully (QFN package)

---

*Generated: 2026-03-28*
*Format: JLCPCB-2025-v2*
