
CPP_FILES = $(wildcard src/*.cpp)
EXAMPLE_CPP_FILES = $(wildcard examples/console/*.cpp)
HELPER_CPP_FILES = $(wildcard helpers/*.cpp)
JSON_CPP_FILES = jsoncpp/dist/jsoncpp.cpp jsoncpp/dist/json/json.h jsoncpp/dist/json/json-forwards.h
JSON_OBJ_FILES = generated/jsoncpp.o
OBJ_FILES := $(addprefix generated/,$(notdir $(CPP_FILES:.cpp=.o)))
CC_FLAGS := -std=c++11 -Ipublic


all: libanimata example

libanimata: generated/libanimata.a

generated:
	mkdir -p generated

generated/%.o: src/%.cpp $(JSON_CPP_FILES) generated
	$(CC) $(CC_FLAGS) -c -o $@ $<

generated/libanimata.a: $(OBJ_FILES) generated
	$(AR) rcs generated/libanimata.a $(OBJ_FILES)

example: generated/example

$(JSON_CPP_FILES): 
	cd jsoncpp; python amalgamate.py

$(JSON_OBJ_FILES): $(JSON_CPP_FILES)
	$(CC) $(CC_FLAGS) -c -o $@ $<

generated/example: $(EXAMPLE_CPP_FILES) $(HELPER_CPP_FILES) $(JSON_OBJ_FILES) libanimata generated
	$(CC) $(CC_FLAGS) -Ihelpers -Ijsoncpp/dist/json $(JSON_OBJ_FILES) $(EXAMPLE_CPP_FILES) $(HELPER_CPP_FILES) -o generated/example -Lgenerated -lstdc++ -lanimata

clean:
	rm -f generated/*.o generated/*.a

test: example
	cd generated; ./example
