def parsePart1(text):
    res = []
    splitted = text.split('\n')
    for line in splitted:
        sline = line.split(' ')
        parsedLine = []
        for el in sline:
            x = el.strip()
            if x:
                parsedLine.append(x)
            
        res.append(parsedLine)
    return res

def executeColumn(inp, n):
    i = 0
    nums = []
    x = inp[i][n]
    while x != '*' and x != '+':
        nums.append(int(x))
        i += 1
        x = inp[i][n]
        
    if x == '*':
        res = 1
        for n in nums:
            res *= n

        return res
            
    elif x == '+':
        return sum(nums)
        

def parsePart2(text):
    splitted = text.split('\n')

    # Supposing that all lines have the same length 
    # Last column starts with operation character
    nums = []
    operators = []
    for curr_col in range(len(splitted[0])):
        curr_num = ""
        for line in splitted:
            c = line[curr_col]
            if c.isdigit():
                curr_num += c
            elif c in ['*', '+']:
                operators.append(c)
        
        if curr_num:
            nums.append(int(curr_num))
        else:
            nums.append(" ")

    return nums, operators

def executePart1(parsed):
    res = 0
    for i in range(len(parsed[0])):
        res += executeColumn(parsed, i)
    return res

def execultePart2(nums, column_operators):
    result = 0
    collected = []
    i = 0
    for op in column_operators:
        collected = []
        while i < len(nums):
            num = nums[i]
            i += 1
            if num == " ":
                break
            else:
                collected.append(num)

        if op == '+':
            result += sum(collected)
        elif op == '*':
            tmp = 1
            for n in collected:
                tmp *= n
            result += tmp

    return result
        
        
file = open("input.txt")
inp = file.read()
file.close()

parsed_p1 = parsePart1(inp)
res_part_1 = executePart1(parsed_p1)
print(f"Res part 1: {res_part_1}")

nums, ops = parsePart2(inp)
res_part_2 = execultePart2(nums, ops)
print(f"Res part 2: {res_part_2}")

# That's quite stinky code >.<