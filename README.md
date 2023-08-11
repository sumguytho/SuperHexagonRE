# Intro

Before I begin I got to say all of this was made possible because Terry posted
an unstripped binary for linux version of the game which made the whole
process a lot faster than it would have been otherwise. All the explanations and
code examples are based on linux version of the game but they mostly apply to
windows as well since they share the same codebase when it comes to game
logic (though there are some slight differences, perhaps, they are only due to
different optimizations). Whenever I show code don't mind the irrelevant variable
names, I've given up trying to split register variables in ghidra out.

The notes here may contradict whatever is written in header files for game structures.
The values, variable types and their offsets are correct but their decriptions might
be off, whatever is written in this README takes precedence over header files.

The offsets for windows might be off though, everything that isn't a part of the
patch (explanations and offsets wise) was tested on linux.

## Directory contents

 - other/dump_prints.gdb - a gdb script I used to get the locations of calls to
graphicsclass::print along with strings being printed and their colours
 - other/cull_redundant_prints.py - a python script used to cull redundant strings
in the text file produced by gdb script
 - SuperHexagon.exe.gzf and SuperHexagon.gzf - [Ghidra](https://github.com/NationalSecurityAgency/ghidra)
projects with all the stuff I could figure out about the game, version 10.3.2 is required to open them
 - SuperHexagonLinux.h and SuperHexagonWindows.h - structures exported from Ghidra
projects, everything present in one file but not another is same for both versions of the game

# Game mechanics

## Timer and game jiffies

The game is locked at 60 fps and counts time in 1/60th of a second jiffies.
The fractional part of the timer goes 0 to 59 which is very weird
considering we as humans are used to decimal fractions of a second, i suspect
the function that sets timer text just divides time by 60 and uses quotient
as minutes and remainder as "milliseconds".

## The stages

During the game there are 7 stages only 3 of which are used for the gameplay itself (stage_current variable).

 - stage 0 is hexagon
 - stage 1 is hexagoner
 - stage 2 is hexagonest
 - stage -2 is stage select
 - stage -1 is main menu
 - stage 3 is a stage with no obstacles spawning but with gui and the timer actually
works there
 - stage 4 is used for the "secret ending", you don't see anything other than the game field

All other stage numbers make the game identical in behaviour to that of stage 3 except that the
timer doesn't work (it shows some unrelated number).

Stage select uses different numbers for stages (stage_selected variable):

 - hexagon:             0
 - hexagoner:           5
 - hexagonest:          4
 - hexagonester:        3
 - hexagonestest:       2
 - hexagonestestest:    1

The hyper stages begin when you reach a 60 seconds mark on one of the usual levels and launching
a hyper stage from stage select isn't an exception from this. When you launch some hyper stage
from stage select the switch_to_hypermode flag is set. This flag is checked for a bunch of different
stuff but when it comes to the actual gameplay its main effect is that it adds one minute to the game
time in the part of the game logic where:

 - the obstacles are created
 - the speed of the obstacles is determined
 - the camera rotation kind is determined (its speed and direction)
 - camera angle is determined
 - some extra visual effects (for example, do a zoomout every n waves)

Another thing that happens when you start a hyper stage from stage select is that the number of waves
generated is set to 0x33 (51). There may be some other stuff which I've missed that doesn't seem to
affect gameplay anyway.

When you play a normal stage once you reach a minute mark the palette is changed and nothing else:
by that time your game timer and number of waves generated are what they need to be to start getting
hypermode behaviour from the game, hence, the switch_to_hypermode isn't set (and besides, setting it
would make the game set the record for the wrong stage since it's used to calculate which stage to save
the score for).

## Obstacle generation

The part of code in question is kinda big and the code sharing here makes the individual branches
hard to distinguish but in short there are a lot of conditions to determine a way to generate a
random number which is then used as a third varible to determine the things mentioned above (the ones
between a minute addition and substraction), the first two being current value of game timer and
number of waves generated. As an example:

```
random_num3 = (this->gameclass).current_time;
if (7200.0 < random_num3) {
    hyper_hexagonest_spawns_7200 = (float)ofRandom(7.0);
    hyper_hexagonest_spawns = (int)hyper_hexagonest_spawns_7200;
}
else if (random_num3 <= 3600.0) {
    hyper_hexagonest_spawns_0 = (float)ofRandom(3.0);
    hyper_hexagonest_spawns = (int)hyper_hexagonest_spawns_0;
}
else {
    hyper_hexagonest_spawns_3600 = (float)ofRandom(4.0);
    hyper_hexagonest_spawns = (int)hyper_hexagonest_spawns_3600;
}
if (hyper_hexagonest_spawns == 0) goto LAB_0047307d;
if (hyper_hexagonest_spawns == 1) goto LAB_004731af;
```

What happens is that your current time on the stages determines the "selector" for the kind of waves
the game may generate, what I didn't show here is that there also conditions for the number of waves
generated but then again the code is too big to fit it all here.


## Game start and stop markers

From what I've figured out, the main variables that control whether the game is going on or not are
stage_current and jiffies_after_game_is_lost (a horrible variable name, couldn't come up with a better one).
There sure is more to this than just changing 2 variables but there's still this overall weirdness that simply
changing the value of a variable that holds the number of the current stage can launch the game:

![change_stage](https://github.com/sumguytho/SuperHexagonRE/blob/master/gifs/change_stage.gif)

If you set jiffies_after_game_is_lost to 0 after the game is lost, the game will continue as if nothing has happened,
you will even retain the time you spent on the stage:

![set_jiffies](https://github.com/sumguytho/SuperHexagonRE/blob/master/gifs/set_jiffies.gif)

I have no idea whether the game started and the game going on are distinct states but if you switch stage
to some playable one and then back to -2 you will get this:

![menu_play](https://github.com/sumguytho/SuperHexagonRE/blob/master/gifs/menu_play.gif)

It doesn't generate waves in the main menu though, once the already generated ones pass everything is back to how it
used to be.

## What makes the last stage

Once you reach the 2 minute mark on hexagonest (one minute if you start in hypermode)
the game suddenly starts acting as if it were the last stage. It also checks whether colour palette is that
of the last stage and if it isn't it does the following:

 - set colour palette to the palette of the last stage
 - clear all obstacles
 - do a zoomout + spin
 - flash the screen for 10 game time jiffies (~166 millis)
 - postpone obstacle generation by 180 game jiffies (3 seconds)

There is also a grace period of 13.(3) seconds after you've reached the last stage when the
camera doesn't spin.

Here are the conditions to start getting fully-fledged last stage behaviour:

 - have at least 133.(3) seconds on the stage hexagonest (number 2)
 - have at least 0x33 (51) waves generated
 - have 0x1e as stage theme

# How comprehensive are the modifications?

I wanna think I made everything necessary to replicate the last stage behaviour
although there is always room for doubt. I have an example of how what you would
think should be happening in game logic isn't how it's done actually.

For some time I couldn't figure out how to change colour of the text for the stage in
stage select and the reason is that its colour is detached from
the palette in graphics class. Everytime setpal is called the
shapes on the screen change colours but not the text. You could have noticed
that the hyper hexagonest and the last stage that comes after it use black colour
for text in some labels unlike every other stage that comes before them.

As it turned out, there are drawgui_* functions that are called somewhere from SDL2
either before or after superhex::update which check the value of stage_selected
to determine the text to draw and then the colour of some labels. What's more,
it's done twice. First, the game dips into the game state to pick the stage name (as well
as labels below it like best time, etc.) to draw and then it does so again to determine
the colour for the said labels which should already be known since the game has just
drawn those other stage specific lables.

Even then there's still button outlines which I didn't do correctly, they are probably
handled in some other function. Dipping in the game state. On a separate occasion. Again.

This example is my way of saying that the possibilities of missing something out when
modifying this game are limitless.

# Structure fields

The rest of the document is dedicated to describing the fields of superhex structure that
I have made some observations on. Some are pretty self evident and I just wanted to
provide additional context, some I've left out because I just didn't have anything on them.

## graphics_palette_state

I have no idea what this one does, the only thing I was able to figure out is
that when the game needs to change the graphics theme while the game is running
it won't do so unless this variable is set to 0.

---

## graphics_theme

Keeps track of which game palette is currently used.

---

## keybd_left
## keybd_right
## keybd_enter
## keybd_esc
## keybd_tab

Self evident.

These variables are set in superhex::generickeypoll. The name of this function is a little bit misleading
since it doesn't poll anything in the usual sense, it just tests some variables from
deviceclass and sets these ones accordingly meanwhile the actual keyboard polling is probably done somewhere in SDL2.

---

## do_screen_flash

Once you set this variable, the screen will go white for the amount of time specified (in game jiffies, of course).
This is actually a common pattern for visual effects: once you set a varible to 1 it's then used as a state of that
visual effect and goes back to 0 once the visual effect has been finalized.

---

## jiffies_after_game_is_lost

I named this variable exactly as i saw it: it's 0 unless the game has ended, then it grows to
100 and stays at this value until you either restart the stage or go to stage select, after which it
goes back to 0. I imagine this variable is used as a state of score screen, can't tell for sure though.

---

## camera_zoom

The bigger the value, the bigger the hexagon in the centre of the screen. There is almost always
some value this one should go to on every stage and if you change it the game will bring it back to
that value with corresponding visual effect:

![camera_zoom](https://github.com/sumguytho/SuperHexagonRE/blob/master/gifs/camera_zoom.gif)

---

## palette_current_color_offset_mayb

I imagine this one is used to advance colours on hexagonest stage, it goes 0 to 255. Requires additional
testing to tell for sure (I'm not gonna do that).

---

## game_state

I couldn't figure out the logic of this one. It's set to different values at different game
screens (at stage select, while playing, at score screen) and I would think that these values are
used by the game to differentiate between the states of the game if it wasn't for the fact that those
values are different between game restarts (each "game state value" is like +/- 1 between restarts, e.g.,
while playing it's 180 and in stage select it's 80, after a restart it's 181 and 81 for the same respective scenes).

---

## check_collisions_flag

Self evident.

It's probably rather the number of obstacles currently on the screen, didn't test enough though.

---

## current_time

The game time on current stage in jiffies, 1 second = 60 jiffies.

---

## time_delta

The number of game jiffies passed since the last call to superhex::update.

What's funny about this one is that even though it's almost always set to 1 its type is
double meanwhile the type of current_time, a variable which may and will go hundreds of times
higher than this one, is float.

---

## obstacle_generation_delay

The time in jiffies before the next wave is generated. A wave may be comprised of multiple obstacles.

---

## waves_generated

The amount of waves generated up to this point in time.

---

## obstacle_movespeed

Self evident.

The game has no protection against this variable being set to some value the game doesn't expect.
If you set it too low the game will crash due to obstacles overflowing whatever buffer they are written
to when created. Random demo:

![speedy_bois](https://github.com/sumguytho/SuperHexagonRE/blob/master/gifs/speedy_bois.gif)

---

## arrow_rotation

Self evident. Goes from 0 to 0x167 (359). It follows that everytime you see values like
0x3c (60) and 0x1e (30) being used it's probably arrow rotation calculations. For example,
consider this code:

```
if ((float)((double)fVar20 - time_delta) <= 0.0) {
    *(undefined4 *)(this->gameclass).field56_0x5450 = 0;
    arrow_rotation = ((this->gameclass).stage_selected + 5) % 6;
    (this->gameclass).stage_selected = arrow_rotation;
    arrow_rotation = arrow_rotation * 0x3c + 0x1e;
    (this->gameclass).arrow_rotation = arrow_rotation;
}
```
it's quite obvious it's used to place arrow in the middle of some edge of the hexagon in
the middle of the screen.

---

## arrow_speed

Self evident.

9 for hexagonest, its hypermode and last stage, 4 for main menu (might be a placeholder value),
7 everywhere else.

---

## collision_detected

Set to 1 once a collision is detected. It's actually a float and there are several other values it's
set to, although they all seem to mean the same: game over. Perhaps the reason it's set to several
values is because it has the visual effect behaviour. Which sounds possible except that there's this
inconsistency that visual effect stateful variables are int and this one is float.

---

## camera_rotation_type

Controls the way the camera is spun, for example, when it's 0 the camera slowly spins counter-clockwise
(or clockwise, don't remember, doesn't matter), when it's 1 it does the same in the opposite direction
and this pattern persists until it's lower than 7, values greater or equal to 7 just lock it in place
the way it's seen in the last stage.

---

## stage_rank_maybe_best

I imagine this is the highest rank you've reached on the current stage.

---

## stage_rank

Self evident.

See Rank enum.

---

## do_zoom_out_visual_effect

Does a zoom out when set to 1:

![zoomout](https://github.com/sumguytho/SuperHexagonRE/blob/master/gifs/zoomout.gif)

---

## do_a_spin

Does a spin when set to 1:

![spin](https://github.com/sumguytho/SuperHexagonRE/blob/master/gifs/spin.gif)

---

## do_switch_colors_flag

Set to 1 for hexagonest and its hypermode, makes the field switch colours on adjacent lanes:

![colours](https://github.com/sumguytho/SuperHexagonRE/blob/master/gifs/colours.gif)

---

## obstacle_generation_freeze_duration

The time in game jiffies before the next wave is generated. If it's non-zero it's substracted
from instead of obstacle_generation_delay. In other words, the total time you will wait before
a wave is spawned is obstacle_generation_freeze_duration + obstacle_generation_delay, the
difference is that obstacle_generation_delay is usually set by wave generation logic and is the
main source of delays between waves meanwhile obstacle_generation_freeze_duration is used to
add one time delays. I don't see how setting this one is different from just adding to
obstacle_generation_delay, though.

---

## stage_current

The number of stage that you are currently playing.

---

## hyper_hexagon_unlocked
## hyper_hexagoner_unlocked
## hyper_hexagonest_unlocked

These flags control whether you can start the corresponding hypermode. If not set, you will see
a locked label instead of the stage itself in stage select.

---

## stage_selected

The number of stage currently selected in stage select, used to determine how to start the game
(which stage_current to pick, whether to set switch_to_hypermode flag).

---

## show_secret_ending_change_scene

Didn't look into this one enough, fairly certain it's set to 1 either when secret ending is shown or when the
secret ending can't be shown yet and you get an unavoidable obstacle instead.

---

## some_kind_of_wave_flag

Affects the kind of wave generated in some way.

---

## switch_to_hypermode

This one was explained earlier. You set it - you are in hypermode but not really because generated_waves.

---

## rank1_jiffies
## rank2_jiffies
## rank3_jiffies
## rank4_jiffies
## rank5_jiffies
## rank6_jiffies

The amount of time you have to spend on a stage in game jiffies to reach a certain rank. They are
initialized once in gameclass ctor:

```
(this->m_).rank1_jiffies = 600;
(this->m_).rank2_jiffies = 0x4b0; // 1200
(this->m_).rank3_jiffies = 0x708; // 1800
(this->m_).rank4_jiffies = 0xa8c; // 2700
(this->m_).rank5_jiffies = 0xe10; // 3600
(this->m_).rank6_jiffies = 0x1c20; // 7200
```

(this->m_ is just gameclass)

One notable thing is that the rank is handled before a minute is conditionally added to current_time
(through switch_to_hypermode check) because obviously how else would that work on hyper stages.

---

## switch_to_hypermode_on_restart
## stage_on_restart
## switch_stage_on_restart2
## adjust_tme_on_restart
## switch_stage_on_restart
## adjust_tme_on_restart2

Didn't look too much into these, they are set in gameclass::changetostage and are responsible
for bringing you back to the stage you've started the game on. One such case is when you reach
a 3 minute mark on some stage, are advanced to the next stage and then lose and restart the game.

switch_to_hypermode_on_restart controls whether the restart will be in hypermode.

---

## secret_ending_timer

Didn't look too much into this one, it's used as a timer for the logic of secret ending,
no idea whether it's in game jiffies of millis, the way it's incremented is somewhat obscure
(it's done through other variables from game structure). Its max value is 0x29040 (168000,
which would make 2800 seconds and it definitely doesn't last almost an hour).

---
