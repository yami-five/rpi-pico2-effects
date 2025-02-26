bytes=[0,0,1,64]
result=""
for b in bytes:
    result+=f'{str(hex(b))[2:]} '
print(result)