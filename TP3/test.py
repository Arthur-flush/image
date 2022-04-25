x = 0
y = 1
print("(0, 0)")
while (x != 8) and (y != 8):
    while True:
        print(f"({x}, {y}),")
        if x + 1 >= 8:
            y += 1
            break
        
        if y - 1 < 0:
            x += 1
            break
        
        x += 1
        y -= 1
    
    while True:
        print(f"({x}, {y}),")
        if x - 1 < 0:
            y += 1
            break
        
        if y + 1 >= 8: 
            x += 1
            break
        
        x -= 1
        y += 1
    
