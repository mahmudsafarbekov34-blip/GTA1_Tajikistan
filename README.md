# GTA 1 — TAJIKISTAN

Лоиҳаи прототипии бозии 3D-и Душанбе бо Unreal Engine 5.4.4. Ин repository ҳоло заминаи C++ ва ҳуҷҷатҳои роҳнаморо нигоҳ медорад; он ҳанӯз бозии пурраи бозишаванда ё build-и паҳншаванда нест.

## Вазъи воқеӣ

- Repository аввал холӣ буд. Scaffold-и мавҷудаи `GTA1_UE` ба `Game/` ворид карда шуд.
- `.uproject` версияи ҳадафи 5.4.4-ро нишон медиҳад.
- C++-и прототип: ҳаракати қаҳрамон, blockout-и runtime, нуқтаҳои миссия, SaveGame ва мошини муваққатӣ.
- **UnrealBuildTool/UHT compile, PIE, Windows packaging ва Android packaging иҷро нашудаанд.**
- Дар Mac-и санҷидашуда Unreal Engine ва Xcode-и пурра нестанд; RAM 8 GiB аст, дар ҳоле ки талаботи расмии UE 5.4 ҳадди ақал 16 GB мегӯяд.
- Project-owned `.umap` ва `.uasset` вуҷуд надоранд. Моделҳо, садоҳо, UI, NPC ва asset-и ниҳоӣ ҳанӯз илова нашудаанд.
- Multiplayer, backend, website/launcher ва build-и download ҳанӯз амалӣ нашудаанд. Download-и сохта ё URL-и сохта пешниҳод намешавад.

Барои далелҳо ба [аудити марҳилаи 1](Docs/PHASE1_AUDIT.md) нигаред.

## Сохтор

```text
Game/                 Unreal C++ project scaffold
Docs/                 Audit and project status
Scripts/              Local/static validation
.github/workflows/    Static checks only; no Unreal compilation
```

Файлҳои HTML-и бозии дигар ба ин repository нусхабардорӣ нашудаанд.

## Оғози кор

Барои кушодани муҳаррир, Unreal Engine-и мувофиқи `.uproject` ва toolchain-и мувофиқ лозим аст. Ба [BUILD_WINDOWS.md](BUILD_WINDOWS.md) ё [BUILD_ANDROID.md](BUILD_ANDROID.md) нигаред. Танзими муҳити ин Mac ва сабаби compile нашудан дар [COMPILE_READINESS.md](Game/COMPILE_READINESS.md) сабт шудааст.

Санҷиши дастрас бе Unreal:

```bash
python3 Scripts/validate_project.py
```

Ин танҳо metadata ва сохтори файлҳоро месанҷад; **compile/UHT/PIE нест**.

## Download

Дар ин марҳила build-и воқеии Windows ё Android вуҷуд надорад; аз ин рӯ тугма ё URL-и download ҳоло пешниҳод намешавад. Баъд аз packaging ва санҷиши checksum, release метавонад дар [GitHub Releases](https://github.com/mahmudsafarbekov34-blip/GTA1_Tajikistan/releases) ҷойгир шавад.

## Иҷозатнома ва asset-ҳо

Ҳоло иҷозатномаи asset-и беруна истифода нашудааст. Пеш аз ворид кардани asset бояд иҷозатнома ва ҳуқуқи паҳнкунӣ сабт шаванд. Лоиҳа ҳоло `LICENSE`-и умумӣ надорад; ҳуқуқи коди саҳмгузоронро бе мувофиқаи соҳибон муайян накунед.
