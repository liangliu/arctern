#!/bin/bash
base_dir='/home/liangliu/workspace/arctern_perf_data'
# for i in {0..22}
# do
#     cat ${base_dir}/polygon${i}.csv >> ${base_dir}/polygon.csv
# done
# exit 0



N=22
# for i in $(seq $N)
for i in {61..100}
do
    # python split_data.py gen.csv 12500 0

    # rm -rf data/makevalid.csv
    # touch data/makevalid.csv
    # echo 'geos' >> data/makevalid.csv
    # cat gen.csv >> data/makevalid.csv
    # /usr/local/bin/spark/bin/spark-submit /home/liangliu/workspace/arctern/tests/spark_test.py 1
    # cp /tmp/results/test_makevalid/*.csv ./polygon${i}.csv
    # sed -i 's/"//g' polygon${i}.csv
    # cp polygon${i}.csv ${base_dir}/polygon${i}.csv


    rm -rf data/single.csv
    touch data/single.csv
    echo 'geos' >> data/single.csv
    cat polygon${i}.csv >> data/single.csv

    /usr/local/bin/spark/bin/spark-submit /home/liangliu/workspace/arctern/tests/spark_test.py 1
    cp /tmp/results/test_isvalid/*.csv ./res${i}.csv
    
    # cp data/single.csv data/single5.csv
    # cp polygon${i}.csv data/single.csv

done

