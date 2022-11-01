# Trabalho prático de Introdução à Computação
Esse trabalho foi feito para uma amiga do curso de Engenharia Florestal, utiliza linguagem C simples e tem por objetivo implementar um sistema fictício de gerenciamento bancário. O objetivo principal do projeto é utilizar técnicas básicas de modularização, tipos abstratos de dados, e acesso de arquivos em C.<br>
O código está um pouco repetitivo e extenso, podendo ser melhorado, mas por questões de prazo foi implementado sem uma melhor organização, no entanto corresponde ao exigido no arquivo "Trabalho_Prático_IC.pdf". 

# Objetivo do projeto
As regras e expectativas são descritas no arquivo "Trabalho_Prático_IC.pdf".
**O programa não está compilando mais (01/11/2022) devido a um erro gerado pelo uso da função itoa; Esse erro pode ser posteriormente corrigido alterando os usos da função itoa para a função sprintf.**

# Estrutura do projeto
O código todo está todo contido no arquivo "main.c", não houve separação das funções em um arquivo .h em razão do formato do envio do código na plataforma desejada, que deveria ser feito através de um único arquivo. <br>
Os arquivos .txt na pasta source são os bancos de dados usados pela aplicação para persistência das insformações, e não devem/não precisam ser alterados diretamente.

O arquivo main.c está organizado em:
1. Includes utilizados
2. Define de variáveis globais utilizadas ao longo do código
3. Declaração das structs
4. Algumas variáveis declaradas globalmente por questão de praticidade
5. Declaração de cabeçalho de todas as funções que serão implementadas, agrupadas de acordo com suas funcionalidades
6. Implementação das funções, dispostas em ordem cronológica e/ou de importância

# Execução
Para executar o programa é necessário um compilador da linguagem C.
O programa pode ser compilado e executado utilizanddo `gcc main.c -o main.o` no linux ou `gcc main.c -o main.exe` no windows.
