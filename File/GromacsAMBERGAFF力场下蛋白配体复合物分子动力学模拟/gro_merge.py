import sys

def remove_sep(line_list):
    return [element for element in line_list if element != ""]

def read_pro(gro_protein):
    with open(gro_protein) as f:
        f1 = f.readlines()
    return f1

def merge(gro_protein, gro_ligand):
    pro_lst = read_pro(gro_protein)
    lig_lst = read_pro(gro_ligand)
    total_atom = int(pro_lst[1].strip()) + int(lig_lst[1].strip())
    pro_box = remove_sep(pro_lst[-1].strip().split())
    lig_box = remove_sep(lig_lst[-1].strip().split())
    print(pro_box)
    new_box = []
    for i in range(3):
        new_box.append(str(round(float(pro_box[i])+float(lig_box[i]), 5)))
    result = open("complex.gro","w")
    for i in range(len(pro_lst)-1):
        if i == 1:
            result.write(" " + str(total_atom) + "\n")
        else:
            result.write(pro_lst[i])
    last_resnum = int(remove_sep(pro_lst[-2].strip().split())[0][:3])
    last_atomnum = int(remove_sep(pro_lst[-2].strip().split())[2])
    resn_id = last_resnum + 1
    atom_id = last_atomnum + 1
    for i in range(2, len(lig_lst)-1):
        resn_id = '{:>5}'.format(resn_id)
        resn_name = remove_sep(lig_lst[i].strip().split())[1]
        resn = '{:<5}'.format(resn_name)
        atom = '{:>5}'.format(remove_sep(lig_lst[i].strip().split())[2])
        atom_id = '{:>5}'.format(atom_id)
        atom_x = '{:>8}'.format(remove_sep(lig_lst[i].strip().split())[4])
        atom_y = '{:>8}'.format(remove_sep(lig_lst[i].strip().split())[5])
        atom_z = '{:>8}'.format(remove_sep(lig_lst[i].strip().split())[6])
        line = resn_id + resn + atom + atom_id + atom_x + atom_y + atom_z + "\n"
        result.write(line)
        atom_id = int(atom_id) + 1
        print(resn_name)
        print(remove_sep(lig_lst[min(i+1, len(lig_lst)-2)].strip().split())[1])
        if remove_sep(lig_lst[min(i+1, len(lig_lst)-2)].strip().split())[1] != resn_name:
            resn_id += 1
    line = '{:>10}'.format(new_box[0]) + '{:>10}'.format(new_box[1]) + '{:>10}'.format(new_box[2]) + "\n"
    result.write(line)

def main():
    gro_protein = sys.argv[1]
    gro_ligand = sys.argv[2]
    merge(gro_protein, gro_ligand)

if __name__ == "__main__":
    main()