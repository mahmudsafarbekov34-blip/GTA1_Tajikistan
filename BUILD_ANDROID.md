# Build барои Android

## Вазъият

APK ҳоло сохта ё дар дастгоҳи воқеӣ санҷида нашудааст. Дар Mac-и санҷишӣ Android SDK/NDK/JDK ва Unreal Engine нестанд.

## Версияҳои ҳуҷҷатшудаи UE 5.4.4

- Android Studio Flamingo 2022.2.1 Patch 2.
- Android SDK 34.
- Android NDK r25b.
- Android Build-tools 34.0.0.
- OpenJDK 17.0.6.

Ҳангоми setup танҳо комбинатсияи дастгиришудаи ҳамин версияи муҳаррикро истифода баред. SDK/NDK/JDK-ро байни версияҳои гуногуни UE бе санҷиш омехта накунед. Талаботи ҷории нашр метавонад тағйир ёбад; пеш аз release ҳуҷҷати Epic ва талаботи мағозаи ҳадафро аз нав санҷед.

## Қадамҳо

1. Аввал UE 5.4.4-ро насб карда, Android SDK/NDK support-ро илова кунед.
2. Android Studio ва версияҳои дар боло мувофиқро танзим карда, SDK license-ро қабул кунед.
3. `Game/GTA1_UE.uproject`-ро кушоед; Android target ва ARM64-ро танзим кунед.
4. UHT ва C++-ро compile кунед, баъд Development APK созед.
5. APK-ро дар Android 8+ дастгоҳи воқеии ARM64 санҷед: input, rendering, FPS, RAM, SaveGame ва offline start.
6. Release signing keystore-ро берун аз Git нигоҳ доред.

Package ва APK-и санҷидашуда вуҷуд надоранд; download-link сохта нашудааст.
