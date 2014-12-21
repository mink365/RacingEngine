#ifndef RE_BLOCKPARSER_H
#define RE_BLOCKPARSER_H

#include "PreDeclare.h"

#include <string>
#include <memory>
#include <vector>

#include "Base/Shared.h"
#include "Base/Singleton.h"

namespace re {

class KeyValue : public Shared<KeyValue>
{
public:
    KeyValue(const std::string& key, const std::string& value);

public:
    std::string key;
    std::string value;
};

class Statement : public Shared<Statement>
{
public:
    Statement(const std::string& key, const std::string& name);
    void addStatement(Statement::ptr& state);
    void addKeyValue(KeyValue::ptr& kv);

public:
    std::string type;
    std::string name;

    std::vector<Statement::ptr > children;
    std::vector<KeyValue::ptr > keyValues;
};

class BlockParser : public Singleton<BlockParser>
{
public:
    BlockParser();

    Statement::ptr parse(FilePtr& file);

private:
    void parseBlock(std::istream& is, Statement::ptr& state);
};

} // namespace re

#endif // RE_BLOCKPARSER_H
