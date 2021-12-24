################################################################################
### CUSTOMIZE BELOW HERE #######################################################

CC=gcc # define the compiler to use
TARGET=my_solution # define the name of the executable
SOURCES=main.c functions.c
CFLAGS= -Wall -g3 -gdwarf-2 -DDEBUG
LFLAGS=-lm

################################################################################
### DO NOT EDIT THE FOLLOWING LINES ############################################

# define list of objects
OBJSC=$(SOURCES:.c=.o)
OBJS=$(OBJSC:.cpp=.o)

# the target is obtained linking all .o files
all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o $(TARGET)

purge: clean
	rm -f $(TARGET)

clean:
	rm -f *.o
	
test1: $(TARGET)
	@{ echo Test Result: Done; exit 0; }

################################################################################
################################################################################
