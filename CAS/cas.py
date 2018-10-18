

def trim(text,key=[" "]):
    output = ""
    for c in text:
        if not c in key:
            output += c
    return output

def parse(text):
    output = []
    Operators = ["+","-","*","/"]
    i = 0
    while i < len(text):
        # interpret numbers
        if text[i].isdigit():
            if len(output) > 0 and output[-1][0] == "num":
                output[-1][1] *= 10
                output[-1][1] += (int)(text[i])
            else:
                output.append(["num" , (int)(text[i])])
            i += 1
            continue
        # find single-character operators
        if text[i] in Operators:
            output.append(["op", text[i]])
            i += 1
            continue
        # enable brackets
        if text[i] == '(':
            bracket_end = text.find(')')
            output.append(["bra",parse(text[i+1:bracket_end])])
            i = bracket_end + 1
            continue
        # enable long variable names
        if len(output) > 0 and output[-1][0] == "var":
            output[-1][1] += text[i]
        output.append(["var",text[i]])
        i += 1
    return output

def reparse(Input):
    output = ""
    for element in Input:
        if element[0] == "bra":
            output += "( " + reparse(element[1]) + ") "
        else:
            output += (str)(element[1]) + " "
    return output

def calc(num1,op,num2):
    if op == "+":
        return num1 + num2
    if op == "-":
        return num1 - num2
    if op == "*":
        return num1 * num2
    if op == "/":
        return num1 / num2
    
def SetBrackets(Input):
    output = []
    for i in range(len(Input)):
        if Input[i][0] == "op":
            if Input[i][1] == "*" or Input[i][1] == "/":
                output.append(["bra", [Input[i-1],Input[i],Input[i+1] ]])
                continue
            if Input[i][1] == "+" or Input[i][1] == "-":
                output.append(Input[i-1])
                output.append(Input[i])
                continue
    if (not output[-1][1] == Input[-1][1]) and (not output[-1][0] == "bra"):
        output.append(Input[-1])
    return output
    
def Eval(Input):
    output = []
    change = False
    i = 0
    while i < len(Input):
        #print(Input[i],len(Input),len(Input[i]))
        if Input[i][0] == "bra":
            Input[i][1] = Eval(Input[i][1])
            if len(Input[i][1]) == 1:
                Input[i] = Input[i][1][0]
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
    test = "55 - 12 - 3 + 1 +1 - 2 + 2 * x - z"
    test = trim(test)
    parsed = parse(test)
    parsed = SetBrackets(parsed)
    solution = Eval(parsed)
    print(reparse(solution))
