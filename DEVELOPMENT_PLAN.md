# Нақшаи рушд

Ҳар марҳила танҳо баъд аз далели санҷиш ҳамчун анҷомёфта қайд мешавад. Ҳоло ҳамаи хусусиятҳои бозӣ **planned/not verified** мебошанд.

| Марҳила | Вазъият | Натиҷаи талабшаванда |
|---|---|---|
| 1. Repository audit ва foundation | Дар ин commit — foundation-и ҳуҷҷатӣ ва ворид кардани scaffold; compile нест | Git hygiene, аудит, санҷиши статикӣ |
| 2. Unreal validation ва playable prototype | Blocked: Engine/Xcode нест, RAM 8 GiB | UHT/UBT compile, Entry/map, PIE бо character |
| 3. Душанбе ва character | Not started | Харитаи хурди бозишаванда, collision ва ҳаракати тасдиқшуда |
| 4. Vehicles ва garage | Not started | Савор/фаромадан, идоракунӣ, гаражи санҷидашуда |
| 5. Missions ва save | Код мавҷуд, runtime тасдиқ нашудааст | Миссия, мукофот ва Save/Load-и PIE |
| 6. LAN multiplayer то 10 нафар | Not started | Host/join, replication, санҷиши LAN бо дастгоҳҳо |
| 7. Online multiplayer | Not started | Архитектураи сервери dedicated, lobby ва мониторинг |
| 8. UI, launcher, download website | Not started | UI тоҷикӣ ва танҳо release-и воқеии download |
| 9. Windows/Android packaging | Blocked by toolchains | Package-и насбшаванда ва санҷиши платформа |
| 10. Optimization, testing, deployment | Not started | профайли дастгоҳҳо, QA, deployment-и тасдиқшуда |

## Маҳдудиятҳои кунунӣ

- Таҷҳизоти санҷишӣ: Mac M1, ARM64, 8 GiB RAM. Ҳуҷҷатҳои UE 5.4 ҳадди ақали 16 GB RAM ва тавсияи 32 GB доранд.
- Unreal Engine 5.4.4, UnrealBuildTool ва Xcode-и пурра дар муҳити санҷишӣ нестанд.
- Windows packaging ба Windows/Visual Studio/MSVC/Windows SDK ниёз дорад.
- Android packaging ба Android Studio, SDK 34, NDK r25b, Build-tools 34.0.0 ва OpenJDK 17.0.6 (талаботи UE 5.4.4) ниёз дорад.

Ҳеҷ марҳила бо мавҷуд будани source code танҳо “коркунанда” ҳисоб намешавад.
