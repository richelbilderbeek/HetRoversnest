ls *.h > tmp.txt
sed 's/.h//' tmp.txt > tmp2.txt
sed 's/.*/if (s == "&") return Get&();/' tmp2.txt > tmp3.txt
sed 's/.*/#include "&"/' tmp.txt > tmp4.txt