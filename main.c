/*
 Disciplina: Técnicas de Programação
 Aluno: Eduardo Pessonia Molina Cortez
 Matrícula: 2020215387
 
 Sistema de Loja
 
 Esse programa simula um sistema de loja com as seguintes opções:
 1 - CADASTRAR NOVO CLIENTE
 2 - REMOVER CLIENTE
 3 - ATUALIZAR MONTANTE DE COMPRAS DE CLIENTE
 4 - VIRAR MÊS (zera montantes de compras de todos os clientes)
 5 - VERIFICAR MELHOR COMPRADOR
 6 - VERIFICAR MONTANTE DE COMPRAS DE CLIENTE
 7 - LISTA COMPLETA DE CLIENTES

 Quando os clientes são cadastrados, eles são automaticamente colocados em ordem alfabética.
 
 Quando o usuário deseja realizar alterações em um cliente, o processo se inicia com uma busca na base de dados e com a consequente confirmação de que o cliente a ser alterado é o correto.
 
 A base de dados de clientes está diretamente alocada na memória heap.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>

typedef struct cliente
{
    char nome[31];
    int AnoDeNascimento;
    float MontanteDeCompras;
} CLIENTE;

void NovoCliente(CLIENTE *ListaClientes, int *QtdClientes)
{
    printf("\n\nInsira nome e sobrenome do cliente (limite: 30 caracteres):\n");
    fgets(ListaClientes->nome, 31, stdin);
    ListaClientes->nome[strcspn(ListaClientes->nome, "\n")] = 0;
    fflush(stdin);
    printf("\n\nInsira o ano de nascimento do cliente;\n");
    scanf("%d",&ListaClientes->AnoDeNascimento);
    fflush(stdin);
    ListaClientes->MontanteDeCompras = 0;
    *QtdClientes = *QtdClientes + 1;
}

int BuscaPorNome (CLIENTE ListaClientes[], int *QtdClientes, int *NomesImpressos)
{
    char NomeBuscado[31], NomeTemp[31];
    int i, j;
    *NomesImpressos = 0;
    
    printf("\n\nFaça uma pesquisa para consultar a base de clientes. \nDigite uma parte inicial do nome do cliente que deseja buscar:\n");
    fgets(NomeBuscado, 31, stdin);
    NomeBuscado[strcspn(NomeBuscado, "\n")] = 0;
    
    //Colocar o nome buscado em maiúsculas
    for (i = 0; i <= strlen(NomeBuscado); i++)
        NomeBuscado[i] = toupper(NomeBuscado[i]);

    printf("\nLista de nomes encontrados:");
    for(i = 0; i < *QtdClientes; i++)
    {
        // Passagem temporária do nome do cadastro para maiúsculas
        for (j = 0; j < 31; j++)
            NomeTemp[j] = toupper(ListaClientes[i].nome[j]);
        
        // Impressão dos nomes encontrados
        if (strncmp(NomeBuscado, NomeTemp, strlen(NomeBuscado)) == 0)
        {
            printf("\n%d\t%s", i, ListaClientes[i].nome);
            *NomesImpressos = *NomesImpressos + 1;
        }
    }
    
    return *NomesImpressos;
}

char BuscaFail ()
{
    int RespostaValida = 0;
    char ReiniciarBusca = 'S';
    
    printf("\n\nNENHUM NOME ENCONTRADO\n\n");
    while (RespostaValida == 0)
    {
        printf("Deseja reiniciar a busca? Digite S para SIM ou N para NÃO\n");
        scanf("%c", &ReiniciarBusca);
        fflush(stdin);
        ReiniciarBusca = toupper(ReiniciarBusca);
           
        switch (ReiniciarBusca)
        {
            case 'S':
                RespostaValida = 1;
                //NomesImpressos = 1;
                break;
            case 'N':
                RespostaValida = 1;
                //NomesImpressos = 1;
                break;
            default:
                printf("\nResposta inválida.\n\n");
        }
    }
    return ReiniciarBusca;
}

void RemoverCliente(CLIENTE ListaClientes[], int *QtdClientes)
{
    char ReiniciarBusca;
    int ClienteRemovido; // Armazena a informação sobre qual cliente será removido.
    int ConfirmarOperacao = 0; // Confirma a remoção do cliente.
    int i;
    int NomesImpressos = 0;
    int TotalClientes = *QtdClientes;

    
    ReiniciarBusca = 'S';
    do
    {
        BuscaPorNome(ListaClientes, &TotalClientes, &NomesImpressos);
        
        if (NomesImpressos != 0)
        {
            ReiniciarBusca = 'N';   // Evita o reinício da busca
            printf("\n\n Digite o número do cliente que deseja remover: ");
            scanf("%d", &ClienteRemovido);
            fflush(stdin);
            printf("\nVocê está preste a remover o cliente %s.", ListaClientes[ClienteRemovido].nome);
            printf("\nDigite 1 para continuar ou 0 para cancelar a operação e voltar ao início.\n");
            scanf("%d", &ConfirmarOperacao);
            fflush(stdin);
            
            if(ConfirmarOperacao == 1)
            {
                for(i = ClienteRemovido; i < *QtdClientes; i++)
                {
                    memcpy(ListaClientes[i].nome, ListaClientes[(i+1)].nome, sizeof(char[31]));
                    ListaClientes[i].AnoDeNascimento = ListaClientes[(i+1)].AnoDeNascimento;
                    ListaClientes[i].MontanteDeCompras = ListaClientes[(i+1)].MontanteDeCompras;
                }
                *QtdClientes = *QtdClientes - 1;
            }
        }
        
        //Busca mal sucedida
        if (NomesImpressos == 0)
            ReiniciarBusca = BuscaFail();
        
    } while (ReiniciarBusca == 'S');
}

void AtualizarMontante(CLIENTE ListaClientes[], int *QtdClientes)
{
    char ReiniciarBusca = 'S';
    int TotalClientes = *QtdClientes;
    int NomesImpressos = 0;
    int ClienteComprando;
    int ConfirmarOperacao = 0; // Confirma a alteração do cliente.
    float NovoValor;
    
    while (ReiniciarBusca == 'S')
    {
        ReiniciarBusca = 'N';   // Evita o reinício da busca
        BuscaPorNome(ListaClientes, &TotalClientes, &NomesImpressos);
        
        // Busca mal sucedida
        if (NomesImpressos == 0)
            ReiniciarBusca = BuscaFail();
        
        // Busca bem sucedida
        if (NomesImpressos != 0)
        {
            printf("\n\n Digite o número do cliente que está fazendo a compra: ");
            scanf("%d", &ClienteComprando);
            fflush(stdin);
            printf("\nVocê está prestes a alterar o montante do cliente %s.", ListaClientes[ClienteComprando].nome);
            printf("\nDigite 1 para continuar ou 0 para cancelar a operação e voltar ao início.\n");
            scanf("%d", &ConfirmarOperacao);
            fflush(stdin);
            
            if(ConfirmarOperacao == 1)
            {
                printf("\nDigite o valor a ser somado no montante de compras do cliente \n(Use apenas números e ponto (.) para separar as casas decimais).\n");
                scanf("%f", &NovoValor);
                fflush(stdin);
                ListaClientes[ClienteComprando].MontanteDeCompras = ListaClientes[ClienteComprando].MontanteDeCompras + NovoValor;
                printf("\nMontante atualizado. O valor atual do montante de compras do cliente %s é de R$ %.2f .", ListaClientes[ClienteComprando].nome, ListaClientes[ClienteComprando].MontanteDeCompras);
            }
        }
    }
}

void VirarMes (CLIENTE ListaClientes[], int *QtdClientes)
{
    printf("\n\nVocê está prestes a apagar os montantes de compras de seus clientes. Deseja continuar?");
    printf("\nDigite 1 para continuar ou 0 para cancelar a operação.");
    
    int Confirmar = 0;
    
    if (Confirmar == 1)
    {
        for(int i = 0; i < *QtdClientes; i++)
        {
            ListaClientes[i].MontanteDeCompras = 0;
        }
    }
}

void MelhorComprador (CLIENTE ListaClientes[], int *QtdClientes)
{
    char NomeTemp[31];
    
    strcpy(NomeTemp, ListaClientes[0].nome);
    
    for(int i = 1; i < *QtdClientes; i++)
    {
        if (ListaClientes[1].MontanteDeCompras > ListaClientes[i-1].MontanteDeCompras)
            strcpy(NomeTemp, ListaClientes[i].nome);
    }
    
    printf("\n\nSeu melhor cliente é: %s !!!", NomeTemp);
}

void VerificarMontante (CLIENTE ListaClientes[], int *QtdClientes)
{
    char ReiniciarBusca = 'S';
    int TotalClientes = *QtdClientes;
    int NomesImpressos = 0;
    int VerMontante;
    
    while (ReiniciarBusca == 'S')
    {
        ReiniciarBusca = 'N';   // Evita o reinício da busca
        BuscaPorNome(ListaClientes, &TotalClientes, &NomesImpressos);
        
        // Busca mal sucedida
        if (NomesImpressos == 0)
            ReiniciarBusca = BuscaFail();
        
        // Busca bem sucedida
        if (NomesImpressos != 0)
        {
            printf("\n\n Digite o número do cliente para ver seu montante de compras atual: ");
            scanf("%d", &VerMontante);
            fflush(stdin);
            
            printf("\nO montante de compras atual de %s é de R$ %.2f", ListaClientes[VerMontante].nome, ListaClientes[VerMontante].MontanteDeCompras);
        }
    }
}

void ListaCompleta (CLIENTE ListaClientes[], int *QtdClientes)
{
    printf("\n\nNum\tNome\tNascimento\tMontante de compras");
    for (int i = 0; i < *QtdClientes; i++)
    {
        printf("\n%d\t%s\t%d\t\t%.2f", i, ListaClientes[i].nome, ListaClientes[i].AnoDeNascimento, ListaClientes[i].MontanteDeCompras);
    }
}

void Ordenar (CLIENTE ListaClientes[], int *QtdClientes)
{
    CLIENTE ClienteTemp, ClienteAlfa;
    int PosAlfa;
    int a;
    
    for(int c1 = 0; c1 < *QtdClientes; c1++)
    {
        ClienteAlfa = ListaClientes[c1];
        PosAlfa = c1;
        for(int c2 = c1 + 1; c2 < *QtdClientes; c2++)
        {
            a = 0;
            if(ListaClientes[c2].nome[a] < ListaClientes[c1].nome[a])
            {
                PosAlfa = c2;
                ClienteAlfa = ListaClientes[c2];
            }
            
            while(ListaClientes[c2].nome[a] == ListaClientes[c1].nome[a])
            {
                a = a +1;
                if(ListaClientes[c2].nome[a] < ListaClientes[c1].nome[a])
                {
                    PosAlfa = c2;
                    ClienteAlfa = ListaClientes[c2];
                }
            }
        }
        if(PosAlfa != c1)
        {
            ClienteTemp = ListaClientes[c1];
            ListaClientes[c1] = ListaClientes[PosAlfa];
            ListaClientes[PosAlfa] = ClienteTemp;
        }
    }
}

int main()
{
   /*
        Operações em Stack
            - Declarar variáveis;
            - Configurar um ponteiro para o vetor de clientes
    */
    
    setlocale(LC_ALL, "Portuguese");

    int NumMaxClientes = 10; // Valor para ser alocado em heap
    int QtdClientes = 0; // Quantidade real de clientes cadastrados
    CLIENTE *Lista;
    int loop = 0; // Controla o loop do programa
    int controle; // Opção selecionada pelo usuário
    int i; // Contador genérico
    
    /*
        Operações em Heap
            - Malloc;
            - Looping do programa até que o usuário deseje sair
                - A cada looping, verificar se a quantidade de clientes cadastrados é um múltiplo de 10; se for, incrementar a alocação de memória em sizeof((CLIENTE *) * 10)
     */
    
    for (i = 0; i < NumMaxClientes; i++)
        Lista = (CLIENTE *) malloc(NumMaxClientes * sizeof(CLIENTE));
    
    while (loop == 0)
    {
        // Verificar se é necessário alocar mais espaço
        if (QtdClientes % 10 == 0)
        {
            NumMaxClientes = NumMaxClientes + 10;
            Lista = realloc(Lista, NumMaxClientes * sizeof(CLIENTE));
        }
        
        // Boas vindas
        printf("\n");
        printf("\n######################################");
        printf("\n");
        printf("\n          SISTEMA DA LOJA");
        printf("\n");
        printf("\n######################################");
        printf("\n");
        printf("\n     SELECIONE A OPÇÃO DESEJADA:");
        printf("\n");
        printf("\n1 - CADASTRAR NOVO CLIENTE");
        printf("\n2 - REMOVER CLIENTE");
        printf("\n3 - ATUALIZAR MONTANTE DE COMPRAS DE CLIENTE");
        printf("\n4 - VIRAR MÊS");
        printf("\n5 - VERIFICAR MELHOR COMPRADOR");
        printf("\n6 - VERIFICAR MONTANTE DE COMPRAS DE CLIENTE");
        printf("\n7 - LISTA COMPLETA DE CLIENTES");
        printf("\n0 - SAIR");
        printf("\n");
        
        scanf("%d", &controle);
        fflush(stdin);
        
        switch (controle)
        {
            case 1: // CADASTRAR NOVO CLIENTE
                NovoCliente(&Lista[QtdClientes], &QtdClientes);
                Ordenar(Lista, &QtdClientes);
                break;
            case 2: // REMOVER CLIENTE
                if (QtdClientes == 0)
                {
                    printf("\nVocê ainda não possui nenhum cliente cadastrado.");
                    printf("\n\nPressione \"enter\" para continuar...\n");
                    getchar();
                    fflush(stdin);
                }
                else
                    RemoverCliente(Lista, &QtdClientes);
                break;
            case 3: // ATUALIZAR MONTANTE DE COMPRAS DE CLIENTE
                if (QtdClientes == 0)
                {
                    printf("\nVocê ainda não possui nenhum cliente cadastrado.");
                    printf("\n\nPressione \"enter\" para continuar...\n");
                    getchar();
                    fflush(stdin);
                }
                else
                {
                    AtualizarMontante(Lista, &QtdClientes);
                    printf("\n\nPressione \"enter\" para continuar...\n");
                    getchar();
                    fflush(stdin);
                }
                break;
            case 4: // VIRAR MÊS
                if (QtdClientes == 0)
                {
                    printf("\nVocê ainda não possui nenhum cliente cadastrado.");
                    printf("\n\nPressione \"enter\" para continuar...\n");
                    getchar();
                    fflush(stdin);
                }
                else
                {
                    VirarMes(Lista, &QtdClientes);
                    printf("\n\nPressione \"enter\" para continuar...\n");
                    getchar();
                    fflush(stdin);
                }
                break;
            case 5: // VERIFICAR MELHOR COMPRADOR
                if (QtdClientes == 0)
                {
                    printf("\nVocê ainda não possui nenhum cliente cadastrado.");
                    printf("\n\nPressione \"enter\" para continuar...\n");
                    getchar();
                    fflush(stdin);
                }
                else
                {
                    MelhorComprador(Lista, &QtdClientes);
                    printf("\n\nPressione \"enter\" para continuar...\n");
                    getchar();
                    fflush(stdin);
                }
                break;
            case 6: // VERIFICAR MONTANTE DE COMPRAS DE CLIENTE
                if (QtdClientes == 0)
                {
                    printf("\nVocê ainda não possui nenhum cliente cadastrado.");
                    printf("\n\nPressione \"enter\" para continuar...\n");
                    getchar();
                    fflush(stdin);
                }
                else
                {
                    VerificarMontante(Lista, &QtdClientes);
                    printf("\n\nPressione \"enter\" para continuar...\n");
                    getchar();
                    fflush(stdin);
                }
                break;
            case 7: // LISTA COMPLETA DE CLIENTES
                if (QtdClientes == 0)
                {
                    printf("\nVocê ainda não possui nenhum cliente cadastrado.");
                    printf("\n\nPressione \"enter\" para continuar...\n");
                    getchar();
                    fflush(stdin);
                }
                else
                {
                    ListaCompleta(Lista, &QtdClientes);
                    printf("\n\nPressione \"enter\" para continuar...\n");
                    getchar();
                    fflush(stdin);
                }
                break;
            case 0: // SAIR
                loop = 1; // sai do loop
                break;
            default:
                printf("\nOpção inválida.\n");
                break;
        }
        
    }
    
    /*
        Saída do programa ==> Liberar heap.
     */
    
    free(Lista);
}
