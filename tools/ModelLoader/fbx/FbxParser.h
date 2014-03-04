#ifndef FBXPARSER_H
#define FBXPARSER_H

#include <streambuf>
#include <vector>
#include <scene/Mesh.h>

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

    std::vector<Node *> getNodes() const;

private:
    Node *readNode(std::istream *st);
    void readMesh(std::istream *st, Mesh *node);

    void readNodeTransform(std::istream *st, Node *node);

    void readMaterial(std::istream *st, Mesh *node);

    std::vector<Node *> nodes;

    ReadCommon *reader;
};

#endif // FBXPARSER_H
