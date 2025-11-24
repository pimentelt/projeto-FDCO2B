
main: menu_principal.o funcoes.o
	@echo "Gerando arquivo executavel"
	gcc menu_principal.o funcoes.o -o main


menuprincipal.o: menu_principal.c funcoes.h
	@echo "Compilando e gerando arq menu_principal.o"
	gcc -c menu_principal.c

funcoes.o: funcoes.c funcoes.h
	@echo "Compilando e gerando arq funcoes.o"
	gcc -c funcoes.c


clean:
	@echo "Apagando os objetos e executáveis antigos"
	rm -f *.o main


run:
	@echo "Executando o programa..."
	./main