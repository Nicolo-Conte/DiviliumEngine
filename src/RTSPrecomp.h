#pragma once

#include <cstdint>
#include <cstddef>
#include <assert.h>
#include "imgui-docking/imgui.h"
#include <cstdio>

#include <cstdlib>

#include <string.h>
#include <cstdarg>
#include <ctype.h>

#include <string>
#include <string_view>
#include <type_traits>
#include <array>
#include <initializer_list>
#include <vector>
#include <unordered_map>

#include "Core/Core.h"

#if defined(__ORBIS__)
// This is never going to be fixed, so let's disable this warning
#pragma GCC diagnostic ignored "-Wformat-security"
#pragma GCC diagnostic ignored "-Wwritable-strings"
#endif

enum Kind
{
	//enemies
	Kind_Default,
	Kind_Enemy,
	Kind_EnemyTransporter,
	Kind_EnemySniper,
	Kind_EnemyTank,
	Kind_EnemyKamikaze,
	//buildings
	Kind_Tower,
	Kind_Turret,
	Kind_Core,
	Kind_Mine,
	//others
	Kind_EnemySpawner,
};

enum EntityFlag
{
	EntityFlag_Ignore = 1
};