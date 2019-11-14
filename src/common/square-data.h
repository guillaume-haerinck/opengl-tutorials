#pragma once

#include <glm/glm.hpp>

namespace squareData {
    //    v6----- v5
    //   /|      /|
    //  v1------v0|
    //  | |     | |
    //  | |v7---|-|v4
    //  |/      |/
    //  v2------v3
    const glm::vec3 positions[] = {
        // Front v0,v1,v2,v3
        glm::vec3(1, 1, 1), glm::vec3(-1, 1, 1), glm::vec3(-1, -1, 1), glm::vec3(1, -1, 1),
        // Right v0,v3,v4,v5
        glm::vec3(1, 1, 1), glm::vec3(1, -1, 1), glm::vec3(1, -1, -1), glm::vec3(1, 1, -1),
        // Top v0,v5,v6,v1	
        glm::vec3(1, 1, 1), glm::vec3(1, 1, -1), glm::vec3(-1, 1, -1), glm::vec3(-1, 1, 1), 
        // Left v1,v6,v7,v2	
        glm::vec3(-1, 1, 1), glm::vec3(-1, 1, -1), glm::vec3(-1, -1, -1), glm::vec3(-1, -1, 1),  
        // Bottom v7,v4,v3,v2
        glm::vec3(-1, -1, -1), glm::vec3(1, -1, -1), glm::vec3(1, -1, 1), glm::vec3(-1, -1, 1), 
        // Back v4,v7,v6,v5	
        glm::vec3(1, -1, -1), glm::vec3(-1, -1, -1), glm::vec3(-1, 1, -1), glm::vec3(1, 1, -1)
    };

    const glm::vec3 normals[] = {
        glm::vec3(0, 0, 1), glm::vec3(0, 0, 1), glm::vec3(0, 0, 1), glm::vec3(0, 0, 1),
        glm::vec3(1, 0, 0), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0),
        glm::vec3(0, 1, 0), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0),
        glm::vec3(-1, 0, 0), glm::vec3(-1, 0, 0), glm::vec3(-1, 0, 0), glm::vec3(-1, 0, 0),
        glm::vec3(0,-1, 0), glm::vec3(0,-1, 0), glm::vec3(0,-1, 0), glm::vec3(0,-1, 0),
        glm::vec3(0, 0,-1), glm::vec3(0, 0,-1), glm::vec3(0, 0,-1), glm::vec3(0, 0,-1)
    };

    const glm::vec2 texCoords[] = {
        glm::vec2(1, 0), glm::vec2(0, 0), glm::vec2(0, 1), glm::vec2(1, 1),
        glm::vec2(0, 0), glm::vec2(0, 1), glm::vec2(1, 1), glm::vec2(1, 0),
        glm::vec2(1, 1), glm::vec2(1, 0), glm::vec2(0, 0), glm::vec2(0, 1),
        glm::vec2(1, 0), glm::vec2(0, 0), glm::vec2(0, 1), glm::vec2(1, 1),
        glm::vec2(0, 1), glm::vec2(1, 1), glm::vec2(1, 0), glm::vec2(0, 0),
        glm::vec2(0, 1), glm::vec2(1, 1), glm::vec2(1, 0), glm::vec2(0, 0)
    };

    const unsigned short indices[] = {
        0, 1, 2,   2, 3, 0,       // front
        4, 5, 6,   6, 7, 4,       // right
        8, 9, 10,  10,11,8,       // top
        12,13,14,  14,15,12,      // left
        16,17,18,  18,19,16,      // bottom
        20,21,22,  22,23,20		  // back
    };
}
