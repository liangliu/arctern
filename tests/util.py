import sys
import os
from yaml import full_load
from shapely import wkt
# from osgeo import ogr
from ogr import *


EPOCH = 1e-6

geo_types = ['POLYGON', 'POINT', 'LINESTRING']
geo_collection_types = [
    'MULTIPOLYGON', 'MULTIPOINT', 'MULTILINESTRING', 'GEOMETRYCOLLECTION'
]


def is_empty(geo):
    geo = geo.strip().upper()
    return geo.endswith('EMPTY')

def is_point(geo):
    geo = geo.strip().upper()
    return geo.startswith('POINT')


def is_linestring(geo):
    geo = geo.strip().upper()
    return geo.startswith('LINESTRING')


def is_polygon(geo):
    geo = geo.strip().upper()
    return geo.startswith('POLYGON')


def is_geometry(geo):
    geo = geo.strip().upper()

    for x in geo_types:
        if geo.startswith(x) and len(geo) != len(x):
            return True
        
        continue

    return False


def is_geometrycollection(geo):
    geo = geo.strip().upper()

    for x in geo_collection_types:
        if geo.startswith(x):
            return True
        
        continue

    return False


def line_to_points(geom):
    if geom.strip().upper().startswith('LINESTRING'):
        points = geom.split('(')[1].split(')')[0]
        arr = points.split(',')
        xs = ['POINT (%s)' % x.strip() for x in arr]
        return xs


def polygon_to_points(geom):
    if geom.strip().upper().startswith('POLYGON'):
        points = geom.split('((')[1].split('))')[0]
        arr = points.split(',')
        xs = ['POINT (%s)' % x.strip().replace('(', '').replace(')', '') for x in arr]
        return xs


def geometrycollection_tolist(geom):
    if is_geometrycollection(geom):
        gc = wkt.loads(geom)
        return [x.to_wkt() for x in list(gc)]


def point_distance(geox, geoy):
    if is_geometry(geoy) and geox.strip().upper().startswith('POINT'):
        p = wkt.loads(geox)
        g = wkt.loads(geoy)
        return p.distance(g)


def linestring_distance(geox, geoy):
    if is_geometry(geoy) and geox.strip().upper().startswith('LINESTRING'):
        xs = line_to_points(geox)
        distance_arr = [point_distance(x, geoy) for x in xs]

        return max(distance_arr)


def polygon_distance(geox, geoy):
    if is_geometry(geoy) and geox.strip().upper().startswith('POLYGON'):
        xs = polygon_to_points(geox)
        distance_arr = [point_distance(x, geoy) for x in xs]

        return max(distance_arr)


def geometry_distance(geox, geoy):
    if is_point(geox):
        d = point_distance(geox, geoy)

    if is_linestring(geox):
        d = linestring_distance(geox, geoy)

    if is_polygon(geox):
        d = polygon_distance(geox, geoy)

    return d


def arc_distance(geox, geoy):
    if is_empty(geox) or is_empty(geoy):
        return 0.0

    if is_geometrycollection(geox) and is_geometrycollection(geoy):
        gcx = geometrycollection_tolist(geox)
        gcy = geometrycollection_tolist(geoy)

        arr = []
        for gx in gcx:
            distance_arr = [geometry_distance(gx, gy) for gy in gcy]
            arr.append(min(distance_arr))

        return max(arr)

    if is_geometry(geox) and is_geometrycollection(geoy):
        gc = geometrycollection_tolist(geoy)
        distance_arr = [geometry_distance(geox, x) for x in gc]

        return max(distance_arr)

    if is_geometry(geoy) and is_geometrycollection(geox):
        return arc_distance(geoy, geox)

    if is_geometry(geox) and is_geometry(geoy):
        return geometry_distance(geox, geoy)


def get_tests():
    tests = get_config()['tests']
    names = [x['case']['name'] for x in tests]
    table_names = [x['case']['spark_result_name'] for x in tests]
    expects = [x['case']['expected_name'] for x in tests]

    return names, table_names, expects


def get_config():
    cwd = os.path.dirname(os.path.abspath(__file__))
    config_path = os.path.join(cwd, 'config.yml')

    with open(config_path, 'r') as f:
        content = f.read()

    return full_load(content)


config = get_config()
spark_result = config['path']['spark_result']
arctern_result = config['path']['arctern_result']
expected_result = config['path']['expected_result']

# if __name__ == '__main__':
#     config = get_config()
