# Plano — Mighty Final Fight decompilado e portado (sem hardware simulado)

> Continuação do [PLAN_WINDOWS.md](PLAN_WINDOWS.md). O recomp cycle-accurate deste repo está pronto
> (100% nativo, igual ao TriCNES bit a bit) e passa a ser o **oráculo** do port. Escrito em 2026-10-16
> sobre nesrecomp `2b4fcdb`.

## 0. Objetivo e decisões

Hoje o `.exe` é o código 6502 traduzido mecanicamente, rodando sobre um modelo ciclo a ciclo da CPU,
PPU, APU e MMC3. O objetivo é um **port**: a lógica do jogo em C legível e com nomes, desenhando e tocando
som por APIs próprias, **sem** modelo de barramento, sem PPU por ciclo e sem a ROM em tempo de execução.

| Decisão | Escolha |
|---|---|
| Ponto de chegada | **Em etapas**: etapa 1 = port fiel com renderizador por quadro; etapa 2 = renderizador de alto nível com extras (widescreen, sem sprite piscando) |
| ROM | **Extrair no build**: o usuário fornece o dump uma vez, um extrator gera os assets; repo e binário só têm código |
| Plataformas | **Windows + macOS + Linux** (CMake + SDL2) |
| Onde | Pasta **`port/`** neste repo, ao lado do recomp (que vira o oráculo dos testes) |
| Slowdown | **Fiel por padrão + opção de desligar** |
| Áudio | **Driver de som da Capcom reimplementado + síntese 2A03** (pulso, triângulo, ruído, DMC) |

**O que "sem simulação" quer dizer aqui.** Sai do caminho: CPU 6502, barramento, registradores
`$2000`–`$4017` como interface, PPU ponto a ponto, IRQ do MMC3 e troca de bancos. Fica: um renderizador
que desenha tiles e sprites a partir do estado do jogo, e um sintetizador que gera as ondas do 2A03. As
ondas são o próprio som do jogo, não um detalhe do hardware.

**Por que o port é viável agora.** Três coisas que um decomp normalmente não tem:

1. **Mapa completo de código.** As 15.548 seeds de `cyc_seeds.txt` são todos os pontos de entrada que o
   jogo executa (banco + endereço), dos 5 rounds até os créditos.
2. **Oráculo exato.** O recomp bate com o TriCNES em trace, memória, imagem e ciclos, e o `--frame-log`
   grava RAM, nametables, OAM, paleta e imagem de cada frame.
3. **Rotas replayáveis.** `inputs/*.txt` e as sessões do `tools/bot.py` reproduzem jogatinas inteiras
   byte a byte. Viram testes de regressão de graça.

### Status (2026-10-16)

- **P0 concluída.** CDL no runner (`--cdl-out`, branch local `feat/cyc-cdl` do nesrecomp, commit
  `24e7462`; o superprojeto ainda aponta para `2b4fcdb`). 104 logs (rotas replayáveis, sessões god,
  varreduras por round e os "tours" com skip a cada 1.200/3.000 frames, que passam por todas as áreas).
  **PRG 80,45% classificado, CHR 81,2% desenhado.** Mapa em `docs/prg_map.md`. O resto precisa de
  entendimento (fim do banco `08` com bytes de lixo, dado sem referência, caminhos raros).
- **P1 no critério principal.** `disasm/tools/disasm.py` gera ca65 a partir do dump + mapa;
  `disasm/tools/verify.py` remonta com ca65/ld65 (cc65 `a6530f4`, compilado em `build_cc65/`) e
  **os 16 bancos saem byte a byte iguais**. Tabelas de ponteiros tipadas (`.lobytes/.hibytes/.word`),
  nomes de hardware e de RAM, `labels.toml` com 64 rótulos com evidência.
- **P2 começada.** `docs/engine.md`: escalonador de 4 tarefas cooperativas com pilhas próprias, NMI com
  retorno sequestrado (`nmi_tail` roda o som), IRQ por ponteiro em RAM, troca de bancos reentrante,
  fluxo do jogo (attract = `JMP reset`), laço de gameplay, despacho de estados do jogador e de
  comportamento de objetos (160 ids, banco por id: `tools/mff_dispatch.py`), desenho com rotação de
  ordem (o piscar). Mudanças de plano que isso traz: a P4 precisa de **corrotinas** para as tarefas.
- **P3 concluída com um limite conhecido.** `port/scaffold` roda no backend legacy (870 entradas do
  mapa, `push_all_jsr`, `nmi_tail` substituído, NMI segurada no meio de tarefa via `g_nmi_veto`).
  Lockstep (`port/tests/`): RAM idêntica ao oráculo do power-on até o primeiro lag de cada rota
  (Cody 7.637 frames, Guy 10.433, Haggar 7.669). Depois disso o andaime, que não faz lag, segue
  outro caminho (tarefa em outro ponto do código, RNG mexido pelo `$92`). **Mudança de plano:** a
  regressão da P4 é andaime original × andaime limpo (exata no jogo todo); o oráculo certifica o
  andaime nos trechos sem lag; lag fiel (NMI no PC interrompido do oráculo) fica para a P7.
- **P4 (17/10): todas as rotinas arrumadas em arquivos por sistema.** Como se chegou aqui: (1) em vez
  de ejetar o código gerado inteiro (dezenas de MB, o mesmo código repetido em cada chamada), cada
  rotina ganhou um corpo próprio em `port/src/game/`, que entra no andaime por `[[replace_func]]`;
  (2) um *lifter* (`port/tools/lift.py`, `lift_bank.py`, `lift_opt.py`, `lift_structure.py`)
  converteu o que não foi escrito à mão, com a semântica tirada da ROM e cada decisão de controle e de
  ciclos tirada da cópia do andaime puro; (3) nove agentes em paralelo levaram cada banco para arquivos
  por sistema (jogador, chefes, inimigos comuns, objetos, adereços, fase, fluxo de telas, som), com
  nomes, comentários que dizem o que o código faz e citam endereços da ROM como evidência, tabelas
  nomeadas e `if`/laços no lugar dos `goto`. Não sobra nenhum `lifted_*.c`; o mapa por sistema está no
  `port/README.md`. Hoje são 1.168 pontos de entrada em 74 arquivos `.c` e 14 `.h` (55 mil linhas);
  com `MFF_EJECTED` o port compila sem código gerado e, sem o interpretador do runtime rodar nenhuma
  instrução da ROM (`find_entries.py` acha estaticamente cada destino de salto sem rotina), fica
  idêntico ao andaime puro nas 9 rotas (o round 1 com os três personagens, três *tours* de 50.000
  frames e três jogos de 60.000 com o bot dentro do andaime): RAM, OAM, paleta, imagem **e a contagem
  de ciclos de cada imagem**. O `selfcheck.py` só mostrava a primeira diferença de ciclos, `-1` na
  imagem 0 em toda rota; um trace de cada cobrança de ciclos (andaime puro × port, alinhados) mostrou
  dois erros do port, invisíveis na RAM porque a NMI só entra no topo do laço ocioso do escalonador:
  a NMI cobrava `$C029-$C041` antes do `STA $4014`, 5 ciclos de atraso que trocavam a paridade do DMA
  de OAM (513 ou 514 ciclos), e um `mff_cycles(0xC9A7, 0)` do lifter que o runtime cobra como 1 ciclo
  em todo frame do `gameplay_draw`. Corrigidos, os ciclos batem exatamente, e o check falha em
  qualquer desvio (o runtime da P5 cobra por desenho os 7 ciclos da entrada da NMI: `--nmi-cycles 7`
  modela isso). **Critério de pronto:** nenhum endereço de RAM cru fora do `state.h` (os 7 ponteiros
  `nes_read16zp(0x..)` e a página da pilha do `mff.h` viraram nomes), nenhum registrador de hardware
  fora dos nomes do `hw.h`, as 9 rotas verdes nos dois runtimes; o lockstep com lag tirado do oráculo
  fica para a P7. `labels.toml`: 864 rótulos e 255 variáveis de RAM, sem nome repetido (356 nomes de
  rotina que os agentes deixaram como "proposed" nos comentários foram coletados por script, com a
  evidência do comentário), e o disassembly continua remontando byte a byte. **Fica:** 55 `goto` em 10
  arquivos, todos saltos para uma cauda que vários desvios do 6502 compartilham (`player_jump.c` 21,
  `enemy_moves.c` 8, `player_state_idle.c` 7, `player_state_walk.c` 6, `player_states.c` 4, o
  escalonador 2); 83 INC/DEC de memória pelo temporário `t` do lifter; o modelo de registradores do
  lifter (A/X/Y e flags em variáveis locais sincronizadas com `g_cpu` em cada chamada e saída), que
  sai quando o estado deixar os 2 KB (P9); e os nomes ainda marcados "(proposed)".
- **P5: o port roda no próprio runtime e sem a ROM.** `port/src/runtime` (C11) é a máquina que as
  rotinas precisam, escrita para este jogo: CPU e RAM, barramento, registradores da PPU, MMC3,
  a APU ligada ao sintetizador 2A03 da P6, relógio de ciclos com a fronteira de frame e a NMI,
  a IRQ de raster, despacho por endereço com o achatamento de laços de JMP e as corrotinas das
  tarefas. Implementa a interface do runtime do andaime (`nes_runtime.h`), então nenhuma rotina
  mudou; `port/CMakeLists.txt` gera `MightyFinalFight` só com `port/src/game`, `dispatch.c` e o
  runtime e o sintetizador (`port/src/audio`), nada do nesrecomp. A imagem é composta uma vez por frame (`render_frame.c`) a partir de um
  registro que o runtime preenche na fronteira do frame: o estado no início e uma região por split,
  aberta pelo handler de IRQ depois da linha em que o MMC3 disparou. Os dados vêm só de
  `BUILD/assets`, extraídos do dump uma vez quando o CMake configura (`-DMFF_ROM=dump`;
  `extract_assets.py --only runtime`): `chr.bin` e `prg.bin`, este sem os 46.368 bytes que o mapa da
  P0 classifica como código, menos 297 que o jogo também lê como dado (as telas que a lista de cada
  área nomeia depois do fim, lidas quando a câmera para, e um byte de tabela lido além do fim; ler um
  byte que falta para a execução e diz qual). Onde o runtime do andaime não é o console, o oráculo
  decidiu: os ciclos incluem o que o andaime deixa de fora e o console gasta (a sequência de reset e a
  entrada da NMI, 7 cada; as instruções do `nmi_tail`, 52 + 26, o andaime também cobra desde 17/10),
  que o jogo não vê (a NMI continua entrando só em `$FEBD`) e o som precisa (`selfcheck.py
  --nmi-cycles` desconta os 7 por NMI e falha em qualquer outra diferença); o `nmi_tail` faz os pushes
  do console, o do JSR também (o `extras.c` do andaime chamava `sound_update` sem o push e o RTS dele
  tirava dois bytes: depois de toda NMI o S ficava em `$01`, o frame de interrupção da IRQ de raster
  caía em `$0101/$0100/$01FF` e zerava `bg_pal_anim_on` das paletas 0 e 1, e a animação de paleta do
  round 3 área 2 parava; corrigido no andaime em 17/10, com bases novas); a RAM de
  nametable começa com o padrão de power-on do oráculo (o elevador do round 4 mostra a outra linha
  de nametable, que o jogo nunca escreve, na última linha da tela). Contra as bases novas do andaime
  as 9 rotas ficam idênticas em RAM, OAM, paleta e imagem a cada frame, e em ciclos a menos das
  entradas da NMI.
  A janela
  mostra a imagem como a PPU desenha (`--picture console`: 8 sprites por linha, a posição vertical
  seguindo pelos splits) e toca o som (abaixo, P6). **Imagem (17/10):** em todas as rotas de
  `inputs/` que partem do power-on (as três `10_*`, as três `01_*_s1`, `probe` e `probe2`), do
  power-on até o primeiro lag do oráculo, as 57.175 imagens são idênticas às do oráculo pixel a
  pixel (`port/tests/oracle_pictures.py`). O log de imagem das rotas continua com a do renderizador
  do andaime, que é o que as bases têm. **Tours com o oráculo** (`port/tests/oracle_tour.py`): o
  oráculo joga em segmentos com save state, RAM remendada como no `bot.py --god` (vida, skip stage)
  e o mesmo remendo no port por `MFF_POKE`, na mesma contagem de NMI; seis tours (os três
  personagens, dois ritmos de skip) passam pelos rounds 1-5, o chefe final e entram no final: as
  95.342 imagens até o primeiro lag de cada uma são idênticas, RAM igual (menos o `$C3` do som). Foi
  assim que apareceram o bug do `nmi_tail` e o da nametable de power-on. **Falta para o critério da
  P5:** comparar depois do primeiro lag de cada rota (os jogos se separam ali: o lag fiel é da P7),
  o que inclui o resto do final e os créditos. Compilar e jogar:

  ```bash
  cmake -S port -B build_mff -A x64 -DMFF_ROM="rom/Mighty Final Fight (USA).nes" -DMFF_SDL=ON
  cmake --build build_mff --config Release
  build_mff/Release/MightyFinalFight.exe --window      # ou --smoke N, sem janela
  MFF_EXE=build_mff/Release/MightyFinalFight.exe port/tests/run_routes.sh check
  ```

  Detalhes (teclas, variáveis de teste, formato dos dados) no `port/README.md`.
- **P6: o som toca no port.** `port/src/audio/synth_2a03.c` é o 2A03 (igual ao oráculo amostra por
  amostra com as escritas e os ciclos dele, `docs/sound.md`); o driver de som em C está em
  `port/src/game/sound_*.c` (P4). O runtime da P5 entrega cada escrita de APU ao sintetizador no
  relógio de frames do console (o primeiro vblank no ciclo 27.394, cada frame com o seu comprimento;
  o relógio do runtime corre mais, porque a NMI segurada espera sem dívida, e o som atrasava 170 ms
  em dois minutos), começa como o APU do oráculo (um clock no power-on sem os geradores de tom) e
  toca na janela (SDL, a fila de áudio dita o ritmo) e em `MFF_WAV`. **Verificado (17/10)** nas três
  rotas `10_*` do power-on até o primeiro lag do oráculo: a RAM do driver é a do oráculo em todo
  frame (menos `$C3`, o Y de quem chamou, como a `docs/sound.md` explica), e cada frame tem as mesmas
  escritas de APU na mesma ordem (156.483 escritas, `port/tests/apu_writes.py` contra a captura de
  `port/tests/synth`), 6-7 ciclos depois das do oráculo (mediana; 96-97% a até 16 ciclos). Com o
  sintetizador partindo assim, as escritas do oráculo nos ciclos dele dão o WAV do oráculo amostra
  por amostra, e nos ciclos do port dão o WAV do port: a diferença é só onde as escritas caem
  (o modelo de ciclos das rotinas convertidas, assunto da P7), inaudível (a fase do triângulo e a
  posição do registrador de ruído); `sound_wavcmp.py` põe as passagens no piso que a P6 mediu para a
  fase dos osciladores. **Falta:** o WAV aprovado no ouvido nas 5 fases, nos chefes e no final.
- **P9 (17/10, começada): a imagem da etapa 2.** `port/src/runtime/render_hd.c` (`--picture hd`)
  desenha a partir do estado do jogo: o playfield do mapa da fase (telas, metatiles, blocos e paletas
  decodificados por área do `prg.bin`, sem dado novo extraído), posto na tela pelo estado do escritor de
  colunas (`N = 32*$29 + $24`, a coluna nametable c guarda a coluna do mundo ≡ c mod 32 em N-31..N, as
  linhas 4 e 24 em branco, o atraso de atributo da coluna mais nova) mais os pedidos de VRAM do próprio
  jogo sobre o playfield (as listas `$19` e as tiras `$64`, como a porta do round 1 área 2); os sprites dos
  slots de objeto, por um modelo do objects_draw (`stage_draw.c` passo a passo) rodado sobre a RAM salva
  quando ele começa (`$E9BC`, gancho no `timing.c`), usado quando a página de OAM que o modelo monta é a
  do frame byte a byte. A barra de status e as telas que não são gameplay (título, história, seleção,
  mapa, continue, final) continuam vindo das nametables/OAM. **M1:** `MFF_HDCHECK` compara a imagem HD
  com tudo desligado com a do console a cada frame (`port/tests/hd_check.py`): nas 9 rotas e nas 5 rotas
  de power-on de `inputs/` que não estão entre elas, 427.520 imagens, todas idênticas; playfield do mapa
  em 308.156, sprites dos objetos em 417.089, e cada imagem que não vem do estado tem a sua classe (tela,
  bancos de CHR de outra área, colunas sendo desenhadas, saída de área, OAM escrita por uma tela). **M2:**
  `--no-flicker`: cada objeto uma vez por frame, todos os sprites, ordem fixa (efeitos, jogador, slots
  1-13), sem 8 por linha nem limite de OAM (nas rotas o console perde sprites em 103.255 imagens).
  **M3:** `--wide` (426x240, ou `--wide W`): a câmera do jogo presa dentro da área, o fundo do mapa
  inteiro, a barra e as telas no meio com o backdrop em volta (as duas salas de uma tela também); os
  objetos fora da tela original vêm do modelo (os "perto", a menos de `$68` px), e como os pontos de spawn
  continuam na borda original (lista da fase em x 255, spawner temporizado em -16/-4 e 276), um objeto só
  aparece fora das 256 colunas originais depois de ter estado inteiro dentro delas, e até lá só a parte
  dentro: o inimigo entra pela borda original como no console. `hd_check.py --wide` conta objetos que
  surgem nas margens: 0 nas 14 rotas (sem a regra, 10 no `guy_round1` e 23 no `cody_tour`). O estado do
  jogo não muda: `run_routes.sh check` idêntico nas 9 rotas com as opções desligadas, `--wide` e `--wide
  --no-flicker`. Detalhes no `port/README.md`, "P9". **Falta:** a barra de status e as telas a partir do
  estado, o estado livre dos 2 KB, pacotes de textura, filtros e escala inteira, e o efeito dos lag
  frames da P7 (NMI no meio do objects_draw: aquele frame usa a OAM).

---

## 1. Estrutura

```
MightyFinalFightRecomp/
├── PLAN_DECOMP.md
├── (recomp atual: CMakeLists.txt, game.toml, cyc_seeds.txt, tools/, inputs/ ...)  ← oráculo
├── disasm/
│   ├── labels.toml          ← nomes, comentários, regiões de dado, tipos (versionado)
│   ├── tools/disasm.py      ← gera build/disasm/bankNN.s a partir do SEU dump
│   └── tools/verify.py      ← remonta com ca65/ld65 e exige PRG byte a byte igual
└── port/
    ├── CMakeLists.txt
    ├── include/mff/state.h  ← struct que espelha os 2 KB de RAM do jogo (mesmos offsets)
    ├── src/game/            ← lógica decompilada, por sistema
    │   ├── main_loop.c  nmi.c  input.c  rng.c
    │   ├── objects.c  player_*.c  enemy_*.c  boss_*.c  items.c  collision.c
    │   ├── stage.c  scroll.c  hud.c  title.c  select.c  map.c  cutscene.c  ending.c
    │   └── sound_driver.c
    ├── src/engine/
    │   ├── render_frame.c   ← etapa 1: nametables/OAM/paleta por quadro + split da barra de status
    │   ├── render_hd.c      ← etapa 2: desenho direto do estado (widescreen, sem limite de sprites)
    │   ├── synth_2a03.c  timing.c  platform_sdl.c  savestate.c  config.c
    ├── tools/extract_assets.py  ← dump → assets/ (PNG/JSON/bin), com verificação de CRC
    └── tests/
        ├── lockstep/        ← roda port e oráculo frame a frame e compara a RAM
        └── routes/          ← as rotas de inputs/ como casos de teste
```

O `build/` continua ignorado. O disassembly gerado e os assets extraídos **nunca** entram no repo.

---

## 2. Estratégias por camada

### 2.1 Código vs. dado

- As **seeds** já dão os inícios de instrução executados. Falta o outro lado: quais bytes são **lidos
  como dado**, e de que banco.
- **Estratégia:** adicionar um **CDL** (code/data log) ao runner cycle do nesrecomp, num branch do
  submódulo: a cada leitura de PRG, marcar o offset físico como `CODE`, `DATA`, `INDIRECT` (lido via
  ponteiro) ou `JUMP_TABLE`. Formato compatível com o CDL do Mesen. Vira contribuição upstream.
- Rodar o CDL com as rotas e as sessões do bot (as mesmas que deram a cobertura) e cruzar com as seeds.
  O que não for nem código nem dado lido aparece como **"não visto"**, para investigar à mão (dado de
  trecho raro, lixo de padding ou código morto).
- Alternativa se o patch no runner travar: CDL do Mesen2 com as rotas via Lua.

### 2.2 Disassembly

- `disasm/tools/disasm.py` lê o dump, o CDL e o `labels.toml` e gera um `.s` por banco de 8 KB em
  sintaxe **ca65**, com tabelas de ponteiros como `.word label` e não como bytes soltos.
- **Critério de verdade:** `verify.py` remonta com `ca65`/`ld65` e o PRG resultante tem de ser **byte a
  byte igual** ao do dump. Isso garante que nenhum rótulo ou tabela está errado.
- O repo guarda só o `labels.toml` (nomes, comentários e tipos), nunca o `.s` gerado.
- Antes de nomear rotinas com truques de pilha (PHA/PLA mexendo no endereço de retorno, RTS como goto,
  dados inline depois de JSR), ler `nesrecomp/PATTERNS.md`.

### 2.3 Entendimento (nomes e estruturas)

- **Fontes:** Ghidra headless + GhidraNes via MCP (`nesrecomp/.mcp.json.example`), `docs/ram_map.md` (já
  corrigido em execução), `tools/state_ram.py` para experimentos ("o que acontece se...") e o
  `--frame-log` para ver quem escreve onde.
- **Primeiro o esqueleto:** reset, NMI, loop principal, máquina de estados do jogo (`$0030`), leitura do
  controle, RNG, tabela de objetos (`$0300`+), despacho por id de objeto.
- **Depois por sistema:** jogador (cada personagem), cada inimigo, chefes, itens, colisão, rolagem,
  carregamento de fase, HUD, telas (título, seleção, mapa, cutscenes, final) e o driver de som.
- Cada descoberta vai para o `labels.toml` com a evidência (frame, rota, endereço). A regra é a mesma do
  recomp: não adivinhar comportamento do 6502.

### 2.4 Da tradução ao C legível (estratégia "estrangulamento")

Reescrever 15 mil instruções de uma vez é inviável de testar. Em vez disso:

1. **Andaime.** Gerar o jogo no **backend legacy** do NESRecomp (uma função C por sub-rotina 6502, JSR =
   chamada de função, PPU por quadro do `runner/src/ppu_renderer.c`). O MFF vai pedir `game.toml` com
   trampolins de banco e despacho inline, achados no disassembly (o `game.toml` do YoshisCookieRecomp é o
   modelo). O andaime precisa bater com o oráculo nos frames sem lag.
2. **Ejeção.** Copiar a saída do andaime para `port/src/game/` **uma única vez**. A partir daí o código é
   mantido à mão: é o ponto em que o projeto deixa de ser recomp (a regra "nunca editar o gerado" do
   nesrecomp vale para o recomp na raiz, não para o `port/`).
3. **Limpeza função por função.** Cada função passa de `cpu.a = ram[0x320 + x]` para
   `obj[i].id`, com tipos e nomes. Toda função limpa precisa manter o teste lockstep verde.
4. **RAM compatível durante a etapa 1.** O estado do jogo continua nos mesmos 2 KB, só que acessado por
   `state.h` (structs sobrepostos aos endereços originais). É isso que permite comparar a RAM com o
   oráculo frame a frame. A troca para estruturas livres só acontece na etapa 2.
5. **Fim dos registradores.** Escritas em `$2006/$2007` (nametable), `$4014` (OAM DMA),
   `$2000/$2005` (scroll) e `$4000`–`$4017` (som) viram chamadas de API (`gfx_put_tiles`,
   `gfx_set_scroll`, `snd_*`). O MMC3 some: dados de banco viram ponteiros comuns para os assets.

### 2.5 Assets (extrair no build)

- `port/tools/extract_assets.py` confere o CRC32 (`15EB0BEE`) e gera em `build/assets/`:
  - CHR → folhas PNG de 8×8 com metadados de banco (o jogo troca bancos de CHR; o extrator registra cada
    conjunto);
  - paletas; metasprites (como cada personagem e inimigo é montado por sprites);
  - mapas de fase e blocos (o formato vem do disassembly; provavelmente comprimido);
  - textos (intro, diálogos, final) e as sequências de música e efeitos.
- O port lê só `build/assets/`. Sem os assets, mostra uma tela pedindo o dump e roda o extrator.
  (Hoje, P5: o executável carrega `chr.bin` e `prg.bin` da parte "runtime" do extrator e, sem eles,
  diz na saída como gerá-los; o CMake roda o extrator com `-DMFF_ROM`.)
- O formato dos assets fica documentado e estável: é a base para pacotes HD na etapa 2.

### 2.6 Renderização

**Etapa 1, `render_frame.c` (fiel):**
- Mantém nametables, OAM e paleta como estruturas comuns e compõe a imagem 256×240 uma vez por quadro.
- A barra de status, que no NES é um split por IRQ do MMC3 numa linha fixa, vira **duas regiões com
  scroll próprio** na composição.
- Reproduz o limite de 8 sprites por linha e a ordem de prioridade, porque o jogo depende disso (o
  piscar é intencional).
- **Critério:** imagem idêntica ao `--frame-log` do oráculo em todas as rotas, com divergências de
  efeitos no meio da linha (se houver) listadas e explicadas.

**Etapa 2, `render_hd.c` (alto nível):**
- Desenha direto do estado: camadas de fundo a partir dos mapas da fase, sprites a partir dos objetos
  vivos e dos metasprites, sem nametable e sem OAM.
- Opções: sem limite de sprites (acaba o piscar), widescreen 16:9 (a câmera mostra mais; o gatilho de
  spawn dos inimigos continua na borda original, para não mudar o jogo), filtros e escala inteira,
  pacotes de texturas.
- **Critério:** com tudo desligado, a imagem é igual à da etapa 1, tirando o piscar.

### 2.7 Áudio

- `sound_driver.c` reimplementa o driver de som da Capcom a partir do disassembly: interpretador de
  sequências, envelopes, vibrato e prioridade entre música e efeitos. Ele roda uma vez por quadro, como
  no NMI original.
- O driver gera **estado de canal** (período, volume, duty, ruído, amostra DMC), e não escritas em
  registrador. `synth_2a03.c` transforma isso em ondas a 48 kHz, com os mesmos filtros de saída do runner
  cycle (front-loader do NES).
- **Critério:** o estado de canal por frame bate com o do oráculo (derivado do `--frame-log` e do trace
  da APU), e o WAV fica perceptualmente igual ao `--wav-out` do recomp. Diferença de amostra é aceitável,
  porque o oráculo atualiza no meio do quadro.

### 2.8 Timing e slowdown

- O slowdown do NES vem de **lag frames**: o loop principal não termina antes do próximo NMI, e o frame
  repete a lógica anterior. O RNG e muita coisa dependem disso, então o port fiel precisa reproduzir.
- **Modelo:** custo em ciclos 6502 por rotina, medido no próprio recomp (os blocos compilados sabem
  quantos ciclos gastam). `timing.c` soma o custo do quadro e, se passar do orçamento de um frame NTSC,
  marca lag.
- **Desacoplamento para teste:** no modo lockstep, o port pode **pegar a decisão de lag do oráculo**.
  Assim a lógica é testada separadamente do modelo de tempo, e o modelo tem a própria métrica: lag frames
  iguais aos do oráculo nas rotas.
- Opção do usuário: "sem slowdown" (sempre 60 fps). Esse modo sai do lockstep por definição.

### 2.9 Testes (o coração do plano)

- **Lockstep offline:** o oráculo grava `--frame-log` de cada rota; o port roda a mesma rota e compara
  RAM, OAM, nametables e paleta a cada frame. Ao primeiro desvio, reporta frame, endereço, valor esperado
  e obtido, e as últimas funções do port que escreveram ali.
- **Lockstep em processo** (mais rápido para depurar): um binário de teste linka o port e o runtime cycle
  e avança os dois juntos.
- **Rotas:** `inputs/10_cody_poweron.txt`, as sessões do bot por personagem e por round, e rotas novas
  geradas pelo bot para cada sistema reescrito.
- **Imagem e som:** comparação por frame contra o oráculo (seções 2.6 e 2.7).
- **CI:** GitHub Actions compila nos três sistemas sem ROM (só build e testes que não precisam do dump).
  Os testes contra o oráculo rodam localmente, onde o dump existe.

### 2.10 Plataformas e build

- CMake, C11, SDL2 (SDL3 quando valer a pena). Sem dependência de MSVC no `port/`: MSVC no Windows,
  Clang no macOS, GCC/Clang no Linux.
- `cmake -DMFF_ROM=<dump>` roda o extrator; depois disso o build não precisa mais da ROM.
- Save states nativos (serialização do estado do jogo, não da máquina), config, remapeamento, gamepads,
  tela cheia. O launcher do `recomp-ui` pode ser reaproveitado se encaixar sem o runtime cycle.

---

## 3. Fases

Estimativas em dedicação parcial, com agentes fazendo o grosso.

### Fase P0 — Mapa de código e dado (≈1 semana)

1. CDL no runner cycle (branch do nesrecomp), com teste próprio.
2. Rodar CDL com todas as rotas e sessões do bot; cruzar com as seeds.
3. Relatório por banco: código, dado, indireto e "não visto".

**Pronto quando:** cada byte dos 128 KiB de PRG tem uma classificação, e os "não vistos" estão listados.

### Fase P1 — Disassembly remontável (≈1–2 semanas)

1. `disasm.py` + `labels.toml` inicial (vetores, NMI, reset, RAM map já verificado).
2. Tabelas de ponteiros e de saltos tipadas.
3. `verify.py` com ca65/ld65.

**Pronto quando:** o disassembly gerado remonta para um PRG **byte a byte igual** ao do dump.

### Fase P2 — Entendimento do esqueleto (≈2 semanas)

Reset, NMI, loop principal, estados do jogo, input, RNG, objetos, despacho por id, carregamento de fase,
troca de banco. Tudo nomeado no `labels.toml`, com `docs/` explicando cada sistema.

**Pronto quando:** dá para seguir um frame inteiro do jogo no disassembly só lendo nomes.

### Fase P3 — Andaime legacy + lockstep (≈2 semanas)

1. `game.toml` legacy (trampolins, despacho inline, regiões de dado) e geração por função.
2. Harness lockstep (offline e em processo).
3. Andaime batendo com o oráculo nos frames sem lag, nas rotas do Round 1.

**Pronto quando:** o andaime roda o Round 1 inteiro com RAM igual à do oráculo, tirando os lag frames
conhecidos.

### Fase P4 — Ejeção e limpeza (≈2–4 meses; a fase longa)

Ejetar para `port/src/game/` e limpar sistema por sistema, na ordem do esqueleto: loop, input e RNG →
objetos → jogador (Cody, Guy, Haggar) → inimigos → chefes → fase/rolagem → HUD → telas → final. Cada
sistema limpo vai com a sua rota de teste.

**Pronto quando:** nenhum acesso a `ram[0x....]` cru fora do `state.h`, nenhum registrador de hardware
no código do jogo, e todas as rotas em lockstep verde (com lag tirado do oráculo).

### Fase P5 — Assets e renderizador por quadro (≈3 semanas)

Extrator completo; `render_frame.c`; o port passa a ler só `build/assets/`.

**Pronto quando:** imagem igual ao oráculo em todas as rotas, rodando **sem a ROM**.

### Fase P6 — Driver de som + 2A03 (≈2–3 semanas)

**Pronto quando:** estado de canal igual ao oráculo por frame, e o WAV aprovado no ouvido nas 5 fases,
nos chefes e no final.

### Fase P7 — Timing e slowdown (≈2 semanas)

**Pronto quando:** lag frames iguais aos do oráculo em ≥ 99% dos frames das rotas, e o toggle "sem
slowdown" funcionando. O lockstep passa a usar o modelo próprio, sem consultar o oráculo.

### Fase P8 — Três plataformas e polimento (≈2 semanas)

CI nos três sistemas, save states nativos, config, gamepads, empacotamento sem assets.

**Pronto quando:** num Windows, macOS e Linux limpos dá para compilar, apontar o dump uma vez e zerar o
jogo. **Fim da etapa 1.**

### Fase P9 — Renderizador de alto nível (etapa 2, ≈1–2 meses)

`render_hd.c`, estado livre dos 2 KB (a sobreposição de RAM sai), widescreen, sem piscar, pacotes de
textura. Os testes passam a comparar a imagem da etapa 2 (sem extras) com a da etapa 1.

**Pronto quando:** o jogo inteiro roda em 16:9 sem cortes, sem inimigos surgindo na tela, e com as
opções desligadas igual à etapa 1.

---

## 4. Regras para o agente

- **O recomp da raiz é o oráculo e não muda** para acomodar o port. Correções no motor vão para um
  branch do submódulo (como o CDL), nunca para o código gerado.
- **No `port/`, código ejetado é código-fonte.** Editar à mão é o trabalho; regenerar por cima de código
  limpo é proibido.
- **Nenhum commit no `port/` com lockstep vermelho.** Se uma limpeza quebra o teste, a limpeza está
  errada, mesmo que o jogo "pareça" certo.
- **Evidência antes de nome:** cada rótulo e cada struct cita frame, rota ou endereço onde foi
  confirmado.
- Nunca commitar ROM, disassembly gerado, assets extraídos, states ou frame-logs.
- Ferramentas nativas rodam ocultas; a janela do jogo só abre quando o dono pede.

### Prompts prontos

- **P0:** "Adicione um CDL ao runner cycle do nesrecomp num branch (`--cdl-out FILE`, formato Mesen),
  com teste. Rode com todas as rotas de `inputs/` e as sessões do bot e gere o relatório por banco."
- **P3:** "Monte o andaime legacy do MFF seguindo o PLAN_DECOMP.md §2.4: `game.toml` com os trampolins
  do disassembly, harness lockstep, e faça o Round 1 bater com o oráculo fora dos lag frames."
- **P4:** "Limpe o sistema X no `port/`: nomes, tipos e `state.h`, mantendo o lockstep verde nas rotas
  Y. Cite no `labels.toml` a evidência de cada nome novo."

---

## 5. Riscos

| Risco | Mitigação |
|---|---|
| Truques de pilha e despacho por RTS confundem a tradução por função | `PATTERNS.md`; o despacho inline do `game.toml` legacy; testes lockstep pegam o erro no primeiro frame |
| Formato de fase comprimido e desconhecido | Começar pelo descompressor no disassembly; validar comparando nametables com o `--frame-log` |
| Lag frames muito sensíveis ao custo exato | Lag tirado do oráculo durante a P4; o modelo próprio é uma fase separada com métrica própria |
| Efeitos no meio da linha além da barra de status | O `--frame-log` mostra onde o scroll muda por linha; modelar como regiões, não como pontos |
| A fase P4 cansar (meses de limpeza) | Cada sistema limpo é uma entrega com teste; o jogo roda em toda etapa |
| Widescreen mudando o jogo (inimigos visíveis antes do gatilho) | Gatilhos de spawn e de câmera continuam na geometria original |
| Diferenças de compilador nos três sistemas | CI desde a P8; aritmética de 8 bits explícita (`uint8_t`) no estado |

## 6. Legal

- O repo não terá ROM, disassembly gerado nem assets: o extrator e o gerador rodam sobre o dump do
  usuário.
- **Atenção:** o **código C do `port/` é uma obra derivada do código da Capcom**, mesmo reescrito com
  nomes novos. Projetos como os decomps de *Super Mario 64* e *Zelda: A Link to the Past* publicam o
  código assim e aceitam esse risco, mas a decisão é do dono do repo. **Até essa decisão, o `port/` não
  vai para o GitHub público** (branch local, ou um repo privado).
- Binários distribuídos, se um dia houver, só com código: na primeira execução pedem o dump e extraem os
  assets.
