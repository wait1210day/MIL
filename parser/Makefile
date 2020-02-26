PARSER_DIR = .
PARSER_SRC = y.tab.cc lex.yy.cc

all: $(PARSER_SRC)

$(PARSER_SRC): scanner.l parser.y
	bison -vdty parser.y
	flex scanner.l
	mv y.tab.c y.tab.cc
	mv lex.yy.c lex.yy.cc
