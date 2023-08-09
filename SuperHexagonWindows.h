typedef unsigned char   undefined;

typedef unsigned char    byte;
typedef unsigned int    dword;
typedef long long    longlong;
typedef unsigned char    uchar;
typedef unsigned int    uint;
typedef unsigned int    uint3;
typedef unsigned long    ulong;
typedef unsigned long long    ulonglong;
typedef unsigned char    undefined1;
typedef unsigned short    undefined2;
typedef unsigned int    undefined4;
typedef unsigned long long    undefined8;
typedef unsigned short    ushort;
typedef unsigned short    word;
typedef struct superhex_vftable superhex_vftable, *Psuperhex_vftable;

struct superhex_vftable { /* placeholder struct 'cause ghidra won't let me specify the type directly */
    void * func[18];
};

typedef struct superhex superhex, *Psuperhex;

typedef struct gameclass_impl gameclass_impl, *Pgameclass_impl;

typedef struct graphicsclass_impl graphicsclass_impl, *Pgraphicsclass_impl;

typedef ulonglong uint64_t;

typedef enum class Rank {
    Point=0,
    Line=1,
    Triangle=2,
    Square=3,
    Pentagon=4,
    Hexagon=5,
    Awesome=6
} Rank;

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
    char field10_0x31[299];
    float do_screen_flash; /* once set, screen goes white for the amount of jiffies set */
    char field12_0x160[24];
    int jiffies_after_game_is_lost; /* see gamelogic for comments */
    char field14_0x17c[4];
    int camera_zoom; /* no matter what it's set to, it zooms back to 40 fairly quickly when in game */
    char field16_0x184[24];
    float palette_current_color_offset_mayb; /* goes 0 to 255 float */
    char field18_0x1a0[20];
    int game_state; /* 181 when playing first stage, 90 when playing others, 0 for stage select, 398 for starting screen */
    char field20_0x1b8[12];
    char field21_0x1c4[10060]; /* 0x2894 */
    int check_collisions_flag;
    float current_time; /* jiffies, 1 second = 60 jiffies */
    char field24_0x2918[40]; /* 0x28c0 */
    double time_delta;
    char field26_0x2948[4]; /* 0x28d8 */
    float obstacle_generation_delay;
    int waves_generated; /* starting value for hypermodes is 51 */
    float float2; /* rate at which float 1 decreases during game, float1 then reaches 0 and goes to 10 */
    float obstacle_movespeed;
    char field31_0x295c[4];
    float float1; /* changes from -40 to 100, somehow affects obstacle creation */
    char field33_0x2964[12];
    int arrow_rotation; /* 0 to 0x168 */
    int arrow_speed; /* 0x2b88 */
    float collision_detected; /* any non-zero value appears to translate to true, although constansts 1.0 and 2.0 are used in code, i have no idea about their possible meaning */
    char field37_0x297c[4];
    int camera_rotation_type; /* values higher than 7 just lock it in place the way it's seen in secret stage */
    char field39_0x2984[52];
    int stage_rank_maybe_best; /* maybe this is current best */
    enum Rank stage_rank; /* -1 when not in a game */
    char field42_0x29c0[10936];
    int do_zoom_out_visual_effect;
    char field44_0x547c[20];
    int do_a_spin;
    int do_switch_colors_flag;
    char field47_0x5498[8];
    float obstacle_generation_freeze_duration;
    int stage_current; /* stage number while playing, they go: 0, 1, 2, 0(hyper flag), 1(hyper flag), 2(hyper flag); stage 4 is used in secret ending, stage 3 is empty but the time counts */
    char field50_0x54a8[8];
    int hyper_hexagon_unlocked;
    int hyper_hexagoner_unlocked;
    int hyper_hexagonest_unlocked;
    char field54_0x54bc[8];
    char field55_0x54c4[32];
    int stage_selected; /* used on stage select, they go: 0, 5, 4, 3, 2, 1, see Stage enum */
    char field57_0x54e8[4];
    int show_secret_ending_change_scene; /* didn't investigate this one enough, it's set in winlevel when hexagon is reached on non-hyper hexagonest but in the code it triggers sequence similar to secret scene */
    char field59_0x54f0[4];
    int some_kind_of_wave_flag;
    char switch_to_hypermode; /* once set, the gamemode becomes hyper */
    char field62_0x54f9[31];
    int rank1_jiffies;
    int rank2_jiffies;
    int rank3_jiffies;
    int rank4_jiffies;
    int rank5_jiffies;
    int rank6_jiffies;
    char field69_0x5530[292];
    char switch_to_hypermode_on_restart;
    char field71_0x5655[3];
    int stage_on_restart;
    int switch_stage_on_restart2;
    int adjust_tme_on_restart;
    int switch_stage_on_restart;
    int adjust_tme_on_restart2;
    char field77_0x566c[52];
    uint64_t secret_ending_timer; /* in milliseconds */
    char field79_0x56a8[456];
    uint64_t scoreclass;
    char field81_0x5878[16120];
    uint64_t xmlsettings1; /* type is ofxXmlSettings */
    char field83_0x9778[128];
    uint64_t xmlsettings2;
    char field85_0x9800[128];
    uint64_t some_string;
    char field87_0x9888[144];
};

struct superhex { /* a total of 0x40d00 is allocated for this struct */
    struct superhex_vftable * vftable;
    char field1_0x4[12];
    struct gameclass_impl gameclass;
    char field3_0x9928[26768]; /* 0x9928 */
    struct graphicsclass_impl graphicsclass;
    uint64_t musicclass;
    char field6_0x226a0[121472];
    uint64_t helpclass;
    char field8_0x40128[112];
    uint64_t deviceclass;
    char field10_0x401a0[1896];
    double last_frame_time;
    int game_framerate1;
    char field13_0x40914[4];
    double game_frame_time;
    int game_framerate2;
    int game_framerate3;
    double game_time_elapsed_total;
    char field18_0x40930[976];
};

