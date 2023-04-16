PROJECT := ALGC
VERSION := 0.0.1

BUILD_TYPE ?= Debug
BUILD_DIR := build

algc_srcs := src/kmp.c
algc_tests_srcs := tests/algc-test-string.c

algc_objs := $(algc_srcs:%.c=$(BUILD_DIR)/%.o)
algc_tests_objs := $(algc_tests_srcs:%.c=$(BUILD_DIR)/%.o)

CFLAGS :=
ifeq ($(BUILD_TYPE), Debug)
CFLAGS += -g
endif

$(algc_objs): CFLAGS += -fPIC
$(algc_tests_objs): CFLAGS += -Isrc

algc_lib := $(BUILD_DIR)/src/libalgc.so
algc_tests := $(algc_tests_objs:%.o=%)

LDFLAGS :=
$(algc_lib): LDFLAGS += -shared
$(algc_tests): LDFLAGS += -L$(BUILD_DIR)/src -lalgc -Wl,-rpath=$(BUILD_DIR)/src

all: $(algc_lib) $(algc_tests)

$(algc_lib): $(algc_objs)
	$(CC) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) -MM -MT $@ $< > $(@:%.o=%.d)
	$(CC) -c $(CFLAGS) -o $@ $<

%: %.o
	mkdir -p $(dir $@)
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	rm -rf $(algc_objs) $(algc_tests_objs) $(algc_lib) $(algc_tests)

.PHONY: all clean

-include $(algc_objs:%.o=%.d)
-include $(algc_tests_objs:%.o=%.d)