NEDATASTRUCTURES_SRCS_PATH = src
NEDATASTRUCTURES_SRCS      = $(wildcard $(NEDATASTRUCTURES_SRCS_PATH)/*.c)
NEDATASTRUCTURES_INC_PATH  = inc
NEDATASTRUCTURES_OBJS      = $(NEDATASTRUCTURES_SRCS:$(NEDATASTRUCTURES_SRCS_PATH)/%.c=objs/%.o)
# infortunately, everything rebuilds when only one header is changed
NEDATASTRUCTURES_DEP       = $(wildcard $(NEDATASTRUCTURES_INC_PATH)/*.h)
NEDATASTRUCTURES_CFLAGS	= -Ofast

INC		= $(NEDATASTRUCTURES_INC_PATH) 
CFLAGS	= $(foreach inc,$(INC),-I$(inc))

CFLAGS += -ggdb3 -gdwarf-4 -Wall 
CFLAGS += -dD
CFLAGS += -Wno-unused-function

ARFLAGS   = rcs

OBJSDIR = objs

LIBDIR  = lib

CC = gcc
AR = ar

BIN = $(LIBDIR)/libne_datastructures.a


all: $(LIBDIR) $(OBJSDIR) $(BIN) 

$(LIBDIR):
	if [ ! -d "$(LIBDIR)" ]; then mkdir $(LIBDIR); fi

$(OBJSDIR):
	if [ ! -d "$(OBJSDIR)" ]; then mkdir $(OBJSDIR); fi

# compile mmdsp 
$(NEDATASTRUCTURES_OBJS): $(OBJSDIR)/%.o: $(NEDATASTRUCTURES_SRCS_PATH)/%.c $(NEDATASTRUCTURES_DEP)
	$(CC) -c $(CFLAGS) $< -o $@ $(NEDATASTRUCTURES_CFLAGS)

$(BIN): $(NEDATASTRUCTURES_OBJS)
	$(AR) $(ARFLAGS) $@ $^

clean:
	rm -r $(LIBDIR) $(OBJSDIR)
