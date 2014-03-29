#ifndef FBXPARSER_H
#define FBXPARSER_H

#include <streambuf>
#include <vector>
#include <scene/Mesh.h>

namespace re {

class Node;
class ReadCommon;

enum FbxNodeAttributeType {
    NODE_GROUP = 1,
    NODE_SKELETON = 3,
    NODE_MESH = 4
};

class membuf : public std::basic_streambuf<char>
{
public:
  membuf(char* p, size_t n) {
    setg(p, p, p + n);
    setp(p, p + n);
  }
};

class FbxParser
{
public:
    FbxParser();

    void parseStream(std::istream *st);

    void parseData(void *data, long datalen);

    std::vector<NodePtr> getNodes() const;

private:
    NodePtr readNode(std::istream *st);
    void readMesh(std::istream *st, MeshPtr node);

    void readNodeTransform(std::istream *st, NodePtr node);

    void readMaterial(std::istream *st, MeshPtr node);

    std::vector<NodePtr> nodes;

    ReadCommon *reader;
};

} // namespace re

#endif // FBXPARSER_H
