.PHONY: test fixture_check check_no_dynamic_memory check_no_hr_bpm

test: fixture_check check_no_dynamic_memory check_no_hr_bpm
	@echo "all checks passed"

fixture_check:
	@python3 -c "import csv;from pathlib import Path;p=Path('tests/fixtures/sample_ppg.csv');assert p.exists(),'missing tests/fixtures/sample_ppg.csv';rows=list(csv.DictReader(p.open(newline='',encoding='utf-8-sig')));assert rows,'empty csv';assert list(rows[0].keys())==['timestamp_ms','PPG_G1','PPG_G2','PPG_G3','PPG_G4'];ts=[int(r['timestamp_ms']) for r in rows];assert len(rows)==1001,len(rows);assert ts[0]==0,ts[0];assert ts[-1]==20000,ts[-1];assert all((b-a)==20 for a,b in zip(ts,ts[1:])), 'timestamp step not 20ms';print('fixture ok:',len(rows),'rows',ts[0],ts[-1])"

check_no_dynamic_memory:
	@if [ -d include ] || [ -d src ] || [ -d tests ]; then \
		if grep -R -n -E '\\b(malloc|calloc|realloc|free)[[:space:]]*\\(' include src tests 2>/dev/null; then \
			echo 'dynamic memory call found'; \
			exit 1; \
		fi; \
	fi

check_no_hr_bpm:
	@if [ -d include ] || [ -d src ] || [ -d tests ]; then \
		if grep -R -n 'hr_bpm' include src tests 2>/dev/null; then \
			echo 'forbidden hr_bpm field found'; \
			exit 1; \
		fi; \
	fi
