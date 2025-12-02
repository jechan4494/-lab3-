#include <gtk/gtk.h>

static void show_message(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog = gtk_message_dialog_new
    (GTK_WINDOW(data), GTK_DIALOG_MODAL,
     GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
     "hello simple_msg_box.");

    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

int main(int argc, char **argv) {
    GtkWidget *window, *button;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Message Dialog");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);

    button = gtk_button_new_with_label("give up msg");
    g_signal_connect(button, "clicked", G_CALLBACK(show_message), window);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_container_add(GTK_CONTAINER(window), button);
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

