#include "obj_loader.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <map>
/*/
This is the script that runs the methods in the game program to load, set, and draw the 3D objects into the scene
*/
using namespace std;
using namespace glm;

static bool compareObjectIndexPtr(const OBJIndex* a, const OBJIndex* b); 
static inline unsigned int FindNextChar(unsigned int start, const char* str, unsigned int length, char token);
static inline unsigned int parseObjectIndexValue(const string& token, unsigned int start, unsigned int end);
static inline float parseObjectFloatValue(const string& token, unsigned int start, unsigned int end);
static inline vector<string> SplitString(const string &s, char delim);

/*/
Constructing the faces of the 3D model from the local file
*/
object3DModel::object3DModel(const string& fileName)
{
	hasUVs = false;
	hasNormals = false;
    ifstream file;
    file.open(fileName.c_str());

    string line;
    if(file.is_open())
    {
        while(file.good())
        {
            getline(file, line);
        
            unsigned int lineLength = line.length();
            
            if(lineLength < 2)
                continue;
            
            const char* lineCStr = line.c_str();
            
            switch(lineCStr[0])
            {
                case 'v':
                    if(lineCStr[1] == 't')
                        this->uvs.push_back(parseObjectVec2(line));
                    else if(lineCStr[1] == 'n')
                        this->normals.push_back(parseObjectVec3(line));
                    else if(lineCStr[1] == ' ' || lineCStr[1] == '\t')
                        this->vertices.push_back(parseObjectVec3(line));
                break;
                case 'f':
                    create3DObjectFace(line);
                break;
                default: break;
            };
        }
    }
    else
    {
        cerr << "Unable to load mesh: " << fileName << endl;
    }
}
/*/
Calculated the indexed 3D model's normals using the indices of the object
*/
void IndexedModel::calculateNormals()
{
    for(unsigned int i = 0; i < indices.size(); i += 3)
    {
        int i0 = indices[i];
        int i1 = indices[i + 1];
        int i2 = indices[i + 2];

        vec3 v1 = positions[i1] - positions[i0];
        vec3 v2 = positions[i2] - positions[i0];
        
        vec3 normal = normalize(cross(v1, v2));
            
        normals[i0] += normal;
        normals[i1] += normal;
        normals[i2] += normal;
    }
    
    for(unsigned int i = 0; i < positions.size(); i++)
        normals[i] = normalize(normals[i]);
}
/*/
Model stored in memory
*/
IndexedModel object3DModel::index3DModel()
{
    IndexedModel result;
    IndexedModel normalModel;
    
    unsigned int numIndices = OBJIndices.size();
    
    vector<OBJIndex*> indexLookup;
    
    for(unsigned int i = 0; i < numIndices; i++)
        indexLookup.push_back(&OBJIndices[i]);
    
    sort(indexLookup.begin(), indexLookup.end(), compareObjectIndexPtr);
    
    map<OBJIndex, unsigned int> normalModelIndexMap;
    map<unsigned int, unsigned int> indexMap;
    
    for(unsigned int i = 0; i < numIndices; i++)
    {
        OBJIndex* currentIndex = &OBJIndices[i];
        
        vec3 currentPosition = vertices[currentIndex->vertexIndex];
        vec2 currentTexCoord;
        vec3 currentNormal;
        
        if(hasUVs)
            currentTexCoord = uvs[currentIndex->uvIndex];
        else
            currentTexCoord = vec2(0,0);
            
        if(hasNormals)
            currentNormal = normals[currentIndex->normalIndex];
        else
            currentNormal = vec3(0,0,0);
        
        unsigned int normalModelIndex;
        unsigned int resultModelIndex;
        
        //Create model to properly generate normals on
        map<OBJIndex, unsigned int>::iterator it = normalModelIndexMap.find(*currentIndex);
        if(it == normalModelIndexMap.end())
        {
            normalModelIndex = normalModel.positions.size();
        
            normalModelIndexMap.insert(pair<OBJIndex, unsigned int>(*currentIndex, normalModelIndex));
            normalModel.positions.push_back(currentPosition);
            normalModel.texCoordinates.push_back(currentTexCoord);
            normalModel.normals.push_back(currentNormal);
        }
        else
            normalModelIndex = it->second;
        
        //Create model which properly separates texture coordinates
        unsigned int previousVertexLocation = FindLastVertexInIndex(indexLookup, currentIndex, result);
        
        if(previousVertexLocation == (unsigned int)-1)
        {
            resultModelIndex = result.positions.size();
        
            result.positions.push_back(currentPosition);
            result.texCoordinates.push_back(currentTexCoord);
            result.normals.push_back(currentNormal);
        }
        else
            resultModelIndex = previousVertexLocation;
        
        normalModel.indices.push_back(normalModelIndex);
        result.indices.push_back(resultModelIndex);
        indexMap.insert(pair<unsigned int, unsigned int>(resultModelIndex, normalModelIndex));
    }
    
    if(!hasNormals)
    {
        normalModel.calculateNormals();
        
        for(unsigned int i = 0; i < result.positions.size(); i++)
            result.normals[i] = normalModel.normals[indexMap[i]];
    }
    
    return result;
};
/*/
Connects vertexes to form shape of 3D object
*/
unsigned int object3DModel::FindLastVertexInIndex(const vector<OBJIndex*>& indexLookup, const OBJIndex* currentIndex, const IndexedModel& result)
{
    unsigned int start = 0;
    unsigned int end = indexLookup.size();
    unsigned int current = (end - start) / 2 + start;
    unsigned int previous = start;
    
    while(current != previous)
    {
        OBJIndex* testIndex = indexLookup[current];
        
        if(testIndex->vertexIndex == currentIndex->vertexIndex)
        {
            unsigned int countStart = current;
        
            for(unsigned int i = 0; i < current; i++)
            {
                OBJIndex* possibleIndex = indexLookup[current - i];
                
                if(possibleIndex == currentIndex)
                    continue;
                    
                if(possibleIndex->vertexIndex != currentIndex->vertexIndex)
                    break;
                    
                countStart--;
            }
            
            for(unsigned int i = countStart; i < indexLookup.size() - countStart; i++)
            {
                OBJIndex* possibleIndex = indexLookup[current + i];
                
                if(possibleIndex == currentIndex)
                    continue;
                    
                if(possibleIndex->vertexIndex != currentIndex->vertexIndex)
                    break;
                else if((!hasUVs || possibleIndex->uvIndex == currentIndex->uvIndex) 
                    && (!hasNormals || possibleIndex->normalIndex == currentIndex->normalIndex))
                {
                    vec3 currentPosition = vertices[currentIndex->vertexIndex];
                    vec2 currentTexCoord;
                    vec3 currentNormal;
                    
                    if(hasUVs)
                        currentTexCoord = uvs[currentIndex->uvIndex];
                    else
                        currentTexCoord = vec2(0,0);
                        
                    if(hasNormals)
                        currentNormal = normals[currentIndex->normalIndex];
                    else
                        currentNormal = vec3(0,0,0);
                    
                    for(unsigned int j = 0; j < result.positions.size(); j++)
                    {
                        if(currentPosition == result.positions[j] 
                            && ((!hasUVs || currentTexCoord == result.texCoordinates[j])
                            && (!hasNormals || currentNormal == result.normals[j])))
                        {
                            return j;
                        }
                    }
                }
            }
        
            return -1;
        }
        else
        {
            if(testIndex->vertexIndex < currentIndex->vertexIndex)
                start = current;
            else
                end = current;
        }
    
        previous = current;
        current = (end - start) / 2 + start;
    }
    
    return -1;
}
/*/
Creates the faces of the 3D object
*/
void object3DModel::create3DObjectFace(const string& line)
{
    vector<string> tokens = SplitString(line, ' ');

    this->OBJIndices.push_back(ParseObjectIndex(tokens[1], &this->hasUVs, &this->hasNormals));
    this->OBJIndices.push_back(ParseObjectIndex(tokens[2], &this->hasUVs, &this->hasNormals));
    this->OBJIndices.push_back(ParseObjectIndex(tokens[3], &this->hasUVs, &this->hasNormals));

    if((int)tokens.size() > 4)
    {
        this->OBJIndices.push_back(ParseObjectIndex(tokens[1], &this->hasUVs, &this->hasNormals));
        this->OBJIndices.push_back(ParseObjectIndex(tokens[3], &this->hasUVs, &this->hasNormals));
        this->OBJIndices.push_back(ParseObjectIndex(tokens[4], &this->hasUVs, &this->hasNormals));
    }
}
/*/
Stores definitions of 3D object model
*/
OBJIndex object3DModel::ParseObjectIndex(const std::string& token, bool* hasUVs, bool* hasNormals)
{
    unsigned int tokenLength = token.length();
    const char* tokenString = token.c_str();
    
    unsigned int vertIndexStart = 0;
    unsigned int vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, '/');
    
    OBJIndex result;
    result.vertexIndex = parseObjectIndexValue(token, vertIndexStart, vertIndexEnd);
    result.uvIndex = 0;
    result.normalIndex = 0;
    
    if(vertIndexEnd >= tokenLength)
        return result;
    
    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, '/');
    
    result.uvIndex = parseObjectIndexValue(token, vertIndexStart, vertIndexEnd);
    *hasUVs = true;
    
    if(vertIndexEnd >= tokenLength)
        return result;
    
    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, '/');
    
    result.normalIndex = parseObjectIndexValue(token, vertIndexStart, vertIndexEnd);
    *hasNormals = true;
    
    return result;
}
/*/
Stores vector3 information of 3D model mesh
*/
vec3 object3DModel::parseObjectVec3(const string& line) 
{
    unsigned int tokenLength = line.length();
    const char* tokenString = line.c_str();
    
    unsigned int vertIndexStart = 2;
    
    while(vertIndexStart < tokenLength)
    {
        if(tokenString[vertIndexStart] != ' ')
            break;
        vertIndexStart++;
    }
    
    unsigned int vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');
    
    float x = parseObjectFloatValue(line, vertIndexStart, vertIndexEnd);
    
    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');
    
    float y = parseObjectFloatValue(line, vertIndexStart, vertIndexEnd);
    
    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');
    
    float z = parseObjectFloatValue(line, vertIndexStart, vertIndexEnd);
    
    return vec3(x,y,z);

    
}

vec2 object3DModel::parseObjectVec2(const string& line)
{
    unsigned int tokenLength = line.length();
    const char* tokenString = line.c_str();
    
    unsigned int vertIndexStart = 3;
    
    while(vertIndexStart < tokenLength)
    {
        if(tokenString[vertIndexStart] != ' ')
            break;
        vertIndexStart++;
    }
    
    unsigned int vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');
    
    float x = parseObjectFloatValue(line, vertIndexStart, vertIndexEnd);
    
    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');
    
    float y = parseObjectFloatValue(line, vertIndexStart, vertIndexEnd);
    
    return vec2(x,y);
}
/*/
Compares vertex between points
*/
static bool compareObjectIndexPtr(const OBJIndex* a, const OBJIndex* b)
{
    return a->vertexIndex < b->vertexIndex;
}

static inline unsigned int FindNextChar(unsigned int start, const char* str, unsigned int length, char token)
{
    unsigned int result = start;
    while(result < length)
    {
        result++;
        if(str[result] == token)
            break;
    }
    
    return result;
}

static inline unsigned int parseObjectIndexValue(const string& token, unsigned int start, unsigned int end)
{
    return atoi(token.substr(start, end - start).c_str()) - 1;
}

static inline float parseObjectFloatValue(const string& token, unsigned int start, unsigned int end)
{
    return atof(token.substr(start, end - start).c_str());
}

static inline vector<string> SplitString(const string &s, char delim)
{
    vector<string> elems;
        
    const char* cstr = s.c_str();
    unsigned int strLength = s.length();
    unsigned int start = 0;
    unsigned int end = 0;
        
    while(end <= strLength)
    {
        while(end <= strLength)
        {
            if(cstr[end] == delim)
                break;
            end++;
        }
            
        elems.push_back(s.substr(start, end - start));
        start = end + 1;
        end = start;
    }
        
    return elems;
}
