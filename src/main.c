#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>

#include "fullscreen.h"

int main() {
  const char *display;

  while(1) {
    display = getenv("DISPLAY");

    if(display != NULL) {
      Display *dpy = XOpenDisplay(NULL);
        if(dpy != NULL) {
          fullscreencheck(dpy);
          XCloseDisplay(dpy);
        }
    }
    sleep(10);
  }
  return 0;
}
