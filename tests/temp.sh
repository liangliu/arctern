# base_dir='/home/liangliu/workspace/arctern_perf_data'
base_dir='.'
start=27
end=35
echo $((start))
echo $((end))

for i in {61..70}
do
    cat $base_dir/polygon${i}.csv >> $base_dir/polygon.csv
done
