#include <gtk/gtk.h>
#include <gtkgl/gtkglarea.h>

#include <wayland-client.h>
#include <wayland-egl.h>

#include <GLES/gl.h>
#include "jwzgles.h"


int borderw;
int titlebarw;


gint init (GtkWidget *widget)
{
      gtk_gl_area_make_current (GTK_GL_AREA(widget));
      glViewport(0,0, gtk_widget_get_allocated_width(widget), gtk_widget_get_allocated_height (widget));

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glOrtho(0,100, 100,0, -1,1);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();

      return TRUE;
}

gboolean draw(GtkWidget *widget, cairo_t *cr)
{
	if (gtk_gl_area_make_current(GTK_GL_AREA(widget)))
	{
	      glViewport(0,0, gtk_widget_get_allocated_width(widget), gtk_widget_get_allocated_height (widget));

		/* draw a triangle */
	      glClearColor(0,0,0,1);
	      glClear(GL_COLOR_BUFFER_BIT);
	      glColor3f(1,1,1);
	      glBegin(GL_TRIANGLES);
	      glVertex2f(10,10);
	      glVertex2f(10,90);
	      glVertex2f(90,90);
	      glEnd();

		/* refresh-draw ! */
	      gtk_gl_area_swap_buffers (GTK_GL_AREA(widget));
	}

	return TRUE;
}

gboolean resize (GtkWidget *widget, cairo_t *cr)
{
	 /* resize the EGL window dynamically */
	struct wl_egl_window *ewin = EGLX_wl_egl_window_get (1);
	wl_egl_window_resize (ewin, gtk_widget_get_allocated_width(widget)-borderw*2, gtk_widget_get_allocated_height(widget)-borderw*2-titlebarw, 0,0);

	glViewport(0,0, gtk_widget_get_allocated_width(widget), gtk_widget_get_allocated_height (widget));

	return FALSE;
}


int main(int argc, char *argv[])
{
	GtkWidget *window, *box, *glarea;

	gtk_init (&argc, &argv);

	EGLX_main(1);

	if (gdk_gl_query() == FALSE)
	{
		g_print ("GL not supported !\n");
		return 0;
	}

	gchar *info_str;
	info_str = gdk_gl_get_info();
	g_print (info_str);
	g_free (info_str);

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW(window), "Simple Wayland-EGL");
	gtk_container_set_border_width (GTK_CONTAINER(window), 10);
	borderw = gtk_container_get_border_width (GTK_CONTAINER(window));
	titlebarw = 30; /* HOW DO WE OBTAIN THAT ? */

	int attrs[] = {
	    GDK_GL_RGBA,
	    GDK_GL_RED_SIZE,1,
	    GDK_GL_GREEN_SIZE,1,
	    GDK_GL_BLUE_SIZE,1,
	    GDK_GL_DOUBLEBUFFER,
	    GDK_GL_NONE
	  };

	guint myborder = gtk_container_get_border_width (GTK_CONTAINER(window));

	glarea = GTK_WIDGET(gtk_gl_area_new(attrs));
	gtk_widget_set_size_request (GTK_WIDGET(glarea), 320,240);

	g_signal_connect (GTK_WIDGET(window), "destroy",
			 G_CALLBACK(gtk_main_quit), NULL);

	g_signal_connect (GTK_WIDGET(glarea), "draw",
			 G_CALLBACK(draw), NULL);
	g_signal_connect (GTK_WIDGET(glarea), "realize",
			 G_CALLBACK(init), NULL);

	g_signal_connect (GTK_WIDGET(window), "draw",
			G_CALLBACK(resize), NULL);

	box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
	gtk_container_add (GTK_CONTAINER(box), GTK_WIDGET(glarea));
	gtk_container_add (GTK_CONTAINER(window), GTK_WIDGET(box));

	gtk_widget_show (window);
	gtk_widget_show (glarea);
	gtk_widget_show (box);

	gtk_main();

	return 0;
}
