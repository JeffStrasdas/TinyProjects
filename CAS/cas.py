
# removes any character that is in 'key' from 'text' and returns it
def trim(text,key=[" "]):
    output = ""
    for c in text:
        if not c in key:
            output += c
    return output

# parses a string into a custom data format which is an array of marked elements 
# e.g. "1+2" -> [["num",1],["op",'+'],["num",2]]
# currently used tags:
# "num" -> integer numbers
# "op"  -> mathematical operator
# "bra" -> brackets and what is inside them, e.g. "1+(2+3)" -> [["num",1],["op",'+'],["bra",[["num",2],["op",'+'],["num",3]]]]
# "var" -> variables
# "eq"  -> equal sign
    
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
        if text[i] == '=':
            output.append(["eq",text[i]])
        # enable long variable names
        if len(output) > 0 and output[-1][0] == "var":
            output[-1][1] += text[i]
        output.append(["var",text[i]])
        i += 1
    return output

# reads custom data format and returns a text
# e.g. [["num",1],["op",'*'],["num",1]] -> "1+1"
def reparse(Input):
    output = ""
    for element in Input:
        if element[0] == "bra":
            output += "( " + reparse(element[1]) + ") "
        else:
            output += (str)(element[1]) + " "
    return output

# evaluates operators and numbers
def calc(num1,op,num2):
    if op == "+":
        return num1 + num2
    if op == "-":
        return num1 - num2
    if op == "*":
        return num1 * num2
    if op == "/":
        return num1 / num2

# ensures proper order of operation by adding brackets for multiplication
# 1+2*3 -> 1+(2*3)
# input/output is in the form of an array
def SetBrackets(Input):
    output = []
    for i in range(len(Input)):
        if Input[i][0] == "op":
            if Input[i][1] == "*" or Input[i][1] == "/":
                # if there are many multiplications, put them in the same bracket instead of creating a new one
                # a*b*c -> (a*b*c)
                if len(output) == 0 or not output[-1][0] == "bra":
                    output.append(["bra", [Input[i-1],Input[i],Input[i+1] ]])
                else:
                    output[-1][1].append(Input[i])
                    output[-1][1].append(Input[i+1])
                continue
            # append addition/substraction directly
            if Input[i][1] == "+" or Input[i][1] == "-":
                if len(output) == 0 or not output[-1][0] == "bra":
                    output.append(Input[i-1])
                output.append(Input[i])
                continue
    # don't forget the last item
    if (not output[-1][1] == Input[-1][1]) and (not output[-1][0] == "bra"):
        output.append(Input[-1])
    return output

# tries to evaluate as much as possible by using calc whenever applicable
def Eval(Input):
    output = []
    change = False
    i = 0
    while i < len(Input):
        # recursively evaluate brackets
        if Input[i][0] == "bra":
            Input[i][1] = Eval(Input[i][1])
            if len(Input[i][1]) == 1:
                Input[i] = Input[i][1][0]
        # evaluate number,operator,number elements directly
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

# new data format, inspired by lisp
# ['type',[data1,data2,data3,...]]
# 1+2+3 -> ['+',1,2,3]
def new_parse(text,root = True, op = ['+','*','^'], special = ['(',')']):
    output = []
    if root:
        # classify the type of input
        type_index = max(text.find('='),text.find('<'),text.find('>'))
        if type_index > -1:
            output = [text[type_index]]
        else:
            output = []
        # parse the rest
        if type_index > -1:
            output.append(new_parse(text[:type_index],False))
            output.append(new_parse(text[type_index+1:],False))
        else:
            output = new_parse(text,False)
        return output
    else:
        # actually parse the rest

        # make a list of all occurences of the current operator in the string
        ind_op = [-1,text.find(op[0])]
        while ind_op[-1] > -1:
            ind_op.append(text.find(op[0],ind_op[-1]+1))
            
        # make a list of all special characters (which have a beginning and an end)
        ind_sp = [text.find(special[0])]
        while ind_sp[-1] > -1:
            ind_sp.append(text.find(special[1],ind_sp[-1]))
            if ind_sp[-1] == -1:
                print("uneven special operators...")
                ind_sp.pop()
                break
            ind_sp.append(text.find(special[0],ind_sp[-1]))
        # remove last -1 element
        ind_op.pop()
        ind_sp[-1] = len(text)
        
        # remove everything inbetween brackets
#        i = 0
#        k = 0
#        while i  < len(ind_sp)-1 and k < len(ind_op):
#            if ind_sp[i] < ind_op[k] and ind_op[k] < ind_sp[i+1]:
#                ind_op.pop(k)
#            elif ind_op[k] > ind_sp[i+1]:
#                i += 2
#            else:
#                k += 1
        
        special_index = 0
        # add all terms to output
        if len(ind_op) > 1:
            output.append([op[0]])
            i = 0
            while i  < len(ind_op)-1:
                # catch special op and put it in a nested list
                
                # if a bracket got skipped, increase index
                while special_index < len(ind_sp)-1 and ind_sp[special_index+1] < ind_op[i+1]:
                    special_index += 2    
                # check if operation is inside a bracket
                if ind_sp[special_index] < ind_op[i+1]:
                    output[-1].append(text[ind_op[i]+1:ind_sp[special_index]])
                    output[-1].append(new_parse(text[ind_sp[special_index]+1 : ind_sp[special_index+1]],False))
                    # find next valid i
                    while ind_op[i] < ind_sp[special_index+1] and i < len(ind_op) - 1:
                        i += 1
                    special_index += 2
                else:
                    output[-1].append(text[ind_op[i]+1:ind_op[i+1]])
                    i += 1
            if len(ind_sp) == 1 or ind_op[-1] > ind_sp[-2]:
                output[-1].append(text[ind_op[-1]+1:])
            elif ind_sp[-2] < len(text)-1:
                output[-1].append(text[ind_sp[-2]+1:])
        
        # done with current operation, remove it
        op = op[1:]
        
        
        
        # continue with other operators
        if len(op) > 0:
            if len(output) > 0:
                i = 1
                while i < len(output[-1]):
                    # skip lists as they are already parsed
                    if type(output[-1][i]) == str:
                        # remove zero-length entries
                        if len(output[-1][i]) > 0:
                            output[-1][i] = new_parse(output[-1][i],False,op)
                        else:
                            output[-1].pop(i)
                            i -= 1
                    else:
                        print(output)
                        # inject into neighbours
                        
                        #left
                        if i > 2 and type(output[-1][i-1]) == list:
                            output[-1][i-1].append(output[-1][i])
                            output[-1].pop(i)
                            i -= 1
                        # right
                        if i < len(output[-1])-1 and len(output[-1][i+1]) > 0 and output[-1][i+1][0] in op:
                            output[-1][i+1] = new_parse(output[-1][i+1],False,op)
                            output[-1][i+1].append(output[-1][i])
                            output[-1].pop(i)
                    i += 1
                    
            else:
                return new_parse(text,False,op)
        
        if len(output) == 0:
            return text
        return output[0]

# check if it's already a string, a list of length one or a list with at least three elements
# there shouldn't be any lists with two elements so that case is not dealt with
def new_reparse(Root):
    if type(Root) == list:
        if len(Root) >= 3:
            temp = ""
            for a in Root[1:-1]:
                if Root[0] == '*' and type(a) == list and a[0] == '+':
                    temp += "("
                    temp += new_reparse(a)
                    temp += ") * "
                else:
                    temp += new_reparse(a) + Root[0]
            temp += new_reparse(Root[-1])
            return temp
        else:
            return (str)(Root[0])
    else:
        return (str)(Root)


def Simplify(Root):
    isNumber = []    
    # find all numbers recursively
    for i in range(len(Root)):
        if type(Root[i]) == str:
            try:
                (float)(Root[i])
                isNumber.append(True)
            except:
                isNumber.append(False)
        else:
            Root[i] = Simplify(Root[i])
            if type(Root[i]) == str:
                try:
                    (float)(Root[i])
                    isNumber.append(True)
                except:
                    isNumber.append(False)
            else:
                isNumber.append(False)
    
    
    output = []
    # calculate numbers, leave variables and nested operations untouched
    temp = 0
    # calulate addition
    if Root[0] == '+':
        output = ['+']
        for i in range(1,len(Root)):
            if isNumber[i]:
                temp += (float)(Root[i])
            else:
                output.append(Root[i])
        output.append((str)(temp))
    # calulate multiplication
    elif Root[0] == '*':
        output = ['*']
        temp = 1
        for i in range(1,len(Root)):
            if isNumber[i]:
                temp *= (float)(Root[i])
            else:
                output.append(Root[i])
        output.append((str)(temp))
    # calulate power of
    elif Root[0] == '^':
        output = ['^']
        temp = None
        for i in range(1,len(Root)):
            if isNumber[i]:
                if temp == None:
                    temp = (float)(Root[i])
                else:
                    temp = temp ** (float)(Root[i])
            else:
                output.append(Root[i])
        output.append((str)(temp))
    # ignore any unknown operations or special symbols
    else:
        output = Root

    # only one element left, return single element
    if len(output) == 2:
        return (str)(output[1])
    return output


# Finds the path of all occurences of variable in Root
# example:
#        Find(new_parse(" 1 + x = 2 * x^3 "),'x')
# or
#        Find(['=', ['+', '1', 'x '], ['*', ' 2', ['^', 'x', '3']]])
# returns
#        [2, [2, [1]]]
def Find(Root,variable):
    path = []
    for i in range(len(Root)):
        if type(Root[i]) == list:
            temp = Find(Root[i],variable)
            if len(temp) > 0:
                path.append(i)
                path.append(temp)
#                for t in temp:
#                    path.append(t)
        elif type(Root[i]) == str:
            if Root[i] == variable:
#                if len(path) == 0:
#                    return [i]
                path.append(i)
#                return path      
    return path

def Solve(Root,variable,text=False):
    if type(Root) == str:
        Root = new_parse(Root)
        text = True
    # Find path of variable
    path = Find(Root,variable)
    # if the path is trivial, then there's nothing to solve
    if len(path) == 1:
        if text:
            return new_reparse(Root)
        else:
            return Root
    # make sure it deals with either an equation or an inequation
    equation = True
    if Root[0] == '=':
        equation = True
    elif Root[0] == '>' or Root[0] == '<':
        equation = False
    else:
        print("not an (in-)equation")
        return
    
    tempside  = []
    leftside  = Root[    path[0]]
    rightside = Root[3 - path[0]]
#    print(20*"=")
#    print(Root,variable)
#    print(path)
#    print(leftside,rightside)
#    print(20*"=")
    temp = path[1]
    
    # collect all terms which are to be moved to the right side
    for i in range(len(leftside)):
        if not i == temp[0]:
            tempside.append(leftside[i])
    
    # update both sides        
    leftside = leftside[temp[0]]
    # if rightside is an expression
    if len(rightside) >= 3:
        if tempside[0] == '+':
            # if rightside is not already adding things, then nest outermost operations inside an addition
            if not rightside[0] == '+':
                rightside = ['+',rightside]
            if len(tempside) == 2:
                rightside.append(['*','-1',tempside[1]])
            else:
                rightside.append(['*','-1',tempside])
        elif tempside[0] == '*':
            # if rightside is not already multiplying things, then nest outermost operations inside a multiplication
            if not rightside[0] == '*':
                rightside = ['*',rightside]
            if len(tempside) == 2:
                rightside.append(['^',tempside[1],'-1'])
            else:
                rightside.append(['^',tempside,'-1'])
    # if rightside is a number/variable
    else:
        if tempside[0] == '+':
            rightside = ['+',rightside]
            if len(tempside) == 2:
                rightside.append(['*','-1',tempside[1]])
            else:
                rightside.append(['*','-1',tempside])
        elif tempside[0] == '*':
            rightside = ['*',rightside]
            if tempside[0] == '*':
                if len(tempside) == 2:
                    rightside.append(['^',tempside[1],'-1'])
                else:
                    rightside.append(['^',tempside,'-1'])
        
    return Solve([Root[0],leftside,Simplify(rightside)],variable,text)
    
def main():
    test = "55 - 12 - 3 + 1 +1 - 2 + 2 * x - z"
    test = trim(test)
    parsed = parse(test)
    parsed = SetBrackets(parsed)
    solution = Eval(parsed)
    print(reparse(solution))
