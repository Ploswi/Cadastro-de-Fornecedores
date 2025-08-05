#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#define MAX_NOME     100
#define MAX_CNPJ     20
#define MAX_TELEFONE 20
#define MAX_EMAIL    100
#define ARQUIVO "fornecedores.txt"
#define PORTA_TCP 9000
#define PORTA_HTTP 8080
#define BUFFER_SIZE 4096

// Estrutura que representa um fornecedor
typedef struct Fornecedor
{
    char nome[MAX_NOME];
    char cnpj[MAX_CNPJ];
    char telefone[MAX_TELEFONE];
    char email[MAX_EMAIL];
    struct Fornecedor *prox;  // Ponteiro para o próximo fornecedor na lista
} Fornecedor;

// Cria dinamicamente um novo fornecedor e retorna seu ponteiro
Fornecedor* criar_fornecedor(const char *nome, const char *cnpj, const char *telefone, const char *email)
{
    Fornecedor *novo = (Fornecedor*)malloc(sizeof(Fornecedor));
    strncpy(novo->nome, nome, MAX_NOME);
    strncpy(novo->cnpj, cnpj, MAX_CNPJ);
    strncpy(novo->telefone, telefone, MAX_TELEFONE);
    strncpy(novo->email, email, MAX_EMAIL);
    novo->prox = NULL;
    return novo;
}

// Adiciona um fornecedor ao final da lista encadeada
void adicionar_fornecedor(Fornecedor **lista, Fornecedor *novo)
{
    if (*lista == NULL)
    {
        *lista = novo;  // Primeiro elemento da lista
    }
    else
    {
        Fornecedor *atual = *lista;
        while (atual->prox) atual = atual->prox;  // Percorre até o final
        atual->prox = novo;  // Adiciona ao final
    }
}

// Remove um fornecedor da lista com base no CNPJ
// Retorna 1 se encontrado e removido, 0 se não encontrado
int remover_fornecedor(Fornecedor **lista, const char *cnpj)
{
    Fornecedor *atual = *lista, *ant = NULL;
    while (atual)
    {
        if (strcmp(atual->cnpj, cnpj) == 0)
        {
            if (!ant)  // É o primeiro elemento
            {
                *lista = atual->prox;
            }
            else
            {
                ant->prox = atual->prox;
            }
            free(atual);
            return 1;
        }
        ant = atual;
        atual = atual->prox;
    }
    return 0;
}

// Imprime todos os fornecedores da lista no terminal
void listar_fornecedores(Fornecedor *lista)
{
    if (!lista)
    {
        printf("Nenhum fornecedor.\n");
        return;
    }
    while (lista)
    {
        printf("Nome: %s\nCNPJ: %s\nTelefone: %s\nEmail: %s\n-----------------------------------------------\n",
               lista->nome, lista->cnpj, lista->telefone, lista->email);
        lista = lista->prox;
    }
    printf("\n");
}

// Retorna uma string com todos os fornecedores formatados (envio via servidor)
char* listar_fornecedores_str(Fornecedor *lista)
{
    static char buf[BUFFER_SIZE * 2];
    buf[0] = '\0';

    if (!lista) return "Nenhum fornecedor cadastrado.\n";

    while (lista)
    {
        char tmp[512];
        snprintf(tmp, sizeof(tmp), "Nome: %s\nCNPJ: %s\nTelefone: %s\nEmail: %s\n-----------------------------------------------\n",
                 lista->nome, lista->cnpj, lista->telefone, lista->email);
        strncat(buf, tmp, sizeof(buf) - strlen(buf) - 1);  // Concatena ao buffer principal
        lista = lista->prox;
    }
    return buf;
}

// Libera toda a memória alocada da lista de fornecedores
void liberar_lista(Fornecedor *lista)
{
    while (lista)
    {
        Fornecedor *tmp = lista;
        lista = lista->prox;
        free(tmp);
    }
}

// Carrega os fornecedores do arquivo de texto para a lista encadeada
// Retorna 0 em caso de sucesso, -1 se não conseguir abrir o arquivo
int carregar_fornecedores(Fornecedor **lista)
{
    FILE *f = fopen(ARQUIVO, "r");
    if (!f)
    {
        return -1;
    }

    char nome[MAX_NOME], cnpj[MAX_CNPJ], tel[MAX_TELEFONE], email[MAX_EMAIL];
    while (fscanf(f, " %[^;];%[^;];%[^;];%[^\n]\n", nome, cnpj, tel, email) == 4)
    {
        adicionar_fornecedor(lista, criar_fornecedor(nome, cnpj, tel, email));
    }

    fclose(f);
    return 0;
}

// Salva a lista de fornecedores em um arquivo de texto
void salvar_fornecedores(Fornecedor *lista)
{
    FILE *f = fopen(ARQUIVO, "w");
    while (lista)
    {
        fprintf(f, "%s;%s;%s;%s\n", lista->nome, lista->cnpj, lista->telefone, lista->email);
        lista = lista->prox;
    }
    fclose(f);
}

void menu_generico(int modo, void *contexto) {
    int opcao;
    char buf[BUFFER_SIZE];

    do {
        printf("1 - Adicionar\n2 - Listar\n3 - Remover\n4 - Sair\nOpcao: ");
        scanf("%d", &opcao);
        getchar(); // remove '\n'

        if (modo == 0) {
            // Local Mode
            Fornecedor **lista = (Fornecedor **)contexto;
            if (opcao == 1) {
                system("cls"); // Limpa o console
                char nome[MAX_NOME], cnpj[MAX_CNPJ], tel[MAX_TELEFONE], email[MAX_EMAIL];
                printf("Nome: "); fgets(nome, sizeof(nome), stdin); nome[strcspn(nome, "\n")] = 0;
                printf("CNPJ: "); fgets(cnpj, sizeof(cnpj), stdin); cnpj[strcspn(cnpj, "\n")] = 0;
                printf("Telefone: "); fgets(tel, sizeof(tel), stdin); tel[strcspn(tel, "\n")] = 0;
                printf("Email: "); fgets(email, sizeof(email), stdin); email[strcspn(email, "\n")] = 0;
                adicionar_fornecedor(lista, criar_fornecedor(nome, cnpj, tel, email));
            }
            else if (opcao == 2)
            {
                system("cls"); // Limpa o console
                listar_fornecedores(*lista);
            }
            else if (opcao == 3)
            {
                system("cls"); // Limpa o console
                char cnpj[MAX_CNPJ];
                printf("CNPJ: "); fgets(cnpj, sizeof(cnpj), stdin); cnpj[strcspn(cnpj, "\n")] = 0;
                if (!remover_fornecedor(lista, cnpj))
                {
                    printf("Fornecedor nao encontrado.\n");
                }
                else
                {
                    printf("Removido com sucesso.\n");
                }
            }
        }
        else if (modo == 1) {
            // TCP Client Mode
            SOCKET sock = *(SOCKET *)contexto;
            if (opcao == 1) {
                system("cls"); // Limpa o console
                char nome[MAX_NOME], cnpj[MAX_CNPJ], tel[MAX_TELEFONE], email[MAX_EMAIL];
                printf("Nome: "); fgets(nome, sizeof(nome), stdin); nome[strcspn(nome, "\n")] = 0;
                printf("CNPJ: "); fgets(cnpj, sizeof(cnpj), stdin); cnpj[strcspn(cnpj, "\n")] = 0;
                printf("Telefone: "); fgets(tel, sizeof(tel), stdin); tel[strcspn(tel, "\n")] = 0;
                printf("Email: "); fgets(email, sizeof(email), stdin); email[strcspn(email, "\n")] = 0;
                snprintf(buf, sizeof(buf), "ADD %s;%s;%s;%s", nome, cnpj, tel, email);
                send(sock, buf, strlen(buf), 0);
            }
            else if (opcao == 2) {
                strcpy(buf, "READ");
                send(sock, buf, strlen(buf), 0);
            }
            else if (opcao == 3) {
                system("cls"); // Limpa o console
                char cnpj[MAX_CNPJ];
                printf("CNPJ: "); fgets(cnpj, sizeof(cnpj), stdin); cnpj[strcspn(cnpj, "\n")] = 0;
                snprintf(buf, sizeof(buf), "DELETE %s", cnpj);
                send(sock, buf, strlen(buf), 0);
            }
            if(opcao != 4){
                memset(buf, 0, sizeof(buf));
                recv(sock, buf, sizeof(buf)-1, 0);
                system("cls"); // Limpa o console
                printf("Resposta do servidor:\n%s\n", buf);
            }
        }
    }while (opcao != 4);
}

// Servidor TCP
void servidor_tcp(){
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);
    Fornecedor *lista = NULL;
    carregar_fornecedores(&lista);

    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in srv;
    srv.sin_family = AF_INET;
    srv.sin_addr.s_addr = INADDR_ANY;
    srv.sin_port = htons(PORTA_TCP);
    bind(s, (struct sockaddr*)&srv, sizeof(srv));
    listen(s, 1);
    printf("Servidor TCP iniciado na porta %d\n", PORTA_TCP);

    struct sockaddr_in cli;
    int clilen = sizeof(cli);
    SOCKET cs = accept(s, (struct sockaddr*)&cli, &clilen);

    char buf[BUFFER_SIZE];
    while(1)
    {
        memset(buf, 0, sizeof(buf));
        int n = recv(cs, buf, sizeof(buf)-1, 0);
        if(n <= 0) break;
        buf[strcspn(buf, "\r\n")] = 0; // remover \n e \r

        if(_stricmp(buf, "READ") == 0)
        {
            send(cs, listar_fornecedores_str(lista), strlen(listar_fornecedores_str(lista)), 0);
        }
        else if(_strnicmp(buf, "DELETE ", 7) == 0)
        {
            char *cnpj = buf + 7;
            if(remover_fornecedor(&lista, cnpj))
            {
                salvar_fornecedores(lista);
                send(cs, "Removido com sucesso.\n", 23, 0);
            } else {
                send(cs, "Fornecedor nao encontrado.\n", 28, 0);
            }
        }
        else if(_strnicmp(buf, "ADD ", 4) == 0)
        {
            char nome[MAX_NOME], cnpj[MAX_CNPJ], telefone[MAX_TELEFONE], email[MAX_EMAIL];
            if(sscanf(buf + 4, "%[^;];%[^;];%[^;];%[^\n]", nome, cnpj, telefone, email) == 4)
            {
                adicionar_fornecedor(&lista, criar_fornecedor(nome, cnpj, telefone, email));
                salvar_fornecedores(lista);
                send(cs, "Fornecedor adicionado com sucesso.\n", 36, 0);
            }
            else
            {
                send(cs, "Formato invalido. Use: nome;cnpj;telefone;email\n", 52, 0);
            }
        }
        else
        {
            send(cs, "Comando invalido\n", 17, 0);
        }
    }

    salvar_fornecedores(lista);
    liberar_lista(lista);
    closesocket(cs);
    closesocket(s);
    WSACleanup();
}

// Cliente TCP 127.0.0.1
void cliente_tcp(const char *ip, int porta)
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in srv;
    srv.sin_family = AF_INET;
    srv.sin_port = htons(porta);
    inet_pton(AF_INET, ip, &srv.sin_addr);

    if(connect(s, (struct sockaddr*)&srv, sizeof(srv)) < 0)
    {
        printf("Erro conexão\n");
        WSACleanup();
        return;
    }

    menu_generico(1, &s);

    closesocket(s);
    WSACleanup();
}

// Servidor HTTP
void servidor_http(){
    WSADATA wsa; WSAStartup(MAKEWORD(2,2), &wsa);
    Fornecedor *lista = NULL; carregar_fornecedores(&lista);

    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in srv; srv.sin_family = AF_INET;
    srv.sin_addr.s_addr = INADDR_ANY; srv.sin_port = htons(PORTA_HTTP);
    bind(s, (struct sockaddr*)&srv, sizeof(srv));
    listen(s, 5);
    printf("Servidor HTTP iniciado na porta %d\n", PORTA_HTTP);

    while(1)
    {
        struct sockaddr_in cli; int clilen = sizeof(cli);
        SOCKET cs = accept(s, (struct sockaddr*)&cli, &clilen);

        char req[BUFFER_SIZE]; recv(cs, req, sizeof(req)-1, 0);
        char method[8], path[256];
        sscanf(req, "%s %s", method, path);

        if(strcmp(method, "GET") == 0)
        {
            if(strcmp(path, "/") == 0)
            {
                char html[BUFFER_SIZE*2] = "<html><body><h1>Fornecedores</h1><ul>";
                Fornecedor *at = lista;
                while(at){
                    char li[512];
                    snprintf(li, sizeof(li), "<li>%s (%s) - %s - %s [<a href=\"/delete/%s\">Remover</a>]</li>",
                             at->nome, at->cnpj, at->telefone, at->email, at->cnpj);
                    strcat(html, li);
                    at = at->prox;
                }
                strcat(html, "</ul></body></html>");
                char resp[BUFFER_SIZE*3];
                sprintf(resp, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n%s", html);
                send(cs, resp, strlen(resp), 0);
            }
            else if(strncmp(path, "/delete/", 8) == 0)
            {
                if(remover_fornecedor(&lista, path+8))
                {
                    salvar_fornecedores(lista);
                    char resp[] = "HTTP/1.1 302 Found\r\nLocation: /\r\n\r\n";
                    send(cs, resp, strlen(resp), 0);
                }
                else
                {
                    char resp[] = "HTTP/1.1 404 Not Found\r\n\r\nNao encontrado";
                    send(cs, resp, strlen(resp), 0);
                }
            }
            else
            {
                char resp[] = "HTTP/1.1 404 Not Found\r\n\r\nPagina nao encontrada";
                send(cs, resp, strlen(resp), 0);
            }
        }
        closesocket(cs);
    }
    liberar_lista(lista);
    closesocket(s); WSACleanup();
}

void modo_normal()
{
    Fornecedor *lista = NULL;
    if(carregar_fornecedores(&lista) == -1)
    {
        printf("Arquivo de fornecedores nao encontrado. Iniciando lista vazia.\n");
    }
    menu_generico(0, &lista);
    salvar_fornecedores(lista);
    liberar_lista(lista);
}

void modo_servidor_tcp()
{
    // A função servidor_tcp() já carrega e salva os dados internamente
    servidor_tcp();
}

void menu_cliente_tcp()
{
    char ip[100];
    int porta;
    printf("Digite o IP do servidor TCP: ");
    scanf("%s", ip);
    printf("Digite a porta do servidor TCP: ");
    scanf("%d", &porta);
    cliente_tcp(ip, porta);
}

void modo_http()
{
    // A função servidor_http() já carrega e salva os dados internamente
    servidor_http();
}

void selecionar_modo()
{
    int opcao = 0;
    do{
        system("cls"); // Limpa o console quando sair de uma operação
        printf("Modo de operacao:\n");
        printf("1 - Modo Normal (menu local)\n");
        printf("2 - Servidor TCP\n");
        printf("3 - Cliente TCP\n");
        printf("4 - Servidor HTTP\n");
        printf("5 - Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);
        getchar(); // remove '\n'
        system("cls"); // Limpa o console quando escolhe alguma operação
        switch(opcao) {
            case 1:
                modo_normal();
                break;
            case 2:
                modo_servidor_tcp();
                break;
            case 3:
                menu_cliente_tcp();
                break;
            case 4:
                modo_http();
                break;
            case 5:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while(opcao != 5);
}

// ===== Main =====
int main()
{
    selecionar_modo();
    return 0;
}
