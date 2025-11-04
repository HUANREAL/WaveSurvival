# Wave Survival - Co-op Sci-Fi Horror Game MVP

A semi-realistic sci-fi horror co-op shooter inspired by COD Zombies. Survive escalating waves of alien enemies, purchase randomized upgrades between waves, and either beat a 10-wave campaign or push as far as possible in infinite Survival mode.

## Overview

Wave Survival is a 1-4 player cooperative first-person shooter built in Unreal Engine 5.3. Players select from 6 unique character classes, each with signature weapons and abilities, to fight through waves of increasingly difficult alien enemies.

## Game Modes

### Main Mode (10 Waves)
- Waves 1-9: Progressive difficulty with predetermined enemy counts (250 per player base)
- Wave 10: Final boss encounter
- Victory condition: Defeat the final boss
- Player-selected difficulty before starting

### Survival Mode (Endless)
- Infinite waves with continuously scaling difficulty
- No win condition - survive as long as possible
- Leaderboard tracks highest wave reached
- Starts easier, ramps to extreme challenge

## Core Gameplay Loop

1. **Fight**: Clear all enemies in the current wave
2. **Shop**: Enter timed inter-wave phase (45 seconds), draw and purchase upgrade cards
3. **Repeat**: Progress to next wave with increased intensity

## Player Characters

### 1. Arkhrin (Rogue)
- **Weapon**: Shotgun (damage increases at close range)
- **Passive**: Backstabs always critical
- **Ability 1**: Ground slam to launch into air
- **Ability 2**: Temporary invisibility
- **Ultimate**: All shots become headshots + life steal

### 2. Elite Trooper (Assault)
- **Weapon**: Assault Pistol
- **Passive**: KOs reload weapon
- **Ability 1**: Piercing ammo + increased fire rate
- **Ability 2**: Triple roll with frag grenades
- **Ultimate**: Spawn 2 AI copies

### 3. Formiops (Ranged Specialist)
- **Weapon**: Rifle
- **Passive**: Headshot KOs reduce ability cooldowns
- **Ability 1**: Area slow field
- **Ability 2**: Teleportation pad
- **Ultimate**: Piercing laser beam shots

### 4. Gastarian (Heavy Gunner)
- **Weapon**: Gatling Gun (ramp-up mechanics)
- **Passive**: Damage increases with sustained fire
- **Ability 1**: Instant ramp-up + shield
- **Ability 2**: Dash through enemies
- **Ultimate**: Infinite ammo + shields

### 5. Mallearian Knight (Staff Combat)
- **Weapon**: Staff (melee)
- **Passive**: Multiple jumps
- **Ability 1**: Attack speed + health regen
- **Ability 2**: Launch and slam with stuns
- **Ultimate**: Teleport heal + no cooldowns

### 6. Juggernaut (Medic)
- **Weapon**: Lightning Gun (bounces between enemies, heals allies)
- **Passive**: Shots heal nearby allies
- **Ability 1**: AoE healing aura
- **Ability 2**: Dash with damage
- **Ultimate**: Lightning storm (damages enemies, heals allies)

## Enemy Types

### Basic Enemies

#### Aalix (Standard Grunt - Rapax)
- Health: Medium
- Damage: Low
- Speed: Standard
- Most common enemy type

#### Krals (Spider - Arack)
- Health: Low
- Damage: Low ranged
- Special: Spits elemental webs creating damage zones
- Speed: Standard

#### Xurkex (Tank Death - Anglerox)
- Health: Medium
- Damage: Medium
- Special: Explodes on death (AoE damage)
- Speed: Standard

#### Crelmor (Tank Armored - Onyscidus)
- Health: High
- Damage: Low
- Special: Immune to critical hits
- Speed: Slow

#### Gid (Fast Glass Cannon - Ceratoferox)
- Health: Low
- Damage: Very High
- Special: Targets last damager (or closest at full HP)
- Speed: Very High

### Bosses

#### Vruuknuix (The Queen - Ingenalvus)
- **Phase 1**: Standard attacks, targets highest damage dealer
- **Phase 2**: Spawns larvae creatures periodically
- **Phase 3**: Shielded, constant spawns, shockwave attacks (jumpable)
- **Queen Larvae**: Fly randomly, then target nearest player, explode on impact

#### Saarlae (The Squid - Titanockarce)
- **Phase 1**: Tentacle attacks one at a time
- **Phase 2**: Tentacles from ground + overhead + laser attacks
- **Phase 3**: Shields with tentacles, continuous laser attacks
- **Laser Attack**: Damages and locks ultimate ability temporarily

## Upgrade Card System

### Card Types
- **Generic** (~33%): Available to all characters
- **Character-Specific** (~66%): Tailored to selected class
- **Legendary** (Low %): Powerful character-specific upgrades

### Shop Mechanics
- One card revealed at a time
- **Purchase**: Spend currency to apply upgrade (removed from deck)
- **Destroy**: Discard for free (removed from deck permanently)
- **Hold**: Leave for future purchase opportunity
- Deck reshuffles when empty (Survival mode)

### Generic Upgrades (All Stack)
- Damage increase
- Max health increase
- Ammo total increase
- Reload speed
- Damage reduction
- Health regen (lifesteal on damage)
- Cooldown reductions
- Currency gain increase
- Elemental damage
- Critical damage increase

### Character-Specific Examples

**Rogue**:
- More pellets per shot
- Movement speed during invisibility
- Ability 1 height increase
- Grenade on Ability 1
- Health regen during Ability 2
- Ammo restore chance during ultimate

**Assault**:
- Ability 1 fire rate increase
- Additional rolls
- Copies inherit elemental bonuses
- Copies inherit crit damage
- Copies explode on death

(Similar upgrades for other characters...)

## Death and Respawn

1. **Downed**: Player can be revived by teammates within time window
2. **Dead**: Cannot be revived, must respawn
3. **Respawn**: Drop from UFO into safest area (can aim/shoot while falling)
4. **Game Over**: All players dead simultaneously with no revive/respawn available

## Economy

- **Currency**: Earned from kills and wave completion
- **Spending**: Between-wave shop only
- **Scaling**: Currency gains can be upgraded

## Multiplayer

- **Platform**: PC (Steam) at launch
- **Players**: 1-4 players
- **Modes**: Local co-op and online multiplayer
- **Technology**: Epic Online Services
- **Scaling**: Enemy counts scale per active player
- **Anti-Cheat**: EOS Easy Anti-Cheat

## Technical Features

### Weapons
- Mostly hitscan firearms
- Reload management required
- Elemental damage variants
- Upgrade system for weapon stats

### AI Behavior
- Standard enemies: Target nearest player
- Bosses: Target highest damage dealer during run
- Gid exception: Targets last damager

### Spawning
- UFOs visible during shop phase indicate spawn locations
- Randomized spawn positions to reduce predictability

### Leaderboards
- Track highest wave in Survival mode
- Record character choices and metadata
- Verify run legitimacy

## Project Structure

```
WaveSurvival/
├── Config/                      # Engine and game configuration
│   ├── DefaultEngine.ini       # Engine settings, EOS configuration
│   ├── DefaultGame.ini         # Project settings
│   └── DefaultInput.ini        # Input bindings
├── Content/                     # Game assets (to be created)
│   ├── Characters/             # Player character blueprints
│   ├── Enemies/                # Enemy blueprints
│   ├── Weapons/                # Weapon blueprints
│   ├── UI/                     # User interface widgets
│   └── Maps/                   # Game levels
└── Source/WaveSurvival/        # C++ source code
    ├── Public/                 # Header files
    │   ├── WSTypes.h           # Game enums and structs
    │   ├── WSGameInstance.h    # Persistent game data
    │   ├── WSGameMode.h        # Game rules and spawning
    │   ├── WSGameState.h       # Match state tracking
    │   ├── WSPlayerState.h     # Individual player data
    │   ├── WSPlayerController.h # Player input and UI
    │   ├── WSCharacterBase.h   # Player character base
    │   ├── WSWeaponBase.h      # Weapon base class
    │   └── WSEnemyBase.h       # Enemy base class
    └── Private/                # Implementation files
        └── [corresponding .cpp files]
```

## Core Systems Implemented

### 1. Game Framework
- **Game Instance**: Handles online subsystem, leaderboards, persistent data
- **Game Mode**: Manages wave spawning, player respawn, game rules
- **Game State**: Tracks current wave, enemy counts, game phase
- **Player State**: Individual player stats, currency, upgrades, kills
- **Player Controller**: Input handling, shop UI, card deck management

### 2. Character System
- Base character class with movement, combat, abilities
- Cooldown management with upgrade modifiers
- Down/revive state handling
- Health regeneration system

### 3. Weapon System
- Hitscan, projectile, and melee support
- Damage calculation with crits and multipliers
- Elemental damage types
- Reload mechanics with speed modifiers
- Ammo management

### 4. Enemy System
- Base enemy with stats and elemental variants
- AI targeting (nearest player or highest damage dealer)
- Special death behaviors (explosions, etc.)
- Currency drops based on damage contribution

### 5. Upgrade System
- Card deck management with rarities
- Generic and character-specific upgrades
- Stack tracking and limits
- Stat modification system
- Deck reshuffling for endless mode

### 6. Wave Management
- Predetermined Main Mode configurations (Waves 1-10)
- Dynamic Survival Mode scaling
- Enemy composition varies by wave
- Boss waves on Wave 10 (Main) or every 10 waves (Survival)

## Building the Project

### Prerequisites
- Unreal Engine 5.3 or later
- Visual Studio 2022 (Windows) or Xcode (Mac)
- Epic Games account with EOS configuration

### Setup Steps

1. **Clone the repository**
   ```bash
   git clone https://github.com/HUANREAL/WaveSurvival.git
   cd WaveSurvival
   ```

2. **Generate project files**
   - Right-click `WaveSurvival.uproject`
   - Select "Generate Visual Studio project files"

3. **Build the project**
   - Open `WaveSurvival.sln` in Visual Studio
   - Set build configuration to "Development Editor"
   - Build solution (Ctrl+Shift+B)

4. **Configure Epic Online Services**
   - Create an application in EOS Developer Portal
   - Update `Config/DefaultEngine.ini` with your credentials
   - Configure EOS Anti-Cheat

5. **Open in Unreal Editor**
   - Launch `WaveSurvival.uproject`
   - Wait for initial shader compilation

## Content Creation Guide

### Creating Character Blueprints

1. Create Blueprint derived from `WSCharacterBase`
2. Set `CharacterClass` to appropriate enum value
3. Configure ability cooldowns
4. Assign weapon class
5. Implement character-specific abilities in Blueprint

### Creating Enemy Blueprints

1. Create Blueprint derived from `WSEnemyBase`
2. Set `EnemyType` to appropriate enum value
3. Configure `EnemyStats` (health, damage, speed, etc.)
4. Set elemental type for variants
5. Configure currency drop amount
6. Implement special behaviors (explosions, spawning, etc.)

### Creating Weapon Blueprints

1. Create Blueprint derived from `WSWeaponBase`
2. Set `WeaponType` and `FireMode`
3. Configure damage, fire rate, magazine size
4. Set reload time and range
5. Assign weapon mesh
6. Configure elemental damage if applicable

### Creating UI Widgets

The system expects these widget classes:
- **HUD Widget**: Displays health, ammo, currency, cooldowns, wave info
- **Shop Widget**: Shows upgrade cards with buy/destroy/hold options
- **Death Screen**: Shown when player is downed
- **Victory Screen**: Shown on Main Mode completion
- **Game Over Screen**: Shown when all players die

## Next Steps for Full Implementation

### High Priority
1. Create actual 3D assets and animations for all characters and enemies
2. Implement character-specific ability logic
3. Create game maps with proper spawn points
4. Design and implement UI widgets
5. Add visual and audio effects
6. Configure EOS online multiplayer

### Medium Priority
1. Implement boss-specific mechanics and phases
2. Create elemental effect systems
3. Add particle effects for weapons and abilities
4. Implement proper AI behaviors with navigation
5. Create tutorial and main menu

### Low Priority
1. Add cosmetic system
2. Implement battle pass
3. Create achievements/challenges
4. Add post-processing and atmosphere
5. Optimize for console ports

## Analytics and Telemetry

The system tracks:
- Upgrade cards drawn, purchased, destroyed, held
- Character pick rates and average progression
- Time per wave
- Enemies killed per player per wave
- Deaths per wave and causes
- Highest wave reached in Survival

This data should be sent to an analytics backend for balance adjustments.

## License

[MIT License](LICENSE)

## Contributing

This is an MVP implementation. Contributions are welcome! Please ensure:
- Code follows Unreal Engine coding standards
- New features include appropriate comments
- Systems integrate with existing framework
- Changes don't break core gameplay loop

## Support

For issues, questions, or suggestions:
- Create an issue on GitHub

---

**Note**: This is an MVP (Minimum Viable Product) implementation. Many features are implemented at a basic level and require additional content creation, polish, and testing before release. The C++ framework provides a solid foundation for Blueprint-based content creation in Unreal Engine.
