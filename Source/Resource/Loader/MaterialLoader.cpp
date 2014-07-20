#include "MaterialLoader.h"
#include "Resource/Common/BlockParser.h"
#include "Texture/TextureManager.h"

namespace re {

MaterialLoader::MaterialLoader()
{
}

void MaterialLoader::loadMaterial(Statement::ptr &statement)
{
    if (statement->type == "material") {
        Material::ptr material = Material::create();

        for (auto kv : statement->keyValues) {
            if (kv->key == "queue_id") {
                int v = To<int>(kv->value);
                material->setQueueID(v);
            } else if (kv->key == "transparent") {
                material->setTransparent(To<bool>(kv->value));
            } else if (kv->key == "depth_write") {
                material->getRenderState().depthState.depthWrite = To<bool>(kv->value);
            } else if (kv->key == "depth_test") {
                material->getRenderState().depthState.depthTestEnable = To<bool>(kv->value);
            }
        }

        for (auto child : statement->children) {
            if (child->type == "pass") {
                Pass::ptr pass = Pass::create();

                this->loadPass(child, pass);

                material->addPass(pass);
            }
        }
    }
}

void MaterialLoader::loadPass(Statement::ptr &statement, Pass::ptr &pass)
{
    for(auto childState : statement->children) {
        if (childState->type == "texture_unit") {
            TextureUnitState::ptr unit = TextureUnitState::create();
            loadTextureUnit(childState, unit);
            pass->addTextureUnit(unit);
        }
    }
}

void MaterialLoader::loadTextureUnit(Statement::ptr &statement, TextureUnitState::ptr& unit)
{
    for (auto child : statement->children) {
        if (child->type == "texture_frames") {
            for (auto kv : child->keyValues) {
                if (kv->key == "texture") {
                    Texture::ptr frame = TextureManager::getInstance().getTexture(kv->value);

                    unit->addTextureFrame(frame);
                }
            }
        }
    }

    for (auto kv : statement->keyValues) {
        // TODO: scale ?
    }
}

} // namespace re
