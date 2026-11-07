# Ҳисоботи санҷиш

Санаи санҷиш: 2026-11-07

## Иловаи марҳилаи омодасозии compile

- Ҳадафи engine дар `.uproject`: association `5.4`, версия `5.4.4`.
- Барои character blockout-и кубии Engine илова шуд; модели Skeletal/asset ворид нашудааст.
- Спавни минтақа ва PlayerStart ба `InitGame`, сохтани геометрия ба `PostInitializeComponents` гузаронида шуд, то collision пеш аз оғози pawn сохта шавад.
- Нуқтаи PlayerStart-и муайян ва spawn-и мошину миссияҳо ба координатҳои роҳ гузошта шуданд.
- Scalar-и touch-interface аз `+bAlwaysShowTouchInterface` ба `bAlwaysShowTouchInterface` ислоҳ шуд; Gamepad LeftY барои forward/throttle чаппа карда шуд.
- Dependency-и `PhysicsCore` илова шуд; BuildSettingsVersion.V5 ба ҳар ду target илова шуд.
- Имзои `AGameModeBase::InitGame` ва `USceneComponent::SetAbsolute` бо API reference-и расмии UE 5.4 муқоиса шуд.
- JSON-и `.uproject`, версия/модул, мавҷудияти include-ҳои лоиҳавӣ, тартиби include-и `.generated.h`, конфигуратсияи Entry/input ва тавозуни қавсҳои C++ статикӣ санҷида шуданд.
- Ин тағйирот танҳо статикӣ баррасӣ шуданд. Compile/PIE ҳанӯз иҷро нашуданд.

## Муҳит

- macOS 27.2, Apple M1, 8 GB unified memory, тақрибан 175 GB фазои диск.
- `UnrealEditor`, `UnrealBuildTool`, `UnrealVersionSelector`, `adb`, `sdkmanager`, Android Studio ва Android SDK ёфт нашуданд.
- `xcodebuild -version` иҷро шуд; натиҷа: developer directory ба `/Library/Developer/CommandLineTools` ишора мекунад ва toolchain мегӯяд, ки Xcode-и пурра лозим аст.
- Windows/Visual Studio дар ин муҳит мавҷуд нест ва Windows target сохта нашудааст.

## Санҷишҳои иҷрошуда

- Рӯйхати файли scaffold ва сохтори модули `GTA1` аз назар гузаронда шуд.
- `.uproject` бо `python3 -m json.tool` санҷида шуд.
- Файлҳои Config, Build.cs, Target.cs, C++ ва документация хонда шуданд.
- Файлҳои ҳуҷҷатии зарурӣ ва include-ҳои лоиҳавии C++ мавҷуданд.
- Баланси қавсҳои C++ бо санҷиши лексикии маҳаллӣ тасдиқ шуд. Ин compile ё санҷиши синтаксиси C++ нест.
- Фармонҳои дастрасии toolchain ва роҳҳои эҳтимолии Unreal/Android санҷида шуданд.

## Санҷишҳои иҷронашуда

| Санҷиш | Вазъият | Сабаб |
|---|---|---|
| C++ / UHT compile | Иҷро нашуд | Unreal Engine ва UnrealBuildTool насб нестанд |
| Кушодани `.uproject` ва PIE | Иҷро нашуд | Unreal Editor насб нест |
| Character / camera / input | Иҷро нашуд | PIE ва дастгоҳи Android дастрас нестанд |
| Миссия ва мукофот | Иҷро нашуд | Рафтори runtime санҷида нашудааст |
| Save/Load | Иҷро нашуд | Unreal SaveGame-ро бе муҳаррик иҷро карда намешавад |
| Vehicle controls / physics | Иҷро нашуд | Физика ва collision бояд дар UE танзим шаванд |
| Android APK / touchscreen | Иҷро нашуд | Android Studio, SDK/NDK, `adb` ва дастгоҳ санҷида нашудаанд |
| Windows package | Иҷро нашуд | Windows ва Visual Studio дастрас нестанд |
| FPS / RAM profiling | Иҷро нашуд | Бозии runtime ва профайлер дастрас нестанд |

## ## Хулоса

Тайёр будани scaffold ва дурустии JSON compile-ро исбот намекунад. Пеш аз идомаи бозигарӣ, муҳаррики мақсаднокро насб карда ҳамаи санҷишҳои иҷрошуданашударо аз нав гузаронед. Код ва конфигуратсияҳои охирин compile-verified нестанд.
