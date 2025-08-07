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
HEADER_LINES = 12  # Anzahl Zeilen im Headerblock

def generate_header(filename, lines, is_header_file, created_date=None):
    HEADER_WIDTH = 70
    LABEL_WIDTH = 14
    today = datetime.date.today().isoformat()
    created = created_date if created_date else today

    def format_header_line(label, content):
        label = label.rjust(LABEL_WIDTH) + ":"
        line = f"│ {label} {content}"
        return line.ljust(HEADER_WIDTH - 1) + "│"

    border_top = "┌" + "─" * (HEADER_WIDTH - 2) + "┐"
    border_bottom = "└" + "─" * (HEADER_WIDTH - 2) + "┘"

    description = generate_description(lines, is_header_file)

    lines = [
        format_header_line("File", os.path.basename(filename)),
        format_header_line("Description", description),
        format_header_line("Project", PROJECT_NAME),
        format_header_line("Author", AUTHOR),
        format_header_line("Created", created),
        format_header_line("Last Modified", today),
        format_header_line("License", "GNU GPL v3.0-only"),
    ]

    header_lines = [
        f"// {LICENSE}",
        "//",
        f"// {border_top}"
    ] + [f"// {line}" for line in lines] + [
        f"// {border_bottom}"
    ]

    return "\n".join(header_lines) + "\n"

def extract_class_name(lines, is_header_file):
    if is_header_file:
        # Suche nach Klassendefinition
        for line in lines:
            match = re.search(r"\bclass\s+(\w+)", line)
            if match:
                return match.group(1)
    else:
        # Suche nach Konstruktor: ClassName::ClassName(
        for line in lines:
            match = re.search(r"(\w+)::\1\s*\(", line)
            if match:
                return match.group(1)
    return None

def generate_description(lines, is_header_file):
    class_name = extract_class_name(lines, is_header_file)
    if class_name:
        if is_header_file:
            return f"definition of {class_name} class"
        else:
            return f"implementation of {class_name} class"
    return "[Beschreibung hier einfügen]"

def extract_created_date(lines):
    for line in lines[:HEADER_LINES]:
        match = re.search(r"Created:\s+(\d{4}-\d{2}-\d{2})", line)
        if match:
            return match.group(1)
    return None

def has_spdx_header(lines):
    return any(LICENSE in line for line in lines[:5])

def find_insertion_index(lines, is_header_file):
    for i, line in enumerate(lines):
        stripped = line.strip()
        if is_header_file:
            if stripped.startswith("#pragma once") or stripped.startswith("#ifndef"):
                return i
        else:
            if stripped.startswith("#include"):
                return i
    return 0  # Fallback: Anfang der Datei

def process_file(filepath):
    with open(filepath, "r", encoding="utf-8") as f:
        lines = f.readlines()

    is_header_file = filepath.endswith((".h", ".hpp"))
    # Finde Ende des alten Headers (SPDX + ASCII)
    start_content_index = find_insertion_index(lines, is_header_file)
    header_end_index = 0
    if start_content_index > 0:
        header_end_index = start_content_index - 1
    
    header_lines_old = [line for line in lines[:header_end_index] if line.strip() != ""]
    content_lines = lines[start_content_index:]

    if has_spdx_header(header_lines_old):
        print(f"Aktualisiere Header: {filepath}")
        created_date = extract_created_date(header_lines_old)
        new_header = generate_header(filepath, content_lines, is_header_file, created_date).splitlines(keepends=True)
        with open(filepath, "w", encoding="utf-8") as f:
            f.writelines(new_header + ["\n"] + content_lines)
    else:
        print(f"Füge neuen Header hinzu: {filepath}")
        new_header = generate_header(filepath, content_lines, is_header_file).splitlines(keepends=True)
        with open(filepath, "w", encoding="utf-8") as f:
            f.writelines(new_header + ["\n"] + content_lines)
    return None

def scan_directory(root_dir):
    for dirpath, _, filenames in os.walk(root_dir):
        for filename in filenames:
            if any(filename.endswith(ext) for ext in FILE_EXTENSIONS):
                process_file(os.path.join(dirpath, filename))

if __name__ == "__main__":
#    scan_directory(".")
    scan_directory(os.path.join(os.path.dirname(__file__), "..", "src"))

