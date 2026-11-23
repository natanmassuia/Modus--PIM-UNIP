# MODUS — AI Agent Registry

**Catálogo pessoal de agentes de Inteligência Artificial**, desenvolvido em linguagem C como **PIM II** (Projeto Integrado Multidisciplinar) do curso de **CST em Análise e Desenvolvimento de Sistemas — UNIP Ribeirão Preto, 2026/2**.

---

## Sobre o projeto

Quem usa assistentes de IA cria instruções (prompts) que funcionam bem, mas elas ficam espalhadas em históricos de conversa e anotações, são reescritas a cada uso e não há como saber qual versão foi a melhor. O **MODUS** resolve isso com um catálogo em console no qual o usuário cadastra, lista, busca, edita e exclui agentes de IA, cada um com ID permanente, nome, prompt, categoria, tags e nota.

O MVP gerencia as **configurações** dos agentes; a execução real da IA está fora do escopo.

## Funcionalidades

- **Cadastro progressivo com preview**: nome, prompt, categoria, tags e nota; o agente só é salvo após a confirmação, e cancelar não consome ID.
- **Listagem** dos agentes ativos (ID, nome, nota e categoria).
- **Busca** por ID (mostra todos os dados), por nome, por categoria ou por tag (busca parcial, sem diferenciar maiúsculas de minúsculas).
- **Edição** campo a campo, mantendo o ID.
- **Exclusão lógica** com confirmação: o agente sai das listagens, mas o ID nunca é reaproveitado.
- **Relatório por categoria** (diferencial do projeto): quantidade de agentes ativos e nota média de cada categoria.
- **Persistência em arquivo**: os dados são gravados em `modus_dados.txt` a cada alteração e recarregados ao abrir o programa.

### Regras principais

| Campo | Regra |
|---|---|
| ID | Gerado automaticamente, permanente e nunca reaproveitado |
| Nome | Obrigatório, até 50 caracteres, sem repetição entre agentes ativos |
| Prompt | Obrigatório, até 500 caracteres |
| Categoria | Uma entre 7: Estudos, Culinária, Filmes e Séries, Notícias, Trabalho, Organização, Outros |
| Tags | De 0 a 5 por agente, até 20 caracteres cada |
| Nota | Inteiro de 0 a 5 |
| Limite | 50 agentes ativos |

Os dados ficam em **vetores paralelos**: a mesma posição representa o mesmo agente em todos os vetores.

## Como compilar e executar

É necessário um compilador C, como o **GCC**. O programa usa apenas a biblioteca padrão.

**Linux / macOS**

```bash
gcc PIM_Final.c -o modus
./modus
```

**Windows (MinGW)**

```bash
gcc PIM_Final.c -o modus.exe
modus.exe
```

Na primeira execução o catálogo começa vazio; o arquivo `modus_dados.txt` é criado na primeira gravação, na mesma pasta do programa.

## Estrutura do repositório

```
.
├── PIM_Final.c                  # Código-fonte completo e comentado do programa
├── MODUS_SCRIPT_SQL_1.5.sql     # Banco MySQL 8: criação, dados de exemplo e consultas
├── diagramas/
│   ├── DER_MODUS.png            # Diagrama Entidade-Relacionamento
│   ├── rede_local_MODUS.png     # Diagrama da rede local proposta
│   └── rede_local_MODUS.svg     # Mesmo diagrama em formato vetorial
└── README.md
```

## Banco de dados

O programa persiste os dados em arquivo, como pede o PIM. O script `MODUS_SCRIPT_SQL_1.5.sql` traz a modelagem relacional equivalente (tabelas `categoria`, `agente`, `tag` e `agente_tag`), com dados de exemplo e consultas que reproduzem as funções do programa, inclusive o relatório por categoria. Para rodar em MySQL 8:

```bash
mysql -u usuario -p < MODUS_SCRIPT_SQL_1.5.sql
```

## Limitações conhecidas

- Testado apenas em Linux com GCC.
- Cancelar um cadastro iniciado sobre a vaga de um agente excluído sobrescreve os dados daquele registro inativo.
- Versão de usuário único, sem login; o uso em rede é uma proposta descrita no trabalho escrito.

## Equipe

| Integrante | RA | Frente principal |
|---|---|---|
| Bernardo Moreira Paz | R4180F0 | Normalização ABNT e documentação |
| Daniel José Medeiros | H00CCG0 | Programação em C |
| João Vitor dos Santos Mello | H007GB6 | Redes de computadores, banco de dados e SQL |
| Natan Miguel Gafo Massuia | H006DG9 | Programação em C, Engenharia de Software Ágil, testes e integração do trabalho |

## Disciplinas contempladas

Engenharia de Software Ágil · Modelagem de Banco de Dados e NoSQL · Redes de Computadores e Sistemas Distribuídos · Programação Estruturada em C · Inteligência Artificial, Pesquisa Tecnológica e Inovação · Direitos Humanos · Educação Ambiental.

---

> Projeto acadêmico — PIM II · ADS · UNIP · 2026/2.
