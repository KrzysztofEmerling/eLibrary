gcc src/main.c src/eLibrary.c   \
src/windows/window_menager.c    \
src/windows/login_window.c      \
src/windows/panel_window.c      \
src/windows/admin_panel.c       \
src/utility.c                   \
-lssl -lcrypto  `pkg-config --cflags --libs gtk4` \
-o eLibrary
