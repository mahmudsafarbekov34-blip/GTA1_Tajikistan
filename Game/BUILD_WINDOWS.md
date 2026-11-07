# Сохтан ва санҷиш барои Windows

## Талабот

- Windows 10/11 64-bit.
- Unreal Engine 5 ва source build tools-и C++.
- Visual Studio 2022 бо **Game development with C++**, MSVC toolset ва Windows SDK.
- Фазои диск ва RAM-и кофӣ барои муҳаррик ва intermediate files.

## Қадамҳо

1. Аз роҳи расмии Epic Games Launcher UE5-ро насб кунед.
2. `GTA1_UE.uproject`-ро дар Windows кушоед; агар ассоциация талаб шавад, версияи насбшударо интихоб кунед.
3. **Generate Visual Studio project files**-ро интихоб кунед ё файлро аз Editor кушоед ва module-ро rebuild кунед.
4. Дар Visual Studio `GTA1Editor`-ро ҳамчун target ва `Development Editor | Win64`-ро интихоб карда build кунед.
5. Лоиҳаро дар Editor кушоед, PIE-ро санҷед ва хатогиҳоро ислоҳ кунед.
6. Пас аз гузаштани санҷишҳо: **Platforms → Windows → Package Project** ва натиҷаро дар папкаи нави build гиред.
7. Бастаи тозаеро бе Editor дар Windows-и дигар оғоз карда, ҳаракат, мошин, миссия ва захираро санҷед.

Ин дастур иҷрои build-ро тасдиқ намекунад. Build-и Windows бояд дар Windows бо toolchain-и мувофиқ иҷро шавад; дар macOS-и кунунӣ ин тасдиқ нашудааст.
