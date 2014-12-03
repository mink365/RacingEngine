/*****************************************************************************

 Copyright (c) 2014 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#include "JSONAtlasParser.h"

#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"

using namespace rapidjson;

namespace re {

JSONAtlasParser::JSONAtlasParser()
{
}

void JSONAtlasParser::parseAtlasFile(FilePtr& file) {
    this->atlasFile = file;

    ByteBufferPtr buf = file->read();

    buf->getData();

    Document doc;
    doc.Parse((const char*)buf->getData());

    auto& meta = doc["meta"];
    this->imageSize.width = meta["size"]["w"].GetInt();
    this->imageSize.height = meta["size"]["h"].GetInt();

    auto imageName = meta["image"].GetString();
    this->imageFile = GetImageFile(file, imageName);

    this->getTextureByImageFile();

    Value& frames = doc["frames"];
    for (Value::ConstMemberIterator iter = frames.MemberBegin(); iter != frames.MemberEnd(); ++iter) {
        auto frameName = iter->name.GetString();
        auto& frame = iter->value;

        TextureFrameData& data = this->allocateFrameData();
        data.name = GetFrameName(file, frameName);

        data.isTrimmed = frame["trimmed"].GetBool();
        data.isRotated = frame["rotated"].GetBool();

        auto& uv = frame["frame"];
        data.region.set(uv["x"].GetInt(), uv["y"].GetInt(), uv["w"].GetInt(), uv["h"].GetInt());

        if (data.isTrimmed) {
            auto& sourceSize = frame["sourceSize"];
            data.originalSize.width = sourceSize["w"].GetInt();
            data.originalSize.height = sourceSize["h"].GetInt();

            auto& spriteSourceSize = frame["spriteSourceSize"];
            data.offset.x = spriteSourceSize["x"].GetInt();
            data.offset.y = spriteSourceSize["y"].GetInt();
        } else {
            data.offset = Vec2::Zero;
            data.originalSize = {data.region.getWidth(), data.region.getHeight()};
        }
    }
}


}
