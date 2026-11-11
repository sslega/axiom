"""Snippet-level licence scan of Axiom's own source code, using SCANOSS.

Fingerprints first-party files locally, sends only the hashes (with obfuscated file
names) to the SCANOSS API, and reports any file or snippet that matches a known
open-source project. Third-party code (Engine/external, Editor/external, FetchContent
dependencies) is out of scope: it is vendored on purpose and listed in
THIRD_PARTY_NOTICES.md.

Usage (run with the SCANOSS environment's Python, see CLAUDE.md):
    Z:/Tools/scanoss/Scripts/python.exe Tools/licence_scan.py          # changed files vs HEAD
    Z:/Tools/scanoss/Scripts/python.exe Tools/licence_scan.py --all    # every first-party file

Exit codes: 0 = no unreviewed matches, 1 = matches to review, 2 = the scan itself failed.

A match is reviewed by adding a `bom.remove` entry (path + purl + comment saying why it
is not a copy) to scanoss.json at the repo root. Never add one without that review.
"""

import argparse
import json
import subprocess
import sys
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parent.parent
RESULTS_DIR = REPO_ROOT / "build" / "licence-scan"

FIRST_PARTY_ROOTS = ("Engine/", "EditorCore/", "Editor/", "Game/", "Tests/", "Tools/")
THIRD_PARTY_ROOTS = ("Engine/external/", "Editor/external/")
SOURCE_EXTENSIONS = {".h", ".hpp", ".c", ".cpp", ".inl", ".glsl", ".hlsl", ".py"}


def git_lines(*args: str) -> list[str]:
    out = subprocess.run(["git", *args], cwd=REPO_ROOT, capture_output=True, text=True, check=True)
    return [line.strip() for line in out.stdout.splitlines() if line.strip()]


def collect_files(scan_all: bool) -> list[str]:
    if scan_all:
        candidates = git_lines("ls-files")
    else:
        candidates = git_lines("diff", "--name-only", "HEAD") + git_lines("ls-files", "--others", "--exclude-standard")

    files = set()
    for path in candidates:
        if not path.startswith(FIRST_PARTY_ROOTS) or path.startswith(THIRD_PARTY_ROOTS):
            continue
        if Path(path).suffix.lower() not in SOURCE_EXTENSIONS:
            continue
        if (REPO_ROOT / path).is_file():  # deleted files show up in the diff
            files.add(path)
    return sorted(files)


def is_copyleft(spdx_id: str) -> bool:
    try:
        from scanoss.osadl import Osadl
        return Osadl(quiet=True).is_copyleft(spdx_id)
    except Exception:
        return spdx_id.upper().startswith(("GPL", "LGPL", "AGPL", "MPL", "EPL", "CC-BY-SA", "CC-BY-NC"))


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--all", action="store_true", help="scan every first-party file, not only changed ones")
    args = parser.parse_args()

    files = collect_files(args.all)
    if not files:
        print("Licence scan: no first-party source files to scan.")
        return 0

    scanner = Path(sys.executable).parent / "scanoss-py.exe"
    if not scanner.is_file():
        print(f"Licence scan: scanoss-py not found next to {sys.executable}. Run with Z:/Tools/scanoss/Scripts/python.exe.")
        return 2

    RESULTS_DIR.mkdir(parents=True, exist_ok=True)
    results_file = RESULTS_DIR / "results.json"
    file_list = RESULTS_DIR / "files.txt"
    file_list.write_text("\n".join(files), encoding="utf-8")

    print(f"Licence scan: {len(files)} file(s) {'(all)' if args.all else '(changed vs HEAD)'}...")
    command = [str(scanner), "scan", "--obfuscate", "--all-extensions", "--quiet",
               "--files-from", str(file_list), "--output", str(results_file)]
    scan = subprocess.run(command, cwd=REPO_ROOT, capture_output=True, text=True)
    if scan.returncode != 0 or not results_file.is_file():
        print("Licence scan: SCANOSS failed (offline or API error?).")
        print(scan.stderr.strip())
        return 2

    results = json.loads(results_file.read_text(encoding="utf-8"))
    findings = []
    for path, matches in sorted(results.items()):
        for match in matches:
            if match.get("id") in (None, "none"):
                continue
            licences = [lic.get("name", "?") for lic in match.get("licenses", [])]
            findings.append((path, match, sorted(set(licences))))

    if not findings:
        print("Licence scan: clean, no unreviewed matches.")
        return 0

    print(f"\nLicence scan: {len(findings)} match(es) to review.\n")
    for path, match, licences in findings:
        copyleft = [lic for lic in licences if is_copyleft(lic)]
        tag = "COPYLEFT " if copyleft else ""
        print(f"  {tag}{path}  [{match.get('id')} {match.get('matched')}]")
        print(f"    our lines {match.get('lines')}  <->  {match.get('component')} {match.get('file')} lines {match.get('oss_lines')}")
        print(f"    {match.get('url')}")
        purls = match.get("purl") or ["?"]
        print(f"    purl {purls[0]}   licences: {', '.join(licences) or 'unknown'}")
    print("\nFor each match: compare both sides. A real copy is rewritten or attributed (and a")
    print("copyleft one is removed). Generic boilerplate gets a bom.remove entry in scanoss.json")
    print("with path, purl and a comment saying why it is not a copy.")
    print(f"Full results: {results_file}")
    return 1


if __name__ == "__main__":
    sys.exit(main())
