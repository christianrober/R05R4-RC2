cmd_scripts/kconfig/qconf := g++  -o scripts/kconfig/qconf  scripts/kconfig/kconfig_load.o scripts/kconfig/zconf.tab.o  scripts/kconfig/qconf.o  -L/usr/lib/qt/lib/. -Wl,-rpath,/usr/lib/qt/lib/. -lqt-mt -ldl