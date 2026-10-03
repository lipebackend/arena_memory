# ⚡ Learn_C — Do Hardware ao Sistema Operacional

> **Objetivo:** Dominar computação de baixo nível, gerenciamento de memória bruta, ponteiros, estruturas de dados manuais e syscalls do Linux sem frameworks ou abstrações mágicas.

---

## 📂 Estrutura do Projeto

```text
Learn_C/
├── Makefile          # Build automatizado com flags rigorosas (-Wall -Wextra -Werror)
├── README.md         # Documentação e guia da jornada
├── bin/              # Binários executáveis compilados
└── src/
    └── main.c        # Laboratório 01: Stack, Heap, Ponteiros e Padding
```

---

## 🚀 Como Compilar e Executar

Abra o terminal dentro desta pasta e execute:

```bash
# Compilar e rodar
make run

# Apenas compilar
make

# Limpar binários
make clean
```

---

## 🗺️ Roadmap de Laboratórios

- [x] **Lab 01: A Anatomia da Memória** — Diferença prática entre Stack e Heap, operador `&` (endereço), ponteiros, `sizeof` e alinhamento de memória (padding).
- [ ] **Lab 02: Lista Encadeada na Mão** — Criar um repositório dinâmico de tarefas em C usando nós encadeados (`struct TaskNode* next`), inserção, busca e liberação recursiva de memória.
- [ ] **Lab 03: I/O do Sistema Operacional (Syscalls)** — Persistir e ler dados diretamente no disco usando chamadas POSIX puras do Linux Kernel (`open`, `read`, `write`, `close`).
- [ ] **Lab 04: Servidor HTTP em C com Sockets POSIX** — Abrir a porta 8080 usando `socket()`, `bind()`, `listen()`, `accept()` e responder requisições do `curl`.
