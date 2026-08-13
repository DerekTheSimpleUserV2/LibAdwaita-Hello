#include <adwaita.h>

// El código fuente que se mostrará en la interfaz
const char *SOURCE_CODE = 
"#include <adwaita.h>\n\n"
"static void on_button_clicked(GtkButton *button, gpointer user_data) {\n"
"    GtkWindow *parent_window = GTK_WINDOW(user_data);\n"
"    AdwDialog *dialog = adw_alert_dialog_new(\"Mensaje\", \"Hello\");\n"
"    adw_alert_dialog_add_response(ADW_ALERT_DIALOG(dialog), \"close\", \"Cerrar\");\n"
"    adw_alert_dialog_set_close_response(ADW_ALERT_DIALOG(dialog), \"close\");\n"
"    adw_alert_dialog_choose(ADW_ALERT_DIALOG(dialog), GTK_WIDGET(parent_window), NULL, on_dialog_closed, NULL);\n"
"}";

// Función para manejar el cierre del diálogo de saludo
static void on_dialog_closed(GObject *source_object, GAsyncResult *res, gpointer user_data) {
    adw_alert_dialog_choose_finish(ADW_ALERT_DIALOG(source_object), res);
}

// Acción al presionar el botón de "Saludar"
static void on_button_clicked(GtkButton *button, gpointer user_data) {
    GtkWindow *parent_window = GTK_WINDOW(user_data);
    AdwDialog *dialog = adw_alert_dialog_new("Mensaje", "Hello");

    adw_alert_dialog_add_response(ADW_ALERT_DIALOG(dialog), "close", "Cerrar");
    adw_alert_dialog_set_close_response(ADW_ALERT_DIALOG(dialog), "close");
    adw_alert_dialog_choose(ADW_ALERT_DIALOG(dialog), GTK_WIDGET(parent_window), NULL, on_dialog_closed, NULL);
}

// Acción para copiar el código al portapapeles
static void on_copy_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *widget = GTK_WIDGET(button);
    GdkClipboard *clipboard = gtk_widget_get_clipboard(widget);
    gdk_clipboard_set_text(clipboard, SOURCE_CODE);
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window = adw_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Libadwaita Hello");
    gtk_window_set_default_size(GTK_WINDOW(window), 550, 450);

    GtkWidget *header = adw_header_bar_new();
    GtkWidget *notebook = gtk_notebook_new();

    // ================= PESTAÑA 1: VISTA DE LA APP =================
    GtkWidget *app_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    GtkWidget *hello_button = gtk_button_new_with_label("Saludar");
    gtk_widget_set_halign(hello_button, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(hello_button, GTK_ALIGN_CENTER);
    gtk_widget_set_hexpand(hello_button, TRUE);
    gtk_widget_set_vexpand(hello_button, TRUE);
    
    g_signal_connect(hello_button, "clicked", G_CALLBACK(on_button_clicked), window);
    gtk_box_append(GTK_BOX(app_box), hello_button);
    
    GtkWidget *label1 = gtk_label_new("Aplicación");
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), app_box, label1);

    // ================= PESTAÑA 2: VER CÓDIGO =================
    GtkWidget *code_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_margin_start(code_box, 15);
    gtk_widget_set_margin_end(code_box, 15);
    gtk_widget_set_margin_top(code_box, 15);
    // CORRECCIÓN: Se eliminó la línea errónea y se mantiene la sintaxis válida de GTK4
    gtk_widget_set_margin_bottom(code_box, 15);

    // Botón para copiar el código
    GtkWidget *copy_button = gtk_button_new_from_icon_name("edit-copy-symbolic");
    gtk_widget_set_halign(copy_button, GTK_ALIGN_END);
    gtk_widget_set_tooltip_text(copy_button, "Copiar código al portapapeles");
    g_signal_connect(copy_button, "clicked", G_CALLBACK(on_copy_clicked), NULL);
    gtk_box_append(GTK_BOX(code_box), copy_button);

    // Área de texto con scroll
    GtkWidget *scrolled_window = gtk_scrolled_window_new();
    gtk_widget_set_vexpand(scrolled_window, TRUE);
    gtk_widget_set_hexpand(scrolled_window, TRUE);

    GtkTextBuffer *buffer = gtk_text_buffer_new(NULL);
    gtk_text_buffer_set_text(buffer, SOURCE_CODE, -1);

    GtkWidget *text_view = gtk_text_view_new_with_buffer(buffer);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_monospace(GTK_TEXT_VIEW(text_view), TRUE);
    
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), text_view);
    gtk_box_append(GTK_BOX(code_box), scrolled_window);

    GtkWidget *label2 = gtk_label_new("Ver Código");
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), code_box, label2);

    // ================= ENSAMBLAJE FINAL =================
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append(GTK_BOX(main_box), header);
    gtk_box_append(GTK_BOX(main_box), notebook);

    adw_application_window_set_content(ADW_APPLICATION_WINDOW(window), main_box);
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char *argv[]) {
    g_autoptr(AdwApplication) app = adw_application_new("com.ejemplo.LibadwaitaHello", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    return g_application_run(G_APPLICATION(app), argc, argv);
}
