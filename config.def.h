/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx = 2; /* border pixel of windows */
static const unsigned int gappx = 10;   /* gaps between windows */
static const unsigned int snap = 12;    /* snap pixel */
static const unsigned int systraypinning =
    0; /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor
          X */
static const unsigned int systrayonleft =
    0; /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2; /* systray spacing */
static const int systraypinningfailfirst =
    1; /* 1: if pinning fails, display systray on the first monitor, False:
          display systray on the last monitor*/
static const int showsystray = 1; /* 0 means no systray */
static const int showbar = 1;     /* 0 means no bar */
static const int topbar = 1;      /* 0 means bottom bar */

/*  Display modes of the tab bar: never shown, always shown, shown only in  */
/*  monocle mode in the presence of several windows.                        */
/*  Modes after showtab_nmodes are disabled.                                */
enum showtab_modes {
  showtab_never,
  showtab_auto,
  showtab_nmodes,
  showtab_always
};
static const int showtab = showtab_auto; /* Default tab bar show mode */
static const int toptab = True;          /* False means bottom tab bar */

static const char *fonts[] = {"Iosevka Nerd Font Propo:size=15"};
static const char dmenufont[] = "Iosevka Nerd Font Propo:size=15";
static const char col_gray1[] = "#222222";
static const char col_gray2[] = "#555555";
static const char col_white[] = "#ffffff";
static const char accent_color[] = "#fc0fc0";
static const char *colors[][3] = {
    /*                              fg                bg         border   */
    [SchemeNorm] = {col_white, col_gray1, col_gray2},
    [SchemeSel] = {col_gray1, accent_color, accent_color},
    [SchemeStatus] = {col_white, col_gray1,
                      "#000000"}, // Statusbar right {text,background,not used
                                  // but cannot be empty}
    [SchemeTagsSel] = {col_gray1, accent_color,
                       "#000000"}, // Tagbar left selected {text,background,not
                                   // used but cannot be empty}
    [SchemeTagsNorm] =
        {col_white, col_gray1,
         "#000000"}, // Tagbar left unselected {text,background,not used but
                     // cannot be empty}
    [SchemeInfoSel] =
        {col_gray1, accent_color,
         "#000000"}, // infobar middle  selected {text,background,not used but
                     // cannot be empty}
    [SchemeInfoNorm] =
        {col_white, col_gray1,
         "#000000"}, // infobar middle  unselected {text,background,not used but
                     // cannot be empty}
};

/* tagging */
static const char *tags[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};

static const Rule rules[] = {
    /* xprop(1):
     *	WM_CLASS(STRING) = instance, class
     *	WM_NAME(STRING) = title
     */
    /* class      instance    title       tags mask     isfloating   monitor */
    {"Catfish", NULL, NULL, 0, 1, -1},
    {"Parole", NULL, NULL, 0, 1, -1},
    {"Firefox", NULL, NULL, 1 << 8, 0, -1},
};

/* layout(s) */
#include "centeredmaster.c"
#include "fibonacci.c"
#include "tatami.c"

static const float mfact = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster = 1;    /* number of clients in master area */
static const int resizehints =
    0; /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen =
    1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
    /* symbol     arrange function */
    {"[]=", tile}, /* first entry is default */
    {"><>", NULL}, /* no layout function means floating behavior */
    {"[M]", monocle},  {"|M|", centeredmaster}, {"[@]", spiral},
    {"[\\]", dwindle}, {"[#]", tatami},         {">M<", centeredfloatingmaster},
};

/* window following */
#define WFACTIVE '>'
#define WFINACTIVE 'x'
#define WFDEFAULT WFACTIVE

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY, TAG)                                                      \
  {MODKEY, KEY, view, {.ui = 1 << TAG}},                                       \
      {MODKEY | ControlMask, KEY, toggleview, {.ui = 1 << TAG}},               \
      {MODKEY | ShiftMask, KEY, tag, {.ui = 1 << TAG}},                        \
      {MODKEY | ControlMask | ShiftMask, KEY, toggletag, {.ui = 1 << TAG}},

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd)                                                             \
  {                                                                            \
    .v = (const char *[]) { "/bin/sh", "-c", cmd, NULL }                       \
  }

/* commands */
static char dmenumon[2] =
    "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = {
    "dmenu_run", "-m",  dmenumon,     "-fn", dmenufont, "-nb", col_gray1, "-nf",
    col_white,   "-sb", accent_color, "-sf", col_gray1, NULL};

/* scratchpad */
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = {"st", "-t",    scratchpadname,
                                      "-g", "68x18", NULL};

static const char *termcmd[] = {"st", NULL};
/* custom commands */
static const char *brightnessUp[] = {"brightnessctl", "set", "+10%", NULL};
static const char *brightnessDown[] = {"brightnessctl", "set", "10%-", NULL};
static const char *browserCmd[] = {"firefox", NULL};
static const char *fileManagerCmd[] = {"pcmanfm", NULL};
static const char *whatsapp[] = {"wasistlos", NULL};
static const char *telegram[] = {"telegram-desktop", NULL};
static const char *screenshot[] = {"xfce4-screenshooter", NULL};
static const char *catfish[] = {"catfish", NULL};
static const char *emacs[] = {"emacs", NULL};
static const char *lock_screen[] = {"i3lock", "--color=#000000", NULL};

#include "movestack.c"
#include <X11/XF86keysym.h>
static const Key keys[] = {
    /* modifier     key               function       argument */

    /* Custom commands' bindings*/
    {0, XF86XK_AudioRaiseVolume, spawn,
     SHCMD("pamixer -i 5; kill -44 $(pidof dwmblocks)")},
    {0, XF86XK_AudioLowerVolume, spawn,
     SHCMD("pamixer -d 5; kill -44 $(pidof dwmblocks)")},
    {0, XF86XK_AudioMute, spawn,
     SHCMD("pamixer -t; kill -44 $(pidof dwmblocks)")},
    {0, XF86XK_MonBrightnessUp, spawn, {.v = brightnessUp}},
    {0, XF86XK_MonBrightnessDown, spawn, {.v = brightnessDown}},
    {0, XK_Print, spawn, {.v = screenshot}},
    {MODKEY, XK_f, spawn, {.v = browserCmd}}, // swapped with setlayout float
    {MODKEY, XK_e, spawn, {.v = fileManagerCmd}},
    {MODKEY, XK_w, spawn, {.v = whatsapp}},
    {MODKEY, XK_t, spawn, {.v = telegram}},
    {MODKEY, XK_c, spawn, {.v = catfish}},
    {MODKEY | ShiftMask, XK_e, spawn, {.v = emacs}},
    {MODKEY | ControlMask, XK_l, spawn, {.v = lock_screen}},

    /* toggle scratchpad */
    {MODKEY, XK_backslash, togglescratch, {.v = scratchpadcmd}},

    /* toggle window following */
    {MODKEY | ShiftMask, XK_n, togglefollow, {0}},

    /* window tab mode */
    {MODKEY | ShiftMask, XK_t, tabmode, {showtab_auto}},

    /* default bindings (where not modified) */
    /* modifier                           key        function         argument
     */
    {MODKEY, XK_Return, spawn, {.v = termcmd}}, // switched with zoom
    {MODKEY, XK_p, spawn, {.v = dmenucmd}},
    {MODKEY, XK_b, togglebar, {0}},
    {MODKEY, XK_j, focusstack, {.i = +1}},
    {MODKEY, XK_k, focusstack, {.i = -1}},
    {MODKEY, XK_i, incnmaster, {.i = +1}},
    {MODKEY, XK_d, incnmaster, {.i = -1}},
    {MODKEY, XK_h, setmfact, {.f = -0.05}},
    {MODKEY, XK_l, setmfact, {.f = +0.05}},
    {MODKEY, XK_Tab, view, {0}},
    {MODKEY, XK_n, setlayout, {.v = &layouts[0]}},
    {MODKEY, XK_space, setlayout, {.v = &layouts[1]}},
    {MODKEY, XK_m, setlayout, {.v = &layouts[2]}},
    {MODKEY, XK_u, setlayout, {.v = &layouts[3]}},
    {MODKEY, XK_s, setlayout, {.v = &layouts[4]}},
    {MODKEY, XK_q, setlayout, {.v = &layouts[5]}},
    {MODKEY, XK_y, setlayout, {.v = &layouts[6]}}, // swapped with telegram
    {MODKEY, XK_r, setlayout, {.v = &layouts[7]}},
    {MODKEY, XK_o, togglefloating, {0}}, // swappe with setlayout &layout[1]
    {MODKEY, XK_Down, moveresize, {.v = "0x 25y 0w 0h"}},
    {MODKEY, XK_Up, moveresize, {.v = "0x -25y 0w 0h"}},
    {MODKEY, XK_Right, moveresize, {.v = "25x 0y 0w 0h"}},
    {MODKEY, XK_Left, moveresize, {.v = "-25x 0y 0w 0h"}},
    {MODKEY, XK_0, view, {.ui = ~0}},
    {MODKEY, XK_comma, focusmon, {.i = -1}},
    {MODKEY, XK_period, focusmon, {.i = +1}},
    {MODKEY | ShiftMask,
     XK_j,
     movestack,
     {.i = +1}}, // added via movestack patch
    {MODKEY | ShiftMask,
     XK_k,
     movestack,
     {.i = -1}},                                // added via movestack patch
    {MODKEY | ShiftMask, XK_Return, zoom, {0}}, // swapped with termcmd
    {MODKEY | ShiftMask, XK_c, killclient, {0}},
    {MODKEY | ShiftMask, XK_m, fullscreen, {0}},
    {MODKEY | ShiftMask, XK_Down, moveresize, {.v = "0x 0y 0w 25h"}},
    {MODKEY | ShiftMask, XK_Up, moveresize, {.v = "0x 0y 0w -25h"}},
    {MODKEY | ShiftMask, XK_Right, moveresize, {.v = "0x 0y 25w 0h"}},
    {MODKEY | ShiftMask, XK_Left, moveresize, {.v = "0x 0y -25w 0h"}},
    {MODKEY | ControlMask, XK_Up, moveresizeedge, {.v = "t"}},
    {MODKEY | ControlMask, XK_Down, moveresizeedge, {.v = "b"}},
    {MODKEY | ControlMask, XK_Left, moveresizeedge, {.v = "l"}},
    {MODKEY | ControlMask, XK_Right, moveresizeedge, {.v = "r"}},
    {MODKEY | ShiftMask, XK_0, tag, {.ui = ~0}},
    {MODKEY | ShiftMask, XK_comma, tagmon, {.i = -1}},
    {MODKEY | ShiftMask, XK_period, tagmon, {.i = +1}},
    {MODKEY | ShiftMask, XK_q, quit, {0}},
    {MODKEY | ControlMask | ShiftMask, XK_Up, moveresizeedge, {.v = "T"}},
    {MODKEY | ControlMask | ShiftMask, XK_Down, moveresizeedge, {.v = "B"}},
    {MODKEY | ControlMask | ShiftMask, XK_Left, moveresizeedge, {.v = "L"}},
    {MODKEY | ControlMask | ShiftMask, XK_Right, moveresizeedge, {.v = "R"}},
    TAGKEYS(XK_1, 0) TAGKEYS(XK_2, 1) TAGKEYS(XK_3, 2) TAGKEYS(XK_4, 3)
        TAGKEYS(XK_5, 4) TAGKEYS(XK_6, 5) TAGKEYS(XK_7, 6) TAGKEYS(XK_8, 7)
            TAGKEYS(XK_9, 8)};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle,
 * ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
    /* click            event mask           button          function argument
     */
    {ClkLtSymbol, 0, Button1, setlayout, {0}},
    {ClkLtSymbol, 0, Button3, setlayout, {.v = &layouts[2]}},
    {ClkFollowSymbol, 0, Button1, togglefollow, {0}},
    {ClkStatusText, 0, Button2, spawn, {.v = termcmd}},
    {ClkClientWin, MODKEY, Button1, movemouse, {0}},
    {ClkClientWin, MODKEY, Button2, togglefloating, {0}},
    {ClkClientWin, MODKEY, Button3, resizemouse, {0}},
    {ClkTagBar, 0, Button1, view, {0}},
    {ClkTagBar, 0, Button3, toggleview, {0}},
    {ClkTagBar, MODKEY, Button1, tag, {0}},
    {ClkTagBar, MODKEY, Button3, toggletag, {0}},
    {ClkTabBar, 0, Button1, focuswin, {0}},
};
