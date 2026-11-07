# Build барои Windows

## Вазъият

Дар ин repository ҳоло Windows package нест ва Windows build иҷро нашудааст. Source-и C++ ҳанӯз бо UE 5.4.4 compile/UHT тасдиқ нашудааст.

## Талабот

- Windows 10/11 64-bit.
- Ҳамон версияи Unreal Engine 5.4.4, ки дар `Game/GTA1_UE.uproject` нишон дода шудааст.
- Visual Studio 2022, workload **Desktop development with C++** ё **Game development with C++**, MSVC toolset ва Windows SDK мувофиқ ба UE 5.4.
- Фазои диск ва RAM-и кофӣ барои Editor, intermediate ва packaged output.

## Қадамҳо

1. Unreal Engine 5.4.4 ва Visual Studio 2022-ро насб кунед.
2. `Game/GTA1_UE.uproject`-ро кушоед ва project files-ро эҷод кунед.
3. Target-и `GTA1Editor`, configuration-и Development Editor / Win64-ро build кунед.
4. UHT/compile хато диҳад, аввал онро ислоҳ кунед; build-и нокомро ҳамчун натиҷаи тайёр ҳисоб накунед.
5. Пас аз кушодани Editor, map ва PIE-ро санҷед.
6. Танҳо баъд package созед ва executable-ро дар Windows-и дигар санҷед.

Windows build аз Mac-и ҳозира тасдиқ карда намешавад. Package, версия ва checksum вуҷуд надоранд.
