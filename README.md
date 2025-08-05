# Sistema de Cadastro de Fornecedores

Este projeto em linguagem C permite cadastrar, listar e remover **fornecedores** tanto localmente quanto via conexão **TCP (cliente-servidor)**.

## 💡 Funcionalidades

- Adicionar fornecedor (nome, CNPJ, telefone, e-mail)
- Listar fornecedores
- Remover fornecedor por CNPJ
- Execução local ou via rede (cliente TCP conecta a servidor remoto)

## 📁 Estrutura do Projeto

```
.
├── main.c              # Ponto de entrada principal
├── fornecedor.c        # Funções relacionadas ao fornecedor
├── fornecedor.h        # Header com definição da struct e protótipos
├── servidor_tcp.c      # Implementação do modo servidor TCP
├── cliente_tcp.c       # Implementação do cliente TCP
├── Makefile (opcional)
└── README.md           # Este arquivo
```

## 🚀 Execução

### Compilação

```bash
gcc main.c fornecedor.c servidor_tcp.c cliente_tcp.c -o fornecedores
```

### Modo Local

```bash
./fornecedores
```

Siga as instruções no terminal para adicionar, listar ou remover fornecedores localmente.

### Modo TCP

#### 1. Servidor

Em um terminal, inicie o servidor TCP:

```bash
./fornecedores servidor
```

#### 2. Cliente

Em outro terminal, conecte-se como cliente:

```bash
./fornecedores cliente
```

Informe o IP do servidor e a porta (por padrão, a porta é 12345).

## 📡 Comunicação TCP

A comunicação cliente-servidor ocorre via socket TCP. O cliente envia comandos em texto (como `ADD`, `LIST`, `REMOVE`) e o servidor responde com mensagens de confirmação ou a lista de fornecedores.

## ⚙️ Dependências

- Compilador C (GCC recomendado)
- Sistema compatível com sockets (Linux ou WSL recomendado para testes locais)

## ✅ Exemplo de uso

```
1-Adicionar
2-Listar
3-Remover
4-Sair
Opção: 1
Nome: ACME Corp
CNPJ: 12345678000190
Telefone: (11) 99999-9999
Email: contato@acme.com
```

## 🧠 Observações

- O campo `CNPJ` é usado como identificador único para remoção.
- Toda a memória alocada dinamicamente é liberada ao final da execução.
- A função `listar_fornecedores_str()` permite formatar a saída para resposta via socket.

## 👨‍💻 Autores

- Pedro Bonifácio
- [Outros contribuidores, se houver]