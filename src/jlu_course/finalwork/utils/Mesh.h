#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

#define MAX_BONE_INFLUENCE 4

struct Vertex{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;

    glm::vec3 Tangent;

    glm::vec3 Bitangent;

	int m_BoneIDs[MAX_BONE_INFLUENCE];

	float m_Weights[MAX_BONE_INFLUENCE];
};

struct Texture{
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
    public:
        unsigned int VAO, VBO, EBO;
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;
        Mesh(const std::vector<Vertex>& vs, const std::vector<unsigned int>& is, const std::vector<Texture>& ts)
        {
            vertices = vs;
            indices = is;
            textures = ts;
            setupMesh();
        }
        
        void Draw(Shader& shader) {
            shader.use();
            unsigned int diffuseNr = 1;
            unsigned int specularNr = 1;
            unsigned int normalNr   = 1;
            unsigned int heightNr   = 1;
            // 绑定纹理
            for (unsigned int i = 0; i < textures.size(); ++i) {
                glActiveTexture(GL_TEXTURE0 + i);
                std::string number;
                std::string name = textures[i].type;
                if (name == "texture_diffuse")
                    number = std::to_string(diffuseNr++);
                else if(name == "texture_specular")
                    number = std::to_string(specularNr++);
                else if(name == "texture_normal")
                    number = std::to_string(normalNr++); 
                else if(name == "texture_height")
                    number = std::to_string(heightNr++); 
                shader.setInt(("material." + name + number).c_str(), i);
                //std::cout<<textures[i].path<< " " << name<<std::endl;
                glBindTexture(GL_TEXTURE_2D, textures[i].id);
            }
            glBindVertexArray(VAO);
            if (indices.size() != 0) {
                glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
            }
            else {
                glDrawArrays(GL_TRIANGLES, 0, vertices.size());
            }
            glBindVertexArray(0);
            glActiveTexture(GL_TEXTURE0);
        }

    private:
        // unsigned int VAO, VBO, EBO;
        void setupMesh() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);  

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

            //顶点位置
            glEnableVertexAttribArray(0);	
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
            //顶点法线
            glEnableVertexAttribArray(1);	
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
            //glEnableVertexAttribArray(1);
            //顶点纹理
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
            //glEnableVertexAttribArray(2);
            // vertex tangent
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
            glEnableVertexAttribArray(3);
            // vertex bitangent
            glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
            glEnableVertexAttribArray(4);
		    // id
		    glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));
            glEnableVertexAttribArray(5);

		    // weights
		    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));
            glEnableVertexAttribArray(6);

            glBindVertexArray(0);
        }

};

#endif

