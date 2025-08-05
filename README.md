# Sistema de Cadastro de Fornecedores

Este projeto em linguagem C implementa um sistema de gerenciamento de fornecedores com suporte a diferentes modos de operação, incluindo:

- Interface local via terminal (modo normal)
- Servidor e cliente TCP
- Servidor HTTP básico

O sistema permite adicionar, listar e remover fornecedores, com persistência em arquivo de texto (`fornecedores.txt`).

## 📁 Estrutura de Dados

A estrutura principal utilizada é uma lista encadeada de fornecedores, contendo os seguintes campos:

- `nome`
- `cnpj`
- `telefone`
- `email`

## 🚀 Funcionalidades

- Adicionar, listar e remover fornecedores
- Persistência dos dados em arquivo texto (`fornecedores.txt`)
- Interface de menu local (terminal)
- Comunicação via socket TCP entre cliente e servidor
- Interface web básica via servidor HTTP

## 🛠️ Requisitos

- Sistema Windows (usa `<winsock2.h>`)
- Compilador compatível com C (ex: GCC via MinGW ou Visual Studio)
- Biblioteca `ws2_32.lib` para sockets no Windows

## 🧭 Modos de Operação

Ao iniciar o programa, você poderá selecionar o modo desejado:

### 1. Modo Normal (Menu Local)
Interface baseada em terminal para gerenciar fornecedores localmente.

### 2. Servidor TCP
Executa um servidor TCP que aceita conexões de clientes, recebendo comandos para gerenciar fornecedores remotamente.

### 3. Cliente TCP
Cliente que se conecta ao servidor TCP e envia comandos para adicionar, listar e remover fornecedores.

### 4. Servidor HTTP
Servidor web básico que exibe os fornecedores em uma página HTML. Permite exclusão via link.

## 💡 Comandos TCP

Os seguintes comandos são suportados via cliente TCP:

- `ADD nome;cnpj;telefone;email`
- `READ`
- `DELETE cnpj`

## 🌐 Interface Web (HTTP)

- Acessar: `http://localhost:8080`
- Lista todos os fornecedores cadastrados
- Permite excluir diretamente via link

## 🗂️ Arquivo de Dados

- O arquivo `fornecedores.txt` é utilizado para salvar os dados entre execuções.
- Formato: `nome;cnpj;telefone;email` por linha.

## 🔧 Compilação

Se estiver utilizando GCC no Windows (via MinGW), compile com:

```bash
gcc -o fornecedores.exe fornecedores.c -lws2_32
```

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

## 📌 Observações

- Este sistema é apenas para fins educacionais e demonstração.
- O servidor TCP aceita apenas uma conexão por vez.
- O servidor HTTP não possui autenticação e é básico.

## 📄 Licença

Este projeto é de domínio público ou pode ser utilizado livremente para fins educacionais.

## 👨‍💻 Autores

- Pedro Bonifácio
- Paulo Henrique