/** // In�cio do bloco de coment�rios para documenta��o do arquivo.
 * Autor: Gemini // Informa o autor do c�digo.
 * Data: Novembro de 2025 // Data da cria��o/�ltima modifica��o do c�digo.
 *
 * Sistema Bancario Basico em C // Breve descri��o do objetivo do programa.
 * Implementa cadastro e manutencao de contas correntes usando structs, vetores // Detalha as estruturas de dados utilizadas.
 * e funcoes. Todos os dados sao mantidos apenas em memoria (RAM). // Informa que os dados s�o vol�teis (em mem�ria).
 *
 * Compilacao: // Instru��es sobre como compilar o c�digo.
 * Para compilar, utilize um compilador C padrao como o GCC: // Sugere o uso do compilador GCC.
 * gcc banco_simples.c -o banco // Comando de compila��o.
 *
 * Execucao: // Instru��es sobre como executar o programa.
 * ./banco // Comando de execu��o.
 */ // Fim do bloco de coment�rios de documenta��o.

#include <stdio.h> // Inclui a biblioteca padr�o de I/O (Input/Output), essencial para printf e scanf.
#include <stdlib.h> // Inclui a biblioteca padr�o (Standard Library), usada geralmente para fun��es como exit().
#include <string.h> // Inclui a biblioteca para manipulacao de strings (e.g., strcmp, strncpy).
#include <ctype.h> // Inclui a biblioteca para manipulacao de caracteres (e.g., tolower).

// -----------------------------------------------------------------------------
// CONSTANTES E ESTRUTURAS // Se��o para defini��es de constantes e a estrutura principal.
// -----------------------------------------------------------------------------

#define MAX_CONTAS 100 // Define o tamanho maximo do vetor de contas (capacidade maxima).
#define TAM_NOME 100 // Define o tamanho maximo para a string do nome.
#define TAM_CPF 15      // Ex: "000.000.000-00\0" // Define o tamanho maximo para a string do CPF.
#define TAM_AGENCIA 10 // Define o tamanho maximo para a string da agencia.
#define TAM_TELEFONE 20 // Ex: "(XX) XXXXX-XXXX\0" // Define o tamanho maximo para a string do telefone.
#define ATIVA 1 // Define a constante para o status de conta ATIVA.
#define ENCERRADA 0 // Define a constante para o status de conta ENCERRADA.
#define SUCESSO 1 // Define um codigo de retorno para sucesso nas funcoes.
#define ERRO 0 // Define um codigo de retorno para erro nas funcoes.
#define FILTRO_TODAS 2 // Define um codigo de filtro para listar todas as contas.

/** // In�cio do coment�rio de documentacao da estrutura.
 * Estrutura que representa uma Conta Corrente. // Descricao da estrutura.
 */ // Fim do coment�rio de documentacao da estrutura.
typedef struct { // Define a nova estrutura 'Conta'.
    int numero;             /* numero unico da conta */ // Campo para o numero sequencial e unico da conta.
    char nome[TAM_NOME]; // Vetor de caracteres para armazenar o nome do cliente.
    char cpf[TAM_CPF]; // Vetor de caracteres para armazenar o CPF do cliente.
    char agencia[TAM_AGENCIA]; // Vetor de caracteres para armazenar o numero da agencia.
    char telefone[TAM_TELEFONE]; // Vetor de caracteres para armazenar o telefone de contato.
    double saldo; // Variavel para armazenar o saldo atual da conta (tipo ponto flutuante).
    int status;             /* ATIVA (1) ou ENCERRADA (0) */ // Variavel para armazenar o status da conta.
} Conta; // Nome do tipo da estrutura.

// -----------------------------------------------------------------------------
// PROTOTIPOS DE FUNCOES AUXILIARES // Se��o para declaracao de funcoes auxiliares internas.
// -----------------------------------------------------------------------------

void limpar_buffer(); // Funcao para limpar o buffer de entrada (stdin).
int ler_string(char *dest, int max_len); // Funcao para ler strings com seguranca.
int validar_cpf_unico(const Conta contas[], int qtd, const char* cpf); // Funcao para verificar se o CPF ja esta em uso em uma conta ativa.
void mostrar_dados(const Conta* c); // Funcao para exibir os detalhes de uma unica conta.

// -----------------------------------------------------------------------------
// PROTOTIPOS DE FUNCOES OBRIGATORIAS (Logica do Sistema) // Se��o para declaracao das funcoes principais do sistema.
// -----------------------------------------------------------------------------

int encontrar_conta_por_numero(const Conta contas[], int qtd, int numero); // Busca o indice de uma conta pelo numero.
int encontrar_conta_por_cpf(const Conta contas[], int qtd, const char* cpf); // Busca o indice de uma conta pelo CPF.

int abrir_conta(Conta contas[], int* qtd, int proximo_numero, // Funcao para criar e inicializar uma nova conta.
                const char* nome, const char* cpf, // Parametros: nome e CPF do cliente.
                const char* agencia, const char* telefone); // Parametros: agencia e telefone.

int depositar(Conta contas[], int qtd, int numero, double valor); // Funcao para adicionar valor ao saldo da conta.
int sacar(Conta contas[], int qtd, int numero, double valor); // Funcao para retirar valor do saldo da conta.
int transferir(Conta contas[], int qtd, int origem, int destino, double valor); // Funcao para transferir valor entre duas contas.

void consultar_saldo_e_dados(const Conta contas[], int qtd); // Funcao que gerencia a consulta de dados por numero ou CPF.

int atualizar_contato(Conta contas[], int qtd, int numero, // Funcao para atualizar os dados de contato e agencia.
                      const char* novo_telefone, const char* nova_agencia); // Parametros: novo telefone e nova agencia.

int encerrar_conta(Conta contas[], int qtd, int numero); // Funcao para mudar o status de uma conta para ENCERRADA.

void listar_contas(const Conta contas[], int qtd, int filtro_status); // Funcao para listar todas as contas com filtro de status.

// -----------------------------------------------------------------------------
// IMPLEMENTACAO DE FUNCOES AUXILIARES E DE E/S // Se��o para a implementacao das funcoes auxiliares.
// -----------------------------------------------------------------------------

/** // In�cio do comentario de documentacao da funcao.
 * Limpa o buffer de entrada (stdin) para evitar problemas com fgets/scanf misturados. // Descricao da funcao.
 */ // Fim do comentario de documentacao.
void limpar_buffer() { // In�cio da funcao limpar_buffer.
    int c; // Declara uma variavel para armazenar caracteres.
    while ((c = getchar()) != '\n' && c != EOF); // Le e descarta caracteres ate encontrar nova linha ou fim do arquivo.
} // Fim da funcao limpar_buffer.

/** // In�cio do comentario de documentacao da funcao.
 * Le uma string do usuario, garante que nao haja estouro de buffer e remove o \n. // Descricao da funcao.
 * Retorna SUCESSO ou ERRO. // Retorno da funcao.
 */ // Fim do comentario de documentacao.
int ler_string(char *dest, int max_len) { // In�cio da funcao ler_string.
    if (fgets(dest, max_len, stdin) == NULL) { // Tenta ler a string, limitando pelo tamanho maximo.
        printf("Erro na leitura de entrada.\n"); // Mensagem de erro se a leitura falhar.
        return ERRO; // Retorna ERRO.
    } // Fim do bloco if.
    // Remove a quebra de linha se estiver presente // Comentario sobre a limpeza da string.
    size_t len = strlen(dest); // Obtem o comprimento da string lida.
    if (len > 0 && dest[len - 1] == '\n') { // Verifica se o ultimo caractere e a quebra de linha.
        dest[len - 1] = '\0'; // Substitui a quebra de linha por um terminador nulo.
    } else { // Caso a quebra de linha nao tenha sido lida (input maior que o buffer).
        // Se a string lida for maior que o buffer, limpa o restante // Comentario sobre a limpeza do buffer.
        if (len == max_len - 1) { // Verifica se o buffer foi preenchido totalmente (sem ler o \n).
            limpar_buffer(); // Chama a funcao para limpar o restante do buffer.
        } // Fim do bloco if interno.
    } // Fim do bloco else.
    return SUCESSO; // Retorna SUCESSO.
} // Fim da funcao ler_string.

/** // In�cio do comentario de documentacao da funcao.
 * Exibe todos os dados de uma conta formatados. // Descricao da funcao.
 */ // Fim do comentario de documentacao.
void mostrar_dados(const Conta* c) { // In�cio da funcao mostrar_dados.
    if (c == NULL) return; // Verifica se o ponteiro da conta e nulo e sai se for.

    printf("----------------------------------------\n"); // Imprime linha separadora.
    printf("  DADOS DA CONTA\n"); // Titulo da secao.
    printf("----------------------------------------\n"); // Imprime linha separadora.
    printf("  Numero: %d\n", c->numero); // Exibe o numero da conta.
    printf("  Cliente: %s\n", c->nome); // Exibe o nome do cliente.
    printf("  CPF: %s\n", c->cpf); // Exibe o CPF do cliente.
    printf("  Agencia: %s\n", c->agencia); // Exibe a agencia da conta.
    printf("  Telefone: %s\n", c->telefone); // Exibe o telefone do cliente.
    printf("  Status: %s\n", c->status == ATIVA ? "ATIVA" : "ENCERRADA"); // Exibe o status da conta (ternario).
    printf("  Saldo: R$ %.2f\n", c->saldo); // Exibe o saldo da conta formatado.
    printf("----------------------------------------\n"); // Imprime linha separadora.
} // Fim da funcao mostrar_dados.

/** // In�cio do comentario de documentacao da funcao.
 * Valida se o CPF ja existe em alguma conta ATIVA (requisito para abrir nova conta). // Descricao da funcao.
 * Retorna SUCESSO (1) se o CPF for unico entre contas ativas, ou ERRO (0) caso contrario. // Retorno da funcao.
 */ // Fim do comentario de documentacao.
int validar_cpf_unico(const Conta contas[], int qtd, const char* cpf) { // In�cio da funcao validar_cpf_unico.
    int i; // Declara a variavel de iteracao.
    for (i = 0; i < qtd; i++) { // Inicia o loop para percorrer todas as contas validas.
        // Verifica se o CPF e igual E se a conta esta ATIVA // Comentario sobre a condicao de validacao.
        if (contas[i].status == ATIVA && strcmp(contas[i].cpf, cpf) == 0) { // Checa status ATIVA E comparacao de strings de CPF.
            return ERRO; // CPF ja existe em conta ativa, retorna ERRO.
        } // Fim do bloco if.
    } // Fim do loop for.
    return SUCESSO; // CPF e unico entre contas ativas, retorna SUCESSO.
} // Fim da funcao validar_cpf_unico.

// -----------------------------------------------------------------------------
// IMPLEMENTACAO DE FUNCOES OBRIGATORIAS (Logica do Sistema) // Se��o para a implementacao das funcoes principais.
// -----------------------------------------------------------------------------

/** // In�cio do comentario de documentacao da funcao.
 * Busca uma conta pelo numero. // Descricao da funcao.
 * Retorna o indice da conta no vetor ou -1 se nao encontrar. // Retorno da funcao.
 */ // Fim do comentario de documentacao.
int encontrar_conta_por_numero(const Conta contas[], int qtd, int numero) { // In�cio da funcao.
    int i; // Declara a variavel de iteracao.
    for (i = 0; i < qtd; i++) { // Inicia o loop para percorrer todas as contas.
        if (contas[i].numero == numero) { // Verifica se o numero da conta atual e o procurado.
            return i; // Retorna o indice da conta encontrada.
        } // Fim do bloco if.
    } // Fim do loop for.
    return -1; // Se o loop terminar sem encontrar a conta, retorna -1.
} // Fim da funcao encontrar_conta_por_numero.

/** // In�cio do comentario de documentacao da funcao.
 * Busca uma conta pelo CPF. // Descricao da funcao.
 * Retorna o indice da conta no vetor ou -1 se nao encontrar. // Retorno da funcao.
 */ // Fim do comentario de documentacao.
int encontrar_conta_por_cpf(const Conta contas[], int qtd, const char* cpf) { // In�cio da funcao.
    int i; // Declara a variavel de iteracao.
    for (i = 0; i < qtd; i++) { // Inicia o loop para percorrer todas as contas.
        // Nao precisamos verificar o status aqui, pois a busca e apenas para exibicao // Comentario sobre a otimizacao da busca.
        if (strcmp(contas[i].cpf, cpf) == 0) { // Compara a string do CPF da conta atual com o CPF procurado.
            return i; // Retorna o indice da conta encontrada.
        } // Fim do bloco if.
    } // Fim do loop for.
    return -1; // Se o loop terminar sem encontrar a conta, retorna -1.
} // Fim da funcao encontrar_conta_por_cpf.

/** // In�cio do comentario de documentacao da funcao.
 * Abre uma nova conta no sistema. // Descricao da funcao.
 * Retorna o numero da conta criada em caso de SUCESSO, ou ERRO (0) em falha. // Retorno da funcao.
 */ // Fim do comentario de documentacao.
int abrir_conta(Conta contas[], int* qtd, int proximo_numero, // In�cio da funcao.
                const char* nome, const char* cpf, // Parametros (cont.).
                const char* agencia, const char* telefone) { // Parametros (cont.).

    // 1. Validacao de capacidade // Comentario sobre a primeira validacao.
    if (*qtd >= MAX_CONTAS) { // Verifica se o vetor de contas esta cheio.
        printf("\n[ERRO] O vetor de contas esta cheio. Nao e possivel abrir novas contas.\n"); // Mensagem de erro.
        return ERRO; // Retorna ERRO.
    } // Fim do bloco if.

    // 2. Validacao de CPF unico (apenas em contas ATIVAS) // Comentario sobre a segunda validacao.
    if (!validar_cpf_unico(contas, *qtd, cpf)) { // Chama a funcao auxiliar para validar unicidade do CPF.
        printf("\n[ERRO] CPF ja cadastrado em uma conta ATIVA.\n"); // Mensagem de erro.
        return ERRO; // Retorna ERRO.
    } // Fim do bloco if.

    // 3. Criacao da conta // Comentario sobre a criacao.
    int novo_indice = *qtd; // O novo indice e a quantidade atual de contas.

    contas[novo_indice].numero = proximo_numero; // Atribui o numero sequencial a nova conta.
    contas[novo_indice].saldo = 0.0; // Inicializa o saldo com zero.
    contas[novo_indice].status = ATIVA; // Define o status inicial como ATIVA.

    // Copia de dados // Comentario sobre a copia de strings.
    strncpy(contas[novo_indice].nome, nome, TAM_NOME - 1); // Copia o nome com limite de seguranca.
    contas[novo_indice].nome[TAM_NOME - 1] = '\0'; // Garante o terminador nulo para evitar lixo de memoria.

    strncpy(contas[novo_indice].cpf, cpf, TAM_CPF - 1); // Copia o CPF com limite.
    contas[novo_indice].cpf[TAM_CPF - 1] = '\0'; // Garante o terminador nulo.

    strncpy(contas[novo_indice].agencia, agencia, TAM_AGENCIA - 1); // Copia a agencia com limite.
    contas[novo_indice].agencia[TAM_AGENCIA - 1] = '\0'; // Garante o terminador nulo.

    strncpy(contas[novo_indice].telefone, telefone, TAM_TELEFONE - 1); // Copia o telefone com limite.
    contas[novo_indice].telefone[TAM_TELEFONE - 1] = '\0'; // Garante o terminador nulo.

    // Atualiza a quantidade total de contas // Comentario sobre a atualizacao.
    (*qtd)++; // Incrementa a quantidade total de contas validas (passado por ponteiro).

    printf("\n[SUCESSO] Conta %d aberta para %s.\n", proximo_numero, nome); // Mensagem de sucesso.
    return proximo_numero; // Retorna o numero da conta criada.
} // Fim da funcao abrir_conta.

/** // In�cio do comentario de documentacao da funcao.
 * Realiza um deposito na conta especificada. // Descricao da funcao.
 * Retorna SUCESSO ou ERRO. // Retorno da funcao.
 */ // Fim do comentario de documentacao.
int depositar(Conta contas[], int qtd, int numero, double valor) { // In�cio da funcao.
    int indice = encontrar_conta_por_numero(contas, qtd, numero); // Encontra o indice da conta.

    if (indice == -1) { // Verifica se a conta foi encontrada.
        printf("\n[ERRO] Conta numero %d nao encontrada.\n", numero); // Mensagem de erro.
        return ERRO; // Retorna ERRO.
    } // Fim do bloco if.

    // Validacao de valor // Comentario sobre a validacao.
    if (valor <= 0) { // Verifica se o valor e positivo.
        printf("\n[ERRO] Valor de deposito deve ser positivo.\n"); // Mensagem de erro.
        return ERRO; // Retorna ERRO.
    } // Fim do bloco if.

    // Validacao de status // Comentario sobre a validacao.
    if (contas[indice].status == ENCERRADA) { // Verifica se a conta esta encerrada.
        printf("\n[ERRO] Operacao nao permitida. A conta %d esta encerrada.\n", numero); // Mensagem de erro.
        return ERRO; // Retorna ERRO.
    } // Fim do bloco if.

    contas[indice].saldo += valor; // Adiciona o valor ao saldo da conta.
    printf("\n[SUCESSO] Deposito de R$ %.2f realizado na conta %d.\n", valor, numero); // Mensagem de sucesso.
    printf("Novo saldo: R$ %.2f\n", contas[indice].saldo); // Exibe o novo saldo.
    return SUCESSO; // Retorna SUCESSO.
} // Fim da funcao depositar.

/** // In�cio do comentario de documentacao da funcao.
 * Realiza um saque na conta especificada. // Descricao da funcao.
 * Retorna SUCESSO ou ERRO. // Retorno da funcao.
 */ // Fim do comentario de documentacao.
int sacar(Conta contas[], int qtd, int numero, double valor) { // In�cio da funcao.
    int indice = encontrar_conta_por_numero(contas, qtd, numero); // Encontra o indice da conta.

    if (indice == -1) { // Verifica se a conta foi encontrada.
        printf("\n[ERRO] Conta numero %d nao encontrada.\n", numero); // Mensagem de erro.
        return ERRO; // Retorna ERRO.
    } // Fim do bloco if.

    // Validacao de valor // Comentario sobre a validacao.
    if (valor <= 0) { // Verifica se o valor e positivo.
        printf("\n[ERRO] Valor de saque deve ser positivo.\n"); // Mensagem de erro.
        return ERRO; // Retorna ERRO.
    } // Fim do bloco if.

    // Validacao de status // Comentario sobre a validacao.
    if (contas[indice].status == ENCERRADA) { // Verifica se a conta esta encerrada.
        printf("\n[ERRO] Operacao nao permitida. A conta %d esta encerrada.\n", numero); // Mensagem de erro.
        return ERRO; // Retorna ERRO.
    } // Fim do bloco if.

    // Validacao de saldo // Comentario sobre a validacao.
    if (valor > contas[indice].saldo) { // Verifica se ha saldo suficiente.
        printf("\n[ERRO] Saldo insuficiente (R$ %.2f) para sacar R$ %.2f.\n", contas[indice].saldo, valor); // Mensagem de erro.
        return ERRO; // Retorna ERRO.
    } // Fim do bloco if.

    contas[indice].saldo -= valor; // Subtrai o valor do saldo.
    printf("\n[SUCESSO] Saque de R$ %.2f realizado na conta %d.\n", valor, numero); // Mensagem de sucesso.
    printf("Novo saldo: R$ %.2f\n", contas[indice].saldo); // Exibe o novo saldo.
    return SUCESSO; // Retorna SUCESSO.
} // Fim da funcao sacar.

/** // In�cio do comentario de documentacao da funcao.
 * Realiza uma transferencia entre duas contas. // Descricao da funcao.
 * Retorna SUCESSO ou ERRO. // Retorno da funcao.
 */ // Fim do comentario de documentacao.
int transferir(Conta contas[], int qtd, int origem, int destino, double valor) { // In�cio da funcao.
    int idx_origem = encontrar_conta_por_numero(contas, qtd, origem); // Encontra o indice da conta de origem.
    int idx_destino = encontrar_conta_por_numero(contas, qtd, destino); // Encontra o indice da conta de destino.

    // 1. Validacao de Contas // Comentario sobre o bloco de validacao.
    if (idx_origem == -1) { // Verifica se a conta de origem existe.
        printf("\n[ERRO] Conta de origem (%d) nao encontrada.\n", origem); // Mensagem de erro.
        return ERRO; // Retorna ERRO.
    } // Fim do bloco if.
    if (idx_destino == -1) { // Verifica se a conta de destino existe.
        printf("\n[ERRO] Conta de destino (%d) nao encontrada.\n", destino); // Mensagem de erro.
        return ERRO; // Retorna ERRO.
    } // Fim do bloco if.
    if (origem == destino) { // Verifica se as contas de origem e destino sao diferentes.
        printf("\n[ERRO] Contas de origem e destino sao as mesmas.\n"); // Mensagem de erro.
        return ERRO; // Retorna ERRO.
    } // Fim do bloco if.

    // 2. Validacao de Status // Comentario sobre o bloco de validacao.
    if (contas[idx_origem].status == ENCERRADA) { // Verifica se a conta de origem esta ATIVA.
        printf("\n[ERRO] Conta de origem (%d) esta encerrada.\n", origem); // Mensagem de erro.
        return ERRO; // Retorna ERRO.
    } // Fim do bloco if.
    if (contas[idx_destino].status == ENCERRADA) { // Verifica se a conta de destino esta ATIVA.
        printf("\n[ERRO] Conta de destino (%d) esta encerrada.\n", destino); // Mensagem de erro.
        return ERRO; // Retorna ERRO.
    } // Fim do bloco if.

    // 3. Validacao de Valor e Saldo // Comentario sobre o bloco de validacao.
    if (valor <= 0) { // Verifica se o valor e positivo.
        printf("\n[ERRO] Valor de transferencia deve ser positivo.\n"); // Mensagem de erro.
        return ERRO; // Retorna ERRO.
    } // Fim do bloco if.
    if (valor > contas[idx_origem].saldo) { // Verifica se a conta de origem tem saldo suficiente.
        printf("\n[ERRO] Saldo insuficiente na conta de origem (%d).\n", origem); // Mensagem de erro.
        return ERRO; // Retorna ERRO.
    } // Fim do bloco if.

    // 4. Execucao da Transferencia // Comentario sobre a execucao.
    contas[idx_origem].saldo -= valor; // Debita o valor da conta de origem.
    contas[idx_destino].saldo += valor; // Credita o valor na conta de destino.

    printf("\n[SUCESSO] Transferencia de R$ %.2f de conta %d para conta %d realizada.\n", // Mensagem de sucesso.
           valor, origem, destino); // Parametros da mensagem.
    printf("Conta Origem %d: Novo Saldo R$ %.2f\n", origem, contas[idx_origem].saldo); // Exibe novo saldo da origem.
    printf("Conta Destino %d: Novo Saldo R$ %.2f\n", destino, contas[idx_destino].saldo); // Exibe novo saldo do destino.
    return SUCESSO; // Retorna SUCESSO.
} // Fim da funcao transferir.

/** // In�cio do comentario de documentacao da funcao.
 * Funcao unificada para consultar saldo e dados, buscando por numero ou CPF. // Descricao da funcao.
 */ // Fim do comentario de documentacao.
void consultar_saldo_e_dados(const Conta contas[], int qtd) { // In�cio da funcao.
    int opcao_busca; // Variavel para armazenar a opcao de busca (1 ou 2).
    int indice = -1; // Variavel para armazenar o indice da conta encontrada.

    printf("\n--- CONSULTAR CONTA ---\n"); // Titulo.
    printf("Buscar por:\n"); // Opcoes de busca.
    printf("1. Numero da Conta\n"); // Opcao 1.
    printf("2. CPF do Cliente\n"); // Opcao 2.
    printf("Escolha: "); // Pede a escolha do usuario.
    if (scanf("%d", &opcao_busca) != 1) { // Tenta ler a opcao.
        printf("[ERRO] Entrada invalida.\n"); // Mensagem de erro se nao for um inteiro.
        limpar_buffer(); // Limpa o buffer apos o erro de leitura.
        return; // Sai da funcao.
    } // Fim do bloco if.
    limpar_buffer(); // Limpa o buffer apos leitura bem-sucedida.

    if (opcao_busca == 1) { // Se a opcao for buscar por numero da conta.
        int numero_busca; // Variavel para armazenar o numero procurado.
        printf("Digite o numero da conta: "); // Pede o numero da conta.
        if (scanf("%d", &numero_busca) != 1) { // Tenta ler o numero.
            printf("[ERRO] Numero de conta invalido.\n"); // Mensagem de erro.
            limpar_buffer(); // Limpa o buffer.
            return; // Sai da funcao.
        } // Fim do bloco if.
        limpar_buffer(); // Limpa o buffer.
        indice = encontrar_conta_por_numero(contas, qtd, numero_busca); // Chama a funcao de busca por numero.
    } else if (opcao_busca == 2) { // Se a opcao for buscar por CPF.
        char cpf_busca[TAM_CPF]; // Variavel para armazenar o CPF procurado.
        printf("Digite o CPF (max %d caracteres): ", TAM_CPF - 1); // Pede o CPF.
        if (ler_string(cpf_busca, TAM_CPF) == ERRO) return; // Chama a funcao segura para ler o CPF e sai em caso de erro.
        indice = encontrar_conta_por_cpf(contas, qtd, cpf_busca); // Chama a funcao de busca por CPF.
    } else { // Se a opcao for invalida (diferente de 1 e 2).
        printf("[ERRO] Opcao de busca invalida.\n"); // Mensagem de erro.
        return; // Sai da funcao.
    } // Fim do bloco if/else if/else.

    if (indice != -1) { // Verifica se o indice encontrado e valido.
        mostrar_dados(&contas[indice]); // Se for, exibe os dados da conta.
    } else { // Se o indice for -1.
        printf("\n[ERRO] Conta nao encontrada com o criterio fornecido.\n"); // Mensagem de erro.
    } // Fim do bloco if/else.
} // Fim da funcao consultar_saldo_e_dados.


/** // In�cio do comentario de documentacao da funcao.
 * Atualiza o telefone e a agencia de uma conta. // Descricao da funcao.
 * Retorna SUCESSO ou ERRO. // Retorno da funcao.
 */ // Fim do comentario de documentacao.
int atualizar_contato(Conta contas[], int qtd, int numero, // In�cio da funcao.
                      const char* novo_telefone, const char* nova_agencia) { // Parametros (cont.).

    int indice = encontrar_conta_por_numero(contas, qtd, numero); // Encontra o indice da conta.

    if (indice == -1) { // Verifica se a conta foi encontrada.
        printf("\n[ERRO] Conta numero %d nao encontrada.\n", numero); // Mensagem de erro.
        return ERRO; // Retorna ERRO.
    } // Fim do bloco if.

    // Atualiza Telefone // Comentario sobre a atualizacao.
    strncpy(contas[indice].telefone, novo_telefone, TAM_TELEFONE - 1); // Copia o novo telefone, truncando se necessario.
    contas[indice].telefone[TAM_TELEFONE - 1] = '\0'; // Garante o terminador nulo.

    // Atualiza Agencia // Comentario sobre a atualizacao.
    strncpy(contas[indice].agencia, nova_agencia, TAM_AGENCIA - 1); // Copia a nova agencia, truncando se necessario.
    contas[indice].agencia[TAM_AGENCIA - 1] = '\0'; // Garante o terminador nulo.

    printf("\n[SUCESSO] Dados de contato e agencia da conta %d atualizados.\n", numero); // Mensagem de sucesso.
    printf("Novo Telefone: %s\n", contas[indice].telefone); // Exibe o novo telefone.
    printf("Nova Agencia: %s\n", contas[indice].agencia); // Exibe a nova agencia.
    return SUCESSO; // Retorna SUCESSO.
} // Fim da funcao atualizar_contato.

/** // In�cio do comentario de documentacao da funcao.
 * Lista as contas no vetor, aplicando um filtro de status. // Descricao da funcao.
 */ // Fim do comentario de documentacao.
void listar_contas(const Conta contas[], int qtd, int filtro_status) { // In�cio da funcao.
    printf("\n--- LISTAGEM DE CONTAS ---\n"); // Titulo.

    if (qtd == 0) { // Verifica se ha contas cadastradas.
        printf("Nenhuma conta cadastrada no momento.\n"); // Mensagem para vetor vazio.
        return; // Sai da funcao.
    } // Fim do bloco if.

    int i; // Variavel de iteracao.
    int encontradas = 0; // Contador para saber quantas contas foram listadas.

    for (i = 0; i < qtd; i++) { // Inicia o loop para percorrer todas as contas.
        // Verifica o filtro // Comentario sobre a condicao de filtro.
        if (filtro_status == FILTRO_TODAS || contas[i].status == filtro_status) { // Checa se lista todas OU se o status corresponde ao filtro.
            printf("----------------------------------------\n"); // Separador.
            printf("  Conta: %d\n", contas[i].numero); // Exibe o numero da conta.
            printf("  Cliente: %s\n", contas[i].nome); // Exibe o nome do cliente.
            printf("  Agencia: %s\n", contas[i].agencia); // Exibe a agencia.
            printf("  Saldo: R$ %.2f\n", contas[i].saldo); // Exibe o saldo.
            printf("  Status: %s\n", contas[i].status == ATIVA ? "ATIVA" : "ENCERRADA"); // Exibe o status.
            encontradas++; // Incrementa o contador.
        } // Fim do bloco if.
    } // Fim do loop for.

    if (encontradas == 0) { // Se o contador for zero (nenhuma conta listada apos o filtro).
        if (filtro_status == ATIVA) { // Se o filtro era por ativas.
            printf("Nenhuma conta ATIVA encontrada.\n"); // Mensagem especifica.
        } else if (filtro_status == ENCERRADA) { // Se o filtro era por encerradas.
            printf("Nenhuma conta ENCERRADA encontrada.\n"); // Mensagem especifica.
        } // Fim do bloco if/else if.
    } else { // Se pelo menos uma conta foi listada.
         printf("----------------------------------------\n"); // Separador.
         printf("Total de contas listadas: %d\n", encontradas); // Exibe o total encontrado.
    } // Fim do bloco if/else.
} // Fim da funcao listar_contas.

/** // In�cio do comentario de documentacao da funcao.
 * Encerra uma conta (muda o status para ENCERRADA). // Descricao da funcao.
 * Retorna SUCESSO ou ERRO. // Retorno da funcao.
 */ // Fim do comentario de documentacao.
int encerrar_conta(Conta contas[], int qtd, int numero) { // In�cio da funcao.
    int indice = encontrar_conta_por_numero(contas, qtd, numero); // Encontra o indice da conta.

    if (indice == -1) { // Verifica se a conta foi encontrada.
        printf("\n[ERRO] Conta numero %d nao encontrada.\n", numero); // Mensagem de erro.
        return ERRO; // Retorna ERRO.
    } // Fim do bloco if.

    if (contas[indice].status == ENCERRADA) { // Verifica se a conta ja esta encerrada.
        printf("\n[ERRO] A conta %d ja esta encerrada.\n", numero); // Mensagem de erro.
        return ERRO; // Retorna ERRO.
    } // Fim do bloco if.

    // Validacao de saldo // Comentario sobre a validacao.
    if (contas[indice].saldo != 0.0) { // Verifica se o saldo e diferente de zero.
        printf("\n[ERRO] Encerramento nao permitido. Saldo deve ser zero (R$ %.2f).\n", contas[indice].saldo); // Mensagem de erro.
        return ERRO; // Retorna ERRO.
    } // Fim do bloco if.

    // Executa o encerramento // Comentario sobre a execucao.
    contas[indice].status = ENCERRADA; // Altera o status da conta para ENCERRADA.
    printf("\n[SUCESSO] Conta %d encerrada com sucesso.\n", numero); // Mensagem de sucesso.
    return SUCESSO; // Retorna SUCESSO.
} // Fim da funcao encerrar_conta.


// -----------------------------------------------------------------------------
// FUNCAO PRINCIPAL (MAIN) // Se��o para a funcao principal do programa.
// -----------------------------------------------------------------------------

int main() { // In�cio da funcao main.
    Conta contas[MAX_CONTAS]; // Declara o vetor de structs Conta (dados do sistema).
    int qtd_contas = 0; // Variavel para rastrear a quantidade atual de contas validas (inicia em 0).
    int proximo_numero_conta = 1001; // Variavel que guarda o proximo numero de conta a ser usado (inicia em 1001).
    int opcao = 0; // Variavel para armazenar a opcao do menu escolhida pelo usuario.
    char temp_nome[TAM_NOME], temp_cpf[TAM_CPF], temp_agencia[TAM_AGENCIA], temp_telefone[TAM_TELEFONE]; // Buffers temporarios para leitura de strings.
    int num_origem, num_destino, num_conta, filtro_status; // Variaveis para numeros de conta e filtros.
    double valor; // Variavel para armazenar valores de transacao (deposito, saque, transferencia).

    printf("==========================================\n"); // Imprime separador.
    printf("      SISTEMA BANCARIO SIMPLES (C)\n"); // Titulo do sistema.
    printf("==========================================\n"); // Imprime separador.

    // Loop principal do menu // Comentario sobre o loop.
    do { // In�cio do laco 'do-while' (executa pelo menos uma vez).
        printf("\n--- MENU PRINCIPAL ---\n"); // Titulo do menu.
        printf("1. Abrir conta\n"); // Opcao 1: Abrir conta.
        printf("2. Depositar\n"); // Opcao 2: Depositar.
        printf("3. Sacar\n"); // Opcao 3: Sacar.
        printf("4. Transferir\n"); // Opcao 4: Transferir.
        printf("5. Consultar saldo e dados (Numero/CPF)\n"); // Opcao 5: Consultar dados.
        printf("6. Atualizar telefone e agencia\n"); // Opcao 6: Atualizar dados.
        printf("7. Listar contas\n"); // Opcao 7: Listar contas.
        printf("8. Encerrar conta\n"); // Opcao 8: Encerrar conta.
        printf("9. Sair\n"); // Opcao 9: Sair do programa.
        printf("----------------------\n"); // Separador.
        printf("Total de Contas Ativas: %d. Proxima: %d\n", qtd_contas, proximo_numero_conta); // Exibe o status atual do sistema.
        printf("Escolha uma opcao: "); // Pede a entrada do usuario.

        // Garante que apenas um inteiro seja lido para a opcao // Comentario sobre a leitura segura.
        if (scanf("%d", &opcao) != 1) { // Tenta ler a opcao do menu.
            printf("\n[ERRO] Opcao invalida. Digite um numero de 1 a 9.\n"); // Mensagem de erro se nao for um inteiro.
            limpar_buffer(); // Limpa o buffer apos a leitura falhar.
            opcao = 0; // Reseta a opcao para garantir que o loop continue e nao entre em um 'case' invalido.
            continue; // Pula para a proxima iteracao do loop.
        } // Fim do bloco if.
        limpar_buffer(); // Limpa o buffer apos scanf (necessario antes de futuras leituras de string).

        switch (opcao) { // In�cio do bloco switch para processar a opcao.
            case 1: // Abrir conta // Caso 1: Abrir conta.
                printf("\n--- ABRIR CONTA ---\n"); // Titulo da operacao.

                printf("Nome do Cliente (max %d): ", TAM_NOME - 1); // Pede o nome.
                if (ler_string(temp_nome, TAM_NOME) == ERRO) break; // Le o nome de forma segura e quebra se falhar.

                printf("CPF (max %d): ", TAM_CPF - 1); // Pede o CPF.
                if (ler_string(temp_cpf, TAM_CPF) == ERRO) break; // Le o CPF de forma segura e quebra se falhar.

                printf("Agencia (max %d): ", TAM_AGENCIA - 1); // Pede a agencia.
                if (ler_string(temp_agencia, TAM_AGENCIA) == ERRO) break; // Le a agencia de forma segura e quebra se falhar.

                printf("Telefone (max %d): ", TAM_TELEFONE - 1); // Pede o telefone.
                if (ler_string(temp_telefone, TAM_TELEFONE) == ERRO) break; // Le o telefone de forma segura e quebra se falhar.

                if (abrir_conta(contas, &qtd_contas, proximo_numero_conta, // Chama a funcao para abrir a conta.
                                temp_nome, temp_cpf, temp_agencia, temp_telefone) > 0) { // Se a abertura for bem-sucedida (retorna > 0).
                    proximo_numero_conta++; // Incrementa o contador do proximo numero de conta.
                } // Fim do bloco if.
                break; // Sai do switch.

            case 2: // Depositar // Caso 2: Depositar.
                printf("\n--- DEPOSITAR ---\n"); // Titulo.
                printf("Numero da conta: "); // Pede o numero da conta.
                if (scanf("%d", &num_conta) != 1) { printf("[ERRO] Entrada invalida.\n"); limpar_buffer(); break; } // Le o numero com validacao.
                printf("Valor do deposito: R$ "); // Pede o valor.
                if (scanf("%lf", &valor) != 1) { printf("[ERRO] Entrada invalida.\n"); limpar_buffer(); break; } // Le o valor com validacao.
                limpar_buffer(); // Limpa o buffer.
                depositar(contas, qtd_contas, num_conta, valor); // Chama a funcao de deposito.
                break; // Sai do switch.

            case 3: // Sacar // Caso 3: Sacar.
                printf("\n--- SACAR ---\n"); // Titulo.
                printf("Numero da conta: "); // Pede o numero da conta.
                if (scanf("%d", &num_conta) != 1) { printf("[ERRO] Entrada invalida.\n"); limpar_buffer(); break; } // Le o numero com validacao.
                printf("Valor do saque: R$ "); // Pede o valor.
                if (scanf("%lf", &valor) != 1) { printf("[ERRO] Entrada invalida.\n"); limpar_buffer(); break; } // Le o valor com validacao.
                limpar_buffer(); // Limpa o buffer.
                sacar(contas, qtd_contas, num_conta, valor); // Chama a funcao de saque.
                break; // Sai do switch.

            case 4: // Transferir // Caso 4: Transferir.
                printf("\n--- TRANSFERIR ---\n"); // Titulo.
                printf("Numero da conta de ORIGEM: "); // Pede a origem.
                if (scanf("%d", &num_origem) != 1) { printf("[ERRO] Entrada invalida.\n"); limpar_buffer(); break; } // Le a origem com validacao.
                printf("Numero da conta de DESTINO: "); // Pede o destino.
                if (scanf("%d", &num_destino) != 1) { printf("[ERRO] Entrada invalida.\n"); limpar_buffer(); break; } // Le o destino com validacao.
                printf("Valor da transferencia: R$ "); // Pede o valor.
                if (scanf("%lf", &valor) != 1) { printf("[ERRO] Entrada invalida.\n"); limpar_buffer(); break; } // Le o valor com validacao.
                limpar_buffer(); // Limpa o buffer.
                transferir(contas, qtd_contas, num_origem, num_destino, valor); // Chama a funcao de transferencia.
                break; // Sai do switch.

            case 5: // Consultar saldo e dados // Caso 5: Consultar saldo e dados.
                consultar_saldo_e_dados(contas, qtd_contas); // Chama a funcao de consulta unificada.
                break; // Sai do switch.

            case 6: // Atualizar telefone e agencia // Caso 6: Atualizar dados.
                printf("\n--- ATUALIZAR DADOS ---\n"); // Titulo.
                printf("Numero da conta a atualizar: "); // Pede o numero da conta.
                if (scanf("%d", &num_conta) != 1) { printf("[ERRO] Entrada invalida.\n"); limpar_buffer(); break; } // Le o numero com validacao.
                limpar_buffer(); // Limpa o buffer.

                printf("Novo Telefone (max %d): ", TAM_TELEFONE - 1); // Pede o novo telefone.
                if (ler_string(temp_telefone, TAM_TELEFONE) == ERRO) break; // Le o telefone de forma segura.

                printf("Nova Agencia (max %d): ", TAM_AGENCIA - 1); // Pede a nova agencia.
                if (ler_string(temp_agencia, TAM_AGENCIA) == ERRO) break; // Le a agencia de forma segura.

                atualizar_contato(contas, qtd_contas, num_conta, temp_telefone, temp_agencia); // Chama a funcao de atualizacao.
                break; // Sai do switch.

            case 7: // Listar contas // Caso 7: Listar contas.
                printf("\n--- FILTRO DE LISTAGEM ---\n"); // Titulo.
                printf("Listar por status:\n"); // Opcoes de filtro.
                printf("1. Contas ATIVAS\n"); // Filtro ATIVA.
                printf("0. Contas ENCERRADAS\n"); // Filtro ENCERRADA.
                printf("2. TODAS as contas\n"); // Filtro TODAS.
                printf("Escolha (1, 0 ou 2): "); // Pede o filtro.

                if (scanf("%d", &filtro_status) != 1) { // Le o filtro com validacao.
                    printf("[ERRO] Entrada invalida.\n"); // Mensagem de erro.
                    limpar_buffer(); // Limpa o buffer.
                    break; // Sai do switch.
                } // Fim do bloco if.
                limpar_buffer(); // Limpa o buffer.

                if (filtro_status == ATIVA || filtro_status == ENCERRADA || filtro_status == FILTRO_TODAS) { // Verifica se o filtro e valido.
                    listar_contas(contas, qtd_contas, filtro_status); // Chama a funcao de listagem.
                } else { // Se o filtro for invalido.
                    printf("[ERRO] Opcao de filtro invalida.\n"); // Mensagem de erro.
                } // Fim do bloco if/else.
                break; // Sai do switch.

            case 8: // Encerrar conta // Caso 8: Encerrar conta.
                printf("\n--- ENCERRAR CONTA ---\n"); // Titulo.
                printf("Numero da conta a encerrar: "); // Pede o numero da conta.
                if (scanf("%d", &num_conta) != 1) { printf("[ERRO] Entrada invalida.\n"); limpar_buffer(); break; } // Le o numero com validacao.
                limpar_buffer(); // Limpa o buffer.
                encerrar_conta(contas, qtd_contas, num_conta); // Chama a funcao de encerramento.
                break; // Sai do switch.

            case 9: // Sair // Caso 9: Sair do programa.
                printf("\nTem certeza que deseja sair (s/n)? "); // Pede confirmacao.
                char confirmacao; // Variavel para a confirmacao.
                if (scanf(" %c", &confirmacao) == 1) { // Le a confirmacao (o espaco antes de %c ignora espacos em branco).
                    if (tolower(confirmacao) != 's') { // Verifica se a resposta nao e 's' (ou 'S').
                        opcao = 0; // Se for 'n' (ou qualquer outra coisa), reseta a opcao para continuar.
                        printf("Retornando ao menu...\n"); // Mensagem de retorno.
                    } else { // Se for 's' ou 'S'.
                        printf("\nObrigado por utilizar o Sistema Bancario Simples. Ate mais!\n"); // Mensagem de despedida.
                    } // Fim do bloco if/else interno.
                } // Fim do bloco if.
                limpar_buffer(); // Limpa o buffer.
                break; // Sai do switch.

            default: // Opcao invalida (fora do range 1-9).
                printf("\n[ERRO] Opcao invalida. Tente novamente.\n"); // Mensagem de erro.
                break; // Sai do switch.
        } // Fim do bloco switch.

    } while (opcao != 9); // Condicao para repetir o loop (enquanto a opcao nao for 9 - Sair).

    return 0; // Retorna 0, indicando que o programa encerrou com sucesso.
} // Fim da funcao main.
