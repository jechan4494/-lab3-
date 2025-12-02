#include <gtk/gtk.h>

static void on_submit(GtkWidget *widget, gpointer entry) {
    const char *text = gtk_entry_get_text(GTK_ENTRY(entry));
    g_print("write: %s\n", text);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    GtkWidget *entry = gtk_entry_new();
    GtkWidget *button = gtk_button_new_with_label("print");

    g_signal_connect(button, "clicked", G_CALLBACK(on_submit), entry);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_container_add(GTK_CONTAINER(window), box);
    gtk_box_pack_start(GTK_BOX(box), entry, 1, 1, 0);
    gtk_box_pack_start(GTK_BOX(box), button, 0, 0, 0);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

