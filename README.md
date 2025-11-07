# 📚 PERFIL: Conteúdos Tecnológicos e Históricos da Computação — FDCO2B

## 📝 Descrição

Este projeto foi desenvolvido como parte da disciplina **Fundamentos de Programação 2 (FDCO2B)** do curso de Engenharia de Computação na UTFPR — Campus Apucarana. Trata-se de um jogo de perguntas e respostas inspirado no formato do "Perfil", com um sistema completo de cadastro, gerenciamento e execução de partidas com perguntas de diferentes níveis de dificuldade.

O sistema permite cadastrar perguntas, carregá-las de arquivos CSV, realizar buscas, alterar ou excluir registros e jogar uma partida com perguntas selecionadas dinamicamente conforme a dificuldade.

---

## 🎮 Mecânica do Jogo – "Modo Perfil / Adivinhação de Personagem"
O jogo seleciona aleatoriamente um personagem histórico (por exemplo: *Alan Turing*).

1. 💡 É exibida uma dica sobre o personagem  
   *(Exemplo: “Matemático britânico, considerado o pai da computação moderna.”)*

2. 🧩 O jogador digita seu palpite:  
   - Se acertar → ganha pontos e passa para o próximo personagem  
   - Se errar → perde uma tentativa ou tempo restante  

3. ⚙️ O jogador pode utilizar **ajudas**, que afetam a pontuação final.

---

## 💡 Ideias de Ajudas e Funcionalidades Criativas
-  **Revelar Letra:** mostra uma letra do nome do personagem (reduz pontos).  
-  **Nova Dica:** fornece uma nova pista ou contexto adicional.  
-  **Pular Personagem:** troca o personagem atual (com penalidade de pontuação).  
-  **Sistema de Ranking:** exibe os jogadores com maiores pontuações.  
-  **Pontuação Estratégica:** usar menos ajudas resulta em mais pontos.  

---

## 💻 Linguagem, Conteúdo e Dependências

- **Linguagem:** C (Padrão C11)
- **Bibliotecas padrão:** `stdio.h`, `stdlib.h`, `string.h`, `time.h`, `ctype.h`
- **Formato de dados:** CSV (entrada), binário (salvamento futuro)
- **Plataforma:** Linux ou Windows (modo texto)
- **Compilador sugerido:** GCC

---

## 📋 Integrantes

Érika Fernanda Santos de Souza – erika.2004@alunos.utfpr.edu.br

Maria Júlia Ferraz Rocha – mariarocha.2023@alunos.utfpr.edu.br

Tarsila Paiva Pimentel – pimentelt@alunos.utfpr.edu.br

---
