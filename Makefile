MAKEFLAGS = --no-builtin-rules
# We don't use any suffix rules
.SUFFIXES :

PROG_NAME=test

LIBS= -lrt
SOUND_LIBS= -lasound
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

alsa_sources=asound_mute.c asound_read.c asound_write.c
asound_read_sources=asound_read.c Broadcast.c
asound_write_sources=asound_write.c Broadcast.c

sender_sources=sender.c Broadcast.c
client_sources=client.c Broadcast.c
common_sources=Broadcast.c
alsa_objs=$(patsubst %.c,build/%.o,$(alsa_sources))
sender_objs=$(patsubst %.c,build/%.o,$(sender_sources))
client_objs=$(patsubst %.c,build/%.o,$(client_sources))
common_objs=$(patsubst %.c,build/%.o,$(common_sources))

setup:
	@mkdir -p build

built-in.o : $(objs)
	$(call link, $(objs), $@)

host arm : common.o alsa network
	@#$(call shared_executable, $<, $@)
	@#$(call shared_library, $<, $(LIB_NAME).so)
	@#$(call static_library, $<, $(LIB_NAME).a)

common : common.o

common.o : $(common_objs)
	$(call link, $(common_objs), $@)

alsa : LIBS+=$(SOUND_LIBS)

alsa : asound_mute asound_read asound_write

asound_mute : asound_mute.o
	$(call shared_executable, $<, $@)

asound_read : Broadcast.o asound_read.o
	$(call shared_executable, $<, $@)

asound_write: Broadcast.o asound_write.o
	$(call shared_executable, $(), $@)

network: common sender client

sender : $(sender_objs)
	$(call static_executable, $(sender_objs), $@)

client : $(client_objs)
	$(call static_executable, $(client_objs), $@)


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
	rm -rf *.o *.so *.a $(PROG_NAME) build asound_mute asound_read sender client
