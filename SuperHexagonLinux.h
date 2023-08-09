typedef unsigned char   undefined;

typedef unsigned char    byte;
typedef unsigned char    dwfenc;
typedef unsigned int    dword;
typedef unsigned long    qword;
typedef unsigned char    uchar;
typedef unsigned int    uint;
typedef unsigned long    ulong;
typedef unsigned long long    ulonglong;
typedef unsigned char    undefined1;
typedef unsigned short    undefined2;
typedef unsigned int    undefined3;
typedef unsigned int    undefined4;
typedef unsigned long    undefined5;
typedef unsigned long    undefined6;
typedef unsigned long    undefined8;
typedef unsigned short    ushort;
typedef unsigned short    word;

// changed all enums to enum classes as to not flood the global namespace
typedef enum class Graphicstheme {
    Hexagon=0,
    Hexagonester=3,
    Hexagoner=5,
    Hexagonestest=6,
    Hexagonestestest=9,
    Hexagonest=10,
    LastStage=30,
    Error=100,
    SomeEffect=1000
} Graphicstheme;

typedef enum class StageSelected {
    Hexagon=0,
    Hexagoner=5,
    Hexagonest=4,
    Hexagonester=3,
    Hexagonestest=2,
    Hexagonestestest=1
} StageSelected;

typedef enum class StageRunning { /* -1 is main screen, -2 is stage select, ghidra won't let me add these */
    StageSelect = -2,
    MainMenu = -1,

    Hexagon=0,
    Hexagoner=1,
    Hexagonest=2,
    Empty=3,
    SecretEnding=4
} StageRunning;

typedef enum class Rank {
    Point=0,
    Line=1,
    Triangle=2,
    Square=3,
    Pentagon=4,
    Hexagon=5,
    Awesome=6
} Rank;

typedef enum class MusicNum { /* music plays a role in visual effects as well, the hexagon in center twitches according to it */
    Hexagon=1,
    Hexagoner=2,
    Hexagonest=3,
    LastStage=4
} MusicNum;

typedef struct superhex superhex, *Psuperhex;

typedef struct gameclass_impl gameclass_impl, *Pgameclass_impl;

typedef struct graphicsclass_impl graphicsclass_impl, *Pgraphicsclass_impl;

typedef ulonglong uint64_t;

struct graphicsclass_impl {
    uint64_t graphicsclass; /* no idea how big this type is */
    char field1_0x8[10048];
    int graphics_palette_state; /* setpal is only called when it's 0 */
    int graphics_theme; /* see Graphicstheme enum, used to call this menutheme, any references to menutheme actually mean graphics theme */
    char field4_0x2750[64912]; /* 0x1e4a8 */
};

struct gameclass_impl {
    uint64_t gameclass;
    char field1_0x8[33];
    char keybd_idk1;
    char keybd_idk2;
    char keybd_left;
    char keybd_right;
    char keybd_enter;
    char keybd_idk3;
    char keybd_esc;
    char keybd_tab;
    char field10_0x31[159];
    float do_screen_flash; /* once set, screen goes white for the amount of jiffies set */
    char field12_0xd4[24];
    float jiffies_after_game_is_lost; /* see gamelogic for comments */
    char field14_0xf0[4];
    int camera_zoom; /* no matter what it's set to, it zooms back to 40 fairly quickly when in game */
    char field16_0xf8[24];
    float palette_current_color_offset_mayb; /* goes 0 to 255 float */
    char field18_0x114[20];
    int game_state; /* 181 when playing first stage, 90 when playing others, 0 for stage select, 398 for starting screen */
    char field20_0x12c[10060]; /* 0x2894 */
    int check_collisions_flag;
    float current_time; /* jiffies, 1 second = 60 jiffies */
    char field23_0x2880[40]; /* 0x28c0 */
    double time_delta;
    char field25_0x28b0[4]; /* 0x28d8 */
    float obstacle_generation_delay;
    int waves_generated; /* starting value for hypermodes is 51 */
    float float2; /* rate at which float 1 decreases during game, float1 then reaches 0 and goes to 10 */
    float obstacle_movespeed;
    char field30_0x28c4[4];
    float float1; /* changes from -40 to 100, somehow affects obstacle creation */
    char field32_0x28cc[12];
    int arrow_rotation; /* 0 to 0x168 */
    int arrow_speed; /* 0x2b88 */
    float collision_detected; /* any non-zero value appears to translate to true, although constansts 1.0 and 2.0 are used in code, i have no idea about their possible meaning */
    char field36_0x28e4[4];
    int camera_rotation_type; /* values higher than 7 just lock it in place the way it's seen in last stage */
    char field38_0x28ec[52];
    int stage_rank_maybe_best; /* maybe this is current best */
    enum Rank stage_rank; /* -1 when not in a game */
    char field41_0x2928[10936];
    int do_zoom_out_visual_effect;
    char field43_0x53e4[20];
    int do_a_spin;
    int do_switch_colors_flag;
    char field46_0x5400[8];
    float obstacle_generation_freeze_duration;
    int stage_current; /* stage number while playing, they go: 0, 1, 2, 0(hyper flag), 1(hyper flag), 2(hyper flag); stage 4 is used in secret ending, stage 3 is empty but the time counts */
    char field49_0x5410[8];
    int hyper_hexagon_unlocked;
    int hyper_hexagoner_unlocked;
    int hyper_hexagonest_unlocked;
    char field53_0x5424[8];
    char field54_0x542c[32];
    int stage_selected; /* used on stage select, they go: 0, 5, 4, 3, 2, 1, see Stage enum */
    char field56_0x5450[4];
    int show_secret_ending_change_scene; /* didn't investigate this one enough, it's set in winlevel when hexagon is reached on non-hyper hexagonest but in the code it triggers sequence similar to secret scene */
    char field58_0x5458[4];
    int some_kind_of_wave_flag;
    char switch_to_hypermode; /* once set, the gamemode becomes hyper */
    char field61_0x5461[31];
    int rank1_jiffies;
    int rank2_jiffies;
    int rank3_jiffies;
    int rank4_jiffies;
    int rank5_jiffies;
    int rank6_jiffies;
    char field68_0x5498[292];
    char switch_to_hypermode_on_restart;
    char field70_0x55bd[3];
    int stage_on_restart;
    int switch_stage_on_restart2;
    int adjust_tme_on_restart;
    int switch_stage_on_restart;
    int adjust_tme_on_restart2;
    char field76_0x55d4[52];
    uint64_t secret_ending_timer; /* in milliseconds */
    char field78_0x5610[456];
    uint64_t scoreclass;
    char field80_0x57e0[16120];
    uint64_t xmlsettings1; /* type is ofxXmlSettings */
    char field82_0x96e0[128];
    uint64_t xmlsettings2;
    char field84_0x9768[128];
    uint64_t some_string;
    char field86_0x97f0[144];
};


// a total of 0x3c758 is allocated at 0x41d875
// its global variable is at 0x7f5808
struct superhex {
    char field0_0x0[24];
    struct gameclass_impl gameclass;
    char field2_0x9898[10032];
    struct graphicsclass_impl graphicsclass;
    uint64_t musicclass;
    char field5_0x1e4b0[121472];
    uint64_t helpclass;
    char field7_0x3bf38[112];
    uint64_t deviceclass;
    char field9_0x3bfb0[1896];
    double last_frame_time;
    int game_framerate1;
    char field12_0x3c724[4];
    double game_frame_time;
    int game_framerate2;
    int game_framerate3;
    double game_time_elapsed_total;
};

typedef struct graphicsclass graphicsclass, *Pgraphicsclass;

struct graphicsclass { /* PlaceHolder Structure */
    struct graphicsclass_impl m_;
};

typedef struct gameclass gameclass, *Pgameclass;

struct gameclass { /* PlaceHolder Structure */
    struct gameclass_impl m_;
};
