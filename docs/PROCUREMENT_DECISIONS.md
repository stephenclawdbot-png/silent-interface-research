# SILENT-001: Procurement Decisions Log

**Project:** SILENT Interface (SILENT-001)  
**Phase:** Cycle 1 - Prototype  
**Date:** 2026-03-28  
**Decision Authority:** META-SYSTEM Subagent

---

## Decision Summary

This document records procurement decisions for the Cycle 1 prototype build. Total BOM target: ~$330 (initial prototype components).

**Procurement Status:** ⏳ IN PROGRESS - Orders being placed

---

## Decision 1: NFMI Transceiver Selection

**Decision:** Primary: AS3932, Backup: AS3933

**Date:** 2026-03-28  
**Status:** ✅ DECIDED

### Rationale
| Option | Pros | Cons |
|--------|------|------|
| AS3932 | 3D reception, more sensitive | Slightly more expensive |
| AS3933 | Cheaper, simpler | Single channel |

**Selected:** AS3932 (2x units from Digi-Key @ $8.45 each)

**Backup Plan:** If AS3932 unavailable/extended lead time, AS3933 is pin-compatible alternative

**Order Details:**
- Qty: 2
- Supplier: Digi-Key
- Part #: AS3932-BQFT
- Unit Cost: $8.45
- Lead Time: 7 days
- **Action:** ☐ PLACE ORDER

---

## Decision 2: Bone Conduction Transducer Selection

**Decision:** BH01B modules from AliExpress

**Date:** 2026-03-28  
**Status:** ✅ DECIDED

### Rationale
| Option | Pros | Cons | Cost |
|--------|------|------|------|
| BH01B | Available, tested reviews | Long ship time | ~$18/unit |
| Radioear B81 | Clinical quality | $300/unit - too expensive | $300/unit |
| AfterShokz Salvage | Known good quality | Need broken units | ~$15/unit |
| Generic 8mm AliExpress | Cheapest | Unknown quality | ~$8/unit |

**Selected:** BH01B - Best balance of cost and known performance

**Risk Mitigation:** Order 3 units (1 spare) due to long shipping time

**Order Details:**
- Qty: 3
- Supplier: AliExpress (search "BH01B bone conduction")
- Est. Unit Cost: $18.00
- Shipping: ~14 days from China
- **Action:** ☐ PLACE ORDER

---

## Decision 3: MCU Selection - Dev Board vs. Bare Chip

**Decision:** STM32 Nucleo-L476RG dev boards for prototype

**Date:** 2026-03-28  
**Status:** ✅ DECIDED

### Rationale
| Approach | Pros | Cons |
|----------|------|------|
| Nucleo Dev Board | Quick start, debug features, Arduino headers | Larger form factor |
| Bare STM32L476 | Compact, final form factor | Need debugger, more setup |

**Selected:** Nucleo-L476RG for Cycle 1 prototyping

**Future:** Custom PCB with bare STM32L4 in Cycle 2/3

**Order Details:**
- Qty: 2
- Supplier: Digi-Key
- Part #: NUCLEO-L476RG
- Unit Cost: $14.46
- Lead Time: In stock
- **Action:** ☐ PLACE ORDER

---

## Decision 4: EMG Amplifier Selection

**Decision:** INA333 as primary, AD8429 as upgrade option

**Date:** 2026-03-28  
**Status:** ✅ DECIDED

### Rationale
| Part | Input Noise | Cost | Availability |
|------|-------------|------|--------------|
| INA333 | 50nV/√Hz | $2.85 | Digi-Key in stock |
| AD8429 | 1nV/√Hz | $12.50 | Digi-Key in stock |

**Selected:** INA333 for initial build (INA333 sufficient for prototype)

**Rationale:** Success probability (0.85) suggests standard components adequate. AD8429 reserved for Cycle 2 if noise performance insufficient.

**Order Details:**
- Qty: 4 (2 per unit + spares)
- Supplier: Digi-Key
- Part #: INA333AIDGKR
- Unit Cost: $2.85
- **Action:** ☐ PLACE ORDER

---

## Decision 5: Battery Selection

**Decision:** Samsung 30Q 18650 protected cells

**Date:** 2026-03-28  
**Status:** ✅ DECIDED

### Rationale
| Option | Capacity | Cost | Notes |
|--------|----------|------|-------|
| Samsung 30Q | 3000mAh | $6.50 | High drain, reliable |
| LG MJ1 | 3500mAh | $6.50 | Higher capacity |
| LiPo Pouch | Varies | $5-10 | Compact, needs holder |

**Selected:** Samsung 30Q for balance of capacity and high-drain capability

**Safety Note:** Verify protected cells or add protection PCB

**Order Details:**
- Qty: 2
- Supplier: Li-Ion Wholesale (or reputable vape battery supplier)
- Unit Cost: $6.50
- **Action:** ☐ PLACE ORDER

---

## Decision 6: PCB Strategy

**Decision:** Breadboard first, custom PCB only if required for Cycle 2

**Date:** 2026-03-28  
**Status:** ✅ DECIDED

### Rationale
| Approach | When | Reason |
|----------|------|--------|
| Breadboard | Cycle 1 | Rapid iteration, no fab delay |
| Custom PCB | Cycle 2/3 | Miniaturization, robustness |

**Cycle 1:** Dev boards + breadboards + breakout modules

**Savings:** ~$60 (skip PCB fab for Cycle 1)

---

## Pending Decisions

### Decision P1: Coil Winding vs. Off-the-Shelf
**Status:** ⏳ PENDING - Awaiting inductor specs

Options:
- Custom wind 10µH on ferrite core
- Buy Coilcraft 10µH RF inductor

**Decision Needed:** By Day 5 (NFMI integration)

---

### Decision P2: Enclosure Form Factor
**Status:** ⏳ PENDING - Awaiting breadboard sizing

Options:
- Neck-worn pendant (180mm x 25mm)
- Wrist-worn band (larger components)
- Belt clip module

**Decision Needed:** By Day 6 (mechanical constraints known)

---

## Procurement Checklist

### Priority 1 (Order by Day 1 - Mar 28)
| Item | Qty | Supplier | Est. Cost | Ordered | ETA |
|------|-----|----------|-----------|---------|-----|
| AS3932 | 2 | Digi-Key | $16.90 | ☐ | 7 days |
| AS3933 | 2 | Digi-Key | $13.78 | ☐ | 7 days |
| Bone Conduction (BH01B) | 3 | AliExpress | $54.00 | ☐ | 14 days |
| 10µH RF Inductors | 4 | Coilcraft | $6.00 | ☐ | 5 days |
| **P1 Subtotal** | | | **$90.68** | | |

### Priority 2 (Order by Day 2 - Mar 29)
| Item | Qty | Supplier | Est. Cost | Ordered | ETA |
|------|-----|----------|-----------|---------|-----|
| Nucleo-L476RG | 2 | Digi-Key | $28.92 | ☐ | In stock |
| INA333 | 4 | Digi-Key | $11.40 | ☐ | In stock |
| OPA2333 | 4 | Digi-Key | $8.40 | ☐ | In stock |
| TPA2005D1 | 2 | Digi-Key | $2.50 | ☐ | In stock |
| TP4056 | 2 | Digi-Key | $2.20 | ☐ | In stock |
| TPS63001 | 2 | Digi-Key | $6.90 | ☐ | In stock |
| **P2 Subtotal** | | | **$60.32** | | |

### Priority 3 (Order by Day 3 - Mar 30)
| Item | Qty | Supplier | Est. Cost | Ordered | ETA |
|------|-----|----------|-----------|---------|-----|
| Resistor Kit (0603/0805) | 1 | Amazon | $8.00 | ☐ | 2 days |
| Capacitor Kit (0603/0805) | 1 | Amazon | $10.00 | ☐ | 2 days |
| Ag/AgCl Electrodes | 20-pack | Amazon | $10.00 | ☐ | 2 days |
| USB-C Connectors | 4 | Digi-Key | $3.40 | ☐ | In stock |
| JST-PH 2.0 | 10 | Digi-Key | $2.50 | ☐ | In stock |
| 18650 Holders | 2 | Amazon | $3.00 | ☐ | 2 days |
| Samsung 30Q 18650 | 2 | Li-Ion Wholesale | $13.00 | ☐ | 5 days |
| PETG Filament | 1kg | Amazon | $25.00 | ☐ | 2 days |
| Conductive Gel | 1 tube | Amazon | $5.00 | ☐ | 2 days |
| **P3 Subtotal** | | | **$79.90** | | |

### Total Est. Cost: $230.90

**Note:** Difference from $330 BOM includes test equipment and contingency.

---

## Supplier Alternatives

If primary suppliers out of stock:

| Component | Primary | Alt 1 | Alt 2 |
|-----------|---------|-------|-------|
| AS3932/AS3933 | Digi-Key | Mouser | ams-OSRAM Direct |
| STM32 Nucleo | Digi-Key | Mouser | ST Direct |
| Passives | Digi-Key | Mouser | Amazon kit |
| Bone Conduction | AliExpress | Amazon | AfterShokz parts |
| Batteries | Li-Ion Wholesale | 18650BatteryStore | Local vape shop |

---

## Risk Factors

| Risk | Likelihood | Mitigation |
|------|------------|------------|
| AS3932 supply shortage | Low | Order AS3933 backup |
| China shipping delays (BH01B) | Medium | Order early; consider Amazon alternative at 2x cost |
| Battery shipping restrictions | Low | Order from domestic supplier |
| Component EOL | Very Low | All components currently in production |

---

*Document: PROCUREMENT_DECISIONS.md*  
*Project: SILENT-001*  
*Part of META-SYSTEM autonomous research consortium*
