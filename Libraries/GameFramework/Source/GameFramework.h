// -*- mode: c++; -*-
#pragma once

// ----------------------------------------
// Header to include in games source code
// ----------------------------------------

#include "GameFramework/Components.h"
#include "GameFramework/Scene.h"
#include "GameFramework/Entity.h"

#include "GameFramework/Application.h"

#include "GameFramework/ScriptableEntity.h"

// Built-in Layers
#include "GameFramework/GUILayer.h"

/**********************
 *    Entry Point     *
 **********************/
#if defined(GAME_MAIN)
#include "GameFramework/EntryPoint.h"
#endif
