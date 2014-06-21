#ifndef MESSAGECONSTANT_H
#define MESSAGECONSTANT_H

namespace MessageConstant
{
    namespace MessageType {
        extern const int GENERAL_MESSAGE;
        extern const int KEY_MESSAGE;
        extern const int TOUCHSCREEN_MESSAGE;
        extern const int COLLISION_MESSAGE;
        /**
        * 游戏逻辑信息
        */
        extern const int LOGIC_MESSAGE;

        /**
        * 窗口管理信息
        */
        extern const int WINDOW_MESSAGE;

        /**
        * 网络状态通知消息
        */
        extern const int NETWORK_STATE_MESSAGE;

        /**
        * RPC响应
        */
        extern const int RPC_MESSAGE;

        extern const int MINA_RESPONSE_MESSAGE;

        /**
         * Model消息处理
         */
        extern const int MODEL_MESSAGE;
        
        extern const int RPC_MESSAGE_WITH_ERROR_CODE;
        
        /**
         * Layer管理器发出的layer相关系消息
         */
        extern const int LAYER_MESSAGE;
        
        /**
         * 用于数据同步的消息
         */
        extern const int DATA_SYNC_MESSAGE;
        
        /**
         * 异步加载ZCI资源返回的消息
         */
        extern const int LOAD_ZCI_ASYNC_RETURN;
        
        /**
         * 系统的一些消息
         */
        extern const int SYSTEM_MESSAGE;
        
    }

    namespace LogicMessage {
        extern const int BATTLE_ARRAY_DRAGGED;
        extern const int READY_FIGHT;
        
        extern const int EVOLVE_BUTTON_CLICK;
        extern const int UPGRADE_BUTTON_CLICK;
        extern const int HERO_ITEM_BUTTON_CLICK;
        extern const int GACHA_END;
        extern const int BACK_TO_HOME_CLICK;
        extern const int UPGRADE_END;
        extern const int EVO_UPGRADE_END;
        extern const int HOME_BUTTON_CLICK;
        extern const int PAUSE_GUIDE;
        extern const int RESUME_GUIDE;
        
        extern const int SETTLEMENT_ADD_FRIEND_CLICK;
        extern const int ATTACK_BUTTON_CLICK;
        extern const int LIST_ANIM_END;
        
        extern const int APP_RESUME;
        extern const int BATTLE_PRELOAD_COMPLETE;
        extern const int UI_PRELOAD_COMPLETE;
        extern const int FIGHT_THROUGH;
        extern const int SHOW_CHANGE_NAME;
        extern const int SETTLEMENT_LEVEL_ANIMA_END;
        extern const int DROP_DOWN_BOX_DROP_DOWN;
        extern const int BIND_ACCOUNT_SUCCESS;
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

    /**************** Network message php *********************/
    namespace NetStateMessage {
        extern const int PHP_TIME_OUT;
        extern const int PHP_NETWORK_ERROR;
    }
    
    namespace DataSyncMessage {
        extern const int SYNC_CARD_LIST;
        extern const int CLEAR_GUIDE_DUNGEON;
    }
    
    namespace SystemMessage {
        extern const int CLIENT_VERSION_LOW_POP;
    }

    namespace ModelMessage {
        /****************Upgrate Layer Message******************/
        extern const int UPGRADE_CARD;
        extern const int UPGRADE_VIEW_REFRESH;
        
        /****************Evolve Layer Message******************/
        extern const int EVOLVE_CARD_FINISH;
        extern const int EVOLVE_INSTALL;
        
        /****************CardHeroLayer Message******************/
        extern const int CARD_HERO_REFRESH;
        extern const int INIT_NEW_MAN_CARDS;
        extern const int PROTECT_CARD;
        extern const int SWITCH_CARD_SORT_KEY;
        extern const int SELL_CARDS;
        
        /****************FriendListLayer Message******************/
        extern const int FRIEND_LIST_REFRESH;
        extern const int FRIEND_SEARCH_RETURN;
        extern const int SEND_FRIEND_REQUEST_SUCCESS;
        extern const int ACCEPT_ENERGY_SUCC;
        extern const int SEND_ENERGY_SUCC;
        
        /****************MissionLayer Message******************/
        extern const int MISSION_LIST_REFRESH;
        extern const int MISSION_REWARDED;
        extern const int ACHIVE_REWARDED;
        extern const int SET_INVITE_SUCC;
        extern const int INVITE_REWARDED;
        extern const int PVP_MISSION_REWARDED;
        
        /****************DungeonLayer Message******************/
        extern const int DUNGEON_LIST_REFRESH;
        extern const int DUNGEON_LIST_REBUILD;
        
        /****************HomeLayer Message******************/
        extern const int USER_INFO_REFRESH;
        extern const int SLOT_CARDS_REFRESH;
        
        /****************MailListLayer Message******************/
        extern const int MAIL_LIST_REFRESH;
        extern const int MAIL_DELETE_SUCCESS;
        extern const int MAIL_ACCEPT_ATTACHMENT_SUCCESS;
        extern const int SEND_MAILL_SUCC;
        
        /****************BattleUILayer Message******************/
        extern const int BATTLE_UI_REFRESH;
        extern const int BATTLE_SPEED_REFRESH;
        
        /****************BoardCast Message******************/
        extern const int BOARDCAST_REFRESH;
        
        /****************Store Message******************/
        extern const int GACHA_CARD_REFRESH;
        extern const int GACHA_TURN_CARD;
        
        /****************Guide Message******************/
        extern const int GUIDE_SMALL_STEP_CHANGED;
        extern const int GUIDE_BUTTON_CLICK;
        extern const int GUIDE_START_FIGHT;
        
        /****************Upgrade Skill Message******************/
        extern const int SKILL_SELECT_CHANGE;
        extern const int SKILL_UPGRADE_RETURN;
        extern const int LEARN_SKILL_REFRESH;
        extern const int RESET_LEARN_SKILL_SUCC;
        
        /****************Purchase Message******************/
        extern const int PURCHASE_EXTEND_PACKAGE;
        extern const int PURCHASE_EXTEND_FRIEND_LIMIT;
        extern const int PURCHASE_ENERGY;
        
        /****************Arena Message******************/
        extern const int ARENA_INFO_REFRESH;
        extern const int ACCEPT_ARENA_DAILY_REWARD_SUCC;
        extern const int FINISH_LOAD_ARENA_BATTLE_INFO;
        extern const int INSPIRE_SUCC;
        
        /****************Purchase Message******************/
        
        /****************Skill Book Message******************/
        extern const int SKILL_ITEM_REFRESH;
        
        /****************Material Message******************/
        extern const int MATERIAL_REFRESH;
        extern const int MATERIAL_LIST_STATE_REFRESH;
        
        /****************Card Piece Message******************/
        extern const int CARD_PIECE_COMBINED;
        extern const int CARD_PIECE_REFRESH;
        
        /****************Edit Name Message******************/
        extern const int CHANGE_NAME_CLICK;
    }

}

#endif // MESSAGECONSTANT_H
