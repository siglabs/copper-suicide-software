import re
import sys
import csv

def parse_file(filename):

    first = '<<< Wire List >>>'

    file = open(filename, 'r')

    res = []

    skipping = True

    for i, line in enumerate(file):
        if first in line:
            skipping = False

        if not skipping:
            res.append(line)

    assert not skipping

    return res

##! Breaks a space separated line into it's 5 parts
def break_line(line):
    m = re.search(' *([^ ]*) *([^ ]*) *([^ ]*) *([^ ]*) *([^ ]*)', line)

    res = []

    for i in range(1,6):
         res.append(m.group(i).rstrip().lstrip())

    return res


##! Takes all lines in the netlist file
# Breaks into a list where the first element is the header name
# and the next element is a list of lines that have been parsed
def parse_sections(lines):
    # print "got", len(lines), "lines"


    output = []

    last_header = None
    cur_lines = []


    for i, line in enumerate(lines):

        # if we have a section
        if line.startswith('['):

            # if we are starting a new section
            if last_header is not None:
                # Save output section
                output.append([last_header, cur_lines])

                # Reset to initial conditions
                last_header = None
                cur_lines = []


            # search for ] followed by a space and grab everything after that
            m = re.search('] (.*)', line)

            last_header = m.group(1).rstrip()
        else:
            # if we are under a header
            if last_header is not None:
                # If the line isn't blank
                if line.rstrip() != '':
                    # Add it
                    cur_lines.append(break_line(line))

    # The loop above will skip the final section, so grab it here
    if last_header is not None:
        # Save output section
        output.append([last_header, cur_lines])


    return output

def chip_to_chip(sections, harness_name, saving_file):
   	
    length =len(sections)
    trace_names=[]
    subconnects=[]

    FPGA_Ball_Names_A=[]
    FPGA_Ball_Names_B=[]

    for i in range(0,length):
        if harness_name in sections[i][0]:
            #print (sections[i])
            trace_names.append(sections[i][0])
            subconnects.append(sections[i][1])
    
    print ("subconnects:", subconnects)
    print ("trace_names:", trace_names)
    #print ("Length of trace:" , len(trace_names))

    y=len(trace_names)
    
    for z in range(0,y):
        if harness_name+'TB.' in trace_names[z]:
            FPGA_Ball_Names_A.append(subconnects[z][0][1])
            FPGA_Ball_Names_B.append(subconnects[z][1][1])

        if harness_name+'LR.' in trace_names[z]:
            FPGA_Ball_Names_A.append(subconnects[z][0][1])
            FPGA_Ball_Names_B.append(subconnects[z][1][1])
                
    result=[]
    for z in range (0,y):
        result.append(str(trace_names[z]) + ',' + str(FPGA_Ball_Names_A[z]) + ',' + str(FPGA_Ball_Names_B[z]))
        print (result[z].split(","))

    fp=open(str(saving_file)+'.txt',"w")
    '''
    fp.write('Trace_Name' + ',' + 'FPGA_Ball_Names_A' + ',' + 'FPGA_Ball_Names_B' + '\n')
    for z in range (0,y):
        fp.write(result[z])
        fp.write('\n')
    '''
    fp.write('----------Writting .LPF File : FPGA A -----------' + '\n')
    fp.write('DEFINE PORT GROUP "IL68_GRP" "IL68[*]";' + '\n')
    fp.write('IOBUF GROUP "IL68_GRP" IO_TYPE=LVCMOS15;' + '\n')
    for z in range (0,y):
        fp.write('LOCATE COMP IL68['+ str(z) + '] SITE ' + '"' + FPGA_Ball_Names_A[z] + '"' + ';' + '\n')

   
    fp.write('----------Writting .LPF File : FPGA B-----------' + '\n')
    fp.write('DEFINE PORT GROUP "IL07_GRP" "IL07[*]";' + '\n')
    fp.write('IOBUF GROUP "IL07_GRP" IO_TYPE=LVCMOS15;' + '\n')    
    for z in range (0,y):
        fp.write('LOCATE COMP IL07['+ str(z) + '] SITE ' + '"' + FPGA_Ball_Names_B[z] + '"' + ';' + '\n')     

    fp.close()

    with open(str(saving_file)+'.csv','w',newline='') as csvfile:
        file_write=csv.writer(csvfile,delimiter=',')
        file_write.writerow(['Trace_Name','FPGA_Ball_Names_A','FPGA_Ball_Names_B'])
        for z in range(0,y):
            file_write.writerow(result[z].split(",")) 

def sdram(sections, harness_name, saving_file):
    
    length =len(sections)
    trace_names=[]
    subconnects=[]
    FPGA_Ball_Names=[]
    SDRAM_Pin_Names=[]
    SDRAM_Pin_Numbers=[]
    
    for i in range(0,length):
        if harness_name in sections[i][0]:
            print (sections[i])
            trace_names.append(sections[i][0])
            subconnects.append(sections[i][1])
    
    print (subconnects)
    print (trace_names)
    
    y=len(trace_names)
    for z in range(0,y):
        if harness_name+'TB.' in trace_names[z]:
            FPGA_Ball_Names.append(subconnects[z][1][1])
            SDRAM_Pin_Names.append(subconnects[z][2][2])
            SDRAM_Pin_Numbers.append(subconnects[z][2][1])

        if harness_name+'LR.' in trace_names[z]:
            FPGA_Ball_Names.append(subconnects[z][0][1])
            SDRAM_Pin_Names.append(subconnects[z][1][2])
            SDRAM_Pin_Numbers.append(subconnects[z][1][1])

        if '.D4_N' in trace_names[z]:
            FPGA_Ball_Names[z]=subconnects[z][1][1]
            SDRAM_Pin_Names[z]=subconnects[z][2][2]
            SDRAM_Pin_Numbers[z]=subconnects[z][2][1]
            
    result=[]
    for z in range (0,y):
        result.append(str(trace_names[z]) + ',' + str(FPGA_Ball_Names[z]) + ',' + str(SDRAM_Pin_Names[z]) + ',' + str(SDRAM_Pin_Numbers[z]))
        print (result[z].split(","))

    fp=open(str(saving_file)+'.txt',"w")
    fp.write('Trace_Name' + ',' + 'FPGA_Ball_Names' + ',' + 'SDRAM_Pin_Names' + ',' + 'SDRAM_Pin_Numbers' + '\n')
    for z in range (0,y):
        fp.write(result[z])
        fp.write('\n')
    fp.close()

    with open(str(saving_file)+'.csv','w',newline='') as csvfile:
        file_write=csv.writer(csvfile,delimiter=',')
        file_write.writerow(['Trace_Name','FPGA_Ball_Names','SDRAM_Pin_Names','SDRAM_Pin_Numbers'])
        for z in range(0,y):
            file_write.writerow(result[z].split(",")) 


def grav_cs(sections, harness_name, saving_file):
    
    length =len(sections)
    trace_names=[]
    subconnects=[]

    Connector_Pin=[]
    Connector_Ref=[]
    FPGA_Ref=[]
    FPGA_Ball_Names=[]

    for i in range(0,length):
        if harness_name in sections[i][0]:
            print (sections[i])
            trace_names.append(sections[i][0])
            subconnects.append(sections[i][1])
    
    print (subconnects)
    print (trace_names)

    y=len(trace_names)
    
    for z in range(0,y):
        if  harness_name+'TB.' in trace_names[z]:
            Connector_Pin.append(subconnects[z][0][1])
            FPGA_Ball_Names.append(subconnects[z][1][1])
            Connector_Ref.append(subconnects[z][0][0])
            FPGA_Ref.append(subconnects[z][1][0])

        if harness_name+'LR.' in trace_names[z]:
            Connector_Pin.append(subconnects[z][0][1])
            FPGA_Ball_Names.append(subconnects[z][1][1])
            Connector_Ref.append(subconnects[z][0][0])
            FPGA_Ref.append(subconnects[z][1][0])

    print("Connector_Pin", len(Connector_Pin))
    print("FPGA_Ball_Names", len(FPGA_Ball_Names))
    print("Connector_Ref", len(Connector_Ref))
    print("FPGA_Ref", len(FPGA_Ref))
    print("Subconnect", len(subconnects))
    print("Trace_names", len(trace_names))

    result=[]
    for z in range (0,y-1):
        result.append(str(trace_names[z+1]) + ',' + str(FPGA_Ref[z]) + ',' + str(FPGA_Ball_Names[z]) + ',' + str(Connector_Ref[z]) + ',' + str(Connector_Pin[z]))
        print (result[z].split(","))

    fp=open(str(saving_file)+".txt","w")
    fp.write('Trace_Name' + ',' + 'FPGA_Ref' + ',' + 'FPGA_Ball_Names' + ',' + 'Connector_Ref' + ',' + 'Connector_Pins' + '\n')
    for z in range (0,y-1):
        fp.write(result[z])
        fp.write('\n')

    fp.write('----------Writting .LPF File : FPGA A -----------' + '\n')
    fp.write('DEFINE PORT GROUP "IL68_GRP" "IL68[*]";' + '\n')
    fp.write('IOBUF GROUP "IL68_GRP" IO_TYPE=LVCMOS15;' + '\n')
    for z in range (0,y-2):
        fp.write('LOCATE COMP IL68['+ str(z) + '] SITE ' + '"' + FPGA_Ball_Names[z+1] + '"' + ';' + '\n')

    fp.write('----------Writting .LPF File : Connector -----------' + '\n')
    fp.write('DEFINE PORT GROUP "J4_GRP" "J4[*]";' + '\n')
    fp.write('IOBUF GROUP "J4_GRP" IO_TYPE=LVCMOS15;' + '\n')
    for z in range (0,y-2):
        fp.write('LOCATE COMP J4['+ str(z) + '] SITE ' + '"' + Connector_Pin[z+1] + '"' + ';' + '\n')        

    fp.close()

    with open(str(saving_file)+'.csv','w',newline='') as csvfile:
        file_write=csv.writer(csvfile,delimiter=',')
        file_write.writerow(['Trace_Name','FPGA_Ref','FPGA_Ball_Names','Connector_Ref','Connector_Pins'])
        for z in range(0,y-1):
            file_write.writerow(result[z].split(","))               

def run_main(argv):

    argc = len(argv)
    designatorc = argc - 3
	
    filename = argv[1]
    saving_file = argv[3]
    harness_name = argv[2]
    pin_type =argv[4]

    secondary_nets = []

    for i in range(4,argc):
        secondary_nets.append(argv[i])


    # Make header line

    # This is repeated for every primary and secondary net
    csv_header_og = ['REFERENCE', 'PIN #', 'PIN NAME', 'PIN TYPE', 'PART VALUE']
    csv_header = list(csv_header_og)

    for net in secondary_nets:
        for hdr in csv_header_og:
            hdr_name = net + ' ' + hdr
            csv_header.append(hdr_name)

    # Add the first csv header
    csv_header_final = ['NET']
    # Add the rest
    csv_header_final.extend(csv_header)

    res = parse_file(filename)
    sections = parse_sections(res)
    
    if pin_type == "grav_cs":
        grav_cs(sections, harness_name, saving_file)
    elif pin_type == "sdram":
        sdram(sections, harness_name, saving_file)   
    elif pin_type == "chip_to_chip":
        chip_to_chip(sections, harness_name, saving_file)

if __name__ == "__main__":
    run_main(sys.argv)
