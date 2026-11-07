# Ҳисоботи омодагӣ ба compile

Сана: 2026-11-07

## Арзёбӣ

Ҳадафи версия Unreal Engine 5.4.4 аст (`EngineAssociation` = `5.4`). Лоиҳа scaffold-и C++ дорад, вале ҳоло compile-ready тасдиқшуда нест. Unreal Engine, UnrealBuildTool ва Xcode-и пурра дар муҳит дастрас нестанд. Дар натиҷа, UHT, генератори project files, compile-и Editor/Game, load-и `.uproject` ва PIE иҷро нашудаанд.

## Санҷиши сохтор

- `.uproject`: JSON дуруст; `EngineAssociation` ва `EngineVersion` ҳадафи 5.4.4-ро нишон медиҳанд; модули runtime-и `GTA1` таъриф шудааст.
- Targets: `GTA1Target` ва `GTA1EditorTarget` ба модули мавҷуда ишора мекунанд.
- `GTA1.Build.cs`: dependency-ҳои асосии `Core`, `CoreUObject`, `Engine`, `InputCore` доранд.
- Include-ҳои `.generated.h`: дар header-ҳои UCLASS илова шудаанд ва include-и охир мебошанд.
- Config: GameMode-и пешфарз, Engine Entry map, клавиатура, gamepad, virtual joystick ва scalability мавҷуданд.
- Source: персонаж бо кубҳои Engine-и намоёни муваққатӣ, минтақаи runtime, нуқтаҳои миссия, SaveGame ва мошини physics-based мавҷуданд.
- `Entry.Entry` ҳамчун харитаи Engine барои PIE/startup нигоҳ дошта шудааст; харитаи лоиҳавии `.umap` сохта нашудааст. Гузариш ба харитаи лоиҳавӣ танҳо пас аз насб ва истифодаи Unreal Editor имконпазир аст.

## Пеш аз эълони compile-ready ҳал/санҷида шавад

1. Муҳаррики мушаххаси Unreal 5-ро насб карда, `.uproject`-ро ба он associate кунед.
2. Бо UHT compile кунед ва API-и `ChoosePlayerStart_Implementation`, physics setup, input binding ва material parameters-ро ба ҳамон версия мувофиқ намоед.
3. Дар Editor санҷед, ки `/Engine/Maps/Entry.Entry`, `DefaultVirtualJoysticks` ва BasicShapes asset-ҳо воқеан бор мешаванд; натиҷаи `EngineVersion`-ро бо насби 5.4.4 мувофиқ намоед.
4. PIE: санҷиши collision ва spawn, ҳаракати персонаж, ворид/хориҷ аз мошин, тормоз/рул, ҳар се миссия, яккарат иҷро шудани reward ва persistence-и SaveGame.
5. Android: SDK/NDK/JDK-и мувофиқи UE-ро танзим кунед; APK созед ва touch control/FPS/RAM-ро дар дастгоҳи воқеӣ санҷед.
6. Windows: дар Visual Studio-и Windows compile, PIE ва packaged build санҷед.

## Қадамҳои дастӣ пеш аз PIE

1. Unreal Engine 5.4.4 ва Xcode-и пурра насб кунед; Epic Launcher бояд project association-и `5.4`-ро эҷод кунад.
2. `.uproject`-ро кушоод, project files-ро эҷод ва `GTA1Editor`-ро compile кунед. Агар UHT ё C++ хато диҳад, онро пеш аз PIE ислоҳ кунед.
3. Дар Editor `Entry.Entry`-ро кушоед. Барои санҷиши ибтидоӣ GameDefaultMap ва EditorStartupMap ба ҳамин Engine map ишора мекунанд; ҳоло `.umap`-и лоиҳавӣ вуҷуд надорад.
4. PIE-ро оғоз карда, Output Log-ро барои хатои `Cube`, `BasicShapeMaterial`, component registration, PlayerStart ва overlap тафтиш кунед.
5. Қаҳрамони кубиро пайдо кунед; бо W/A/S/D ҳаракат, ҷаҳиш ва давиданро санҷед. Ба мошин наздик шуда E, баъд W/A/S/D/Space/C/E-ро санҷед.
6. Ба се marker наздик шуда E пахш кунед; мукофот бояд як бор дода шавад. F5/F9-ро санҷед, бозиро аз нав оғоз карда вазъи миссия ва transform-ро тасдиқ кунед.
7. Танҳо баъд аз тасдиқи Engine map, дар Editor харитаи `Content/Maps/DushanbeBlockout`-ро бо PlayerStart эҷод ва default map-ҳоро ба он гузаронед. Ин амал Unreal Editor талаб мекунад ва ҳоло анҷом дода нашудааст.

То гузаштани ин санҷишҳо ҳамаи рафтори runtime ҳамчун номуайян ҳисобл шавад. Хусусан physics tuning, visual defaults ва touch action coverage ба тасдиқи Engine/PIE ниёз доранд.
