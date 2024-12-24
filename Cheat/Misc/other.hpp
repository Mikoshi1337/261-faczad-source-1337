#include <array>

namespace other {
	Vector3 m_manipulate = Vector3::Zero();

	void Circle3D1(Vector3 pos, Color clr, float radius, float penis = 0) {
		Vector3 prev = Vector3(pos.x + radius, pos.y, pos.z);
		for (float num = 0.f; num < 6.2831855f; num += 0.20f) {

			float val_x = cos(num) * radius;
			float val_z = sin(num) * radius;
			Vector3 newPos = Vector3(pos.x + val_x, pos.y + penis, pos.z + val_z);

			DDraw::Line(prev, newPos, clr, 0.01f, false, false);

			prev = newPos;
		}
	}	

	inline void vectors(std::vector<Vector3>& re, float max = 2.0f, float maxy = 2.0f, int numPoints = 100)
	{
		float max_radius = max;

		for (int i = 0; i < numPoints; ++i) {
			float vec1 = static_cast<float>((rand)()) / static_cast<float>(RAND_MAX) * 2.0f * M_PI;
			float vec2 = static_cast<float>((rand)()) / static_cast<float>(RAND_MAX) * 2.0f * M_PI;

			float x = max_radius * sinf(vec2) * cosf(vec1);
			float y = maxy * sinf(vec2) * sinf(vec1);
			float z = max_radius * cosf(vec2);

			re.emplace_back(x, y, 0.f);
			re.emplace_back(-0.f, -y, -z);
			re.emplace_back(x, 0.f, z);

			re.emplace_back(0.f, y, 0.f);
			re.emplace_back(0.f, -y, 0.f);

			re.emplace_back(-x, 0.f, 0.f);
			re.emplace_back(x, 0.f, 0.f);

			re.emplace_back(0.f, 0.f, z);
			re.emplace_back(0.f, 0.f, -z);
		}
	}

	void find_manipulate_angle() {
		Vector3 re_p = LocalPlayer::Entity()->transform()->position() + LocalPlayer::Entity()->transform()->up() * (PlayerEyes::EyeOffset().y + LocalPlayer::Entity()->eyes()->viewOffset().y);

		Vector3 choice = Vector3::Zero();			

		if (vars::stuff::best_target != Vector3(0, 0, 0)) {
			if (utils::LineOfSight(re_p, vars::stuff::best_target)) {
				m_manipulate = Vector3::Zero();
				return;
			}
		}
		else {
			if (utils::LineOfSight(re_p, reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer)->get_bone_pos(head))) {
				m_manipulate = Vector3::Zero();
				return;
			}
		}

		float desyncTime = (Time::realtimeSinceStartup() - LocalPlayer::Entity()->lastSentTickTime()) - 0.03125 * 3;
		float mm_max_eye = (0.1f + ((desyncTime + 2.f / 60.f + 0.125f) * 1.5f) * LocalPlayer::Entity()->MaxVelocity()) - 0.05f;
		float max_v_multicplier = 4.5f;
		float v_mm_max_eye = mm_max_eye / max_v_multicplier;

		auto right = LocalPlayer::Entity()->eyes()->MovementRight();
		auto forward = LocalPlayer::Entity()->eyes()->MovementForward();

		std::vector<Vector3> random = {};		

		vectors(random, 2.0f, 2.0f, 15.f);

		if (choice.empty()) {
			m_manipulate = Vector3::Zero();
			return;
		}

		m_manipulate = choice;
	}
}
Vector3 m_hitscan_manipulator;