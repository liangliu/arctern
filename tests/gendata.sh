#!/bin/bash

for i in {10..20}
do
    echo ${i}
done

exit 0



N=22
for i in $(seq $N)
do
    # python split_data.py data/gen.csv 12000 100000

    # rm -rf data/makevalid.csv
    # touch data/makevalid.csv
    # echo 'geos' >> data/makevalid.csv
    # cat data/gen.csv >> data/makevalid.csv

    # /usr/local/bin/spark/bin/spark-submit /home/liangliu/workspace/arctern/tests/spark_test.py 1

    # cp /tmp/results/test_makevalid/*.csv ./polygon.csv

    # sed -i 's/"//g' polygon.csv

    rm -rf data/single.csv
    touch data/single.csv
    echo 'geos' >> data/single.csv
    cat polygon${i}.csv >> data/single.csv

    /usr/local/bin/spark/bin/spark-submit /home/liangliu/workspace/arctern/tests/spark_test.py 1
    cp /tmp/results/test_isvalid/*.csv ./res${i}.csv
    
    # cp data/single.csv data/single5.csv
    # cp polygon${i}.csv data/single.csv

done



# /usr/local/bin/spark/bin/spark-submit /home/liangliu/workspace/arctern/tests/spark_test.py 1
