# 📚 Sistema de Gerenciamento de Pessoas

[English Version Here](README_en.md)

Um sistema completo desenvolvido em C++ para gerenciar informações de alunos e professores com interface de linha de comando, funcionalidades avançadas de pesquisa e persistência de dados.

## ✨ Funcionalidades

### 🎯 Principais Recursos
- **Cadastro de Pessoas**: Registro completo de alunos e professores
- **Pesquisa Inteligente**: Busca por nome (parcial) ou CPF
- **Listagem Organizadas**: Visualização por categoria ou geral
- **Aniversariantes**: Consulta de aniversariantes por mês específico
- **Persistência Automática**: Salvamento em arquivos texto
- **Validação de Dados**: Verificação de CPF, datas e entrada de dados
- **Gestão de Memória**: Controle dinâmico com prevenção de vazamentos

### 👨‍🎓 Dados dos Alunos
- Nome completo
- CPF (formatado automaticamente)
- Data de nascimento
- Número de matrícula

### 👨‍🏫 Dados dos Professores  
- Nome completo
- CPF (formatado automaticamente)
- Data de nascimento
- Titulação (Especialista, Mestre, Doutor)

## 🚀 Como Usar

### Pré-requisitos
- Compilador C++ com suporte ao C++17 ou superior
- Sistema operacional: Windows, Linux ou macOS

### Compilação
```bash
g++ -std=c++17 -o sistema-gerenciamento-pessoas-cpp main.cpp
```

### Execução
```bash
./sistema-gerenciamento-pessoas-cpp
```

## 📖 Menu Principal

```
MENU PRINCIPAL
0 - Salvar Dados
1 - Cadastrar uma Pessoa
2 - Listar Todas as Pessoas
3 - Pesquisar por Nome
4 - Pesquisar por CPF
5 - Excluir Pessoa
6 - Apagar Todas as Pessoas Cadastradas
7 - Aniversariantes do Mês
8 - Mostrar Status do Programa
9 - Sair do Programa
```

## 🏗️ Arquitetura do Sistema

### Classes Principais

#### `Utils`
Classe de utilitários com funções auxiliares:
- Formatação e limpeza de CPF
- Leitura segura de dados
- Interface de usuário (limpeza de tela, pausas, etc.)

#### `Data`
Gerenciamento completo de datas:
- Validação de datas
- Cálculo de anos bissextos
- Formatação de datas
- Verificação de mês específico

#### `Pessoa` (Classe Base Abstrata)
- Atributos básicos: nome, CPF, data de nascimento
- Métodos virtuais para leitura e escrita de dados
- Base para herança de Aluno e Professor

#### `Aluno` (Herda de Pessoa)
- Atributo específico: matrícula
- Contador estático de quantidade
- Implementação específica de entrada/saída

#### `Professor` (Herda de Pessoa)
- Atributo específico: titulação
- Validação de títulos acadêmicos
- Contador estático de quantidade

#### `Sistema`
Operações principais do sistema:
- Cadastro, pesquisa, listagem
- Exclusão individual e em massa
- Busca de aniversariantes

#### `GerenciamentoDados`
Persistência de dados:
- Salvamento em arquivos texto
- Carregamento na inicialização
- Tratamento de erros de arquivo

#### `Menus`
Interface de usuário:
- Menu principal e submenus
- Navegação intuitiva
- Validação de entrada

## 💾 Armazenamento de Dados

### Arquivos Gerados
- `alunos.txt`: Dados dos alunos cadastrados
- `professores.txt`: Dados dos professores cadastrados

### Formato dos Arquivos
```
[quantidade_de_registros]
[nome]
[cpf]
[dia] [mes] [ano]
[campo_especifico] // matrícula para alunos, titulação para professores
```

## 🔍 Funcionalidades Detalhadas

### Pesquisa
- **Por Nome**: Busca parcial (substring)
- **Por CPF**: Busca exata com formatação automática

### Validações
- **CPF**: Aceita formatos com ou sem máscara
- **Datas**: Validação completa incluindo anos bissextos
- **Limites**: Máximo de 100 pessoas por categoria

### Aniversariantes
- Busca por mês específico (1-12)
- Listagem separada: alunos, professores ou todos
- Exibição organizada com tipo de pessoa

## ⚠️ Observações Importantes

### Limitações
- Máximo de 100 alunos e 100 professores
- Validação de anos entre 1900 e 2100
- Interface apenas em português

### Recomendações
- Sempre salve os dados após operações de cadastro/exclusão
- Mantenha backups dos arquivos de dados
- Use o formato correto para CPF e datas

## 🛠️ Recursos Técnicos

### Conceitos Aplicados
- **Programação Orientada a Objetos**: Herança, polimorfismo, encapsulamento
- **Gestão de Memória**: Alocação dinâmica com `new`/`delete`
- **Tratamento de Exceções**: `try`/`catch` para operações críticas
- **Arquivos**: Leitura e escrita em arquivos texto
- **STL**: Uso de `vector`, `string` e algoritmos

### Padrões de Projeto
- **Template Method**: Métodos virtuais em `Pessoa`
- **Singleton-like**: Classes utilitárias estáticas
- **Strategy**: Diferentes implementações para Aluno/Professor

## 📝 Exemplo de Uso

```cpp
// Cadastro de um novo aluno
Aluno* novoAluno = new Aluno("João Silva", "123.456.789-00", 15, 3, 2000, "2024001");

// Pesquisa por nome
sistema.pesquisarPorNomeAluno(); // Digite: "João"

// Listagem de aniversariantes
sistema.listarTodosAniversariantes(3); // Março
```

## 🤝 Contribuições

Este projeto foi desenvolvido como um sistema educacional demonstrando conceitos fundamentais de C++ e programação orientada a objetos. Sugestões e melhorias são bem-vindas!

---

**Desenvolvido em C++** | **Interface CLI** | **Persistência em Arquivos**
