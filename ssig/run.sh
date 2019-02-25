for file in /home/josue/Documents/dataset/$1/*; do
    echo "Reading file ${file}";
    ./ssig ${file} $1 >> $1.csv
done
