#include "MessageConstant.h"

namespace MessageConstant
{
    namespace MessageType {
        const int GENERAL_MESSAGE = 0;
        const int KEY_MESSAGE = 1;
        const int TOUCHSCREEN_MESSAGE = 2;
        const int COLLISION_MESSAGE = 3;
        /**
        * 游戏逻辑信息
        */
        const int LOGIC_MESSAGE = 4;

        /**
        * 窗口管理信息
        */
        const int WINDOW_MESSAGE = 5;

        /**
        * 网络状态通知消息
        */
        const int NETWORK_STATE_MESSAGE = 6;

        /**
        * RPC响应
        */
        const int RPC_MESSAGE = 7;

        const int MINA_RESPONSE_MESSAGE = 8;

        /**
         * Model消息处理
         */
        const int MODEL_MESSAGE=9;
        
        const int RPC_MESSAGE_WITH_ERROR_CODE = 10;
        
        const int LAYER_MESSAGE = 11;
        
        /**
         * 用于数据同步的消息
         */
        const int DATA_SYNC_MESSAGE = 12;
        
        /**
         * 异步加载ZCI资源返回的消息
         */
        const int LOAD_ZCI_ASYNC_RETURN = 13;
        
        /**
         * 系统的一些消息
         */
        const int SYSTEM_MESSAGE = 14;
        
    }

    /**************** Network message php *********************/
    namespace NetStateMessage {
        const int PHP_TIME_OUT = 100;
        const int PHP_NETWORK_ERROR = 101;
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
    
    namespace LogicMessage {
        const int BATTLE_ARRAY_DRAGGED = 400;
        const int READY_FIGHT = 401;
        
        const int EVOLVE_BUTTON_CLICK = 500;
        const int UPGRADE_BUTTON_CLICK = 501;
        const int HERO_ITEM_BUTTON_CLICK = 502;
        const int GACHA_END = 503;
        const int BACK_TO_HOME_CLICK = 504;
        const int UPGRADE_END = 505;
        const int EVO_UPGRADE_END = 506;
        const int HOME_BUTTON_CLICK = 507;
        const int PAUSE_GUIDE = 508;
        const int RESUME_GUIDE = 509;
        const int SETTLEMENT_ADD_FRIEND_CLICK = 510;
        const int ATTACK_BUTTON_CLICK = 511;
        const int LIST_ANIM_END = 512;
        
        const int APP_RESUME = 515;
        const int BATTLE_PRELOAD_COMPLETE = 516;
        const int UI_PRELOAD_COMPLETE = 517;
        const int FIGHT_THROUGH = 519;
        const int SHOW_CHANGE_NAME = 520;
        const int SETTLEMENT_LEVEL_ANIMA_END = 521;
        
        const int DROP_DOWN_BOX_DROP_DOWN = 522;
        
        const int BIND_ACCOUNT_SUCCESS = 550;
    }
    
    namespace DataSyncMessage {
        
        const int SYNC_CARD_LIST = 600;
        const int CLEAR_GUIDE_DUNGEON = 601;
        
    }
    
    namespace SystemMessage {
        const int CLIENT_VERSION_LOW_POP = 700;
    }

    namespace ModelMessage {
        
        /****************Upgrate Layer Message******************/
        const int UPGRADE_CARD = 901;
        const int UPGRADE_VIEW_REFRESH = 902;
        
        /****************Evolve Layer Message******************/
        const int EVOLVE_CARD_FINISH = 911;
        const int EVOLVE_INSTALL = 912;
        
        /****************CardHeroLayer Message******************/
        const int CARD_HERO_REFRESH= 921;
        const int INIT_NEW_MAN_CARDS = 922;
        const int PROTECT_CARD = 923;
        const int SWITCH_CARD_SORT_KEY = 925;
        const int SELL_CARDS = 926;
        
        /****************FriendListLayer Message******************/
        const int FRIEND_LIST_REFRESH = 931;
        const int FRIEND_SEARCH_RETURN = 932;
        const int SEND_FRIEND_REQUEST_SUCCESS = 933;
        const int ACCEPT_ENERGY_SUCC = 934;
        const int SEND_ENERGY_SUCC = 935;
        
        /****************MissionLayer Message******************/
        const int MISSION_LIST_REFRESH = 941;
        const int MISSION_REWARDED = 942;
        const int ACHIVE_REWARDED = 943;
        const int SET_INVITE_SUCC = 944;
        const int INVITE_REWARDED = 945;
        const int PVP_MISSION_REWARDED = 946;
        
        /****************DungeonLayer Message******************/
        const int DUNGEON_LIST_REFRESH = 951;
        const int DUNGEON_LIST_REBUILD = 952;
        
        /****************HomeLayer Message******************/
        const int USER_INFO_REFRESH = 961;
        const int SLOT_CARDS_REFRESH = 962;
        
        /****************MailListLayer Message******************/
        const int MAIL_LIST_REFRESH = 971;
        const int MAIL_DELETE_SUCCESS = 972;
        const int MAIL_ACCEPT_ATTACHMENT_SUCCESS = 973;
        const int SEND_MAILL_SUCC = 974;
        
        /****************BattleUILayer Message******************/
        const int BATTLE_UI_REFRESH = 981;
        const int BATTLE_SPEED_REFRESH = 982;
        
        /****************BoardCast Message******************/
        const int BOARDCAST_REFRESH = 991;
        
        /****************Store Message******************/
        const int GACHA_CARD_REFRESH = 1001;
        const int GACHA_TURN_CARD = 1002;
        
        /****************Guide Message******************/
        const int GUIDE_SMALL_STEP_CHANGED = 1101;
        const int GUIDE_BUTTON_CLICK = 1102;
        const int GUIDE_START_FIGHT = 1103;
        
        /****************Upgrade Skill Message******************/
        const int SKILL_SELECT_CHANGE = 1111;
        const int SKILL_UPGRADE_RETURN = 1113;
        const int LEARN_SKILL_REFRESH = 1114;
        const int RESET_LEARN_SKILL_SUCC = 1115;
        
        /****************Purchase Message******************/
        extern const int PURCHASE_EXTEND_PACKAGE = 1121;
        extern const int PURCHASE_EXTEND_FRIEND_LIMIT = 1122;
        extern const int PURCHASE_ENERGY = 1123;
        
        /****************Arena Message******************/
        extern const int ARENA_INFO_REFRESH = 1131;
        extern const int ACCEPT_ARENA_DAILY_REWARD_SUCC = 1132;
        extern const int FINISH_LOAD_ARENA_BATTLE_INFO = 1133;
        extern const int INSPIRE_SUCC = 1134;
        
        /****************Purchase Message******************/
        
        /****************Skill Book Message******************/
        extern const int SKILL_ITEM_REFRESH = 1301;

        /****************Material Message******************/
        extern const int MATERIAL_REFRESH = 1401;
        extern const int MATERIAL_LIST_STATE_REFRESH = 1402;
        
        /****************Card Piece Message******************/
        extern const int CARD_PIECE_COMBINED = 1501;
        extern const int CARD_PIECE_REFRESH = 1502;
        
        /****************Edit Name Message******************/
        extern const int CHANGE_NAME_CLICK = 1601;
    }
    
    
}
