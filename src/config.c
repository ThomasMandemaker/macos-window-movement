#include "config.h"
#include "layout.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_LOG_SIZE 100

const char* CONFIG_FILE = "/Users/thomasmandemaker/Desktop/Projects/tile-manager/configureoutwhywomenwonttalktome.bin";

void Save(Layout* layout) {
    if (!layout) {
        return;
    }
    FILE* f = fopen(CONFIG_FILE, "wb");
    fwrite(&layout->panelSize, sizeof(layout->panelSize), 1, f);

    for (int i = 0; i < layout->panelSize; i++) {
        Panel* panel = layout->panelList[i];
        fwrite(&panel, sizeof(Panel*), 1, f);
        fwrite(panel, sizeof(Panel), 1, f);
    }

    fclose(f);
}

Layout* Load() {
    Layout* layout = (Layout*)malloc(sizeof(Layout));
    layout->panelList = (Panel**)malloc(sizeof(Panel*)*20);

    FILE* f = fopen(CONFIG_FILE, "rb");
    if (!f) {
        return NULL;
    }

    fread(&layout->panelSize, sizeof(layout->panelSize), 1, f);

    Panel** raw = (Panel**)malloc(sizeof(Panel*)*layout->panelSize);

    for (int i = 0; i < layout->panelSize; i++) {
        layout->panelList[i] = (Panel*)malloc(sizeof(Panel));
        fread(&raw[i], sizeof(Panel*), 1, f);
        fread(layout->panelList[i], sizeof(Panel), 1, f);
    }
    fclose(f);

    for (int i = 0; i < layout->panelSize; i++) {
        Panel* currentPanel = layout->panelList[i];

        for (int j = 0; j < layout->panelSize; j++) {
            if (i == j) {
                continue;
            }

            if (currentPanel->up == raw[j]) {
                currentPanel->up = layout->panelList[j];
            }
            if (currentPanel->right == raw[j]) {
                currentPanel->right = layout->panelList[j];
            }
            if (currentPanel->down == raw[j]) {
                currentPanel->down = layout->panelList[j];
            }
            if (currentPanel->left == raw[j]) {
                currentPanel->left = layout->panelList[j];
            }
        }
    }
    layout->currentPanel = layout->panelList[0];

    free(raw);
    return layout;
}
