#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>

#include "fullscreen.h"

int main() {
  const char *display;
  Display *dpy;

  while(1) {
    if((display = getenv("DISPLAY"))) {
        if((dpy = XOpenDisplay(NULL))) {
          fullscreencheck(dpy);
          XCloseDisplay(dpy);
        }
    }
    sleep(10);
  }
  return 0;
}
