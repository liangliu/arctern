# 环境准备
conda https://github.com/liangliu/arctern/blob/conda/doc/compile-with-conda.md
python 安装python3.7以上的版本即可
spark 安装3.0.0-preview2版本

# 测试目录的结构
data 存放测试使用的原始数据，基本上根据函数名一一对应，具体看配置文件
expected 存放的测试的基线数据，大部分来自于postgis的结果，少部分是我们直接以arctern之前的结果作为expected
以上2个目录中的文件均为csv格式

config.yml 存放测试和结果比对的配置信息
collect_results.py 将原始的spark的结果初步处理，作为本次arctern测试运行的结果
compare.py 比较arctern的结果与expected
draw_map_test.py 图片对比功能的测试脚本
postgis_data.py 获取函数在postgis中的执行结果（不包括所有sql语句）
spark_test.py 回归测试主程序脚本，通过spark-submit指定运行
util.py 测试中所用到的通用方法的集合
wkt_to_geojson.py 将wkt格式转换为geojson格式的小脚本

# 执行arctern的测试
我们是通过spark-submit来提交测试，如果已经在上一步的环境准备中安装了spark，你应该清楚spark-submit的位置

`/usr/local/bin/spark/bin/spark-submit ./spark_test.py`

通过以上命令来运行所有的测试，如果想要定制运行的测试，请修改./spark_test.py中的main函数部分

# 比较arctern和postgis的结果
在上一步中，运行spark-submit命令得到的结果只是arctern测试的初步结果，如果需要和postgis的结果进行比对，还需要进一步的处理

1，需要将初始结果进行改动  
2，需要规范初始结果中不合规范的数据格式  
3，执行compare.py中的compare_all()方法  

## 如果两边结果不相等
比较的方法
1，对于常规的图形，我们


# 如何增加测试用例
增加用例的步骤比较繁琐，请一定按步骤操作  
注意：如果只是为已经编写好的测试函数增加测试数据，可以从步骤3开始

1，在./spark_test.py中编写测试函数（命名请按照run_test_xxxx的格式），并在main中增加对函数的调用  
2，在config.yml文件中增加相应的配置，其中  
   case.name 测试函数的名称，严格一致，全局唯一  
   case.spark_result_name 测试函数中table的名称，严格一致，全局唯一  
   case.expeceted_name postgis sql的执行结果存放的文件名称  

3，在./data中为arctern测试增加数据，目前支持的数据格式为csv
4，将你的测试函数和数据转换成postgis可以识别的sql语句，并存放在./expected/sqls中，参考https://github.com/liangliu/arctern/blob/conda/tests/expected/sqls/st_crosses.sql  
5，在postgis中执行上述sql时，请用当前测试目录的绝对路径替换上述sql文件中的`@path@`  
6，在postgis中执行上述sql，结果已经生成在./expected/results中，去除结果中的前面2行以及最后的1-2行，确保结果文件中只有测试的结果


# 性能回归测试
总体与功能回归测试区别不大，主要如下

1，测试结果不再落盘，规范化，以及与postgis比较
2，每个gis函数会跑3次，取平均结果
3，性能测试