CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Werror -pedantic
INCLUDES = -Iinclude -Isrc

.PHONY: all test clean check_fixture check_forbidden

all: test_api_compile

test_api_compile: src/ppg_ibi.c tests/test_api_compile.c include/ppg_ibi.h include/ppg_ibi_config.h src/ppg_ibi_internal.h
	$(CC) $(CFLAGS) $(INCLUDES) src/ppg_ibi.c tests/test_api_compile.c -o $@

test: test_api_compile
	./test_api_compile
	$(MAKE) check_fixture
	$(MAKE) check_forbidden

check_fixture:
	python3 -c "import csv;from pathlib import Path;p=Path('tests/fixtures/sample_ppg.csv');assert p.exists(),'missing tests/fixtures/sample_ppg.csv';rows=list(csv.DictReader(p.open(newline='',encoding='utf-8-sig')));assert rows,'empty csv';assert list(rows[0].keys())==['timestamp_ms','PPG_G1','PPG_G2','PPG_G3','PPG_G4'];ts=[int(r['timestamp_ms']) for r in rows];assert len(rows)==1001,len(rows);assert ts[0]==0,ts[0];assert ts[-1]==20000,ts[-1];assert all((b-a)==20 for a,b in zip(ts,ts[1:])),'timestamp step not 20ms';print('fixture ok:',len(rows),'rows',ts[0],ts[-1])"

check_forbidden:
	if rg -n -e '\b(malloc|calloc|realloc|free)[[:space:]]*\(' include src tests; then \
		echo 'dynamic memory call found'; \
		exit 1; \
	fi
	if rg -n 'hr_bpm' include src tests; then \
		echo 'forbidden hr_bpm field found'; \
		exit 1; \
	fi

clean:
	rm -f test_api_compile
