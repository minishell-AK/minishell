# Documentação de Implementação dos Builtins

## Builtin: `cd` (Change Directory)

### Visão Geral
O builtin `cd` foi implementado em `src/exec/builtin/ft_cd.c` para permitir a navegação entre diretórios no minishell, compatível com o comportamento do bash.

### Funcionalidades Implementadas

#### 1. **Navegação Básica**
- `cd /caminho/absoluto` - Navega para caminho absoluto
- `cd caminho/relativo` - Navega para caminho relativo ao diretório atual
- `cd ..` - Volta um nível no diretório

#### 2. **Atalhos Especiais**
- `cd` ou `cd ~` - Navega para o diretório HOME do usuário
- `cd -` - Volta para o diretório anterior (usando OLDPWD)

#### 3. **Atualização de Variáveis de Ambiente**
- Atualiza `PWD` com o novo diretório atual
- Atualiza `OLDPWD` com o diretório anterior

#### 4. **Tratamento de Erros**
- Valida se há múltiplos argumentos
- Verifica se o diretório existe e é acessível
- Trata casos onde HOME ou OLDPWD não estão definidos

---

### Arquitetura da Implementação

#### **Função Principal: `ft_cd()`**
```c
int ft_cd(char **args, char ***envp_ref)
```

**Parâmetros:**
- `args` - Array de argumentos do comando (args[0] = "cd", args[1] = caminho)
- `envp_ref` - Referência tripla para o ambiente (permite modificar o array principal)

**Retorno:**
- `0` - Sucesso
- `1` - Erro

**Fluxo de Execução:**

1. **Validação de Argumentos**
   ```c
   if (args[1] && args[2])
   {
       ft_putstr_fd("cd: too many arguments\n", 2);
       return (1);
   }
   ```
   - Verifica se há mais de um argumento além do "cd"
   - Bash aceita apenas um caminho por vez

2. **Captura do Diretório Atual**
   ```c
   current_pwd = get_env_value_simple(*envp_ref, "PWD");
   if (current_pwd)
       ft_strlcpy(old_pwd, current_pwd, sizeof(old_pwd));
   else if (!getcwd(old_pwd, sizeof(old_pwd)))
       old_pwd[0] = '\0';
   ```
   - Tenta obter PWD do ambiente
   - Se não existir, usa `getcwd()` do sistema
   - Salva em buffer local para atualizar OLDPWD depois

3. **Resolução do Caminho**
   ```c
   path = resolve_cd_path(args, *envp_ref);
   ```
   - Delega para função auxiliar que trata casos especiais

4. **Mudança de Diretório**
   ```c
   if (chdir(path) != 0)
   {
       ft_putstr_fd("cd: ", 2);
       perror(path);
       return (1);
   }
   ```
   - Usa syscall `chdir()` para mudar o diretório do processo
   - Em caso de erro (diretório não existe, sem permissão), imprime erro

5. **Atualização das Variáveis de Ambiente**
   ```c
   return (update_pwd_env(envp_ref, old_pwd));
   ```
   - Atualiza PWD e OLDPWD no ambiente global

---

### Funções Auxiliares

#### **1. `get_env_value_simple()`**
```c
static char *get_env_value_simple(char **envp, const char *name)
```

**Propósito:** Buscar valor de uma variável de ambiente pelo nome.

**Implementação:**
- Itera pelo array `envp`
- Compara o início de cada string com `name`
- Retorna ponteiro para o valor após o `=`

**Exemplo:**
```c
// envp = ["HOME=/home/user", "PWD=/tmp", ...]
// get_env_value_simple(envp, "HOME") retorna "/home/user"
```

#### **2. `resolve_cd_path()`**
```c
static char *resolve_cd_path(char **args, char **envp)
```

**Propósito:** Resolver qual caminho usar baseado no argumento.

**Casos Tratados:**

1. **Sem argumento ou `~`:**
   ```c
   if (!args[1] || ft_strncmp(args[1], "~", 2) == 0)
   {
       path = get_env_value_simple(envp, "HOME");
       if (!path)
       {
           ft_putstr_fd("cd: HOME not set\n", 2);
           return (NULL);
       }
       return (path);
   }
   ```
   - `cd` sozinho ou `cd ~` vai para HOME
   - Se HOME não está definido, retorna erro

2. **Argumento `-` (diretório anterior):**
   ```c
   if (ft_strncmp(args[1], "-", 2) == 0)
   {
       path = get_env_value_simple(envp, "OLDPWD");
       if (!path)
       {
           ft_putstr_fd("cd: OLDPWD not set\n", 2);
           return (NULL);
       }
       ft_putstr_fd(path, 1);  // Bash imprime o caminho ao usar cd -
       ft_putstr_fd("\n", 1);
       return (path);
   }
   ```
   - Busca OLDPWD no ambiente
   - Imprime o caminho (comportamento do bash)
   - Retorna caminho salvo

3. **Caminho normal:**
   ```c
   return (args[1]);
   ```
   - Retorna o caminho fornecido (absoluto ou relativo)

#### **3. `update_pwd_env()`**
```c
static int update_pwd_env(char ***envp_ref, const char *old_pwd)
```

**Propósito:** Atualizar PWD e OLDPWD no ambiente após mudança bem-sucedida.

**Fluxo:**

1. **Obter diretório atual real:**
   ```c
   if (!getcwd(cwd, sizeof(cwd)))
   {
       perror("cd: getcwd error");
       return (1);
   }
   ```
   - Usa `getcwd()` para obter caminho absoluto canônico
   - Importante: mesmo que usuário faça `cd ..`, PWD terá caminho absoluto

2. **Atualizar OLDPWD:**
   ```c
   if (old_pwd)
   {
       new_entry = env_make_entry("OLDPWD", old_pwd);
       idx = env_find_index(*envp_ref, "OLDPWD");
       if (idx >= 0)
           (*envp_ref)[idx] = new_entry;
       else
           env_append_entry(envp_ref, new_entry);
   }
   ```
   - Cria string "OLDPWD=caminho_anterior"
   - Se já existe, substitui na posição encontrada
   - Se não existe, adiciona ao final do array

3. **Atualizar PWD:**
   ```c
   new_entry = env_make_entry("PWD", cwd);
   idx = env_find_index(*envp_ref, "PWD");
   if (idx >= 0)
       (*envp_ref)[idx] = new_entry;
   else
       env_append_entry(envp_ref, new_entry);
   ```
   - Mesmo processo para PWD
   - Garante que PWD sempre reflete o diretório atual

---

### Integração com o Executor

#### **Problema: Builtins e Processos Filhos**

Builtins como `cd`, `export`, `unset` e `exit` precisam **modificar o estado do shell pai**. Se executados em processos filhos (via `fork()`), as mudanças são perdidas quando o filho termina.

#### **Solução: Detecção de Builtins do Pai**

**1. Função `is_parent_builtin()` em `is_builtin.c`:**
```c
int is_parent_builtin(char *cmd)
{
    if (!cmd)
        return (0);
    if (ft_strncmp(cmd, "cd", 3) == 0)
        return (1);
    if (ft_strncmp(cmd, "export", 7) == 0)
        return (1);
    if (ft_strncmp(cmd, "unset", 6) == 0)
        return (1);
    if (ft_strncmp(cmd, "exit", 5) == 0)
        return (1);
    return (0);
}
```
- Identifica builtins que afetam o estado do shell

**2. Execução Condicional em `exe_cmd.c`:**
```c
void exec_cmd(t_all_variables *all_variables)
{
    add_pipe(&all_variables->cmd);
    
    /* Se é comando único (sem pipes) e é builtin do pai, executa no pai */
    if (all_variables->cmd && !all_variables->cmd->next
        && all_variables->cmd->args && all_variables->cmd->args[0]
        && is_parent_builtin(all_variables->cmd->args[0]))
    {
        exec_builtin(all_variables->cmd, all_variables->env, all_variables);
        free_all_variables(all_variables);
        return ;
    }
    
    // Resto da execução com fork() para comandos normais...
}
```

**Lógica:**
- Se comando **não tem pipe** (`!cmd->next`)
- E **é builtin do pai** (`is_parent_builtin()`)
- Então executa **no processo pai** (sem `fork()`)
- Caso contrário, executa em filho (permite redirecionamentos sem afetar shell)

#### **Por que `envp_ref` é Triplo Ponteiro?**

```c
int ft_cd(char **args, char ***envp_ref)
```

- `char **envp` - Array de strings (ambiente)
- `char ***envp_ref` - Referência ao array (permite modificar o ponteiro original)

**Necessário porque:**
1. `export` e `unset` podem realocar o array (adicionar/remover variáveis)
2. Se passássemos `char **`, modificaríamos apenas a cópia local
3. Com `char ***`, modificamos o ponteiro original no `main()`

**Exemplo de uso:**
```c
// Em main.c:
char **my_env;  // Ambiente global

// Passamos referência:
add_variables(cmd, &my_env);  // my_env é char**, &my_env é char***

// Em ft_cd:
update_pwd_env(envp_ref, old_pwd);
// Modifica diretamente my_env no main!
```

---

### Estrutura de Dados Modificada

#### **`t_all_variables` em `exec.h`:**
```c
typedef struct s_all_variables
{
    char    **env;          // Cópia do ambiente (para execve)
    char    ***envp_ref;    // Referência ao ambiente original (para modificações)
    char    **path;         // Array de caminhos do PATH
    int     *pids;          // PIDs dos processos filhos
    t_cmd   *cmd;           // Lista de comandos
} t_all_variables;
```

**Dupla Camada de Ambiente:**
- `env` - Cópia usada para `execve()` em comandos externos
- `envp_ref` - Referência para modificar ambiente global em builtins do pai

---

### Testes Realizados

#### **Casos de Sucesso:**
```bash
✅ cd /tmp          → PWD=/tmp
✅ cd ..            → PWD=/
✅ cd ~             → PWD=/home/user
✅ cd               → PWD=/home/user
✅ cd /tmp; cd -    → PWD=/home/user, imprime "/tmp"
```

#### **Casos de Erro:**
```bash
✅ cd /inexistente           → "cd: /inexistente: No such file or directory"
✅ cd /tmp /home             → "cd: too many arguments"
✅ unset HOME; cd            → "cd: HOME not set"
✅ cd - (sem OLDPWD definido) → "cd: OLDPWD not set"
```

---

## Builtin: `export` (Exportar Variáveis de Ambiente)

### Visão Geral
O builtin `export` foi implementado em `src/exec/builtin/ft_export.c` para gerenciar variáveis de ambiente no minishell. Permite criar novas variáveis, atualizar existentes e listar todas as variáveis exportadas.

### Funcionalidades Implementadas

#### 1. **Listagem de Variáveis**
- `export` (sem argumentos) - Lista todas as variáveis de ambiente em ordem alfabética com formato `declare -x VAR=valor`

#### 2. **Criação e Atualização de Variáveis**
- `export VAR=valor` - Cria nova variável ou atualiza se já existir
- `export VAR1=a VAR2=b VAR3=c` - Múltiplas variáveis em um único comando
- `export VAR` (sem `=`) - Marca variável como exportada (comportamento bash), mas nossa implementação simplificada apenas valida sem erro

#### 3. **Validação de Identificadores**
- Nomes válidos: letras (a-z, A-Z), números (0-9) e underscore (_)
- Primeiro caractere deve ser letra ou underscore
- Não pode conter caracteres especiais como `-`, `@`, `$`, etc.

#### 4. **Tratamento de Erros**
- Identifica e reporta identificadores inválidos
- Continua processando outros argumentos mesmo após erro
- Retorna código de erro apropriado

---

### Arquitetura da Implementação

#### **Função Principal: `ft_export()`**
```c
int ft_export(char **args, char ***envp_ref)
```

**Parâmetros:**
- `args` - Array de argumentos (args[0] = "export", args[1..n] = variáveis)
- `envp_ref` - Referência tripla para o ambiente global (permite modificação direta)

**Retorno:**
- `0` - Sucesso (todas variáveis válidas)
- `1` - Erro (pelo menos uma variável inválida)

**Fluxo de Execução:**

1. **Caso Sem Argumentos (Listagem):**
   ```c
   if (!args[1])
   {
       print_sorted_env(*envp_ref);
       return (0);
   }
   ```
   - Se não há argumentos após "export", lista todas variáveis
   - Chama função auxiliar para ordenar e imprimir

2. **Caso Com Argumentos (Exportação):**
   ```c
   ret = 0;
   i = 1;
   while (args[i])
   {
       if (export_variable(args[i], envp_ref) != 0)
           ret = 1;  // Marca erro mas continua processando
       i++;
   }
   return (ret);
   ```
   - Itera por todos os argumentos
   - Tenta exportar cada variável
   - Acumula erros (se qualquer falhar, retorna 1)
   - **Importante:** Não para no primeiro erro (comportamento bash)

---

### Funções Auxiliares

#### **1. `is_valid_identifier()`**
```c
static int is_valid_identifier(const char *str)
```

**Propósito:** Validar se o nome da variável segue as regras POSIX.

**Implementação Detalhada:**

1. **Validação Inicial:**
   ```c
   if (!str || !str[0])
       return (0);
   ```
   - Rejeita strings NULL ou vazias

2. **Validação do Primeiro Caractere:**
   ```c
   if (!ft_isalpha((unsigned char)str[0]) && str[0] != '_')
       return (0);
   ```
   - Primeiro caractere **deve** ser letra (a-z, A-Z) ou underscore (_)
   - Números no início são **inválidos** (123VAR = erro)

3. **Validação dos Caracteres Seguintes:**
   ```c
   i = 1;
   while (str[i] && str[i] != '=')
   {
       if (!ft_isalnum((unsigned char)str[i]) && str[i] != '_')
           return (0);
       i++;
   }
   return (1);
   ```
   - Percorre até encontrar `=` ou fim da string
   - Cada caractere deve ser alfanumérico ou underscore
   - Caracteres especiais (`-`, `@`, `$`, etc.) são **inválidos**

**Exemplos:**
```c
is_valid_identifier("VAR=123")      → 1 (válido)
is_valid_identifier("_VAR=abc")     → 1 (válido, _ no início ok)
is_valid_identifier("VAR_123=x")    → 1 (válido, _ e números no meio ok)
is_valid_identifier("123VAR=x")     → 0 (inválido, começa com número)
is_valid_identifier("VAR-NAME=x")   → 0 (inválido, contém -)
is_valid_identifier("VAR@NAME=x")   → 0 (inválido, contém @)
is_valid_identifier("VAR")          → 1 (válido, mesmo sem =)
```

#### **2. `get_var_name()`**
```c
static char *get_var_name(const char *entry)
```

**Propósito:** Extrair apenas o nome da variável de uma entrada "NOME=valor".

**Implementação:**
```c
i = 0;
while (entry[i] && entry[i] != '=')
    i++;
name = malloc(i + 1);
if (!name)
    return (NULL);
ft_strlcpy(name, entry, i + 1);
return (name);
```

**Processo:**
1. Conta caracteres até encontrar `=` ou fim
2. Aloca memória exata para o nome (+ null terminator)
3. Copia apenas a parte do nome usando `ft_strlcpy`

**Exemplo:**
```c
get_var_name("PATH=/usr/bin:/bin")  → aloca e retorna "PATH"
get_var_name("USER=john")           → aloca e retorna "USER"
get_var_name("EMPTY=")              → aloca e retorna "EMPTY"
```

**Importante:** Chamador é responsável por liberar a memória retornada!

#### **3. `print_sorted_env()`**
```c
static void print_sorted_env(char **envp)
```

**Propósito:** Listar todas as variáveis em ordem alfabética (comportamento bash).

**Implementação - Algoritmo de Ordenação:**

1. **Preparação:**
   ```c
   count = count_size_array_char(envp);
   sorted = malloc(sizeof(char *) * (count + 1));
   if (!sorted)
       return ;
   i = -1;
   while (++i < count)
       sorted[i] = envp[i];
   sorted[count] = NULL;
   ```
   - Conta total de variáveis
   - Aloca array temporário do mesmo tamanho
   - Copia ponteiros (não duplica strings, apenas referências)

2. **Ordenação Bubble Sort:**
   ```c
   i = -1;
   while (++i < count - 1)
   {
       j = i;
       while (++j < count)
       {
           if (ft_strncmp(sorted[i], sorted[j], ft_strlen(sorted[i])) > 0)
           {
               tmp = sorted[i];
               sorted[i] = sorted[j];
               sorted[j] = tmp;
           }
       }
   }
   ```
   - Usa bubble sort (simples para quantidades pequenas/médias)
   - Compara strings lexicograficamente
   - Troca apenas ponteiros (eficiente)

3. **Impressão:**
   ```c
   i = -1;
   while (sorted[++i])
   {
       printf("declare -x %s\n", sorted[i]);
   }
   free(sorted);
   ```
   - Imprime cada variável com prefixo `declare -x` (formato bash)
   - Libera apenas o array temporário (não as strings, pois são referências)

**Complexidade:** O(n²) - Aceitável pois número de variáveis é tipicamente pequeno (<100)

**Exemplo de Saída:**
```bash
declare -x HOME=/home/user
declare -x LANG=en_US.UTF-8
declare -x PATH=/usr/bin:/bin
declare -x PWD=/tmp
declare -x USER=john
```

#### **4. `export_variable()`**
```c
static int export_variable(char *arg, char ***envp_ref)
```

**Propósito:** Processar e exportar uma única variável.

**Fluxo Detalhado:**

1. **Validação do Identificador:**
   ```c
   if (!is_valid_identifier(arg))
   {
       ft_putstr_fd("export: `", 2);
       ft_putstr_fd(arg, 2);
       ft_putstr_fd("': not a valid identifier\n", 2);
       return (1);
   }
   ```
   - Valida nome antes de processar
   - Imprime mensagem de erro em stderr (fd 2)
   - Retorna erro mas não interrompe programa

2. **Verificação de Atribuição:**
   ```c
   if (!ft_strchr(arg, '='))
       return (0);
   ```
   - Se não há `=`, bash apenas marca como exportada (não implementado)
   - Nossa implementação simplificada apenas retorna sucesso
   - **Comportamento:** `export VAR` não faz nada mas não é erro

3. **Extração do Nome:**
   ```c
   name = get_var_name(arg);
   if (!name)
       return (1);
   ```
   - Isola o nome da variável
   - Necessário para buscar se já existe no ambiente

4. **Duplicação da Entrada:**
   ```c
   new_entry = ft_strdup(arg);
   if (!new_entry)
   {
       free(name);
       return (1);
   }
   ```
   - Duplica a string completa "VAR=valor"
   - Gerenciamento de memória: libera nome se falhar

5. **Busca e Atualização/Inserção:**
   ```c
   idx = env_find_index(*envp_ref, name);
   free(name);
   if (idx >= 0)
   {
       free((*envp_ref)[idx]);      // Libera valor antigo
       (*envp_ref)[idx] = new_entry; // Atualiza com novo
   }
   else
       env_append_entry(envp_ref, new_entry); // Adiciona ao final
   return (0);
   ```
   - `env_find_index()` busca variável pelo nome
   - **Se encontrar (idx >= 0):**
     - Libera string antiga para evitar leak
     - Substitui com nova entrada
   - **Se não encontrar:**
     - Adiciona ao final do array
     - `env_append_entry()` pode realocar array se necessário

**Exemplo de Execução:**
```bash
# Estado inicial: envp = ["HOME=/home/user", "PATH=/bin", NULL]

export NEW=123
# → env_find_index retorna -1 (não existe)
# → env_append_entry adiciona "NEW=123"
# → envp = ["HOME=/home/user", "PATH=/bin", "NEW=123", NULL]

export HOME=/root
# → env_find_index retorna 0 (posição do HOME)
# → free(envp[0]) libera "/home/user"
# → envp[0] = "HOME=/root"
# → envp = ["HOME=/root", "PATH=/bin", "NEW=123", NULL]
```

---

### Integração com o Sistema de Variáveis

#### **Problema: Persistência das Modificações**

Originalmente, `add_variables()` criava uma **cópia rasa** (shallow copy) do ambiente:

```c
// Código antigo problemático:
char **copy_env(char **env)
{
    // ...
    while (env[i] != NULL)
    {
        env_copy[i] = env[i];  // ❌ Copia apenas ponteiros!
        i++;
    }
}
```

**Problema:** Modificações em `all->env` não afetavam `my_env` no `main()`.

#### **Solução: Referência ao Ambiente Original**

**Modificação na Estrutura `t_all_variables`:**
```c
typedef struct s_all_variables
{
    char    **env;          // Cópia para execve() em comandos externos
    char    ***envp_ref;    // ⭐ Referência ao ambiente original
    char    **path;
    int     *pids;
    t_cmd   *cmd;
} t_all_variables;
```

**Fluxo de Modificação:**
```c
// 1. main.c - ambiente global
char **my_env = dup_envp(envp);

// 2. add_variables - recebe REFERÊNCIA
all = add_variables(cmd, &my_env);  // Passa &my_env (char ***)
all->envp_ref = &my_env;             // Armazena ponteiro para ponteiro

// 3. export - modifica através da referência
export_variable("VAR=123", all->envp_ref);
// ↓
env_append_entry(envp_ref, "VAR=123");
// ↓
*envp_ref = realloc(*envp_ref, ...);  // Modifica my_env diretamente!

// 4. Próximo comando vê a mudança
expand_tokens(tokens, my_env, 0);  // my_env tem VAR=123!
```

**Por que Funciona:**
- `char ***envp_ref` permite modificar o **ponteiro original** `my_env`
- `realloc` pode mudar o endereço do array
- Com triplo ponteiro, a mudança persiste no escopo do `main()`

#### **Funções Auxiliares do Sistema de Ambiente**

**`env_find_index()` (em `env_utils.c`):**
```c
int env_find_index(char **envp, const char *name)
{
    int     i;
    size_t  len;
    
    len = ft_strlen(name);
    i = 0;
    while (envp[i])
    {
        if (ft_strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
            return (i);
        i++;
    }
    return (-1);
}
```
- Busca variável pelo nome
- Retorna índice se encontrar, -1 caso contrário

**`env_append_entry()` (em `env_utils.c`):**
```c
int env_append_entry(char ***envp_ref, char *entry)
{
    int     count;
    char    **new_envp;
    
    count = count_size_array_char(*envp_ref);
    new_envp = xrealloc(*envp_ref, sizeof(char *) * (count + 2));
    if (!new_envp)
        return (0);
    new_envp[count] = entry;
    new_envp[count + 1] = NULL;
    *envp_ref = new_envp;
    return (1);
}
```
- Realoca array para adicionar nova entrada
- Atualiza ponteiro original através de `envp_ref`

**`env_make_entry()` (usado em cd):**
```c
char *env_make_entry(const char *name, const char *value)
{
    char    *entry;
    size_t  len;
    
    len = ft_strlen(name) + ft_strlen(value) + 2; // "name=value\0"
    entry = malloc(len);
    if (!entry)
        return (NULL);
    ft_strlcpy(entry, name, len);
    ft_strlcat(entry, "=", len);
    ft_strlcat(entry, value, len);
    return (entry);
}
```
- Cria string "NOME=valor" formatada
- Usado por `cd` para atualizar PWD/OLDPWD

---

### Comportamento de Execução

#### **Execução no Processo Pai**

Como `export` modifica o estado do shell, **deve executar no processo pai**:

```c
// exe_cmd.c
if (all_variables->cmd && !all_variables->cmd->next
    && all_variables->cmd->args && all_variables->cmd->args[0]
    && is_parent_builtin(all_variables->cmd->args[0]))
{
    exec_builtin(all_variables->cmd, all_variables->env, all_variables);
    free_all_variables(all_variables);
    return ;
}
```

**Condições:**
- Comando único (sem pipes: `!cmd->next`)
- É builtin do pai (cd, export, unset, exit)
- Executa diretamente sem `fork()`

**Por que não usar fork() para export:**
```bash
# Se executasse em filho:
minishell$ export VAR=123
# ↓ processo filho altera seu próprio ambiente
# ↓ filho termina, mudanças são perdidas
# ↓ processo pai não vê VAR

# Executando no pai:
minishell$ export VAR=123
# ✅ pai altera seu próprio ambiente
# ✅ mudanças persistem
minishell$ echo $VAR
123  # ✅ funciona!
```

---

### Testes Realizados

#### **Casos de Sucesso:**
```bash
✅ export TESTE=123; echo $TESTE           → 123
✅ export VAR1=a VAR2=b VAR3=c             → Cria 3 variáveis
✅ export OLD=old; export OLD=new          → Atualiza valor
✅ export _VALID=ok                        → Underscore válido
✅ export VAR123=test                      → Números no meio ok
✅ export                                  → Lista todas vars ordenadas
✅ export EMPTY=                           → Variável vazia válida
```

#### **Casos de Erro (com continuação):**
```bash
✅ export 123VAR=x                         → "not a valid identifier" (mas continua)
✅ export VAR-NAME=x                       → "not a valid identifier"
✅ export @VAR=x                           → "not a valid identifier"
✅ export VAR1=ok 2VAR=bad VAR3=ok        → Apenas VAR1 e VAR3 exportadas
```

#### **Comportamento de Múltiplos Argumentos:**
```bash
minishell$ export GOOD1=a BAD-VAR=b GOOD2=c
export: `BAD-VAR=b': not a valid identifier
minishell$ echo $GOOD1 $GOOD2
a c  # ✅ GOOD1 e GOOD2 foram exportadas apesar do erro
```

---


### Fluxo Completo de Exemplo

**Comando:** `export NEW_VAR=hello`

```
1. main.c: process_line()
   ↓
2. lexer: tokeniza → ["export", "NEW_VAR=hello"]
   ↓
3. parser: cria comando → {args: ["export", "NEW_VAR=hello", NULL]}
   ↓
4. add_variables: cria all_variables com envp_ref = &my_env
   ↓
5. exe_cmd: detecta builtin do pai
   ↓
6. exec_builtin: chama ft_export(args, all->envp_ref)
   ↓
7. ft_export: args[1] existe, chama export_variable()
   ↓
8. export_variable:
   - is_valid_identifier("NEW_VAR=hello") → ✅ válido
   - ft_strchr(arg, '=') → encontra '='
   - get_var_name("NEW_VAR=hello") → "NEW_VAR"
   - new_entry = ft_strdup("NEW_VAR=hello")
   - env_find_index(envp, "NEW_VAR") → -1 (não existe)
   - env_append_entry(&my_env, "NEW_VAR=hello")
     ↓
9. env_append_entry:
   - Conta variáveis: count = N
   - Realoca: new_envp = xrealloc(my_env, (N+2) * sizeof(char*))
   - new_envp[N] = "NEW_VAR=hello"
   - new_envp[N+1] = NULL
   - *envp_ref = new_envp  // ⭐ Atualiza my_env no main!
   ↓
10. Retorna para main, my_env agora contém NEW_VAR=hello
    ↓
11. Próximo comando: echo $NEW_VAR
    - expand_tokens consulta my_env
    - Encontra NEW_VAR=hello
    - Expande para "hello"
```

---


