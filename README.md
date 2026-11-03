# MODUS — AI Agent Registry

**Catálogo pessoal de agentes de Inteligência Artificial**, desenvolvido em linguagem C como projeto do **PIM II** (Projeto Integrado Multidisciplinar) do curso de **CST em Análise e Desenvolvimento de Sistemas — UNIP, 2026/2 — Equipe 7**.

---

## Sobre o projeto

Hoje as empresas e os usuários criam diversos "agentes de IA" — configurações com um papel específico (tutor de estudos, assistente de e-mails, crítico de filmes etc.), cada um com seu nome, suas instruções (prompt) e sua finalidade. Na prática, esses prompts e configurações acabam espalhados em conversas e ferramentas, difíceis de localizar e reutilizar.

O **MODUS** resolve esse problema oferecendo um catálogo centralizado onde o usuário pode cadastrar, organizar, consultar e gerenciar seus agentes de IA a partir de um programa de console.

No MVP deste PIM, o foco é o **gerenciamento das configurações** dos agentes. A execução real da IA por uma plataforma externa está fora do escopo.

## Funcionalidades

### Implementadas
- Boas-vindas personalizadas com o nome do usuário
- Menu principal interativo (console)
- **Cadastrar agente** — nome, prompt/instruções, categoria, tags e nota (0 a 5), com *preview* do agente ao final
- **Listar agentes** cadastrados
- **Buscar agente** por nome ou parte do nome (busca parcial)
- Validação da nota (apenas inteiros de 0 a 5)
- Controle do limite de 50 agentes

### Em desenvolvimento
- **Editar agente** (opção 4 do menu)
- **Excluir agente** — exclusão lógica (opção 5 do menu)
- **Persistência em arquivo** — salvar e recuperar os agentes ao reabrir o programa
- Validação de nome único (impedir cadastros duplicados)
- Categorias predefinidas (atualmente a categoria é texto livre)

## Especificações técnicas

Conforme os requisitos do PIM, cada agente é armazenado em **vetores paralelos** (a mesma posição de índice guarda os dados de um mesmo agente):

| Campo | Regra |
|---|---|
| ID | Identificador exibido na listagem/busca |
| Nome | Até 80 caracteres (`TAM_CARAC`) |
| Prompt/instruções | Até 500 caracteres (`CARACTERES`) |
| Categoria | Texto da área do agente |
| Tags | Palavras-chave para busca |
| Nota | Valor inteiro entre 0 e 5 |

- Limite de **50 agentes** (`MAX_AGENTES`)
- Uso de estruturas de controle (`do/while`, `switch/case`, `for`), funções e manipulação de strings da biblioteca padrão do C
- Leitura de texto com `fgets` e limpeza de buffer, evitando estouro de entrada

## Como compilar e executar

É necessário um compilador C (ex.: **GCC**).

**Linux / macOS:**
```bash
gcc modus.c -o modus
./modus
```

**Windows (MinGW):**
```bash
gcc modus.c -o modus.exe
modus.exe
```

## Estrutura do repositório

```
.
├── modus.c      # Código-fonte do catálogo (programa principal)
└── README.md    # Este arquivo
```

## Equipe 7

| Integrante | Frente principal |
|---|---|
| Natan + Daniel | Desenvolvimento em C |
| Mello | Banco de Dados / SQL |
| Pablo | Redes e Infraestrutura |
| Rodrigo | Engenharia de Software Ágil / Requisitos |
| Bernardo | ABNT / Documentação |
| João | QA / Testes / Manual |

## Disciplinas contempladas

Engenharia de Software Ágil · Modelagem de Banco de Dados e NoSQL · Redes de Computadores e Sistemas Distribuídos · Programação Estruturada em C · Inteligência Artificial, Pesquisa Tecnológica e Inovação.

---

> Projeto acadêmico — PIM II · ADS · UNIP · 2026/2.
