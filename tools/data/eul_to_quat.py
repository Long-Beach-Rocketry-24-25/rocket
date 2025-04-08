from scipy.spatial.transform import Rotation

import sys

if __name__ == "__main__":
    
    if len(sys.argv) < 2:
        print("Please provide a file name in the first command line argument.")
    else:

        fname = sys.argv[1]

        with open(fname, "r") as r:
            data = [x.strip() for x in r.readlines()]

        for i in data:
            eul = [int(x) for x in i.split(",")[-3:]]
            rot = Rotation.from_euler('xyz', eul, degrees=True).as_quat()
            quat = list(rot[3:]) + list(rot[:3])
            print(','.join(i.split(",")[:-3] +[str(round(x, 2)) for x in quat]))
