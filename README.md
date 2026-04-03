# Murim Chronicles Lite

A Crusader Kings III mod bringing cultivation mechanics and martial arts to the medieval world.

**Version:** 0.1.0  
**Supported CK3:** 1.18.*  
**Tags:** Gameplay, Historical

## Features

- Cultivation realm progression (Qi Condensation → Foundation → Golden Core → Nascent Soul → Spirit Severing)
- Martial arts techniques and sect alignment traits
- Murim dueling character interaction
- Yearly cultivation pulse on-action
- Way of Champions GUI compatibility patch
- Full English localization

## Structure

```
mod/murim_lite/
├── descriptor.mod
├── murim_lite.mod
├── common/
│   ├── character_interactions/murim_duel_interaction.txt
│   ├── decisions/murim_core_decisions.txt
│   ├── decisions/murim_sect_decisions.txt
│   ├── modifiers/murim_core_modifiers.txt
│   ├── on_actions/murim_yearly_pulse.txt
│   ├── scripted_effects/murim_core_effects.txt
│   ├── scripted_guis/zz_murim_woc_gui_patch.txt
│   ├── scripted_triggers/murim_core_triggers.txt
│   ├── traits/murim_cultivation_traits.txt
│   └── traits/murim_sect_alignment_traits.txt
├── events/murim_lite_events.txt
├── gfx/interface/
│   ├── icons/traits/        (cultivation & technique icons)
│   └── illustrations/decisions/
└── localization/english/murim_lite_l_english.yml
```

## Installation

1. Copy the `murim_lite/` folder into your CK3 mod directory:
   - Windows: `%USERPROFILE%\Documents\Paradox Interactive\Crusader Kings III\mod\`
   - Linux: `~/.local/share/Paradox Interactive/Crusader Kings III/mod/`
2. Enable **Murim Chronicles Lite** in the CK3 launcher.

## Compatibility

Includes `zz_murim_woc_gui_patch.txt` for compatibility with **Way of Champions**.
