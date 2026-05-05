
# Design Documentation

## Architecture
The project follows a modular structure:
- Character System → Handles all entities
- Battle System → Controls combat logic
- Game Controller → Manages flow and modes

## Class Hierarchy
- Character (Base Class)
- │
- ├── Player
- └── Enemy
-     ├── NewbieEnemy
-     ├── MediumEnemy
-     └── BossEnemy

## Design Patterns Used
### Factory Pattern
- `createEnemy()` dynamically creates enemies based on level

### Polymorphism
- `attack()` function overridden in derived classes

## Memory Management
- Used `std::unique_ptr` to avoid memory leaks
- No manual `new/delete`

## Game Flow
- 1. Player selects mode
- 2. Enemy is created using factory
- 3. Battle loop executes
- 4. Game ends on win/loss
