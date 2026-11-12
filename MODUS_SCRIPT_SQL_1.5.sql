-- =========================================================
-- MODUS - AI Agent Registry | Script de criação (MySQL 8)
-- Estrutura equivalente aos vetores paralelos de PIM_Final.c
-- =========================================================
CREATE DATABASE IF NOT EXISTS modus;
USE modus;

-- Vetor CATEGORIAS[7] do programa (mesma ordem)
CREATE TABLE categoria (
  id_categoria   INT AUTO_INCREMENT PRIMARY KEY,
  nome           VARCHAR(30) NOT NULL UNIQUE
);

-- Vetores id, ativo, nome, promptIA, categoria e nota
CREATE TABLE agente (
  id_agente      INT AUTO_INCREMENT PRIMARY KEY,
  id_categoria   INT NOT NULL,
  nome           VARCHAR(80)  NOT NULL,
  prompt         VARCHAR(1000) NOT NULL,
  nota           INT NOT NULL,
  ativo          TINYINT NOT NULL DEFAULT 1,
  nome_ativo     VARCHAR(80) GENERATED ALWAYS AS
                 (CASE WHEN ativo = 1 THEN nome END) STORED,
  CONSTRAINT ck_agente_nome   CHECK (TRIM(nome) <> ''),
  CONSTRAINT ck_agente_prompt CHECK (TRIM(prompt) <> ''),
  CONSTRAINT ck_agente_nota   CHECK (nota BETWEEN 0 AND 5),
  CONSTRAINT ck_agente_ativo  CHECK (ativo IN (0, 1)),
  CONSTRAINT uq_agente_nome_ativo UNIQUE (nome_ativo),
  CONSTRAINT fk_agente_categoria FOREIGN KEY (id_categoria)
             REFERENCES categoria(id_categoria)
);

-- Vetor tags (cada tag com até 20 caracteres)
CREATE TABLE tag (
  id_tag         INT AUTO_INCREMENT PRIMARY KEY,
  nome           VARCHAR(20) NOT NULL UNIQUE
);

-- Ligação agente x tag (até 5 tags por agente, controlado pela aplicação)
CREATE TABLE agente_tag (
  id_agente      INT NOT NULL,
  id_tag         INT NOT NULL,
  PRIMARY KEY (id_agente, id_tag),
  CONSTRAINT fk_at_agente FOREIGN KEY (id_agente) REFERENCES agente(id_agente),
  CONSTRAINT fk_at_tag    FOREIGN KEY (id_tag)    REFERENCES tag(id_tag)
);
-- Categorias fixas (mesma ordem do vetor CATEGORIAS)
INSERT INTO categoria (nome) VALUES
  ('Estudos'), ('Culinária'), ('Filmes e Séries'), ('Notícias'),
  ('Trabalho'), ('Organização'), ('Outros');

-- Agentes de exemplo (ativo: 1 = ativo, 0 = excluído)
INSERT INTO agente (id_categoria, nome, prompt, nota, ativo) VALUES
  (1, 'Tutor de Matemática',      'Você é um tutor paciente que explica matemática passo a passo.', 4, 1),
  (2, 'Chef de Receitas Rápidas', 'Sugira receitas simples com os ingredientes informados.',        5, 1),
  (3, 'Crítico de Filmes',        'Recomende filmes e séries com base nas preferências do usuário.', 4, 1),
  (4, 'Resumo de Notícias',       'Resuma as notícias do dia em até cinco tópicos.',               4, 1),
  (1, 'Revisor de Redação ENEM',  'Avalie a redação segundo as cinco competências do ENEM.',       5, 1),
  (6, 'Assistente de E-mails',    'Redija e-mails claros e objetivos para a rotina pessoal.',      3, 1),
  (7, 'Planejador de Viagens',    'Crie roteiros de viagem detalhados por dia.',                   4, 1),
  (2, 'Receitas Fit',             'Sugira receitas saudáveis e de baixa caloria.',                 3, 0),
  (6, 'Organizador de Tarefas',   'Organize as tarefas por prioridade e prazo.',                   4, 1);

-- Tags
INSERT INTO tag (nome) VALUES
  ('matemática'), ('estudos'), ('receitas'), ('lazer'), ('produtividade'),
  ('ENEM'), ('viagem'), ('e-mail'), ('notícias');

-- Ligação agente x tag (N:N)
INSERT INTO agente_tag (id_agente, id_tag) VALUES
  (1, 1), (1, 2), (2, 3), (3, 4), (4, 9), (5, 6), (5, 2),
  (6, 8), (6, 5), (7, 7), (7, 4), (8, 3), (9, 5);

-- ===== Consultas =====
-- Listar agentes ativos (listarAgentes())
SELECT a.id_agente, a.nome, a.nota, c.nome AS categoria
FROM agente a
JOIN categoria c ON c.id_categoria = a.id_categoria
WHERE a.ativo = 1
ORDER BY a.id_agente;

-- Buscar agentes por parte do nome (buscarAgente(), opção 2)
SELECT a.id_agente, a.nome, a.nota, c.nome AS categoria
FROM agente a
JOIN categoria c ON c.id_categoria = a.id_categoria
WHERE a.ativo = 1
  AND a.nome LIKE '%receita%'
ORDER BY a.id_agente;

-- Buscar agentes por tag (buscarAgente(), opção 4)
SELECT DISTINCT a.id_agente, a.nome, a.nota, c.nome AS categoria
FROM agente a
JOIN categoria c   ON c.id_categoria = a.id_categoria
JOIN agente_tag atg ON atg.id_agente = a.id_agente
JOIN tag t         ON t.id_tag = atg.id_tag
WHERE a.ativo = 1
  AND t.nome LIKE '%estudo%'
ORDER BY a.id_agente;

-- Relatório por categoria (relatorioPorCategoria())
SELECT c.nome AS categoria,
       COUNT(a.id_agente)    AS quantidade,
       ROUND(AVG(a.nota), 1) AS nota_media
FROM categoria c
LEFT JOIN agente a
       ON a.id_categoria = c.id_categoria
      AND a.ativo = 1
GROUP BY c.id_categoria, c.nome
ORDER BY c.id_categoria;

-- Quantidade de tags de cada agente (vetor qtdTags)
SELECT a.id_agente, a.nome, COUNT(atg.id_tag) AS qtd_tags
FROM agente a
LEFT JOIN agente_tag atg ON atg.id_agente = a.id_agente
WHERE a.ativo = 1
GROUP BY a.id_agente, a.nome
ORDER BY a.id_agente;

-- Exclusão lógica (excluirAgente)
UPDATE agente SET ativo = 0 WHERE id_agente = 3;
