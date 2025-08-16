#ifndef GAME_WORLDPANEL_H
#define GAME_WORLDPANEL_H

class EntitiesPanel {
public:
    EntitiesPanel();

    void render();
private:
    bool m_show_hierarchy = false;

    void render_list_view();
    void render_hierarchy_view();
};

#endif //GAME_WORLDPANEL_H