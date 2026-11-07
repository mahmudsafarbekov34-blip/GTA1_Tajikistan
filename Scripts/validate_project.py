#!/usr/bin/env python3
"""Static checks for the Unreal project scaffold; this does not compile Unreal."""

from __future__ import annotations

import json
import re
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
PROJECT = ROOT / "Game" / "GTA1_UE.uproject"
SOURCE = ROOT / "Game" / "Source"


def fail(message: str) -> None:
    print(f"FAIL: {message}", file=sys.stderr)
    raise SystemExit(1)


def check_cpp_delimiters(path: Path) -> None:
    text = path.read_text(encoding="utf-8")
    stack: list[tuple[str, int]] = []
    pairs = {")": "(", "]": "[", "}": "{"}
    state = "code"
    line = 1
    index = 0

    while index < len(text):
        char = text[index]
        next_char = text[index + 1] if index + 1 < len(text) else ""
        if char == "\n":
            line += 1

        if state == "code":
            if char == "/" and next_char == "/":
                state = "line_comment"
                index += 1
            elif char == "/" and next_char == "*":
                state = "block_comment"
                index += 1
            elif char == '"':
                state = "string"
            elif char == "'":
                state = "character"
            elif char in "([{":
                stack.append((char, line))
            elif char in ")]}":
                if not stack or stack[-1][0] != pairs[char]:
                    fail(f"{path.relative_to(ROOT)}:{line}: unmatched {char}")
                stack.pop()
        elif state == "line_comment":
            if char == "\n":
                state = "code"
        elif state == "block_comment":
            if char == "*" and next_char == "/":
                state = "code"
                index += 1
        else:
            if char == "\\":
                index += 1
            elif (state == "string" and char == '"') or (
                state == "character" and char == "'"
            ):
                state = "code"
        index += 1

    if stack:
        fail(f"{path.relative_to(ROOT)}:{stack[-1][1]}: unclosed delimiter")


def main() -> None:
    if not PROJECT.is_file():
        fail(f"missing project descriptor: {PROJECT.relative_to(ROOT)}")

    try:
        project = json.loads(PROJECT.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError) as error:
        fail(f"invalid project JSON: {error}")

    if project.get("EngineAssociation") != "5.4":
        fail("EngineAssociation must target UE 5.4")
    if project.get("EngineVersion") != "5.4.4":
        fail("EngineVersion must target UE 5.4.4")
    if not any(
        module.get("Name") == "GTA1" and module.get("Type") == "Runtime"
        for module in project.get("Modules", [])
    ):
        fail("runtime module GTA1 is missing")

    required = [
        ROOT / "Game" / "Source" / "GTA1" / "GTA1.Build.cs",
        ROOT / "Game" / "Source" / "GTA1.Target.cs",
        ROOT / "Game" / "Source" / "GTA1Editor.Target.cs",
        SOURCE / "GTA1" / "GTA1Character.h",
        SOURCE / "GTA1" / "GTA1Character.cpp",
    ]
    for path in required:
        if not path.is_file():
            fail(f"required project file is missing: {path.relative_to(ROOT)}")

    build_rules = (ROOT / "Game" / "Source" / "GTA1" / "GTA1.Build.cs").read_text(
        encoding="utf-8"
    )
    if '"PhysicsCore"' not in build_rules:
        fail("PhysicsCore dependency is missing from GTA1.Build.cs")

    headers = list(SOURCE.rglob("*.h"))
    cpp_files = list(SOURCE.rglob("*.cpp"))
    for path in headers:
        lines = path.read_text(encoding="utf-8").splitlines()
        includes = [
            index
            for index, value in enumerate(lines)
            if value.lstrip().startswith("#include")
        ]
        generated = [
            index for index, value in enumerate(lines) if ".generated.h" in value
        ]
        if generated and (not includes or generated[-1] != includes[-1]):
            fail(f"{path.relative_to(ROOT)}: generated header must be last include")

    for path in cpp_files + headers:
        for line_number, value in enumerate(
            path.read_text(encoding="utf-8").splitlines(), start=1
        ):
            if value.endswith((" ", "\t")):
                fail(f"{path.relative_to(ROOT)}:{line_number}: trailing whitespace")
        check_cpp_delimiters(path)

        for include in re.findall(
            r'^\s*#include\s+"([^"]+)"',
            path.read_text(encoding="utf-8"),
            flags=re.MULTILINE,
        ):
            if include.endswith(".generated.h") or not include.startswith("GTA1"):
                continue
            if not (path.parent / include).is_file() and not (
                ROOT / "Game" / "Source" / include
            ).is_file():
                fail(f"{path.relative_to(ROOT)}: unresolved local include {include}")

    character_cpp = (SOURCE / "GTA1" / "GTA1Character.cpp").read_text(
        encoding="utf-8"
    )
    function = "void AGTA1Character::ApplyBlockoutMaterials()"
    position = character_cpp.find(function)
    if position < 0:
        fail("ApplyBlockoutMaterials definition is missing")
    before_function = character_cpp[:position]
    if before_function.count("{") != before_function.count("}"):
        fail("ApplyBlockoutMaterials is not defined at file scope")

    print("PASS: UE 5.4.4 project metadata and module")
    print("PASS: required targets, build rules, and PhysicsCore dependency")
    print(f"PASS: generated-header order and local includes ({len(headers)} headers)")
    print(f"PASS: delimiter and whitespace scan ({len(cpp_files)} C++ files)")
    print("PASS: ApplyBlockoutMaterials file-scope definition")

    maps = list((ROOT / "Game").rglob("*.umap"))
    assets = list((ROOT / "Game").rglob("*.uasset"))
    if not maps or not assets:
        print(
            "NOTE: project-owned map/assets are absent; this static check does not "
            "establish a playable project."
        )
    print("NOTE: no UnrealBuildTool/UHT compile or PIE test was performed.")


if __name__ == "__main__":
    main()
