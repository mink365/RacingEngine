#include "MessageConstant.h"

namespace MessageConstant
{
    namespace MessageType {
        const int GENERAL_MESSAGE = 0;
        const int KEY_MESSAGE = 1;
        const int TOUCHSCREEN_MESSAGE = 2;
        const int COLLISION_MESSAGE = 3;

        const int WINDOW_MESSAGE = 5;

        const int LAYER_MESSAGE = 11;        
    }

    namespace WindowMessage {
        const int PUSH_WINDOW = 200;
        const int POP_WINDOW = 201;
    }
    
    namespace LayerMessage {
        const int PUSH_LAYER = 300;
        const int POP_LAYER = 301;
        const int CLEAN_TO_LAYER = 302;
    }  
}
