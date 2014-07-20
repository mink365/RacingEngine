#include "BlockParser.h"

#include <sstream>
#include <regex>

#include "FileSystem/File.h"
#include "MemBuffer.h"

namespace re {

static std::string BLOCK_BEGIN = "{";
static std::string BLOCK_END = "}";

std::vector<std::string> &Split(const std::string &line, const std::string& rgxExpression, std::vector<std::string> &elems) {
    std::regex rgx(rgxExpression);
    std::sregex_token_iterator iter(line.begin(),
        line.end(),
        rgx,
        -1);
    std::sregex_token_iterator end;

    while(iter != end) {
        const std::string& token = *iter;
        if (token != "") {
            elems.push_back(*iter);
        }

        iter ++;
    }

    return elems;
}

BlockParser::BlockParser()
{
}

Statement::ptr BlockParser::parse(FilePtr &file)
{
    Buffer::ptr data = file->read();

    membuf mb((char*)(data->getData()), data->getSize());
    std::istream stream(&mb);

    Statement::ptr root = std::make_shared<Statement>("root", "root");

    std::string line;
    std::vector<std::string> parts;
    while(std::getline(stream, line)) {
        if (line.find_first_of('/') == 0) {
            continue;
        }

        parts.clear();
        Split(line, "\\s+", parts);
        int num = parts.size();
        if (num == 0) {
            continue;
        }

        if (num >= 2 && parts[num - 1] == BLOCK_BEGIN) {
            Statement::ptr state = nullptr;

            if (num > 2) {
                state = std::make_shared<Statement>(parts[0], parts[1]);
            } else {
                state = std::make_shared<Statement>(parts[0], "");
            }

            this->parseBlock(stream, state);

            root->addStatement(state);
        }
    }

    return root;
}

void BlockParser::parseBlock(std::istream &stream, Statement::ptr &state)
{
    std::string line;
    std::vector<std::string> parts;
    while(std::getline(stream, line)) {
        if (line.find_first_of('/') == 0) {
            continue;
        }

        parts.clear();
        Split(line, "\\s+", parts);
        int num = parts.size();
        if (num == 0) {
            continue;
        }

        const std::string& type = parts[0];
        if (type == BLOCK_END) {
            return;
        } else if (type.find_first_of('/') == 0) {
            continue;
        } else {
            if (num >= 2 && parts[num - 1] == BLOCK_BEGIN) {
                Statement::ptr state = nullptr;

                if (num > 2) {
                    state = std::make_shared<Statement>(parts[0], parts[1]);
                } else {
                    state = std::make_shared<Statement>(parts[0], "");
                }

                this->parseBlock(stream, state);

                state->addStatement(state);
            } else if (num > 1) {
                // read the key value pairs
                KeyValue::ptr kv = std::make_shared<KeyValue>(parts[0], parts[1]);

                state->addKeyValue(kv);
            } else {
                // error
            }
        }
    }
}

KeyValue::KeyValue(const std::string &key, const std::string &value)
{
    this->key = key;
    this->value = value;
}

Statement::Statement(const std::string &key, const std::string &name)
{
    this->type = key;
    this->name = name;
}

void Statement::addStatement(Statement::ptr &state)
{
    this->children.push_back(state);
}

void Statement::addKeyValue(KeyValue::ptr &kv)
{
    this->keyValues.push_back(kv);
}

} // namespace re
