EXE := ceshi
# wildcard扫描源文件
sources := $(wildcard *.cpp) 
headfile := $(wildcard *.h)
objects  := $(patsubst %.cpp,%.o,$(sources))

CC := g++ -std=c++11 -g
RM := rm -rf
LIB := -lpthread -lrt

$(EXE): $(objects)
	$(CC) -o $@ $^ $(LIB)
%.o: %.cpp 
	$(CC) -o $@ -c $<
	
.PHONY: clean cleanall
cleanall:
	$(RM) $(EXE) $(objects)
clean:
	$(RM) $(objects)