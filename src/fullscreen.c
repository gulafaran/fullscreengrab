#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/extensions/dpms.h>

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "fullscreen.h"

void safefree(void **pp) {
  assert(pp);
  if(pp != NULL) {
    free(*pp);
    *pp = NULL;
  }
}

int get_dpms(Display *dpy) {
  int timeout, dummy;
  unsigned char dpms;
  CARD16 state;

  XGetScreenSaver(dpy, &timeout, &dummy, &dummy, &dummy);
  DPMSInfo(dpy, &state, &dpms);

#ifdef DEBUG
  fprintf(stdout, "fullscreen: querying timeout: %u dpms: %i \n", timeout, dpms);
#endif

  if(timeout > 0 && dpms == 1) {
    return 1;
  }

  return 0;
}

void set_dpms(Display *dpy, int tval, char *dpms) {

  int timeout, interval, prefer_blank, allow_exp, dummy;

  XGetScreenSaver(dpy, &timeout, &interval, &prefer_blank, &allow_exp);
  if(tval == 0)
    timeout = tval;
  else
    timeout = 600;

  if(strcmp(dpms, "+dpms") == 0) {
    if(DPMSQueryExtension(dpy, &dummy, &dummy))
      DPMSEnable(dpy);
  }
  else if(strcmp(dpms, "-dpms") == 0) {
    if(DPMSQueryExtension(dpy, &dummy, &dummy))
      DPMSDisable(dpy);
  }

#ifdef DEBUG
  fprintf(stdout, "fullscreen: setting timeout: %i dpms: %s \n", timeout, dpms);
#endif

  XSetScreenSaver(dpy, timeout, interval, prefer_blank, allow_exp);
}

unsigned long get_active_win(Display *dpy) {
  Window root_win;
  unsigned long n, extra, window;
  unsigned char *data;;
  int scr, format;
  Atom active, real;

  scr = DefaultScreen(dpy);
  root_win = RootWindow(dpy, scr);
  active = XInternAtom(dpy, "_NET_ACTIVE_WINDOW", False);

  if(XGetWindowProperty(dpy, root_win, active, 0, ~0, False,
        AnyPropertyType, &real, &format, &n, &extra, &data) != Success || !data) {
    return 0;
  }

  window = *(unsigned long *)data;
  safefree((void **)&data);

  return window;
}

int get_fullscreen(Display *dpy) {
  unsigned long i, num_items, bytes_after, window;
  int actual_format,retval;
  Atom actual_type, fullscr, state;
  Atom *atoms;

  atoms = NULL;
  retval = 0;

  if(!(window = get_active_win(dpy)))
    return retval;

  state = XInternAtom(dpy, "_NET_WM_STATE", False);
  fullscr = XInternAtom(dpy, "_NET_WM_STATE_FULLSCREEN", False);

  if(XGetWindowProperty(dpy, window, state, 0, 1024, False, XA_ATOM,
      &actual_type, &actual_format, &num_items, &bytes_after, (unsigned char**)&atoms) == Success) {
      for(i=0; i<num_items; ++i) {
      if(atoms[i] == fullscr) {
        retval = 1;
        break;
      }
    }
  }

  if(atoms)
    safefree((void **)&atoms);

  return retval;
}

void fullscreencheck(Display *dpy) {
  if(!dpy) {
    fprintf(stderr, "fullscreen: could not open display\n");
    return;
  }

#ifdef DEBUG
  fprintf(stdout, "fullscreen: checking for fullscreen.\n");
#endif

  if(get_fullscreen(dpy)) {
    if(get_dpms(dpy))
      set_dpms(dpy, 0, "-dpms");
  }
  else {
    if(!get_dpms(dpy))
      set_dpms(dpy, 1, "+dpms");
  }

  XSync(dpy, False);
}

