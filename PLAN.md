# Plano — Mighty Final Fight nativo no Mac (NESRecomp)

> Gerado em 2026-10-15 a partir do handoff + verificação direta dos repositórios
> (nesrecomp @ `2b4fcdb`, 2026-10-07) e do ROM local.

## 0. O que muda em relação ao handoff

| Handoff dizia | Realidade verificada | Impacto |
|---|---|---|
| Disassembly pronta do MFF em `cyneprepou4uk/NES-Games-Disassembly` | MFF está só na seção **"Planning"** do README; não há pasta nem fonte no repo | Sem disassembly. Substituir por **Mesen2 CDL/trace + Ghidra headless** e um arquivo de símbolos próprio |
| Ajustar `game.toml` focando em bank switching, IRQ e entry points | O backend padrão agora é **cycle-accurate** (`runner/cyc`): MMC3 (banking + IRQ A12) é emulado no runtime; SMB3 (MMC3) roda 100% nativo. `trampoline`/`inline_dispatch` são coisas do backend *legacy* | `game.toml` do MFF começa quase vazio. O trabalho real é **cobertura** (`cyc_seeds.txt` via `--miss-log`) |
| Copiar boilerplate MMC3 | Confirmado: `YoshisCookieRecomp` já usa `nesrecomp_add_cycle_game` (template certo). O `CLAUDE.md` do nesrecomp ainda aponta Faxanadu (legacy) | Usar **YoshisCookieRecomp** como template |
| `brew install cmake sdl2 ninja` | SDL2 2.32 ✅, git ✅, Apple Clang (CLT) ✅, arm64. **Faltam cmake e ninja.** Build exige **Python ≥ 3.11** (o `/usr/bin/python3` é 3.9; o Homebrew tem 3.14 ✅) | Fase 0 precisa garantir `python3` 3.11+ no PATH do CMake |
| RAM map do Data Crystal | Existe, mas o site bloqueia `curl` (Cloudflare) | Salvar manualmente em `docs/ram_map.md` (ou via browser do app) |
| REA + Ghidra | O próprio nesrecomp espera **Ghidra headless via MCP** (`.mcp.json.example`). REA é opcional | Ghidra headless só quando travar; REA fica como alternativa |

**ROM verificado:** `Mighty Final Fight (USA).nes` — CRC32 `15EB0BEE` (com header), iNES mapper 4,
PRG 128 KiB (8×16K), CHR 128 KiB (16×8K), sem bateria. É o dump esperado.

---

## 1. Estrutura do projeto

```
mff/                               ← vira o repo git MightyFinalFightRecomp
├── PLAN.md
├── CLAUDE.md                      ← regras pro agente (derivado do nesrecomp + deste plano)
├── CMakeLists.txt                 ← cópia adaptada do YoshisCookieRecomp (só o ramo cycle)
├── game.toml                      ← [game] name/output_prefix (+ data_region/extra_func se preciso)
├── cyc_seeds.txt                  ← cobertura acumulada (gerado por --miss-log, versionado)
├── inputs/                        ← roteiros de input headless por trecho do jogo
│   ├── 00_boot_title.txt
│   ├── 01_select_cody.txt  02_select_guy.txt  03_select_haggar.txt
│   └── stageN_*.txt
├── docs/
│   ├── ram_map.md                 ← RAM map do Data Crystal (salvo à mão)
│   └── symbols.sym                ← labels que formos descobrindo (formato Mesen/ca65)
├── nesrecomp/                     ← submodule (fixado em commit)
├── recomp-ui/                     ← submodule (launcher/menu)
├── rom/                           ← .nes local, **no .gitignore**
└── build/                         ← build-cycle, ignorado
```

---

## 2. Fases

### Fase 0 — Setup (≈30 min)

1. `brew install cmake ninja` (SDL2 já instalado).
2. Garantir Python ≥ 3.11 para o CMake: passar `-DPython3_EXECUTABLE=/opt/homebrew/bin/python3`.
3. `git init` em `mff/`, criar `.gitignore` (`rom/`, `*.nes`, `*.zip`, `build*/`, `saves/`, `*.cycstate`).
4. Extrair o ROM para `rom/Mighty Final Fight (USA).nes`.
5. Submodules: `nesrecomp` (fixar em `2b4fcdb` ou no commit que o YoshisCookie fixa, o mais novo dos dois que buildar) e `recomp-ui`.
6. Instalar **Mesen2** (oráculo) — app oficial para macOS.
7. Salvar o RAM map em `docs/ram_map.md`.

**Pronto quando:** `cmake --version`, `ninja --version`, `python3.14 --version` ok; ROM com CRC `15EB0BEE` em `rom/`.

### Fase 1 — Primeiro boot headless (≈meio dia)

1. Copiar `CMakeLists.txt` do YoshisCookieRecomp; remover o ramo legacy (não temos `extras.c`/`generated/`), renomear target para `MightyFinalFightRecomp`, `PLAYERS 2`.
2. `game.toml` mínimo:
   ```toml
   [game]
   name = "Mighty Final Fight"
   output_prefix = "mighty-final-fight"
   ```
3. `cyc_seeds.txt` vazio (só o cabeçalho de comentário).
4. Build headless:
   ```bash
   cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release \
     -DNESRECOMP_ROM="$PWD/rom/Mighty Final Fight (USA).nes" \
     -DNESRECOMP_HEADLESS=ON \
     -DPython3_EXECUTABLE=/opt/homebrew/bin/python3 \
     -DCMAKE_PREFIX_PATH="$(brew --prefix)"
   cmake --build build
   ```
5. Rodar `--frames 600 --screenshot` e comparar com o mesmo frame no Mesen2.

**Pronto quando:** o binário compila no Mac e o screenshot do frame ~600 mostra a tela de título (Capcom/título), mesmo que haja execução pelo interpretador.

**Prováveis problemas:** quirks do caminho POSIX (paths `C:/temp` hardcoded em ferramentas, `ucontext`), flags de compilador Clang. Regra: **corrigir no nesrecomp, nunca no gerado** (Rule 1 do CLAUDE.md deles) — manter esses patches num branch do submodule para virar PR upstream.

### Fase 2 — Loop de cobertura até 100% nativo (≈1–3 dias, a maior parte do trabalho)

O backend cycle só compila estaticamente o que é alcançável pelos slots fixos; tudo atrás de um bank switch do MMC3 vem de **execução real**:

```
roteiro inputs/*.txt → run headless --input X --miss-log cyc_seeds.txt
      → reconfigura/rebuild (seeds viram blocos nativos) → roda de novo
      → repetir até a run não registrar misses novos
```

Roteiros a escrever (formato do `--input`: ver `nesrecomp/runner/cyc/README.md`), em ordem:

1. Boot → título → attract/demo (deixar rodar sem input — o demo cobre muito código de gameplay).
2. Seleção de cada personagem (Cody, Guy, Haggar) → início da fase 1.
3. Fase 1 completa com cada personagem (golpes, agarrão, especial, pulo, pegar item, morrer, continue).
4. Fases 2–5 (usar save states `.cycstate` do início de cada fase para não repetir tudo).
5. Chefes, level-up (XP), game over, final/créditos.

Usar o RAM map para tornar os roteiros robustos (esperar por estado em vez de contar frames: estado do jogador, número da fase, HP) e checar `fallback_telemetry.jsonl` para ver os hotspots que ainda caem no interpretador.

**Pronto quando:** todos os roteiros rodam com 0 misses novos e o relatório mostra ~100% de ciclos nativos; jogo completável headless por roteiro.

### Fase 3 — Validação contra o oráculo (em paralelo com a Fase 2)

1. `tools/cyc/mesen_shot` (core do Mesen) com os mesmos roteiros: comparar frames-chave por pixel (`cmp_picture.py`).
2. Dump de RAM em checkpoints (`--ram-init zeros --mem-frame N --mem-out`) e comparar com o Mesen nos endereços do RAM map (HP, vidas, XP, fase, posições).
3. Pontos de atenção específicos do MFF: **split da barra de status via IRQ do MMC3**, slowdown com muitos inimigos (timing), áudio (APU/DMC).

**Pronto quando:** frames-chave idênticos ao Mesen (ou diferenças explicadas) e RAM convergente em todos os checkpoints.

### Fase 4 — Análise profunda só onde travar (sob demanda)

Gatilhos: divergência de RAM/pixel que não se explica, crash, bloco que nunca vira nativo, função com padrão estranho de pilha.

1. **Mesen2**: Code/Data Logger (CDL) + Trace Logger no trecho problemático → sabe o que é código vs dado e em qual banco.
2. **Ghidra headless** (+ GhidraNes) via MCP, configurado a partir de `nesrecomp/.mcp.json.example`. Nunca abrir GUI pelo agente.
3. Antes de mexer em qualquer função com PHA/PLA no endereço de retorno, RTS como goto computado ou dados inline após JSR: ler `nesrecomp/PATTERNS.md`.
4. Registrar o que for descoberto em `docs/symbols.sym` (labels) e, se for dado confundido com código, em `[[data_region]]` no `game.toml`.
5. REA (`npx rea-agents setup`) só se o fluxo Ghidra/MCP não bastar.

### Fase 5 — Polimento no Mac (≈1 dia)

1. Build com janela (sem `HEADLESS`), launcher/menu do `recomp-ui`, gamepad via SDL, `F11` fullscreen, save states `F8/F9`.
2. 2 jogadores? (o original é 1P — `PLAYERS 1` se não fizer sentido).
3. Empacotar `.app` (bundle com SDL2 embutido, assinatura ad-hoc `codesign -s -`), **sem ROM** — o launcher pede o arquivo e confere o CRC.
4. Opcional: `cyc_set_extra_scanlines(128)` / `--extra-scanlines 128` — enhancement só-MMC3 que dá CPU extra e pode **eliminar o slowdown** do MFF. Deixar como opção visível e reversível no menu.

**Pronto quando:** abrir o `.app`, escolher o ROM, jogar do início ao fim com controle, sem crash.

---

## 3. Regras para o agente (vão no `CLAUDE.md` do projeto)

- Nunca editar `generated/` nem ler `*_cyc*.c` inteiro. Bug de geração → `recompiler/src/cyc_codegen.c`; bug de hardware → `runner/cyc/hw_*.c` (no submodule, branch próprio).
- Não adivinhar comportamento do 6502: citar evidência (bytes do ROM, trace do Mesen, saída do Ghidra) antes de propor mudança.
- Diffs pequenos, um sistema por vez; validar com roteiro headless + screenshot antes de declarar pronto.
- Nunca commitar ROM, `.nes`, saves ou binários com assets do jogo.

### Prompts prontos

- **Fase 1:** "Monte o projeto MightyFinalFightRecomp seguindo PLAN.md Fase 1. Use YoshisCookieRecomp como template só do ramo cycle. Build headless, rode 600 frames, salve screenshot e compare com o Mesen. Se o build falhar no macOS, corrija no submodule nesrecomp e explique a causa."
- **Fase 2:** "Escreva `inputs/01_select_cody.txt` que vai do boot até o início da fase 1 com o Cody, usando o RAM map em docs/ram_map.md para esperar estados. Rode com `--miss-log`, rebuilde e repita até 0 misses novos. Reporte % nativo."
- **Fase 4:** "O frame N diverge do Mesen na barra de status. Use o trace do Mesen e o Ghidra headless para achar o handler de IRQ e a escrita em $C000/$C001. Cite o assembly antes de sugerir qualquer mudança."

---

## 4. Riscos

| Risco | Mitigação |
|---|---|
| Suporte macOS é experimental; docs/ferramentas assumem Windows (`C:/temp`, `.ps1`) | Variáveis de ambiente (`NESRECOMP_SHOT_DIR`), scripts `.sh` próprios; patches upstream |
| nesrecomp muda rápido | Fixar submodule; atualizar de propósito, não por acaso |
| Sem disassembly do MFF | CDL do Mesen + Ghidra só nos pontos de falha; o backend cycle não precisa de entendimento semântico para rodar |
| Código que roda da RAM ou cruza fronteira de slot | Cai no interpretador (correto, só mais lento); aceitar ou semear |
| Cobertura incompleta em trechos raros (final, game over, continues) | Roteiro dedicado para cada um + telemetria de fallback em sessões jogadas à mão |

## 5. Legal

Usar só o próprio dump. O repo e o `.app` distribuídos não contêm ROM nem código gerado a partir dele;
o usuário fornece o ROM e o build gera tudo localmente.
