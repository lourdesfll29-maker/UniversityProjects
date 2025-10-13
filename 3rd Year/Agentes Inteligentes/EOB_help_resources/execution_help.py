path1 = r"C:/Users/Lourd/Desktop/Uni/AIN/EntregaFinal/EOB_Entrega2/bdifieldop_EOB.asl"
path2 = r"C:/Users/Lourd/Desktop/Uni/AIN/EntregaFinal/EOB_Entrega2/bdimedic_EOB.asl"
path3 = r"C:/Users/Lourd/Desktop/Uni/AIN/EntregaFinal/EOB_Entrega2/bdisoldier_EOB.asl"
path4 = r"C:/Users/Lourd/Desktop/Uni/AIN/EntregaFinal/EOB_Entrega2/bdisniper_EOB.asl"
path5 = r"C:/Users/Lourd/Desktop/Uni/AIN/EntregaFinal/EOB_Entrega2/EOB_behavior.py"

with open(path5, encoding="utf-8") as f:
    lines = f.readlines()

for i, line in enumerate(lines, 1):
    # Convertir caracteres en valores ASCII antes de compararlos
    non_ascii_bytes = [ord(b) for b in line if ord(b) > 127]
    
    if non_ascii_bytes:
        print(f"Línea {i}: {line.strip()}")
        print(f"  Bytes no ASCII: {non_ascii_bytes}")

