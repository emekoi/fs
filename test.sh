mkdir example
for suffix in {1..16}
do
	echo "I said \"Hello World\" to person #$suffix" > "example/hello$suffix.txt"
done

tar -cvf example.tar example
rm -rf example
