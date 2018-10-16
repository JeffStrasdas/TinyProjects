

def trim(text,key=[" "]):
    output = ""
    for c in text:
        if not c in key:
            output += c
    return output

def parse(text):
    output = []
    Operators = ["+","-"]
    
    for c in text:
        # interpret numbers
        if c.isdigit():
            if len(output) > 0 and output[-1][0] == "num":
                output[-1][1] *= 10
                output[-1][1] += (int)(c)
            else:
                output.append(["num" , (int)(c)])
            continue
        # find single-character operators
        if c in Operators:
            output.append(["op", c])
            continue
        # enable brackets
        if c == '(' or c == ')':
            output.append("bra",c)
            continue
        # enable long variable names
        if output[-1][0] == "var":
            output[-1][1] += c
        output.append(["var",c])
        
    return output

def reparse(Input):
    output = ""
    for element in Input:
        output += (str)(element[1]) + " "
    return output

def calc(num1,op,num2):
    if op == "+":
        return num1 + num2
    if op == "-":
        return num1 - num2

def Eval(Input):
    output = []
    change = False
    i = 0
    while i < len(Input):
        if i+2 < len(Input):
            if Input[i][0] == "num" and Input[i+1][0] == "op" and Input[i+2][0] == "num":
                output.append(["num",calc(Input[i][1],Input[i+1][1],Input[i+2][1])])
                change = True
                i += 3
                continue
        output.append(Input[i])
        i += 1
    if change:
        return Eval(output)
    else:
        return Input

def main():
    test = "55 + 12 - 3 + 1 +1 - 2 + x"
    test = trim(test)
    parsed = parse(test)
    solution = Eval(parsed)
    print(solution)
