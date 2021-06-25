#include <vector>
#include <glm/glm.hpp>


void computeNormals(std::vector<glm::vec3>& normals,
                            const std::vector<glm::vec3>& positions,
                            const std::vector<unsigned int>& indices)
{
    for(unsigned int vertex = 0; vertex < positions.size(); vertex++)
    {
        // Loop through each triangle and update the normal
        // each vertex if that vertex is used in the triangle.
        glm::vec3 N = glm::vec3(0.0f, 0.0f, 0.0f);
        for(unsigned int ind = 0; ind < indices.size(); ind += 3)
        {
            if(indices[ind] == vertex || indices[ind + 1] == vertex 
                || indices[ind + 2] == vertex)
            {
                glm::vec3 edge1 = positions[indices[ind + 1]] - positions[indices[ind]];
                glm::vec3 edge2 = positions[indices[ind + 2]] - positions[indices[ind]];

                N += glm::normalize(glm::cross(edge1, edge2));
            }
        }
        // Compute the normal for each vertex.
        normals.push_back(glm::normalize(N));
    }
}

void compute_torus_points(std::vector<glm::vec3>& positions, std::vector<glm::vec2>& uv_coordinates, float innerRadius, 
    float outterRadius, int resolution)
{
    positions.reserve(resolution * resolution);

    for(int i = 0; i < resolution; i++)
    {
	    for(int j = 0; j < resolution; j++)
	    {
            float x = (outterRadius + innerRadius*cos(float(j)*2.0*M_PI / resolution))
                *cos(float(i)*2*M_PI / resolution);
            float y = (outterRadius + innerRadius*cos(float(j)*2.0*M_PI / resolution))
                *sin(float(i)*2*M_PI / resolution);
            float z = outterRadius*sin(float(i)*2*M_PI / resolution);
            positions.emplace_back(x,y,z);
	    uv_coordinates.emplace_back(float(i) / resolution, float(j) / resolution);
	    }
    }
}

void generate_indices(std::vector<unsigned int>& indices, int resolution)
{
    // fill "indices" as needed
    for(int i = 0; i < resolution; i++)
    {
	    for(int j = 0; j < resolution; j++)
	    {
		    indices.push_back(i * resolution + j);
		    indices.push_back(((i + 1) % resolution) * resolution + j);
		    indices.push_back(i * resolution + ((j + 1) % resolution));

		    indices.push_back(i * resolution + ((j + 1) % resolution));
		    indices.push_back(((i + 1) % resolution) * resolution + ((j + 1) % resolution));
		    indices.push_back(((i + 1) % resolution) * resolution + j);
	    }
    }
}
