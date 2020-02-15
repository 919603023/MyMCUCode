CC  = gcc 
cflags = -lpthread -ldl  
OBJ1 += MyCgi.o

MyCgi.cgi:$(OBJ1)  
	$(CC) $(OBJ1) -o $@ $(FLAGS) $(cflags)
	mv MyCgi.cgi ../
%.o:%.c
	$(CC) -c $^ -o $@ $(FLAGS) $(cflags)

.PHONY:clean
clean:
	rm  *.o -rfv
