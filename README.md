# Welcome!
This project is originally used for self-studying OpenGL, however, during the process of study, some useful tools are developed, we will move them out when they become mature, we will list some operational libraries in the following paragraphs.
# Triangle Mesh Viewer
Interfaces is located in triangles_shower.h.
## Build
Currently the project is basically for personal use. However, if you want to build it locally, here is the hint.
```text
1. install vcpkg, then intall glad/glm/glfw3/GTest
2. check CMakeLists.txt, replace the CMAKE_TOOLCHAIN_FILE with your own vcpkg toolchain path.
```
The above steps would be probaboly enough to compile it in your own environment, if you encounter some problems, feel free to contact us. hanwein2@gmail.com.
## Usage
We provide two kinds of method to view triangle mesh.

The first mode is view it by game mode. Pretend youself as a game role, and the model composed by triangles is a huge mountain in the distance. Then you can move your character to view it from different position. This interface is called show_triangles_with_game_player().

The second mode is view it by model mode. Imagine that you sit on a table, and your model is fixed on the table, you can let your model rotate, and you can put your eye closer or further to watch it. This interface is called show_triangles_with_model_viewer(). Example:
```c++
#include "triangles_shower.h"
int main()
{
    // Point3D is the vertex data type, std::vector<int> is the triangle index data type
    DisplayInfo<std::vector<double>, std::vector<int>> display_info;
    // suppose you want to show a tetrahedron
    std::vector<std::vector<double>> vertices{
        {0, 0, 0},
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1}
    };

    std::vector<std::vector<int>> polygons{
        {0, 2, 1},
        {0, 1, 3},
        {0, 3, 2},
        {1, 2, 3}
    };

    std::vector<std::vector<double>> normals{
        {0, 0, -1},
        {0, -1, 0},
        {-1, 0, 0},
        {0.577350f, 0.577350f, 0.577350f}
    };
    display_info.vectices = vertices;
    display_info.normals = normals;
    display_info.triangles = polygons;
    // we designed the model viewer with ability show multiple models
    std::vector<DisplayInfo<std::vector<double>, std::vector<int>>> mul_display_info{display_info};
    show_triangles_with_model_viewer(mul_display_info);
    return 0;
}
```
You can call those two interfaces to show model, and also make sure pass in correct data. We also build a **execute binary**, which is integrated in a visualized tool. You can find it [here](https://github.com/hanxiaowein1/charles_mesh/releases/tag/v0.2). In this visualized tool, you can choose a obj type mode, and view it through model mode.
## Output Examples
![bunny](images/mesh_viewer.gif)
*<p align="center">Mesh Viewer</p>*
## Additional Infomation
```text
1. We render the model by phong lightning model.
2. Currently the library do not support modify some parameters in shader. If there is enough time in the future, we will consider to add it.
3. Adding text about face/vertex/edge is under consideration...
```