OBJ=ycx_mp4
OBJS+=./main.c
OBJS+=./do.c
OBJS+=./show.c
H_FILE+=public.h 
H_FILE+=do.h 
H_FILE+=show.h 
 
$(OBJ):$(OBJS)
	gcc $(OBJS) -o $(OBJ) 
 
.PHONY:
clean:
	rm $(OBJ)
install:
	sudo cp $(OBJ) /bin
uninstall:
	sudo rm $(OBJ) /bin/$(OBJ)