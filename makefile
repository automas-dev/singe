
LD=$(CXX)

SFML=`pkg-config --cflags sfml-all glew`
INCLUDES=-Isrc/includes
CFLAGS=-O0 -g $(SFML) $(INCLUDES)
CPPFLAGS=-O0 -g -std=c++2a $(SFML) $(INCLUDES)
LDFLAGS=`pkg-config --libs sfml-all glew gl`

TARGET=main
OBJECTS=main.o

SRCDIR=src
OBJDIR=obj

_OBJECTS=$(addprefix $(OBJDIR)/, $(OBJECTS))

all: $(TARGET)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(TARGET): $(_OBJECTS)
	$(LD) $(LDFLAGS) $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) -c $(CPPFLAGS) $< -o $@



clean: $(OBJDIR)
	$(RM) $(TARGET) $(OBJDIR)/*

