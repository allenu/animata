
CPP_FILES = $(wildcard src/*.cpp)
EXAMPLE_CPP_FILES = $(wildcard examples/console/*.cpp)
HELPER_CPP_FILES = $(wildcard helpers/*.cpp)
JSON_CPP_FILES = $(wildcard JsonCpp/*.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))
CC_FLAGS := -std=c++11 -Ipublic


all: libanimata example

libanimata: obj/libanimata.a

obj/%.o: src/%.cpp
	mkdir -p obj
	$(CC) $(CC_FLAGS) -c -o $@ $<

obj/libanimata.a: $(OBJ_FILES)
	$(AR) rcs obj/libanimata.a $(OBJ_FILES)

example: obj/example

obj/example: $(EXAMPLE_CPP_FILES) $(HELPER_CPP_FILES) libanimata
	mkdir -p obj
	$(CC) $(CC_FLAGS) -Ihelpers -IJsonCpp/json $(EXAMPLE_CPP_FILES) $(JSON_CPP_FILES) $(HELPER_CPP_FILES) -o obj/example -Lobj -lstdc++ -lanimata

clean:
	rm -f obj/*.o obj/*.a
