# Asset Integration Guide

This document provides guidance for integrating the acquired 3D assets (player and enemy models with animations) into the Wave Survival project.

## Overview

The project expects specific folder structures and naming conventions to properly integrate with the C++ framework.

## Directory Structure

All game assets should be placed in the `Content/` directory:

```
Content/
├── Characters/              # Player character assets
│   ├── Arkhrin/            # Rogue character
│   ├── EliteTrooper/       # Assault character
│   ├── Formiops/           # Ranged Specialist character
│   ├── Gastarian/          # Heavy Gunner character
│   ├── MallearianKnight/   # Staff Combat character
│   └── Juggernaut/         # Medic character
├── Enemies/                 # Enemy assets
│   ├── Rapax/              # Aalix (Standard Grunt)
│   ├── Arack/              # Krals (Spider)
│   ├── Anglerox/           # Xurkex (Tank Death)
│   ├── Onyscidus/          # Crelmor (Tank Armored)
│   ├── Ceratoferox/        # Gid (Fast Glass Cannon)
│   ├── Ingenalvus/         # Vruuknuix (The Queen Boss)
│   └── Titanockarce/       # Saarlae (The Squid Boss)
├── Weapons/                 # Weapon assets
│   ├── Shotgun/
│   ├── AssaultPistol/
│   ├── Rifle/
│   ├── GatlingGun/
│   ├── Staff/
│   └── LightningGun/
├── VFX/                     # Visual effects
│   ├── Muzzle/
│   ├── Impact/
│   ├── Abilities/
│   └── Elemental/
├── UI/                      # User interface
│   ├── Widgets/
│   ├── Icons/
│   └── Fonts/
└── Maps/                    # Game levels
    ├── MainMenu
    └── TestArena
```

## Player Character Integration

### Expected Assets per Character

For each character folder (e.g., `Content/Characters/Arkhrin/`):

```
Arkhrin/
├── Meshes/
│   └── SK_Arkhrin.uasset              # Skeletal mesh
├── Animations/
│   ├── Idle/
│   │   └── Anim_Idle.uasset
│   ├── Movement/
│   │   ├── Anim_Walk.uasset
│   │   ├── Anim_Run.uasset
│   │   └── Anim_Sprint.uasset
│   ├── Combat/
│   │   ├── Anim_Fire.uasset
│   │   ├── Anim_Reload.uasset
│   │   └── Anim_Melee.uasset
│   ├── Abilities/
│   │   ├── Anim_Ability1.uasset
│   │   ├── Anim_Ability2.uasset
│   │   └── Anim_Ultimate.uasset
│   └── Death/
│       └── Anim_Death.uasset
├── Materials/
│   └── M_Arkhrin.uasset
├── Textures/
│   ├── T_Arkhrin_Diffuse.uasset
│   ├── T_Arkhrin_Normal.uasset
│   ├── T_Arkhrin_Roughness.uasset
│   └── T_Arkhrin_Metallic.uasset
└── Blueprints/
    └── BP_Arkhrin.uasset              # Character Blueprint
```

### Character Blueprint Setup

1. **Create Character Blueprint**
   - Right-click in `Content/Characters/Arkhrin/Blueprints/`
   - Create Blueprint Class
   - Parent Class: `WSCharacterBase`
   - Name: `BP_Arkhrin`

2. **Configure Character**
   - Set `Character Class` = `Rogue`
   - Assign skeletal mesh to Character Mesh component
   - Set materials on mesh
   - Configure collision capsule size

3. **Animation Setup**
   - Create Animation Blueprint based on skeletal mesh
   - Set up state machines for: Idle, Movement, Combat, Abilities
   - Configure blend spaces for directional movement
   - Link to character blueprint

4. **Ability Configuration**
   - Set `Ability1Cooldown` = 10.0 (or character-specific value)
   - Set `Ability2Cooldown` = 15.0
   - Set `UltimateCooldown` = 60.0
   - Assign `WeaponClass` to appropriate weapon blueprint

5. **Implement Ability Logic**
   - Override `UseAbility1` event in Blueprint
   - Override `UseAbility2` event in Blueprint
   - Override `UseUltimate` event in Blueprint
   - Add VFX and SFX for each ability

### Required Sockets

Each character skeletal mesh should have these sockets:

- `WeaponSocket` - Right hand for weapon attachment
- `Head` - For headshot detection
- `Spine` - For hit reactions

## Enemy Integration

### Expected Assets per Enemy

For each enemy folder (e.g., `Content/Enemies/Rapax/`):

```
Rapax/
├── Meshes/
│   └── SK_Rapax.uasset
├── Animations/
│   ├── Anim_Idle.uasset
│   ├── Anim_Walk.uasset
│   ├── Anim_Run.uasset
│   ├── Anim_Attack.uasset
│   └── Anim_Death.uasset
├── Materials/
│   ├── M_Rapax_Normal.uasset       # Normal variant
│   ├── M_Rapax_Fire.uasset         # Fire elemental
│   ├── M_Rapax_Ice.uasset          # Ice elemental
│   ├── M_Rapax_Acid.uasset         # Acid elemental
│   ├── M_Rapax_Electric.uasset     # Electric elemental
│   └── M_Rapax_Poison.uasset       # Poison elemental
└── Blueprints/
    └── BP_Rapax.uasset
```

### Enemy Blueprint Setup

1. **Create Enemy Blueprint**
   - Parent Class: `WSEnemyBase`
   - Name: `BP_[EnemyName]`

2. **Configure Enemy Stats**
   - Set `EnemyType` to appropriate enum value
   - Configure `EnemyStats`:
     - `MaxHealth` - Based on enemy type
     - `BaseDamage` - Attack damage
     - `MovementSpeed` - Movement speed
     - `Armor` - Damage reduction (0-1)
     - `bCanCrit` - Can receive critical hits (false for Crelmor)
     - `ElementalType` - Elemental variant
     - `CurrencyDropAmount` - Currency reward

3. **AI Controller**
   - Create AI Controller Blueprint
   - Implement behavior trees for:
     - Moving to target
     - Attacking when in range
     - Special behaviors (Gid targeting last damager)

4. **Special Behaviors**

   **For Xurkex (Tank Death):**
   - Override `OnDeath` function
   - Add explosion VFX
   - Create sphere damage area on death

   **For Krals (Spider):**
   - Implement ranged attack
   - Spawn web projectiles
   - Create ground hazard zones

   **For Crelmor (Armored):**
   - Set `bCanCrit` = false
   - Increase armor value

   **For Gid (Fast Glass Cannon):**
   - Implement custom targeting logic
   - Override `FindNearestPlayer` to target last damager

### Elemental Variants

Create material instances for each elemental type with distinct colors:
- **Fire**: Red/Orange glow
- **Ice**: Blue/Cyan frost
- **Acid**: Green corrosive
- **Electric**: Yellow/Purple crackling
- **Poison**: Dark green/Purple miasma

## Boss Integration

### Boss-Specific Requirements

Bosses need additional phase management:

1. **Multi-Phase Health**
   - Divide health into phases (typically 3)
   - Trigger phase changes at health thresholds

2. **Phase Behaviors**
   - Create separate behavior trees per phase
   - Switch AI logic on phase transition

3. **Boss Vruuknuix (The Queen)**

   **Phase 1 Assets:**
   - Standard attack animations
   - Movement animations

   **Phase 2 Assets:**
   - Body shake animation for spawning
   - Queen Larvae spawn effect

   **Phase 3 Assets:**
   - Roar animation
   - Shield VFX
   - Ground stomp animation
   - Shockwave VFX

   **Implementation:**
   ```
   Blueprint Events:
   - OnPhase1Start
   - OnPhase2Start (enable larvae spawning)
   - OnPhase3Start (enable shield, continuous spawning, shockwave)
   - SpawnQueenLarvae (periodic timer)
   - DoShockwave (periodic attack)
   ```

4. **Boss Saarlae (The Squid)**

   **Phase 1 Assets:**
   - Tentacle attack animations (individual)
   - Main body idle

   **Phase 2 Assets:**
   - Tentacles emerging from ground
   - Laser charging animation
   - Laser beam VFX

   **Phase 3 Assets:**
   - Shield VFX
   - Continuous laser animations
   - Tentacle shield positions

## Weapon Integration

### Weapon Blueprint Setup

1. **Create Weapon Blueprint**
   - Parent Class: `WSWeaponBase`
   - Name: `BP_[WeaponName]`

2. **Configure Weapon Stats**
   - `WeaponType` - Enum value
   - `FireMode` - Hitscan/Projectile/Melee
   - `BaseDamage` - Base damage per shot
   - `FireRate` - Time between shots
   - `MagazineSize` - Ammo capacity
   - `ReloadTime` - Reload duration
   - `Range` - Maximum effective range
   - `bAutomatic` - Full auto or semi-auto

3. **Weapon Mesh**
   - Assign static or skeletal mesh
   - Configure attachment socket
   - Add muzzle socket for VFX

4. **Character-Specific Weapons**

   **Shotgun (Arkhrin):**
   - Multiple pellets per shot
   - Damage falloff with distance
   - Wide spread pattern

   **Assault Pistol (Elite Trooper):**
   - Fast fire rate
   - Medium damage
   - Good accuracy

   **Rifle (Formiops):**
   - High damage per shot
   - Slower fire rate
   - Excellent range and accuracy

   **Gatling Gun (Gastarian):**
   - Ramp-up mechanic (increase fire rate over time)
   - High magazine size
   - Movement penalty while firing

   **Staff (Mallearian Knight):**
   - Melee range
   - Multiple combo attacks
   - Knockback effects

   **Lightning Gun (Juggernaut):**
   - Chain lightning between enemies
   - Healing beam to allies
   - Continuous beam

## VFX Integration

### Required Visual Effects

1. **Weapon Effects**
   - Muzzle flashes (per weapon type)
   - Bullet tracers (hitscan)
   - Impact sparks
   - Shell casings

2. **Ability Effects**
   - Ability 1 VFX per character
   - Ability 2 VFX per character
   - Ultimate VFX per character
   - Cooldown indicators

3. **Elemental Effects**
   - Fire: Burning particles, ember trails
   - Ice: Frost crystals, slow effect
   - Acid: Corrosive bubbles, dissolve
   - Electric: Lightning arcs, chain effects
   - Poison: Gas clouds, dripping

4. **Death Effects**
   - Enemy death dissolve
   - Xurkex explosion
   - Queen Larvae explosion
   - Boss death sequences

5. **Environmental**
   - UFO beam lights (spawn indicators)
   - Respawn drop beam
   - Web hazard zones
   - Shockwave rings

## UI Integration

### HUD Widget Setup

Create widget blueprint: `WBP_HUD`

**Required Elements:**
- Health bar (current/max with color coding)
- Ammo counter (current/magazine)
- Currency display
- Wave number and progress
- Ability cooldown indicators (circular or bar)
- Ultimate charge meter
- Teammate status indicators
- Crosshair

### Shop Widget Setup

Create widget blueprint: `WBP_Shop`

**Required Elements:**
- Large card display (center)
- Card name and description
- Card cost
- Card rarity indicator (color coding)
- Buy button
- Destroy button
- Hold button (close)
- Shop timer countdown
- Current currency display

### Additional UI Widgets

- `WBP_DeathScreen` - Downed overlay with revive timer
- `WBP_VictoryScreen` - Win condition display
- `WBP_GameOverScreen` - Loss condition display
- `WBP_MainMenu` - Title screen and mode selection
- `WBP_LeaderboardEntry` - Single leaderboard entry
- `WBP_Leaderboard` - Full leaderboard display

## Audio Integration

### Required Sound Assets

1. **Weapon Sounds**
   - Fire sounds (per weapon)
   - Reload sounds
   - Empty click
   - Shell casings

2. **Character Sounds**
   - Footsteps
   - Jump/land
   - Ability activation
   - Damage grunts
   - Death sounds

3. **Enemy Sounds**
   - Ambient growls
   - Attack sounds
   - Death sounds
   - Special ability sounds

4. **UI Sounds**
   - Button clicks
   - Card purchase
   - Card destroy
   - Currency gain
   - Level up

5. **Music**
   - Main menu theme
   - Combat music (dynamic layers)
   - Shop/intermission music (calmer)
   - Boss music
   - Victory theme
   - Defeat theme

### Audio Cue Setup

Create Audio Cues in `Content/Audio/` for:
- Layered music system (combat intensity)
- 3D positioned enemy sounds
- Character voice lines (abilities, status)
- Ambient horror atmosphere

## Map Creation

### Test Arena Map

Create `TestArena` map for initial testing:

1. **Basic Layout**
   - Large open area (approx 100m x 100m)
   - Some cover objects
   - Spawn points around perimeter
   - Player start location at center

2. **Lighting**
   - Directional light (sun)
   - Point lights for atmosphere
   - Post-process volume for mood

3. **Nav Mesh**
   - Add Nav Mesh Bounds Volume
   - Configure for enemy AI navigation
   - Test pathfinding

4. **Spawn Points**
   - Place spawn point actors around map
   - Link to Game Mode spawn system

### Main Menu Map

Create simple menu environment or static background

## Testing Checklist

After asset integration:

- [ ] All 6 player characters load and animate correctly
- [ ] All 5 basic enemies spawn and move
- [ ] Both bosses load with correct phases
- [ ] All weapons fire and deal damage
- [ ] Elemental variants display correctly
- [ ] VFX play on actions (shoot, ability, death)
- [ ] UI displays all required information
- [ ] Audio plays for actions
- [ ] Shop widget functions (buy/destroy/hold)
- [ ] Wave progression works
- [ ] Multiplayer spawning works

## Common Issues and Solutions

### Issue: Character doesn't animate
**Solution**: Check Animation Blueprint is assigned and state machine is set up correctly

### Issue: Weapon doesn't attach to character
**Solution**: Verify WeaponSocket exists on character skeleton and is in correct location

### Issue: Enemy doesn't move
**Solution**: Ensure Nav Mesh is built and AI Controller is assigned

### Issue: VFX doesn't spawn
**Solution**: Check particle system is valid and spawn location is correct

### Issue: Boss phases don't trigger
**Solution**: Verify health thresholds and phase transition logic

## Performance Considerations

1. **LODs**: Create LOD levels for all meshes (3-5 LODs recommended)
2. **Materials**: Use material instances instead of unique materials
3. **Particles**: Limit max particle counts, use GPU particles where possible
4. **Enemies**: Pool enemy actors instead of spawning/destroying
5. **Audio**: Use sound attenuation and don't play too many sounds simultaneously

## Additional Resources

- Unreal Engine Documentation: https://docs.unrealengine.com
- Animation Retargeting Guide: For sharing animations between characters
- Material Creation Guide: For elemental variants
- Niagara VFX Guide: For creating particle effects
- UMG UI Designer Guide: For creating widgets

---

This guide should be updated as new assets are integrated and new requirements are discovered during development.
