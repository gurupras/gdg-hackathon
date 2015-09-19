MAKEFLAGS = --no-builtin-rules
# We don't use any suffix rules
.SUFFIXES :

PROG_NAME=test

LIBS= -lasound -lrt

INCLUDE=

vpath %.h $(INCLUDE)

CFLAGS=-I$(INCLUDE) -fPIC -g
LDFLAGS=
CC_OPTS= $(CFLAGS) $($*_CC_OPTS) $(LDFLAGS) -Wall
CC=gcc
LD=ld

all : setup host

host : CROSS=
arm  : CROSS=arm-none-linux-gnueabi-

sources=asound_mute.c
objs=$(patsubst %.c,build/%.o,$(sources))

setup:
	@mkdir -p build

built-in.o : $(objs)
	$(call link, $(objs), $@)

host arm : built-in.o
	$(call shared_executable, $<, $(PROG_NAME))
	@#$(call shared_library, $<, $(LIB_NAME).so)
	@#$(call static_library, $<, $(LIB_NAME).a)

tests : built-in.o


build/%.o: %.c
	$(call compile, $<, $@)

%.o : %.c
	$(call compile, $<, $@)


print_cc = \
	echo "  CC    $(1) --> $(2)";

print_ld = \
	echo "  LD    $(2)";
print_ar = \
	echo "  AR    $(2)";


compile = \
	@$(addprefix $(CROSS), $(CC)) $(CC_OPTS) -c $(1) -o $(2); \
	$(call print_cc, $(1), $(2))

link = \
	@$(addprefix $(CROSS), $(LD)) $(LDFLAGS) -r $(1) -o $(2); \
	$(call print_ld, $(1), $(2))

shared_library = \
	@$(addprefix $(CROSS), $(CC)) $(CFLAGS) -shared $(LDFLAGS) -o $(2) $(1) $(LIBS); \
	$(call print_cc, $(1), $(2))

static_library = \
	@$(addprefix $(CROSS), $(AR)) rcs $(2) $(1); \

shared_executable = \
	@$(addprefix $(CROSS), $(CC)) $(CC_OPTS) -o $(2) $(1) $(LIBS); \
	$(call print_cc, $(1), $(2))

static_executable = \
	@$(addprefix $(CROSS), $(CC)) $(CC_OPTS) -g -static -o $(2) $(1) $(LIBS); \
	$(call print_cc, $(1), $(2))
clean :
	rm -rf *.o *.so *.a $(PROG_NAME) build
