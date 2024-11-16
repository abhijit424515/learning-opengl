start:
	g++ main.cpp glad.c -ldl -lglfw -o runme

clean:
	rm -f runme