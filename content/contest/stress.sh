# Compare sol vs brute on random tests from gen (all compiled).
# gen takes a seed: ./gen 17 > in
for ((i = 1; ; i++)); do
	./gen $i > in; ./sol < in > o1; ./brute < in > o2
	if ! cmp -s o1 o2; then
		echo "Failed on seed $i"; cat in; break
	fi
	(( i % 100 == 0 )) && echo "$i ok"
done
