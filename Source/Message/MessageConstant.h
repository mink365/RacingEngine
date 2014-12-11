#ifndef MESSAGECONSTANT_H
#define MESSAGECONSTANT_H

namespace MessageConstant
{
    namespace MessageType {
        extern const int GENERAL_MESSAGE;
        extern const int KEY_MESSAGE;
        extern const int TOUCHSCREEN_MESSAGE;
        extern const int COLLISION_MESSAGE;
        extern const int LOGIC_MESSAGE;
        extern const int WINDOW_MESSAGE;
        extern const int LAYER_MESSAGE;   
    }
    
    namespace WindowMessage {
        extern const int PUSH_WINDOW;
        extern const int POP_WINDOW;
    }
    
    namespace LayerMessage {
        extern const int PUSH_LAYER;
        extern const int POP_LAYER;
        extern const int CLEAN_TO_LAYER;
    }
}

#endif // MESSAGECONSTANT_H
