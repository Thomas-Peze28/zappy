#!/usr/bin/env python3

from pathlib import Path
import sys

_current_dir = Path(__file__).resolve().parent
_candidate_paths = [
	_current_dir / "AI",
	_current_dir.parent,
	_current_dir.parent / "AI",
]

for _candidate in _candidate_paths:
	if (_candidate / "src" / "main.py").is_file():
		sys.path.insert(0, str(_candidate))
		break

from src.main import main


if __name__ == "__main__":
	main()
