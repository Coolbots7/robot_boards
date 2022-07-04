common_resistances = [10,12,15,18,22,27,33,39,47,56,68,82]

Vout = 5
tollerance = 0.1

# for each common resistance
# calculate r1 using the common resistance as r2
# print r1

print "Vout = " + str(Vout)
print "Tollerance: " + str(tollerance)
for r2 in common_resistances:
    #print resistance
    r1 = r2 * ((Vout/0.8)-1)
    
    r1_low = r1 * (1-tollerance)
    r1_high = r1 * (1+tollerance)
    for res in common_resistances:
        if(res >= r1_low and res <= r1_high):
            #print "R1: " + str(r1) + " (" + str(res) + ") [" +  str(abs(res-r1)/res*100) + "%]" + "\t\tR2: " + str(r2)
            print "R2: " + str(r2) + "\t   R1: " + str(r1) + " (" + str(res) + ") [" +  str(abs(r1-res)/r1*100) + "%]\t\t\tVout: " + "{:.2f}".format((4.0*(res+r2))/(5.0*r2)) + "V"
            break