# Nazwa pliku z funkcjš main() - BEZ ROZSZERZENIA!
TARGET = seiho_temp

# Lista plików zawierajšcych konfigurację programu i sprzętu
# ich zmiana powoduje przebudowanie całego projektu
CONFIG = config.h

# Lista plików ródłowych w języku C
SRC = $(TARGET).c

# Lista plików ródłowych w asemblerze (rozszerzenie S - DUŻE S !)
ASRC = 

# typ mikrokontrolera
MCU = atmega32

# Format pliku wyjciowego (srec, ihex)
FORMAT = ihex

# Poziom optymalizacji (0, 1, 2, 3, s) 
# (Uwaga: 3 nie zawsze jest najlepszym wyborem)
OPT = s

# Katalog z bibliotekami użytkownika
USRLIB	= ../lib

# Lista plików ródłowych bibliotek w języku C
SRCLIB =
#include $(USRLIB)/conv/sources
include $(USRLIB)/lcd/sources
#include $(USRLIB)/i2c/sources
#include $(USRLIB)/i2c_ee/sources
#include $(USRLIB)/kbd/sources
include $(USRLIB)/delay/sources
#include $(USRLIB)/pcf8583/sources
#include $(USRLIB)/uart/sources

# Dodatkowe biblioteki
#
# Minimalna wersja printf
#LDFLAGS += -Wl,-u,vfprintf -lprintf_min
#
# Zmiennoprzecinkowa wersja printf (wymaga biblioteki matematycznej)
#LDFLAGS += -Wl,-u,vfprintf -lprintf_flt
#
# Biblioteka matematyczna
#LDFLAGS += -lm

include $(USRLIB)/avr_make
