i=0
while true; do
  python input_gen.py > in.txt
  ./a.out < in.txt > out1.txt
  python isvalid.py < out1.txt > out2.txt
  # python A.py < in.txt > out2.txt
  # if ! diff out1.txt out2.txt; then
  #   echo "found"
  #   break
  # fi
  if ! -s out2.txt; then
    echo "found"
     break
  fi
  if ((i % 100 == 0)); then
    echo "#Case $i"
  fi  
  ((i++))
done