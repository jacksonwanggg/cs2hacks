#include "aimbot.h"
#include <iostream>

// vec 3 would be built inside of cpp so its easy for this to make 
float aimbot::distance(vec3 p1, vec3 p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2) + pow(p1.z - p2.z, 2));
}


void aimbot::frame() {
	uintptr_t entityList = memory::Read<uintptr_t>(module_base + cs2_dumper::offsets::client_dll::dwEntityList);
	// this is to check if its found 
	if (!entityList) {
		std::cout << "not found entity List\n";
		return;
	}

	uintptr_t localPlayerPawn = memory::Read<uintptr_t>(module_base + cs2_dumper::offsets::client_dll::dwLocalPlayerPawn);
	
	// this is to check if the local player pawn is found
	if (!localPlayerPawn) {
		std::cout << "not found localPlayerPawn\n";
		return;
	}

	BYTE team = memory::Read<BYTE>(localPlayerPawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum);

	vec3 localEyePostion = memory::Read<vec3>(localPlayerPawn + cs2_dumper::schemas::client_dll::C_BasePlayerPawn::m_vOldOrigin) +
		memory::Read<vec3>(localPlayerPawn + cs2_dumper::schemas::client_dll::C_BaseModelEntity::m_vecViewOffset);
	
	// this would find the cloest enemy among all of them 
	float closest_distance = -1;

	vec3 enemyPos;

	for (int i = 0; i < 32; ++i) {
		uintptr_t listEntry = memory::Read<uintptr_t>(entityList + (8 * (i & 0x7FFF) >> 9) + 16);
		if (!listEntry) {
			std::cout << "not found listEntry type shi" << std::endl;
			continue;
		}

		uintptr_t entityController = memory::Read<uintptr_t>(listEntry + 120 * (i & 0x1FF));
		if (!entityController) {
			std::cout << "list entry 0x" << std::hex << listEntry << std::endl;
			std::cout << "entity list 0x" << std::hex << entityController << std::endl;
			std::cout << "on number: " << i << std::endl;
			std::cout << "exits at entity controlleDr" << std::endl;
			continue;
		}

		uintptr_t entityControllerPawn = memory::Read<uintptr_t>(entityController + cs2_dumper::schemas::client_dll::CCSPlayerController::m_hPlayerPawn);
		if (!entityControllerPawn) {
			std::cout << "list entry 0x" << std::hex << listEntry << std::endl;
			std::cout << "entity list 0x" << std::hex << entityController << std::endl;
			std::cout << "exit at entity controller pawn" << std::endl;
			continue;
		}

		uintptr_t list_entry2 = memory::Read<uintptr_t>(entityList + 0x8 * ((entityControllerPawn & 0x7FFF) >> 9) + 16);

		uintptr_t entityPawn = memory::Read<uintptr_t>(list_entry2 + 120 * (entityControllerPawn & 0x1FF));
		if (!entityPawn) continue;



		if (team == memory::Read<BYTE>(entityPawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum)) {
			std::cout << "same team type shi" << std::endl;
			continue;
		}

		int health = memory::Read<int>(entityPawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth);

		// check health
		if (health <= 0 || health > 100) {
			std::cout << "health is 0" << std::endl;
			continue;
		}

		vec3 entityEyePos = memory::Read<vec3>(entityPawn + cs2_dumper::schemas::client_dll::C_BasePlayerPawn::m_vOldOrigin) +
			memory::Read<vec3>(entityPawn + cs2_dumper::schemas::client_dll::C_BaseModelEntity::m_vecViewOffset);

		float current_distance = distance(localEyePostion, entityEyePos);

		if (closest_distance < 0 || current_distance < closest_distance) {
			std::cout << "updates type shit" << std::endl;
			closest_distance = current_distance;
			enemyPos = entityEyePos;
		}
		else {
			std::cout << "doesn update type shi" << std::endl;
		}
				
	}

	std::cout << "Selected Enemy Position: ("
		<< enemyPos.x << ", "
		<< enemyPos.y << ", "
		<< enemyPos.z << ")\n";

	vec3 relativeAngle = (enemyPos - localEyePostion).RelativeAngle();

	memory::Write<vec3>(module_base + cs2_dumper::offsets::client_dll::dwViewAngles, relativeAngle);



}