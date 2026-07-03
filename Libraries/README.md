# Engine Libraries (Modules)

Entix Engine is a collection libraries (or modules). You can
use only what you want and don't include unnecessary things to your project.

## Runtime

This library provides core functionality and implement wrapper for platform dependent features of Entix.

## GameFramework

This library provides all specific things for game development. **ECS**, **Scenes**, **Application** class, cross-platform **Entry Point** and so on.

Also it has a lot of built-in components and systems for your game.

## Vendor

External libraries:

- glad
- imgui

Other external libraries are installed via CMake `FetchContent`.
