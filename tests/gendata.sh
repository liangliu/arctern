#!/bin/bash

python split_data.py data/gen.csv 1000

rm -rf data/makevalid.csv
touch data/makevalid.csv
echo 'geos' >> data/makevalid.csv
cat data/gen.csv >> data/makevalid.csv

/usr/local/bin/spark/bin/spark-submit /home/liangliu/workspace/arctern/tests/spark_test.py 1

cp /tmp/results/test_makevalid/*.csv ./polygon.csv

sed -i 's/"//g' polygon.csv

rm -rf data/single.csv
touch data/single.csv
echo 'geos' >> data/single.csv
cat polygon.csv >> data/single.csv

/usr/local/bin/spark/bin/spark-submit /home/liangliu/workspace/arctern/tests/spark_test.py 1




