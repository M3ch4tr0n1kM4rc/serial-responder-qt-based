# SPDX-License-Identifier: GPL-3.0-only
#
# ┌────────────────────────────────────────────────────────────┐
# │          File: update_file_header.py                       │
# │   Description: Fügt Header in C++-Dateien ein oder         |
# |                aktualisiert sie                            │
# │       Project: serial-responder-qt-based                   │
# │        Author: M3ch4tr0n1kM4rc                             │
# │       Created: 2025-08-07                                  │
# │ Last Modified: 2025-08-07                                  │
# │       License: GNU GPL v3.0-only                           │
# └────────────────────────────────────────────────────────────┘

import os
import datetime
import re

# 🔧 Konfiguration
PROJECT_NAME = "serial-responder-qt-based"
AUTHOR = "M3ch4tr0n1kM4rc"
LICENSE = "SPDX-License-Identifier: GPL-3.0-only"
FILE_EXTENSIONS = [".cpp", ".h"]
HEADER_START = "// " + LICENSE
HEADER_LINES = 10  # Anzahl Zeilen im Headerblock

def generate_header(filename, created_date=None):
    today = datetime.date.today().isoformat()
    created = created_date if created_date else today
    return f"""// {LICENSE}
//
// ┌────────────────────────────────────────────────────────────┐
// │ File: {os.path.basename(filename):<47}│
// │ Description: [Beschreibung hier einfügen]                 │
// │ Project: {PROJECT_NAME:<39}│
// │ Author: {AUTHOR:<41}│
// │ Created: {created:<39}│
// │ Last Modified: {today:<35}│
// │ License: GNU GPL v3.0-only                                 │
// └────────────────────────────────────────────────────────────┘
"""

def extract_created_date(lines):
    for line in lines[:HEADER_LINES]:
        match = re.search(r"Created:\s+(\d{4}-\d{2}-\d{2})", line)
        if match:
            return match.group(1)
    return None

def has_spdx_header(lines):
    return any(LICENSE in line for line in lines[:5])

def process_file(filepath):
    with open(filepath, "r", encoding="utf-8") as f:
        lines = f.readlines()

    if has_spdx_header(lines):
        print(f"Aktualisiere Header: {filepath}")
        created_date = extract_created_date(lines)
        new_header = generate_header(filepath, created_date).splitlines(keepends=True)
        rest_of_file = lines[HEADER_LINES:]  # Entferne alten Header
        with open(filepath, "w", encoding="utf-8") as f:
            f.writelines(new_header + ["\n"] + rest_of_file)
    else:
        print(f"Füge neuen Header hinzu: {filepath}")
        new_header = generate_header(filepath).splitlines(keepends=True)
        with open(filepath, "w", encoding="utf-8") as f:
            f.writelines(new_header + ["\n"] + lines)

def scan_directory(root_dir):
    for dirpath, _, filenames in os.walk(root_dir):
        for filename in filenames:
            if any(filename.endswith(ext) for ext in FILE_EXTENSIONS):
                process_file(os.path.join(dirpath, filename))

if __name__ == "__main__":
    scan_directory(".")
