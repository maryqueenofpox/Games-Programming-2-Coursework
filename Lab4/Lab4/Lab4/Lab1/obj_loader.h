#ifndef OBJ_LOADER_H_INCLUDED
#define OBJ_LOADER_H_INCLUDED

#include <glm/glm.hpp>
#include <vector>
#include <string>
/*/
This header file used for loading 3D game objects.
It is used to setup the script that will take a 3D object from a local file
and determine its geometry to allow for the game to spawn it into the scene.
*/
using namespace std;
using namespace glm;

/*/
Constructor holds the 3D objects geometrical assets 
Such as the virtices, normals and UVs
*/
struct OBJIndex
{
    unsigned int vertexIndex;
    unsigned int uvIndex;
    unsigned int normalIndex;
    
    bool operator<(const OBJIndex& r) const { return vertexIndex < r.vertexIndex; }
};

/*/
Holds the specific vector3 qualities of the 3D object in memory
Such as the object's position and other related properties
*/
class IndexedModel
{
public:
    vector<vec3> positions;
    vector<vec2> texCoordinates;
    vector<vec3> normals;
    vector<unsigned int> indices;
    
    void calculateNormals();
};
/*/
Determines the specific vector3 values of the 3D model that is to be stored and spawned in the game.
Takes the assigned 3D model from the given file location and determines it's geometrical properties
This information is used by the class to then draw the object into the game scene
*/
class object3DModel
{
public:
    vector<OBJIndex> OBJIndices;
    vector<vec3> vertices;
    vector<vec2> uvs;
    vector<vec3> normals;
    bool hasUVs;
    bool hasNormals;
    
    object3DModel(const string& fileName);
    
    IndexedModel index3DModel();
private:
    unsigned int FindLastVertexInIndex(const vector<OBJIndex*>& indexLookup, const OBJIndex* currentIndex, const IndexedModel& result);
    void create3DObjectFace(const string& line);
    
    vec2 parseObjectVec2(const string& line);
    vec3 parseObjectVec3(const string& line);
    OBJIndex ParseObjectIndex(const string& token, bool* hasUVs, bool* hasNormals);
};

#endif // OBJ_LOADER_H_INCLUDED
