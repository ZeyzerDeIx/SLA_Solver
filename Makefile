# Compilateur
CXX = g++

# Options de compilation
CXXFLAGS = -std=c++20 -Iincludes

# Dossiers
SRCDIR = sources
INCDIR = includes
OBJDIR = obj
BINDIR = bin

# Nom de l'exécutable
TARGET = $(BINDIR)/SolGen

# Recherche des fichiers source et objets
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))

# Règle par défaut
all: build

# Cible pour construire seulement
build: $(TARGET)

# Cible pour construire et exécuter
buildAndRun: build
	@$(TARGET)

# Cible pour exécuter
run:
	@$(TARGET)

# Cible pour nttoyer, construire et exécuter
cleanBuildAndRun: clean buildAndRun

# Règle pour créer l'exécutable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@

# Règle pour créer les fichiers objets
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Règle de nettoyage
clean:
ifeq ($(OS),Windows_NT)
	del /f $(OBJDIR)\*.o
else
	rm -f $(OBJECTS)
endif

.PHONY: all build buildAndRun clean
