import os
import sys


base_dir = './'


def detect(resource, result):
    print(resource)
    with open(resource, 'r') as f:
        rs_lines = f.readlines()

    with open(result, 'r') as f:
        rz_lines = f.readlines()

    for rs, rz in zip(rs_lines, rz_lines):
        if rz.strip().lower() == 'false':
            print(rs.strip())
            rs_lines.remove(rs)

    with open(resource, 'w') as f:
        for rs in rs_lines:
            f.writelines(rs)


def do_update(start, end):
    for i in range(start, end):
        rs_name = 'polygon%s.csv' % str(i)
        rz_name = 'res%s.csv' % str(i)
        rs_path = os.path.join(base_dir, rs_name)
        rz_path = os.path.join(base_dir, rz_name)
        detect(rs_path, rz_path)


start = sys.argv[1]
end = sys.argv[2]

if __name__ == '__main__':
    do_update(int(start), int(end))

