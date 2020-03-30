# GNU make手册：http://www.gnu.org/software/make/manual/make.html
# ************ 遇到不明白的地方请google以及阅读手册 *************

# 编译器设定和编译选项
CC = gcc
FLEX = flex
BISON = bison
CFLAGS = -std=c99 -g

# 编译目标：src目录下的所有.c文件
CFILES = $(shell find ./ -name "*.c")
OBJS = $(CFILES:.c=.o)
LFILE = $(shell find ./ -name "*.l")
YFILE = $(shell find ./ -name "*.y")
#把目录下的.l文件改为lex.yy.c输出，也就是只有一个lex.yy.c
LFC = $(shell find ./ -name "*.l" | sed s/[^/]*\\.l/lex.yy.c/)
#把目录下的.y文件改为syntax.tab.c输出，也就是只有一个syntax.tab.c
YFC = $(shell find ./ -name "*.y" | sed s/[^/]*\\.y/syntax.tab.c/)
#lex.yy.c编译结果为lex.yy.o
LFO = $(LFC:.c=.o)
#syntax.tab.c编译结果为syntax.tab.o
YFO = $(YFC:.c=.o)

#把所有.o(除了lex.yy.x编译出的.o,正常也不会有这个.o)编译成parser
parser: syntax $(filter-out $(LFO),$(OBJS))
	$(CC) -o parser $(filter-out $(LFO),$(OBJS)) -lfl -ly

#gcc编译syntax.tab.c成.o,由于syntax.tab.y中include了lex.yy.c，所以只要编译syntax.tab.c，不要再编译lex.yy.c
syntax: lexical syntax-c
	$(CC) -c $(YFC) -o $(YFO)

#用flex把.l文件编译成lex.yy.c,只能有一个.l文件
lexical: $(LFILE)
	$(FLEX) -o $(LFC) $(LFILE)

#用bison把.y文件编译成syntax.tab.c,只能有一个.y文件，否则报错.这个syntax.tab.y中需要include了lex.yy.c
syntax-c: $(YFILE)
	$(BISON) -o $(YFC) -d -v $(YFILE)

-include $(patsubst %.o, %.d, $(OBJS))

# 定义的一些伪目标
.PHONY: clean test
test:
	./parser ../Test/test1.cmm
test2:
	./parser ../Test/test2.cmm
clean:
	rm -f parser lex.yy.c syntax.tab.c syntax.tab.h syntax.output
	rm -f $(OBJS) $(OBJS:.o=.d)
	rm -f $(LFC) $(YFC) $(YFC:.c=.h)
	rm -f *~
