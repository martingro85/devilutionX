#include "utils.h"
#include "modern_control_panel.h"

DEVILUTION_BEGIN_NAMESPACE

void SetPixel(int sx, int sy, BYTE col)
{
	if (sy < 0 || sy >= SCREEN_HEIGHT + SCREEN_Y || sx < SCREEN_X || sx >= SCREEN_WIDTH + SCREEN_X)
		return;

	BYTE *dst;

	dst = &gpBuffer[sx + BUFFER_WIDTH * sy];

	*dst = col;
}


void DrawRectangle(Rect rect, BYTE color, bool dither)
{
	for (int y = 0; y < rect.h; y++) {
		for(int x = 0; x < rect.w; x++) {
			if(dither && x%2 != y%2)
				continue;
			SetPixel(SCREEN_X + rect.x + x, SCREEN_Y + rect.y + y, color);
		}
	}
}


void DrawChar(int x, int y, char c)
{
	int frame = (c - 31) < 1? 1 : (c - 31);
	CelDraw(SCREEN_X + x, SCREEN_Y + y + CHAR_H, ascii_charmap_cel, frame, CHAR_W);
}


void DrawString(int x, int y, char* str)
{
	int curr_col  = 0;
	for(int i = 0; str[i] != '\0'; i++, curr_col++) {
		if(str[i] == '\n') {
			y += CHAR_H;
			curr_col = -1;
			continue;
		}
		int curr_x = x + (curr_col * CHAR_W);
		DrawChar(curr_x, y, str[i]);
	}
}


bool CoordInsideRect(int x, int y, Rect rect)
{
	if(x < rect.x || x > rect.x + rect.w)
		return false;
	if(y < rect.y || y > rect.y + rect.h)
		return false;
	return true;
}


void PositionRectInScreen(Rect* rect) 
{
	Rect screen_rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

	// check if top or bottom not inside screen
	if(!CoordInsideRect(rect->x, rect->y, screen_rect) && !CoordInsideRect(rect->x + rect->w, rect->y, screen_rect))
		rect->y = 0;
	else if(!CoordInsideRect(rect->x, rect->y + rect->h, screen_rect) && !CoordInsideRect(rect->x + rect->w, rect->y + rect->h, screen_rect))
		rect->y = SCREEN_HEIGHT - rect->h;

	//check if left or right not inside screen
	if(!CoordInsideRect(rect->x, rect->y, screen_rect) && !CoordInsideRect(rect->x, rect->y + rect->h, screen_rect))
		rect->x = 0;
	else if(!CoordInsideRect(rect->x + rect->w, rect->y, screen_rect) && !CoordInsideRect(rect->x + rect->w, rect->y + rect->h, screen_rect))
		rect->x = SCREEN_WIDTH - rect->w;
}

DEVILUTION_END_NAMESPACE