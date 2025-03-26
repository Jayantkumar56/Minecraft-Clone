

#pragma once

#include "glm/glm.hpp"


struct Mesh {
	std::vector<glm::vec3> Positions;
	std::vector<glm::vec2> UV;
	std::vector<glm::vec3> Normals;
	std::vector<uint32_t>  Indices;
};
