EXE := server
# wildcard扫描源文件
#sources := $(wildcard *.cpp)   
sources := $(wildcard *.cpp log/*.cpp) 
#headfile := $(wildcard *.h )
headfile := $(wildcard *.h log/*.h)
objects  := $(patsubst %.cpp,%.o,$(sources))

CC := g++ -std=c++11 -g
RM := rm -rf
LIB := -lpthread -lrt -lmysqlclient
PATH_ := -L /usr/lib64/mysql/

$(EXE): $(objects)
	$(CC) -o $@ $^ $(LIB) $(PATH_)
%.o: %.cpp 
	$(CC) -o $@ -c $< $(LIB) $(PATH_)

# 伪目标，意味着clean不代表一个真正的文件名
.PHONY: clean cleanall
cleanall:
	$(RM) $(EXE) $(objects)
clean:
	$(RM) $(objects)
