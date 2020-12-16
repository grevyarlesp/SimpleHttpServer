with open('mimelist') as openfileobject:
    for line in openfileobject:
        test = line.split()
        print('{\"' + test[0] + '\", \"' + test[-1] + '\"},')
        
