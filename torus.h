#include <vector>
#include <glm/glm.hpp>

void computeNormals(std::vector<glm::vec3>& normals,
                            const std::vector<glm::vec3>& positions,
                            const std::vector<unsigned int>& indices);

void compute_torus_points(std::vector<glm::vec3>& positions, std::vector<glm::vec2>& uv_coordinates, float innerRadius, 
    float outterRadius, int resolution);

void generate_indices(std::vector<unsigned int>& indices, int resolution);

