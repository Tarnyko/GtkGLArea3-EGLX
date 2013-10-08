gcc simple-wayland.c -o simple-wayland `pkg-config --cflags --libs wayland-client wayland-egl egl glesv1_cm gtk+-3.0 gtkgl-2.0` libEGLX.so -DHAVE_JWZGLES -DGL_VERSION_ES_CM_1_0 libjwzgles.so 
