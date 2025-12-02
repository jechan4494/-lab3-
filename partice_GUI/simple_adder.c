#include <gtk/gtk.h>
#include <string.h>

GtkWidget *entry;

void on_button_clicked(GtkWidget *widget, gpointer data) {
    const char *button = gtk_button_get_label(GTK_BUTTON(widget));
    const char *current = gtk_entry_get_text(GTK_ENTRY(entry));

    // "=" 버튼일 때: 문자열 수식을 계산
    if (strcmp(button, "=") == 0) {
        double num1, num2;
        char op;
        double result = 0;

        sscanf(current, "%lf %c %lf", &num1, &op, &num2);

        switch (op) {
            case '+': result = num1 + num2; break;
            case '-': result = num1 - num2; break;
            case '*': result = num1 * num2; break;
            case '/':
                if (num2 != 0) result = num1 / num2;
                else { gtk_entry_set_text(GTK_ENTRY(entry), "do not try div"); return; }
                break;
        }

        char buf[100];
        sprintf(buf, "%.2f", result);
        gtk_entry_set_text(GTK_ENTRY(entry), buf);
    }
    else if (strcmp(button, "C") == 0) {
        gtk_entry_set_text(GTK_ENTRY(entry), "");
    }
    else {
        char buf[100];
        sprintf(buf, "%s%s", current, button);
        gtk_entry_set_text(GTK_ENTRY(entry), buf);
    }
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GTK simple adder");
    gtk_window_set_default_size(GTK_WINDOW(window), 250, 200);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 0);

    GtkWidget *grid = gtk_grid_new();

    char *buttons[] = { "7","8","9","+",
                        "4","5","6","-",
                        "1","2","3","*",
                        "C","0","=","/" };

    int pos = 0;
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            GtkWidget *btn = gtk_button_new_with_label(buttons[pos]);
            gtk_grid_attach(GTK_GRID(grid), btn, c, r, 1, 1);
            g_signal_connect(btn, "clicked", G_CALLBACK(on_button_clicked), NULL);
            pos++;
        }
    }

    gtk_box_pack_start(GTK_BOX(vbox), grid, TRUE, TRUE, 0);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}

