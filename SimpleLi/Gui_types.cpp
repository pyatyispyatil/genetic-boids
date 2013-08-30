
#include "Logic\Individ.h"
#include "Logic\Environment.h"
#include "Logic\Types.h"
#include "Gui_types.h"
#include "Gui_color.h"

double Dot(hgeVector hgev1, hgeVector hgev2) {
	Vector <double> v1(hgev1.x, hgev1.y), v2(hgev2.x, hgev2.y);
	return v1.getDot(v2);
}

HTEXTURE getButtonTex(int w, int h, DWORD color) {
	HTEXTURE tex = hge->Texture_Create(w*2, h);
	DWORD *ptr = hge->Texture_Lock(tex, false, 0, 0, w*2, h);
	HSVColor tempCol(color);
	for (int x = 0; x<w*2; x++) 
		for (int y = 0; y<h; y++) {
			if (x == w-1 || x == w || ((x<w*2-1 && x>w-1) && y == 0) || ((x>0 && x<w-1) && y == h-1)) {
				ptr[y*w*2 + x] = tempCol.setValue(tempCol.getValue()/2);
				tempCol = color;
			} else if (x == 0 || x == w*2-1 || (x<w-1 && y == 0) || (x>w && y == h-1)) {
				ptr[y*w*2 + x] = tempCol.setValue(tempCol.getValue()*1.5);
				tempCol = color;
			} else if (x>w) {
				ptr[y*w*2 + x] = tempCol.setValue(tempCol.getValue()/1.5);
				tempCol = color;
			} else ptr[y*w*2 + x] = tempCol;
		}
		hge->Texture_Unlock(tex);
	return tex;
};