
NAME=   fooCoder

### include debug information: 1=yes, 0=no
DBG?= 0
### include O level optimization : 0-3
OPT?= 3
### include profiling info : 1=yes, 0=no
PROF?= 0


BINDIR= bin
INCDIR= inc
SRCDIR= src
EXTRADIR = algorithms
OBJDIR= obj
GSLLIBDIR=/usr/lib/gsl

LIBS= -lm -lgsl -lgslcblas -DHAVE_INLINE
CFLAGS= -std=c99 -march=native -mfpmath=sse -masm=intel
FLAGS=  $(CFLAGS) -Wall -I$(INCDIR) -L$(GSLLIBDIR)

OPT_FLAG = -O$(OPT)
ifeq ($(DBG),1)
SUFFIX= .dbg
FLAGS+= -g2
else
SUFFIX=
FLAGS+= $(OPT_FLAG)
endif
ifeq ($(PROF),1)
FLAGS+= -pg
endif




SRC=    $(wildcard $(SRCDIR)/*.c $(SRCDIR)/$(EXTRADIR)/*.c)
OBJ=    $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o$(SUFFIX)) $(ADDSRC:$(ADDSRCDIR)/%.c=$(OBJDIR)/%.o$(SUFFIX))
BIN=    $(BINDIR)/$(NAME)$(SUFFIX).exe

all: clean objdir_mk bindir_mk yuvdir_mk bin

clean:
	@echo remove all objects
	@rm -rf $(OBJDIR)

bin:    $(OBJ)
	@echo
	@echo 'creating binary "$(BIN)"'
	@$(CC) $(FLAGS) -o $(BIN) $(OBJ) $(LIBS)
	@echo '... done'
	@echo
	
$(OBJDIR)/%.o$(SUFFIX): $(SRCDIR)/%.c
	@echo 'compiling object file "$@" ...'
	@$(CC) -c -o $@ $(FLAGS) $<

$(OBJDIR)/$(EXTRADIR)/%.o$(SUFFIX): $(SRCDIR)/$(EXTRADIR)/%.c
	@echo 'compiling object file "$@" ...'
	@$(CC) -c -o $@ $(FLAGS) $<
	
objdir_mk:
	@echo 'Creating $(OBJDIR) ...'
	@mkdir -p $(OBJDIR)
	@mkdir -p $(OBJDIR)/$(EXTRADIR)

bindir_mk:
	@echo 'Creating $(BINDIR) ...'
	@mkdir -p $(BINDIR)

yuvdir_mk:
	@echo 'Creating yuv folder'
	@mkdir -p yuv

run:						# pesquisa algoritmo subamostragem sorteios
	./$(BIN) park_joy_1088p50.yuv 128 22 0 80
