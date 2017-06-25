
Targets=bfs

all: $(Targets)

clean:
	rm -f $(Targets) *~ 

bfs: bfs.C
	g++  -ggdb -O0 -std=c++11 -Wall bfs.C $(LDFLAGS) -o bfs


