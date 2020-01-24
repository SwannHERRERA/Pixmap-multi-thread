//
// Created by MaleWhere on 22/01/2020.
//

#ifndef PIXMAP_MULTI_THREAD_GTKWINDOW_H
#define PIXMAP_MULTI_THREAD_GTKWINDOW_H
void free_pixels(guchar *pixels, gpointer data) {
    free(pixels);
}
void showImage(ppm_image_t *img, char *fileName, int argc, char **argv) {
    int i, j;
    GtkImage *image;
    GtkWidget *window;

    GdkPixbuf *pb;
    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), img->width, img->height);
    gtk_window_set_title(GTK_WINDOW(window), fileName);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    gtk_container_set_border_width(GTK_CONTAINER(window), 2);


    g_signal_connect_swapped(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), G_OBJECT(window));


    guchar *rgb;
    rgb = malloc(img->width * img->height * 3);

    for (i = 0; i < img->height; i++) {
        for (j = 0; j < img->width; j++) {
            rgb[i * j * 3] = img->data[i * j].red;
            rgb[i * j * 3 + 1] = img->data[i * j].green;
            rgb[i * j * 3 + 2] = img->data[i * j].blue;
        }
    }

    pb = gdk_pixbuf_new_from_data(rgb, GDK_COLORSPACE_RGB, 0, 8, img->width, img->height, 255, free_pixels,
                                  NULL);
    image = GTK_IMAGE(gtk_image_new_from_pixbuf(pb));
    gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(image));
    gtk_widget_show_all(window);
    gtk_main();


}


#endif //PIXMAP_MULTI_THREAD_GTKWINDOW_H
