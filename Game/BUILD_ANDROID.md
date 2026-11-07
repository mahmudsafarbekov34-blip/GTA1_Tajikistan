# Сохтан ва санҷиш барои Android

## Талабот

- Версияи Unreal Engine 5, ки Android-ро дастгирӣ мекунад.
- Android Studio ва версияҳои SDK, NDK ва JDK, ки ҳуҷҷатҳои **ҳамон версияи UE** талаб мекунанд. Версияҳоро худсарона омехта накунед.
- Дастгоҳи Android-и USB debugging фаъол ё emulator-и мувофиқ.
- Барои сохтани/имзои релиз, keystore-и маҳаллии соҳибистифодабаранда.

## Қадамҳо

1. UE-ро насб кунед ва аз **Platforms → Android → Install SDK** ё ҳуҷҷати версияи дақиқи муҳаррик SDK/NDK/JDK-ро танзим кунед.
2. `GTA1_UE.uproject`-ро кушоед ва дар **Project Settings → Platforms → Android** SDK paths, package name, архитектура ва minimum SDK-ро танзим кунед.
3. `DefaultVirtualJoysticks`-ро дар Android санҷед; ҳаракат, камера, ҷаҳиш, давидан, interact ва идораи мошинро дар touchscreen санҷида, агар тугмаҳо намерасанд HUD-и UMG созед.
4. Дар **Project Settings → Rendering** Mobile renderer-ро барои дастгоҳҳои ҳадаф санҷед. Lumen/Nanite шарт нестанд; Mobile HDR дар конфигуратсия ғайрифаъол аст.
5. Дар дастгоҳи воқеӣ `Development` APK созед. Логҳо, FPS, RAM, input ва SaveGame-ро санҷед.
6. Баъд аз гузаштани санҷишҳо keystore-и шахсиро танзим карда `Shipping` package созед. Keystore-ро ба repository илова накунед.

Ин Mac ҳоло Unreal, Android Studio, SDK, NDK, `adb` ва `sdkmanager` надорад. APK сохта ё дар дастгоҳ санҷида нашудааст.
