TARGET = galaxyview

SRCS += main.c Lan.c Jpeg.c Cam.c image_process.c
OBJECTS += main.o Lan.o Jpeg.o Cam.o image_process.o
LIBS += -lpthread -ljpeg -lm

all:
	gcc -c $(SRCS)
	gcc -o $(TARGET) $(OBJECTS) $(LIBS)
	rm -f $(OBJECTS)

