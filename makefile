OBJD = lex.o scanner.o recognizer.o dll.o interpreter.o doc.o
OOPTS = -Wall -std=c99 -Wextra -g -c
LOPTS = -Wall -std=c99 -Wextra -g

all :	final1

final1 :	$(OBJD)
	gcc $(LOPTS) -o final1 $(OBJD)

interpreter.o :	interpreter.c lex.h recognizer.h doc.h dll.h
	gcc $(OOPTS) interpreter.c

recognizer.o : recognizer.c recognizer.h lex.h
	gcc $(OOPTS) recognizer.c

dll.o :	dll.c dll.h
	gcc $(OOPTS) dll.c

doc.o :	doc.c doc.h scanner.h
	gcc $(OOPTS) doc.c

lex.o :	lex.c lex.h scanner.h
	gcc $(OOPTS) lex.c

scanner.o:	scanner.c scanner.h
	gcc $(OOPTS) scanner.c

clean :
	rm -f $(OBJD) final1 *.o