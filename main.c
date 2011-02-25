/* Copyright (C) 2009 Adam Wick
 * See LICENSE for licensing information
 */
#include <string.h>
#include <panel-applet.h>
#include <gtk/gtklabel.h>
#include <dbus/dbus-glib.h>
#include <gconf/gconf-client.h>
#include <stdlib.h>

/* AW: I pulled this code off the interwebs. It makes background 
 * transparency actually work */
void change_bg(PanelApplet *applet, PanelAppletBackgroundType type,
               GdkColor *color, GdkPixmap *pixmap)
{
  GtkRcStyle *rc_style;
  GtkStyle *style;

  /* reset style */
  gtk_widget_set_style(GTK_WIDGET(applet), NULL);
  rc_style = gtk_rc_style_new();
  gtk_widget_modify_style(GTK_WIDGET(applet), rc_style);
  gtk_rc_style_unref(rc_style);

  switch(type) {
    case PANEL_NO_BACKGROUND:
      break;
    case PANEL_COLOR_BACKGROUND:
      gtk_widget_modify_bg(GTK_WIDGET(applet), GTK_STATE_NORMAL, color);
      break;
    case PANEL_PIXMAP_BACKGROUND:
      style = gtk_style_copy(GTK_WIDGET(applet)->style);

      if(style->bg_pixmap[GTK_STATE_NORMAL])
        g_object_unref(style->bg_pixmap[GTK_STATE_NORMAL]);

      style->bg_pixmap[GTK_STATE_NORMAL] = g_object_ref(pixmap);
      gtk_widget_set_style(GTK_WIDGET(applet), style);
      g_object_unref(style);
      break;
  }
}

static void signal_handler(DBusGProxy *obj, const char *msg, GtkWidget *widget)
{
    gtk_label_set_markup(GTK_LABEL(widget), msg);
}

static void set_up_dbus_transfer(GtkWidget *buf)
{
  DBusGConnection *connection;
  DBusGProxy *proxy;
  GError *error= NULL;

  connection = dbus_g_bus_get(DBUS_BUS_SESSION, &error);
  if(connection == NULL) {
    g_printerr("Failed to open connection: %s\n", error->message);
    g_error_free(error);
    exit(1);
  }

  proxy = dbus_g_proxy_new_for_name(connection, "org.xmonad.Log",
                                   "/org/xmonad/Log",
                                   "org.xmonad.Log");
  error = NULL;

  dbus_g_proxy_add_signal(proxy, "Update", G_TYPE_STRING, G_TYPE_INVALID);
  dbus_g_proxy_connect_signal(proxy, "Update", (GCallback)signal_handler,
                              buf, NULL);
}

void xmonadlog_applet_size_change(GConfClient *client,
                                  guint cnxn_id,
                                  GConfEntry *entry,
                                  gpointer user_data) {
    gint width = gconf_client_get_int (client,
                                        "/apps/xmonad-log-applet/width-chars",
                                        NULL);
    gtk_label_set_width_chars(GTK_LABEL(user_data), width);
}

static gboolean xmonadlog_applet_fill(PanelApplet *applet)
{
  GtkWidget *label = gtk_label_new("Waiting for XMonad");

  // Set up Gconf
  GConfClient* client = gconf_client_get_default();
  gconf_client_add_dir(client,
                       "/apps/xmonad-log-applet",
                       GCONF_CLIENT_PRELOAD_NONE,
                       NULL);

  gconf_client_notify_add(client,
                          "/apps/xmonad-log-applet/width-chars",
                          xmonadlog_applet_size_change,
                          label,
                          NULL,
                          NULL);

  GConfValue* gcValue = NULL;
  gcValue = gconf_client_get(client,
                             "/apps/xmonad-log-applet/width-chars",
                             NULL);
  int width = 80;
  if (gcValue && gcValue->type == GCONF_VALUE_INT) {
      width = gconf_value_get_int(gcValue);
  }
  gtk_label_set_width_chars(GTK_LABEL(label), width);

  gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
  gtk_misc_set_alignment((GtkMisc *)label, 0.0, 0.5);
  set_up_dbus_transfer(label);

  g_signal_connect(applet, "change-background", G_CALLBACK(change_bg), NULL);
  gtk_container_add(GTK_CONTAINER(applet), label);
  gtk_widget_show_all(GTK_WIDGET(applet));

  return TRUE;
}


static gboolean xmonadlog_applet_factory(PanelApplet *applet,
                                         const gchar *iid,
                                         gpointer data)
{
  gboolean retval = FALSE;

  if(!strcmp(iid, "OAFIID:XMonadLogApplet"))
    retval = xmonadlog_applet_fill(applet);

  if(retval == FALSE) {
    printf("Wrong applet!\n");
    exit(-1);
  }

  return retval;
}

PANEL_APPLET_BONOBO_FACTORY("OAFIID:XMonadLogApplet_Factory",
                            PANEL_TYPE_APPLET,
                            "XMonadLogApplet",
                            "0.0.1",
                            xmonadlog_applet_factory,
                            NULL);
