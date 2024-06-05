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
all: build run clean

# Cible pour construire seulement
build: create_dirs $(TARGET)

# Cible pour exécuter
run:
	@$(TARGET)

# Règle pour créer l'exécutable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@

# Règle pour créer les fichiers objets
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Crée les dossiers bin et obj s'ils n'existent pas
create_dirs:
ifeq ($(OS),Windows_NT)
	@if not exist $(BINDIR) mkdir $(BINDIR)
	@if not exist $(OBJDIR) mkdir $(OBJDIR)
else
	@mkdir -p $(BINDIR)
	@mkdir -p $(OBJDIR)
endif
	

# Règle de nettoyage
clean:
ifeq ($(OS),Windows_NT)
	@if exist $(OBJDIR) rmdir /s /q $(OBJDIR)
else
	@rm -rf $(OBJECTS)
endif

.PHONY: all build buildAndRun clean cleanBuildAndRun run
