#!/usr/bin/env python

import os
import sys
import argparse
from struct import unpack_from
from ctypes import sizeof, c_double, c_int32, c_uint32, c_char
import numpy as np


class StressBinParser:
    def __init__(self, input_dir):
        self.input_dir = input_dir

    def read_bindata(self):
        fname = os.path.join(self.input_dir, "local_stress.bin")
        if not os.path.exists(fname):
            print("Please specify directory where local_stress.bin exists.")
            sys.exit()

        with open(fname, "rb") as f:
            self.sim_dim = int(unpack_from('<I', f.read(sizeof(c_uint32)))[0])
            vec_format = '<' + 'd' * self.sim_dim
            self.box_low = unpack_from(vec_format, f.read(self.sim_dim * sizeof(c_double)))
            self.box_len = unpack_from(vec_format, f.read(self.sim_dim * sizeof(c_double)))
            self.mesh_dim = unpack_from('<' + 'i' * self.sim_dim,
                                        f.read(self.sim_dim * sizeof(c_int32)))
            self.num_itypes = int(unpack_from('<I', f.read(sizeof(c_uint32)))[0])
            number_of_cells = reduce(lambda x, y: x*y, self.mesh_dim)

            tot_virial = np.zeros((number_of_cells, self.sim_dim*self.sim_dim))
            self.virial = {}
            for i in range(self.num_itypes):
                itype_name_len = int(unpack_from('<I',
                                                 f.read(sizeof(c_uint32)))[0])
                itype = unpack_from('<' + str(itype_name_len) + 's',
                                          f.read(sizeof(c_char) * (itype_name_len)))[0]
                tensor_dim = self.sim_dim * self.sim_dim
                tot_elem = number_of_cells * tensor_dim
                virial_raw = np.fromfile(f, dtype='<d', count=tot_elem)
                virial_raw = np.reshape(virial_raw,
                                        (virial_raw.size / tensor_dim, tensor_dim))
                tot_virial   += virial_raw
                self.virial[itype] = virial_raw
            self.virial["total"] = tot_virial


def save_stress(sbparser):
    mdim = sbparser.mesh_dim

    cell_len = [l / d for (l, d) in zip(sbparser.box_len, mdim)]
    cell_vol = reduce(lambda x,y: x*y, cell_len)
    number_of_cells = reduce(lambda x, y: x*y, mdim)
    if sbparser.sim_dim == 3:
        cell_pos = np.array(map(lambda x: [x % mdim[0], (x / mdim[0]) % mdim[1], x / (mdim[0] * mdim[1])],
                            range(number_of_cells)), dtype=np.float)
        description = np.array(["#X", "Y", "Z",
                                "sxx", "sxy", "sxz",
                                "syx", "syy", "syz",
                                "szx", "szy", "szz"])
        description.shape = (1, 12)
    else:
        cell_pos = np.array(map(lambda x: [x % mdim[0], (x / mdim[0]) % mdim[1]],
                            range(number_of_cells)), dtype=np.float)
        description = np.array(["#X", "Y",
                                "sxx", "sxy",
                                "syx", "syy"])
        description.shape = (1, 6)
    cell_pos += 0.5
    cell_pos *= cell_len

    for (name, vir) in sbparser.virial.items():
        out_fname = name + ".txt"
        out_path = os.path.join(sbparser.input_dir, out_fname)
        np.savetxt(out_path, description, fmt="%s", delimiter="\t")
        stress = vir / cell_vol
        stress_pos = np.hstack((cell_pos, stress))
        np.savetxt(open(out_path, 'a'), stress_pos, delimiter=" ")


def main(input_dir):
    sbparser = StressBinParser(input_dir)
    sbparser.read_bindata()
    save_stress(sbparser)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Post processing script for MDLScalculator")
    parser.add_argument('-i', '--input', dest='input_dir', required=True, help='input directory name.')
    args = parser.parse_args()
    main(args.input_dir)
