CC  = gcc 
cflags = -lpthread -ldl  
OBJ1 += My_Gsm_QT_Server.o

My_Gsm_QT_Server:$(OBJ1)  
	$(CC) $(OBJ1) -o $@ $(FLAGS) $(cflags)
	
%.o:%.c
	$(CC) -c $^ -o $@ $(FLAGS) $(cflags)

.PHONY:clean
clean:
	rm  *.o -rfv
