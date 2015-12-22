#include <X11/Xlib.h>

void safefree(void **pp);
int get_dpms(Display *dpy);
void set_dpms(Display *dpy, int tval, char *dpms);
unsigned long get_active_win(Display *dpy);
int get_fullscreen(Display *dpy);
void fullscreencheck(Display *dpy);
