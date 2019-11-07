#ifndef __MODERNINPUTHANDLE_H__
#define __MODERNINPUTHANDLE_H__

DEVILUTION_BEGIN_NAMESPACE

#define PANEL_ELEMENT_BTN_CHR 0
#define PANEL_ELEMENT_BTN_QST 1
#define PANEL_ELEMENT_BTN_MAP 2
#define PANEL_ELEMENT_BTN_INV 3
#define PANEL_ELEMENT_BTN_SBK 4
#define PANEL_ELEMENT_BTN_MNU 5
#define PANEL_ELEMENT_LIFEBAR 6
#define PANEL_ELEMENT_MANABAR 7
#define PANEL_ELEMENT_BELT_1  8
#define PANEL_ELEMENT_BELT_2  9
#define PANEL_ELEMENT_BELT_3  10
#define PANEL_ELEMENT_BELT_4  11
#define PANEL_ELEMENT_BELT_5  12
#define PANEL_ELEMENT_BELT_6  13
#define PANEL_ELEMENT_BELT_7  14
#define PANEL_ELEMENT_BELT_8  15
#define PANEL_ELEMENT_NONE    255

extern char* btns_tips[];

extern int btns_rects[][4];
extern int meters_rects[][4];

extern int hovered_element;
extern int pressed_element;

void modern_input_handler(int event);

DEVILUTION_END_NAMESPACE

#endif //__MODERNINPUTHANDLE_H__