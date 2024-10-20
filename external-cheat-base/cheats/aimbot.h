#pragma once

#include "../SDK/client.h"
#include "../SDK/offsets.h"

#include "../math/vector.h"
#include "../memory/memory.h"

// this is declaring the structure of it it would be implmeented in aimbot.cpp
namespace aimbot {
	inline uintptr_t module_base;
	inline uintptr_t processId;

	float distance(vec3 p1, vec3 p2);
	void frame();
}