# Plano — Mighty Final Fight nativo no Windows (NESRecomp)

> Versão Windows do [PLAN.md](PLAN.md) (macOS). Mesma estratégia, mesmas fases;
> muda o toolchain, os comandos e o empacotamento. Verificado contra
> nesrecomp @ `2b4fcdb` (2026-10-07) e o template YoshisCookieRecomp.

## 0. Por que o Windows é o caminho mais fácil

- Windows x64 + MSVC é a plataforma **principal/madura** do NESRecomp; o macOS é experimental.
  Toda a documentação, os caminhos de debug (`C:/temp/...`) e os scripts (`.ps1`, `.bat`) assumem Windows.
- O nesrecomp já traz scripts prontos para o backend cycle:
  - `tools/build_cycle_windows.ps1` — configura (VS 2022, x64) e compila Release.
  - `tools/package_cycle_windows.ps1` — gera um ZIP **sem ROM** com SDL2, launcher e licenças.
- O SDL2 vem dentro do repo (`runner/external/SDL2/SDL2.dll`) — não precisa instalar.
- O template (YoshisCookieRecomp) já tem `setup.bat`, `build_all.bat` e `tools/build.ps1`.

As correções ao handoff valem igual aqui (ver PLAN.md §0): **não há disassembly do MFF**
(ele está só em "Planning"), o backend padrão é **cycle-accurate** (MMC3 resolvido no runtime),
o trabalho real é **cobertura via `--miss-log`**, e o template certo é o **YoshisCookieRecomp**.

**ROM:** `Mighty Final Fight (USA).nes` — CRC32 `15EB0BEE`, mapper 4, PRG 128 KiB, CHR 128 KiB, sem bateria.

---

## 1. Estrutura do projeto

Use um caminho curto e sem espaços (ex.: `C:\Projects\MightyFinalFightRecomp`) para evitar
problemas de `MAX_PATH` com os submodules.

```
C:\Projects\MightyFinalFightRecomp\
├── PLAN_WINDOWS.md
├── CLAUDE.md
├── CMakeLists.txt          ← do YoshisCookieRecomp, só o ramo cycle
├── game.toml
├── cyc_seeds.txt
├── setup.bat               ← do template (submodules + junction nestopia-core)
├── build_all.bat           ← do template → tools\build.ps1
├── tools\build.ps1         ← do template → nesrecomp\tools\build_cycle_windows.ps1
├── tools\make_release.ps1  ← do template (adaptar) → package_cycle_windows.ps1
├── inputs\*.txt
├── docs\ram_map.md  docs\symbols.sym
├── nesrecomp\  recomp-ui\  ← submodules fixados
├── rom\                    ← .nes local, no .gitignore
└── build_release\          ← ignorado
```

Adicionar `.gitattributes` (`*.sh text eol=lf`, `*.bat text eol=crlf`, `*.ps1 text eol=crlf`)
para o repo funcionar tanto no Windows quanto no Mac.

---

## 2. Fases

### Fase 0 — Setup (≈1 h, a maior parte é instalar o Visual Studio)

Pré-requisitos (via `winget` ou instaladores oficiais):

| Ferramenta | Observação |
|---|---|
| **Visual Studio 2022** (Community serve) | Workload *Desktop development with C++* (MSVC x64 + Windows SDK). O script usa o gerador `Visual Studio 17 2022` |
| **CMake ≥ 3.20** | Instalar em `C:\Program Files\CMake` — `build_cycle_windows.ps1` procura exatamente `%ProgramFiles%\CMake\bin\cmake.exe` (`winget install Kitware.CMake`) |
| **Git for Windows** | Recomendado `git config --global core.longpaths true` |
| **Python ≥ 3.11** | Exigido pelo configure do backend cycle (`winget install Python.Python.3.12`); conferir `py -3.12 --version` |
| **Mesen2** (Windows) | Oráculo para comparar frames/RAM; CDL e Trace Logger |
| Ghidra + JDK 21 | Só para a Fase 4 |

Passos:

1. `git init` em `C:\Projects\MightyFinalFightRecomp`, `.gitignore` (`rom/`, `*.nes`, `*.zip`, `build*/`, `saves/`, `*.cycstate`).
2. Copiar o ROM para `rom\`. Conferir: `Get-FileHash` não dá CRC32 — usar `py -3.12 -I -c "import zlib,sys;print('%08X'%zlib.crc32(open(sys.argv[1],'rb').read()))" "rom\Mighty Final Fight (USA).nes"` → `15EB0BEE`.
3. Submodules `nesrecomp` (fixar commit) e `recomp-ui`; rodar `setup.bat` (cria a junction `nestopia-core`, não precisa de admin).
4. Salvar o RAM map do Data Crystal em `docs\ram_map.md`.

**Atalho opcional (5 min):** baixar `nesrecomp-cli-windows-x86_64.zip` em *Releases* e rodar
`.\nesrecomp.exe build --rom "...\Mighty Final Fight (USA).nes" --output C:\temp\mff-probe`
+ `.\build.ps1` na saída. Isso só gera uma biblioteca estática (não é jogável), mas prova em
minutos que o ROM recompila e mostra o `game.toml` proposto.

**Pronto quando:** VS 2022, CMake (no caminho esperado), Git e Python 3.11+ respondem; CRC confere.

### Fase 1 — Primeiro boot headless (≈2–4 h)

1. `CMakeLists.txt` do YoshisCookieRecomp: remover o ramo legacy, target `MightyFinalFightRecomp`, `PLAYERS 2`.
2. `game.toml` mínimo:
   ```toml
   [game]
   name = "Mighty Final Fight"
   output_prefix = "mighty-final-fight"
   ```
3. `cyc_seeds.txt` vazio (só cabeçalho).
4. Build headless (o script aceita args extras do CMake):
   ```powershell
   .\nesrecomp\tools\build_cycle_windows.ps1 `
     -ProjectRoot . `
     -Rom ".\rom\Mighty Final Fight (USA).nes" `
     -CMakeArgs '-DNESRECOMP_HEADLESS=ON'
   ```
5. Rodar 600 frames e salvar screenshot:
   ```powershell
   .\build_release\Release\MightyFinalFightRecomp.exe ".\rom\Mighty Final Fight (USA).nes" --frames 600 --screenshot C:\temp\mff_600.png
   ```
6. Comparar com o mesmo frame no Mesen2.

**Pronto quando:** compila e o frame ~600 mostra a tela de título.

**Prováveis problemas:** bem menos que no Mac. Se aparecer algo, a regra é a mesma —
**corrigir no nesrecomp, nunca no gerado** — num branch do submodule.

### Fase 2 — Loop de cobertura até 100% nativo (≈1–3 dias)

Idêntica ao PLAN.md:

```
roteiro inputs\*.txt → exe --input X --miss-log cyc_seeds.txt
      → build_all.bat (seeds viram blocos nativos) → roda de novo
      → repetir até 0 misses novos
```

Roteiros, em ordem: boot/título/demo → seleção de Cody, Guy e Haggar → fase 1 completa
(golpes, agarrão, especial, item, morte, continue) → fases 2–5 (a partir de `.cycstate`) →
chefes, level-up, game over, final. Usar o RAM map para esperar estados; checar
`fallback_telemetry.jsonl` e `dispatch_misses.log` (ficam ao lado do `.exe`).

**Pronto quando:** todos os roteiros com 0 misses novos e ~100% de ciclos nativos.

### Fase 3 — Validação contra o oráculo

No Windows o ferramental de co-simulação está completo (`COSIM.md`, `tools\nes_cosim.py`,
`tools\nesref` com `mesen_libretro.dll`):

1. `tools\cyc\mesen_shot` com os mesmos roteiros + `cmp_picture.py` nos frames-chave.
2. RAM em checkpoints (`--ram-init zeros --mem-frame N --mem-out`) vs Mesen, nos endereços do RAM map.
3. `abram` / `abppu` (COSIM.md) para achar o **primeiro frame divergente** quando algo não bater.
4. Atenção: split da barra de status (IRQ do MMC3), slowdown, áudio.

**Pronto quando:** frames-chave idênticos (ou diferenças explicadas) e RAM convergente.

### Fase 4 — Análise profunda só onde travar

Igual ao PLAN.md: Mesen2 (CDL + Trace Logger) → Ghidra headless via MCP
(`nesrecomp\.mcp.json.example`) → ler `PATTERNS.md` antes de mexer em truques de pilha →
registrar em `docs\symbols.sym` / `[[data_region]]`. REA só se precisar.

### Fase 5 — Polimento e distribuição (≈meio dia)

1. Build com janela (sem `NESRECOMP_HEADLESS`): `build_all.bat`.
2. Launcher/menu do `recomp-ui`, controles XInput/DualSense via SDL, `F11` fullscreen, `F8/F9` save state.
3. Empacotar sem ROM:
   ```powershell
   .\nesrecomp\tools\package_cycle_windows.ps1 `
     -ProjectRoot . -Target MightyFinalFightRecomp -Title "Mighty Final Fight"
   ```
   (adaptar `tools\make_release.ps1` do template para chamar isso.)
4. O `.exe` não é assinado: o SmartScreen vai avisar na primeira execução — esperado.
5. Opcional: `--extra-scanlines 128` (enhancement só-MMC3) para reduzir o slowdown; deixar como opção no menu.

**Pronto quando:** descompactar o ZIP numa máquina limpa, abrir, escolher o ROM e jogar do início ao fim.

---

## 3. Regras para o agente (`CLAUDE.md`)

As mesmas do PLAN.md, mais a regra do `AGENTS.md` do nesrecomp para Windows:

- Rodar ferramentas nativas (git, cmake, python, testes) como **processo oculto**
  (sem abrir janela de terminal nem roubar o foco); invocar os executáveis direto, não via shims.
- Só abrir a janela do jogo quando o usuário pedir.
- Nunca editar `generated/`; correções vão para `recompiler/src/cyc_codegen.c` ou `runner/cyc/hw_*.c`.
- Citar evidência (bytes do ROM, trace do Mesen, Ghidra) antes de qualquer mudança; diffs pequenos.
- Nunca commitar ROM, saves ou binários com assets.

Os prompts prontos do PLAN.md §3 servem igual, trocando os comandos pelos desta versão.

---

## 4. Riscos (diferenças em relação ao Mac)

| Risco | Mitigação |
|---|---|
| CMake fora de `C:\Program Files\CMake` → script falha | Instalar no caminho padrão, ou chamar o `cmake` direto com os mesmos args do script |
| `MAX_PATH` com submodules profundos | Caminho curto + `core.longpaths true` |
| Python da Microsoft Store/alias `python.exe` interceptando | Usar `py -3.12`; desligar os *App execution aliases* do Python se atrapalhar |
| Antivírus segurando o build (muitos `.c` gerados) | Excluir a pasta `build_release` do scan em tempo real, se ficar lento |
| Sem disassembly do MFF | Mesen CDL + Ghidra só nos pontos de falha |

## 5. Legal

Igual ao PLAN.md: só o seu dump; o ZIP distribuído não leva ROM nem código gerado a partir dele.
