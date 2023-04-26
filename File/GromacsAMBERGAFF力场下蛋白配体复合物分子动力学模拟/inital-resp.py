import sys

def remove_sep(line_list):
    return [element for element in line_list if element != ""]

def read_ligand(inital_mol2):

    with open(inital_mol2) as f:
        lines = f.readlines()
    
    ligand_atoms_coords = list()
    flag, index = 0, 0
    
    while index < len(lines):
        line = lines[index].strip()
        if flag == 1:
            if "@<TRIPOS>BOND" not in line:
                line_list = remove_sep(line.split(" "))
                print(line_list)
                ligand_atoms_coords.append([line_list[2], line_list[3], line_list[4]])
            else:
                break
        
        if "@<TRIPOS>ATOM" in line:
            flag = 1
        
        index += 1
    
    return ligand_atoms_coords

def inital_add_resp(inital_mol2, resp_mol2):
    inital_coords = read_ligand(inital_mol2)

    with open(resp_mol2) as f:
        lines = f.readlines()

    result = open("IPR25-RESP-inital.mol2", "w")

    flag, index = 0, 0

    while index < len(lines):
        line = lines[index]
        if flag == 1:
            if "@<TRIPOS>BOND" not in line:
                line_list = remove_sep(line.strip().split(" "))
                print(line_list)
                atom_id = '{:>7}'.format(line_list[0])
                atom = '{:<8}'.format(line_list[1])
                coord_x = '{:>11}'.format(inital_coords[index-start][0])
                coord_y = '{:>11}'.format(inital_coords[index-start][1])
                coord_z = '{:>11}'.format(inital_coords[index-start][2])
                type_atom = '{:<6}'.format(line_list[5])
                num = '{:>6}'.format(line_list[6])
                resname = '{:<9}'.format("IPR")
                charge = '{:>9}'.format(line_list[8])
                line = atom_id + " " + atom + coord_x + coord_y + coord_z + " " + type_atom + num + " " + resname + charge + "\n"
                result.write(line)
        elif flag == 2:
            if "@<TRIPOS>SUBSTRUCTURE" not in line:
                result.write(line)
            else:
                break
        elif flag == 3:
            if name_start == index:
                result.write("IPR\n")
            else:
                result.write(line)
        
        if "@<TRIPOS>ATOM" in line:
            flag = 1
            start = index + 1
        elif "@<TRIPOS>BOND" in line:
            result.write(line)
            flag = 2
        elif "@<TRIPOS>MOLECULE" in line:
            result.write(line)
            flag = 3
            name_start = index + 1        
    
        index += 1

def main():
    inital_mol2 = sys.argv[1]
    resp_mol2 = sys.argv[2]
    inital_add_resp(inital_mol2, resp_mol2)

if __name__ == "__main__":
    main()