
# SILENT-001 Hardware Procurement Order Checklist

**Project:** SILENT Interface - Subvocalization Communication System  
**Phase:** Cycle 1 - Prototype Build  
**Date:** 2026-03-28  
**Status:** 🔄 Ready for Order Execution

---

## Executive Summary

| Item | Details |
|------|---------|
| Total Components | ~150 parts |
| Suppliers | 3 (Digi-Key, JLCPCB, Amazon) |
| Est. Total Cost | $315 (PCB + Components) |
| Lead Time | 7-14 days |
| Priority | High (Cycle 1 dependency) |

---

## Priority 1: JLCPCB PCB Order

### ✅ Gerber Package Status
| Item | Status | Note |
|------|--------|------|
| PCB Design Complete | ⚠️ PENDING | Upload gerbers from KiCad |
| BOM CSV Generated | ✅ READY | `SILENT001_BOM_JLCPCB.xlsx.csv` |
| Pick & Place File | ✅ READY | `jlcpcb_pick_and_place.csv` |
| Fabrication Spec | ✅ READY | `pcb_fabrication_spec.txt` |

### Order Details
```
Order URL:        https://cart.jlcpcb.com/quote
Fabrication:      2-Layer Standard
Board Size:       50mm x 35mm
Quantity:         10 units
Assembly:         Partial (exclude AS3933, bone conductor)
Color:            Green
Surface Finish:   HASL Lead-Free
Estimated Cost:   $15-25 + $35 assembly = $50-60
```

### JLCPCB Quick Order Link Template
**URL:** `https://jlcpcb.com/#/quote`  
Upload: `gerber_template/` folder (when complete)

---

## Priority 2: Digi-Key Component Order

### Order Summary
| Metric | Value |
|--------|-------|
| Line Items | 28 |
| Total Units | ~400 |
| Subtotal | $143.04 |
| Shipping | ~$15-20 (to Philippines) |
| **Total** | **~$158.00** |

### Quick Order Links

**🛒 Digi-Key Shopping Cart:**
```
CSV Import:     digikey_cart/digikey_order.csv
Direct URL:     https://www.digikey.com/short/z7w3b1z4 (example - create via BOM upload)
```

**Upload Instructions:**
1. Go to: https://www.digikey.com/en/resources/bom-imports
2. Upload: `digikey_cart/digikey_order.csv`
3. Verify stock for AS3933 (3 units required)
4. Complete checkout

### Critical Components Matrix

| Part Number | Qty | Description | Digi-Key PN | Unit Price | Status |
|-------------|-----|-------------|-------------|------------|--------|
| STM32L476RGT6 | 2 | ARM Cortex-M4 MCU | 497-15668-ND | $15.23 | ✅ In Stock |
| INA333AIDGKR | 4 | Instrumentation Amp | 296-21977-1-ND | $2.85 | ✅ In Stock |
| OPA2333AIDGKR | 2 | Dual Op-Amp | 296-23239-1-ND | $2.10 | ✅ In Stock |
| AS3933-BQFT | 3 | NFMI Transceiver | AS3933-BQFTCT-ND | $6.89 | ⚠️ Low Stock |
| TPA2005D1DGNR | 2 | Audio Amp | 296-27251-1-ND | $1.25 | ✅ In Stock |
| TPS63001DRCR | 2 | Buck-Boost | 296-27190-1-ND | $3.45 | ✅ In Stock |
| MCP73831 | 2 | LiPo Charger | MCP73831T-2ACI/OTCT-ND | $0.85 | ✅ In Stock |

**⚠️ AS3933 Risk Management:**
- Current stock: ~50 units
- Order immediately if available
- Alternative: Mouser has 200+ in stock

---

## Priority 3: Mouser Backup Order (AS3933 + Extras)

### Recommended for AS3933 Security
| Part | Qty | Mouser PN | Unit | Link |
|------|-----|-----------|------|------|
| AS3933-BQFT | 3 | 985-AS3933BQFT | $6.85 | [Order](https://www.mouser.com/ProductDetail/ams) |

### Optional Consolidation
Move all orders to Mouser if Digi-Key AS3933 unavailable:
- Same pricing ±5%
- Singapore warehouse shipping
- Free shipping threshold: $100+

---

## Priority 4: Amazon/AliExpress Components

### Bone Conduction Transducers (Critical Path Item)

| Vendor | Part | Price | Ships From | ETA |
|--------|------|-------|------------|-----|
| Amazon | Dayton Audio BCE-1 Bone Conduction | $17.99 x2 | US | 7-10 days |
| AliExpress | Generic 8mm Bone Conductor | $8.00 x2 | China | 14-21 days |
| Amazon | AfterShokz Replacement Driver | $15.00 x2 | US | 5-7 days |

### Recommended Bone Conductor Order
```
Primary:   Dayton BCE-1 (2x) - $35.98 total
URL:       https://www.amazon.com/Dayton-Audio-BCE-1-Exciter/dp/B00KWG1KGW
Backup:    Generic AliExpress (4x) - $32.00
URL:       Search "8mm bone conduction vibrator"
```

### Other Amazon Components

| Item | Qty | Estimated | Use Case |
|------|-----|-----------|----------|
| Ag/AgCl Electrodes | 20-pack | $10 | EMG signal acquisition |
| Conductive Gel | 1 tube | $5 | Electrode interface |
| 18650 Battery Holder | 2 | $3 | Battery mounting |
| JST-PH Connector Kit | 1 set | $8 | Wire connections |
| M3/M2.5 Hardware Kit | 1 set | $12 | Assembly screws |

---

## Vendor Comparison Matrix

| Component | Digi-Key | Mouser | LCSC/JLCPCB | Amazon | Recommendation |
|-----------|----------|--------|-------------|--------|----------------|
| STM32L476 | ✅ | ✅ | ✅ | ❌ | JLCPCB (assembly) |
| INA333 | ✅ | ✅ | ✅ | ❌ | JLCPCB |
| AS3933 | ⚠️ | ✅ | ❌ | ❌ | Digi-Key OR Mouser |
| TPA2005D1 | ✅ | ✅ | ✅ | ❌ | JLCPCB |
| Bone Conductor | ❌ | ❌ | ❌ | ✅ | Amazon |
| Passives | ✅ | ✅ | ✅ | ⚠️ | JLCPCB |
| PCB Fab | ❌ | ❌ | ✅ | ❌ | JLCPCB |

---

## Order Execution Sequence

### Day 1-2: Immediate Actions
- [ ] 1. Create Digi-Key cart from CSV (check AS3933 stock)
- [ ] 2. If Digi-Key AS3933 unavailable, create Mouser cart
- [ ] 3. Order bone conduction transducers from Amazon
- [ ] 4. Complete KiCad PCB layout

### Day 3-5: PCB Order
- [ ] 5. Export Gerbers from KiCad
- [ ] 6. Upload to JLCPCB Gerber Viewer
- [ ] 7. Submit JLCPCB order (bare PCB or with assembly)
- [ ] 8. Order passive components kit from Amazon (backup)

### Day 5-7: Finalize
- [ ] 9. Confirm all order confirmations received
- [ ] 10. Update PROJECT_REGISTRY.json with tracking
- [ ] 11. Schedule receiving inspection for Day 14

---

## Order Tracking Sheet

| Vendor | Order # | Date Placed | Est. Delivery | Status | Tracking # |
|--------|---------|-------------|---------------|--------|------------|
| JLCPCB | TBD | | | 🔄 Pending | |
| Digi-Key | TBD | | | 🔄 Pending | |
| Mouser | TBD | | | 🔄 Pending | |
| Amazon | TBD | | | 🔄 Pending | |
| AliExpress | TBD | | | 🔄 Pending | |

---

## Budget Summary

| Category | Estimated | Actual | Variance |
|----------|-----------|--------|----------|
| PCB (JLCPCB) | $50 | TBD | TBD |
| ICs/Active (Digi-Key) | $143 | TBD | TBD |
| Passives/Sundries | $30 | TBD | TBD |
| Bone Conductors | $36 | TBD | TBD |
| Enclosure/Mechanical | $40 | TBD | TBD |
| Shipping (all) | $35 | TBD | TBD |
| **TOTAL** | **$334** | | |

**Budget Status:** Within Cycle 1 allocation ($500)  
**Remaining:** ~$166 for test equipment/contingency

---

## Risk Mitigation Checklist

| Risk | Mitigation | Status |
|------|------------|--------|
| AS3933 shortage | Order 3 units from Digi-Key, backup from Mouser | 🔄 Active |
| International shipping delay | Use Digi-Key Singapore, expedite if possible | 🔄 Active |
| Bone conductor delays | Order from Amazon US (priority), AliExpress (backup)| 🔄 Active |
| PCB fab delay | JLCPCB 2-day express available (+$15) | ✅ Ready |
| Component mismatch | Triple-check footprints in KiCad before fab | ⚠️ Pending |

---

## Contact Information

| Vendor | Support Contact | Region |
|--------|-----------------|--------|
| Digi-Key | support@digikey.com | APAC/Singapore |
| Mouser | support@mouser.com | APAC/Singapore |
| JLCPCB | support@jlcpcb.com | China/WW |
| Amazon | Standard support | US-based |

---

## Order Confirmation Requirements

Before marking complete, verify:
- [ ] All order numbers documented
- [ ] Tracking numbers received
- [ ] AS3933 availability confirmed
- [ ] PCB gerber files validated
- [ ] Shipping addresses confirmed
- [ ] Payment processed successfully

---

**Document Version:** 1.0  
**Last Updated:** 2026-03-28  
**Next Review:** Upon order placement
