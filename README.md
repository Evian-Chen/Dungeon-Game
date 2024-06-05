# Maze Adventure Game

This program is a practice of object-oriented programming and also the final project of NTUST OOP lab 2024 spring.
This is a maze adventure game. The goal of the game is to find the exit and help the protagonist escape from the maze. There are four types of interactive objects in the maze: enemies, shops, hot springs, and random events.

## Features

### Screen Display

- After starting the game, press E to enter the game.
- The protagonist is represented by the symbol R. The player's current status is displayed at the bottom of the map.
- Other interactive objects:
  - Only the bright yellow parts of the map are passable, marked by a dot (.).
  - Shops are marked by a dollar sign ($) and allow you to buy equipment.
  - Enemies are marked by an E, and touching them initiates combat.
  - Hot springs are marked by an H, which consume focus to provide healing.
  - Random events are marked by an exclamation mark (!), offering extra rewards through dice rolls or rock-paper-scissors.
  - The exit is marked by an F. Reaching it ends the game.
- Each stage of the game includes text prompts on how to proceed.
- Movement requires rolling dice to determine the number of steps. Once steps are exhausted, you need to roll again.

### Gameplay

- Use WSAD to move the protagonist.
- Game over conditions:
  - Dying in combat.
  - Finding the exit.
  - Reaching the maximum number of dice rolls without finding the exit.

### Combat Stage

- Engaging with an E triggers the combat stage.
- Turn order is calculated by (number of actions taken) / speed. Ties are broken by default order.

#### Player Attack Skills

- **attack**: Default skill, deals 100% physical attack to a single enemy. Requires one dice roll.
- **provoke**: Inflicts the Angry debuff on an enemy for three turns. Requires one dice roll.
- **shockBlast**: Deals 50% magical attack to all enemies. Requires three dice rolls.
- **heal**: Restores health equal to 150% of magical attack.
- **speedUp**: Increases own speed.

#### Player Equipment (Passive Skills)

- **giantHammer**: When attacking, deals 50% of the target's original damage to other enemies.
- **plateArmor**: Reduces damage taken by 10%.

#### Player Equipment (Active Buffs)

- **woodenSword**: Stat::PAttack + 5, Stat::HitRate + 10
- **ritualSword**: Stat::MAttack + 15
- **giantHammer**: Stat::PAttack + 20, Stat::HitRate - 15
- **woodenShield**: Stat::PDefense + 10
- **plateArmor**: Stat::PDefense + 20, Stat::Speed - 10
- **robe**: Stat::MDefense + 10
- **holyGrail**: Stat::MDefense + 30

Buffs and debuffs are removed after exiting combat.

## Credits

- Developed by Yan-he(Evian) Chen
