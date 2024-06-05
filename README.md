# Maze Adventure Game

This program is a practice of object-oriented programming.
An adventurous maze game using terminal where players navigate through a maze, encountering enemies, shops, hot springs, and random events along the way. 

## Features

- Explore a labyrinthine maze filled with twists and turns.
- Encounter various challenges and obstacles, including enemies, shops, hot springs, and random events.
- Engage in strategic battles with enemies to progress through the maze.
- Visit shops to purchase helpful items and upgrades.
- Restore health and vitality at hot springs scattered throughout the maze.
- Experience unpredictable events that can either aid or hinder your progress.

## How to Play

1. **Objective**: Navigate through the maze to find the exit.
2. **Controls**: Use the arrow keys or WASD keys to move your character.
3. **Encounters**:
   - **Enemies**: Engage in turn-based battles with enemies to defeat them.
   - **Shops**: Visit shops to purchase items and upgrades using in-game currency.
   - **Hot Springs**: Restore your character's health and vitality at hot springs.
   - **Random Events**: Experience unexpected events that can impact your journey.
4. **Combat**:
   - Use your character's skills and abilities strategically to defeat enemies.
   - Manage your resources wisely to survive challenging encounters.
5. **Exploration**:
   - Explore every nook and cranny of the maze to uncover hidden treasures and secrets.
   - Use map features to track your progress and plan your route.
6. **Victory**:
   - Reach the exit of the maze to emerge victorious and complete the game.

## Installation

1. Clone the repository to your local machine.
2. Compile and run the game executable.
3. Follow on-screen instructions to start playing.

## Additional Information (Translated from Chinese)

You can compile the game directly using Visual Studio or run the executable file.

This is a maze adventure game. The goal of the game is to find the exit and help the protagonist escape from the maze. There are four types of interactive objects in the maze: enemies, shops, hot springs, and random events.

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

## Contributing

Contributions to the project are welcome! If you have any ideas for new features, improvements, or bug fixes, please submit a pull request.

## Credits

- Developed by Yan-he(Evian) Chen
