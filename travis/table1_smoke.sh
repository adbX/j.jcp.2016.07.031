echo "AIMGM" > table1_smoke.csv

../example1 -l 9 -a 0.1666667 -N 8 2>/dev/null >> table1_smoke.csv

../example1 -l 9 -a 0.5 -N 32 2>/dev/null >> table1_smoke.csv

../example1 -l 9 -a 0.99 -N 100 2>/dev/null >> table1_smoke.csv

echo "BD-ADI" >> table1_smoke.csv

../example1_BDADI -l 9 -a 0.1666667 -N 8 2>/dev/null >> table1_smoke.csv

../example1_BDADI -l 9 -a 0.5 -N 32 2>/dev/null >> table1_smoke.csv

../example1_BDADI -l 9 -a 0.99 -N 100 2>/dev/null >> table1_smoke.csv
