/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>
/* appearance */
static const unsigned int borderpx  = 0;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int vertpad            = 9;       /* vertical padding of bar */
static const int sidepad            = 13;       /* horizontal padding of bar */
static const char *fonts[]          = { "RobotoMono Nerd Font:size=11.5" };
static const char dmenufont[]       = "RobotoMono Nerd Font:size=20";
static const char normfgcolor[]     = "#ebdbb2";
static const char normbgcolor[]     = "#282828";
static const char normbordercolor[] = "#444444";
static const char selfgcolor[]      = "#ebdbb2";
static const char selbgcolor[]      = "#427b58";
static const char selbordercolor[]  = "#444444";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
	[SchemeSel]  = { selfgcolor, selbgcolor,  selbgcolor  },
};

/* tagging */
static const char *tags[] = { "", "", "", "", "", "", "", "", "" };

static const unsigned int ulinepad	= 0;	/* horizontal padding between the underline and tag */
static const unsigned int ulinestroke	= 2;	/* thickness / height of the underline */
static const unsigned int ulinevoffset	= 0;	/* how far above the bottom of the bar the line should appear */
static const int ulineall 		= 0;	/* 1 to show underline on all tags, 0 for just the active ones */

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "firefox",  NULL,       NULL,       1 << 1,       0,           -1 },
	{ "dolphin",  NULL,       NULL,       1 << 2,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define STATUSBAR "dwmblocks"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *clipmenucmd[] = { "clipmenu", "-i", "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };

/* media commands */
static const char *medplaypausecmd[] = { "playerctl", "play-pause", NULL };
static const char *medstopcmd[] = { "playerctl", "stop", NULL };
static const char *mednextcmd[] = { "playerctl", "next", NULL };
static const char *medprevcmd[] = { "playerctl", "previous", NULL };

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_e,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_e,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },

	/* additional shortcuts */   
	{ MODKEY|ShiftMask,             XK_v,      spawn,          {.v = clipmenucmd } },
	{ MODKEY,                       XK_q,      spawn,          SHCMD("/usr/bin/dolphin")},
	{ MODKEY,                       XK_w,      spawn,          SHCMD("/usr/local/bin/st /usr/local/bin/ranger && exit")},
	{ MODKEY|ShiftMask,             XK_l,      spawn,          SHCMD("/usr/local/bin/slock") },
	{ MODKEY|Mod4Mask,              XK_a,      spawn,          SHCMD("playerctl previous NULL") },
	{ MODKEY|ShiftMask,             XK_a,      spawn,          SHCMD("/usr/bin/flameshot full -p ~/Pictures")},
	{ MODKEY|ControlMask,           XK_a,      spawn,          SHCMD("/usr/bin/flameshot full -c")},
	{ MODKEY|ShiftMask,             XK_s,      spawn,          SHCMD("/usr/bin/flameshot screen -p ~/Pictures")},
	{ MODKEY|ControlMask,           XK_s,      spawn,          SHCMD("/usr/bin/flameshot screen -c")},
	{ MODKEY|ShiftMask,             XK_d,      spawn,          SHCMD("/usr/bin/networkmanager_dmenu") },
        { MODKEY|ControlMask,           XK_d ,     spawn,          SHCMD("/usr/bin/flameshot gui")},
	{ MODKEY|Mod4Mask,              XK_s,      spawn,          SHCMD("playerctl next NULL") },
	{ MODKEY|Mod4Mask,              XK_d,      spawn,          SHCMD("playerctl play-pause NULL") },
        { MODKEY|ControlMask,           XK_k,      spawn,          SHCMD("/usr/bin/setxkbmap -query | grep 'layout:[[:blank:]]*us' && setxkbmap sk || setxkbmap us && kill -46 $(pidof dwmblocks) && setxkbmap -option caps:ctrl_modifier")},
	{ MODKEY|ShiftMask,             XK_o,      spawn,          SHCMD("pkill -USR1 redshift && dunstify 'Redshift' 'Toggled successfully.'") },


        { 0,                            XF86XK_MonBrightnessUp,      spawn,  SHCMD("/usr/bin/light -A 5 ; pkill -RTMIN+11 dwmblocks") },
        { 0,                            XF86XK_MonBrightnessDown,    spawn,  SHCMD("/usr/bin/light -U 5 ; pkill -RTMIN+11 dwmblocks") },
        { 0,                            XF86XK_AudioMute,            spawn,  SHCMD("/usr/bin/pactl set-sink-mute @DEFAULT_SINK@ toggle; pkill -RTMIN+10 dwmblocks") },
        { 0,                            XF86XK_AudioRaiseVolume,     spawn,  SHCMD("/usr/bin/pactl set-sink-volume @DEFAULT_SINK@ +5% ; pkill -RTMIN+10 dwmblocks") },
        { 0,                            XF86XK_AudioLowerVolume,     spawn,  SHCMD("/usr/bin/pactl set-sink-volume @DEFAULT_SINK@ -5% ; pkill -RTMIN+10 dwmblocks") },
	{ 0,                            XF86XK_AudioPlay,            spawn,  {.v = medplaypausecmd } },
	{ 0,                            XF86XK_AudioStop,            spawn,  {.v = medstopcmd } },
	{ 0,                            XF86XK_AudioNext,            spawn,  {.v = mednextcmd } },
	{ 0,                            XF86XK_AudioPrev,            spawn,  {.v = medprevcmd } },
        //{ 0,                            XF86XK_AudioMicMute,         spawn,  SHCMD("") },
	//{ 0,                            XF86XK_Calculator,           spawn,  SHCMD("") },
	//{ Mod4Mask,                     XK_p,                        spawn,  SHCMD("") },
	//{ Mod4Mask,                     XK_i,                        spawn,  SHCMD("") },
	//{ Mod4Mask,                     XK_l,                        spawn,  SHCMD("") },
	//{ MODKEY|ControlMask,           XK_Tab,                      spawn,  SHCMD("") },

};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigstatusbar,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigstatusbar,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigstatusbar,   {.i = 3} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

