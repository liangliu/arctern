import psutil
import time
import os
import shutil
import threading
from multiprocessing import Process
import sys

# encoding: utf-8
# log_dir = '/path/to/arctern_back/tests/process_info/'
# log_dir = '/arctern/tests/nasdata/arctern'
# log_dir = '/cifs/test/arctern'
log_dir = './'
spark = '/usr/local/bin/spark/bin/spark-submit'
times = 3
test_file = os.path.join(os.path.dirname(
    os.path.abspath(__file__)), 'spark_test.py')
# spark_cmd = "/path/to/spark-3.0.0-preview2/bin/spark-submit --master local /path/to/arctern_back/tests/spark_test.py"
spark_cmd = "%s %s %s" % (spark, test_file, str(times))
pname = 'python'
target = 'spark_test'


def submit_task():
    print(os.getpid())
    print(spark_cmd)
    os.system(spark_cmd)


def write_relationship(pid_exist):
    pid_to_write_relationship = []
    for i in pid_exist:
        pid_to_write_relationship.append(i)
    f = open(log_dir + 'relationship.txt', 'a')
    while p1.is_alive():
        for proc in psutil.process_iter():
            if proc.name().find(pname) >= 0 and proc.pid not in pid_to_write_relationship:
                print('-------------------- write_relationship: %s' %
                      str(proc.name()))
                f.write('进程ID:')
                f.write(str(proc.pid))
                f.write('\n父进程ID:')
                f.write(str(proc.ppid()))
                proc_father = psutil.Process(proc.ppid())
                f.write('\n父进程名称:')
                f.write(proc_father.name())
                f.write('\n子进程ID:')
                f.write(str(proc.children()))
                f.write('\n')
                pid_to_write_relationship.append(proc.pid)


def get_pid(pname, pid_exist):
    for proc in psutil.process_iter():
        if proc.name().find(pname) >= 0 and proc.pid not in pid_exist and is_target(proc, target):
            print('-------------------- get_pid: %s' % str(proc.cmdline()))
            file_name = log_dir + 'python_' + str(proc.pid) + '.txt'
            f = open(file_name, 'a')
            f.write(str(time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())))
            f.write('   ')
            f.write(str(proc.memory_info()))
            f.write('    ')
            f.write(str(proc.cpu_percent()))
            f.write('\n')
            proc_father = psutil.Process(proc.ppid())
            if proc_father.name() == 'java':
                file_name = log_dir + 'java_' + str(proc_father.pid) + '.txt'
                f = open(file_name, 'a')
                f.write(str(time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())))
                f.write('   ')
                f.write(str(proc_father.memory_info()))
                f.write('    ')
                f.write(str(proc_father.cpu_percent()))
                f.write('\n')


def is_target(proc, target):
    xs = proc.cmdline()
    flag = False
    for x in xs:
        if target in x:
            return True

    return flag


def write_process(pname, pid_exist):
    start_time = time.time()
    while p1.is_alive():
        time.sleep(1)
        get_pid(pname, pid_exist)
    end_time = time.time()
    print('totally cost', end_time-start_time)


p1 = threading.Thread(target=submit_task)

arr = []


def get_info(proc):
    pinfo = str(proc.memory_info()) + '  ' + \
        str(proc.cpu_percent() / psutil.cpu_count())

    return pinfo


def print_process(proc):
    while proc.is_running():
        print(proc.cmdline())
        xs = proc.children()
        print(get_info(proc))
        if len(xs) > 1:
            print(xs[1].cmdline())
            print(get_info(xs[1]))
            for xc in xs[1].children():
                print(get_info(xc))
        time.sleep(1)


if __name__ == "__main__":
    print(psutil.virtual_memory())
    target_proc = ''

    # p2 = threading.Thread(target=write_process, args=(p1,))
    # p2 = threading.Thread(target=print_process, args=(p1,))

    p1.start()
    time.sleep(3)
    for proc in psutil.process_iter():
        if is_target(proc, 'SparkSubmit'):
            target_proc = proc

    p2 = threading.Thread(target=print_process,
                          args=(target_proc,))

    p2.start()
    # time.sleep(4)
    # p2.start()

